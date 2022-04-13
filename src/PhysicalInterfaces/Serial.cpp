#include "Serial.h"
#include "../Baos.h"
#include "../GD.h"
#include <functional>

namespace Baos
{

Serial::Serial(std::shared_ptr<BaseLib::Systems::PhysicalInterfaceSettings> settings) : IPhysicalInterface(GD::bl, GD::family->getFamily(), settings)
{
  _out.init(GD::bl);
  _out.setPrefix(GD::out.getPrefix() + "Baos Serial \"" + settings->id + "\": ");

  signal(SIGPIPE, SIG_IGN);

  _stopped = true;

  if(!settings)
  {
    _out.printCritical("Critical: Error initializing. Settings pointer is empty.");
    return;
  }
  _device = settings->device;
}

Serial::~Serial()
{
  stopListening();
}

void Serial::sendPacket(std::shared_ptr<BaseLib::Systems::Packet> packet)
{
  try
  {
    PBaosPacket baosPacket(std::dynamic_pointer_cast<BaosPacket>(packet));
    if(!baosPacket) return;

    kdrive::baos::BaosDatapoint datapoint(_connector, baosPacket->getDatapointId());
    if (!baosPacket->setValuesToDatapoint(datapoint)) _out.printError("Error sending packet " + baosPacket->getMethod());
    else _lastPacketSent = BaseLib::HelperFunctions::getTime();
  }
  catch(const std::exception& ex)
  {
    _out.printEx(__FILE__, __LINE__, __PRETTY_FUNCTION__, ex.what());
  }
}

void Serial::startListening()
{
  try
  {
    stopListening();

    if(_device.empty())
    {
      _out.printError("Error: Configuration of KNX BAOS Serial is incomplete (device is missing). Please correct it in \"knxBaos.conf\".");
      return;
    }

    _stopCallbackThread = false;
    if(_settings->listenThreadPriority > -1) _bl->threadManager.start(_listenThread, true, _settings->listenThreadPriority, _settings->listenThreadPolicy, &Serial::listen, this);
    else _bl->threadManager.start(_listenThread, true, &Serial::listen, this);
    IPhysicalInterface::startListening();
  }
  catch(const std::exception& ex)
  {
    _out.printEx(__FILE__, __LINE__, __PRETTY_FUNCTION__, ex.what());
  }
}

void Serial::stopListening()
{
  try
  {
    _stopCallbackThread = true;
    _bl->threadManager.join(_listenThread);
    IPhysicalInterface::stopListening();
  }
  catch(const std::exception& ex)
  {
    _out.printEx(__FILE__, __LINE__, __PRETTY_FUNCTION__, ex.what());
  }
}

void Serial::listen()
{
  try
  {  
    _out.printInfo("Info: Start Listening.");       
        
    kdrive::baos::ScopedSerialBaosConnection connection(_device, true);
    if (!connection.isConnected()) {
      _out.printError("Could not open Serial device " + _device);
      return;
    }
    _stopped = false;
    _connector = connection.getConnector();

    _baosEvent.setDatapointEvent(std::bind(&Serial::onDatapointEvent, this, std::placeholders::_1));
    _baosEvent.setBusConnectedEvent(std::bind(&Serial::onBusConnectedEvent, this, std::placeholders::_1));
    _baosEvent.setConnectorClosedEvent(std::bind(&Serial::onConnectorClosedEvent, this));
    _baosEvent.setConnector(_connector);

    // get the Serial Number
    kdrive::baos::BaosServerItems baosServerItems(_connector);
    _serialNumber = BaseLib::HelperFunctions::getHexString(baosServerItems.getSerialNumber());
    _out.printInfo("Serial Number: " + _serialNumber);

    _out.printInfo("Info: Initialization complete.");

    while (!_stopCallbackThread)
    {
      std::this_thread::sleep_for(std::chrono::milliseconds(1000));
      //_out.printInfo("Info: Sleept 1Sec");
    }
    
    connection.disconnect();
  }
  catch(const std::exception& ex)
  {
    _out.printEx(__FILE__, __LINE__, __PRETTY_FUNCTION__, ex.what());
  }
}
std::string Serial::getSerialNumber() {
  return _serialNumber;
}


kdrive::baos::BaosDatapointDescriptions::Descriptions Serial::searchPeers() {
  _out.printInfo("Info: Read Datapoint Descriptions.");
  // Reads the data point descriptions
  kdrive::baos::BaosDatapointDescriptions baosDatapointDescriptions(_connector);
  baosDatapointDescriptions.readFromDevice();

  const kdrive::baos::BaosDatapointDescriptions::Descriptions descriptions = baosDatapointDescriptions.getDescriptions();
  
  const uint32_t count = descriptions.size();
  _out.printInfo(std::to_string(count) + " items found.");

  for (const auto& item : descriptions)
  {
    const kdrive::baos::BaosDatapointDescription& d = item.second;
    const int id = d.getId();
    const int dpt = d.getDatapointType();
    const int bits = d.getValueTypeSizeBits();
    const int bytes = d.getValueTypeSizeBytes();
    
    const std::string size = d.isBitType() ? std::to_string(bits) + " Bits" : std::to_string(bytes) + " Bytes";
    _out.printInfo("Id: " + std::to_string(id) + ", Datapoint type " + std::to_string(dpt) + ", Size: " + size);
    try {
      kdrive::baos::BaosDatapoint datapoint(_connector, id);
      const std::string description = datapoint.getDescriptionString();
    }
    catch (const std::exception& ex) {
      _out.printInfo("getDescriptionString Error");
      _out.printEx(__FILE__, __LINE__, __PRETTY_FUNCTION__, ex.what());
    }   
  }
  
  return descriptions;
}

/*!
    Called when a DatapointValue indication is received
*/
void Serial::onDatapointEvent(kdrive::baos::BaosDatapoint& baosDatapoint)
{
  const std::vector<uint8_t>& data = baosDatapoint.getData();
  auto baosDatapointType = baosDatapoint.getDatapointDescription().getDatapointType();
  _out.printInfo("Info: Received datapoint value for id " + std::to_string(baosDatapoint.getId()) + " = " + BaseLib::HelperFunctions::getHexString(data));
  try
  {
    auto baosPacket = std::make_shared<BaosPacket>(_serialNumber, baosDatapoint);
    raisePacketReceived(baosPacket);
  }
  catch (const std::exception& ex)
  {
    _out.printEx(__FILE__, __LINE__, __PRETTY_FUNCTION__, ex.what());
  }
}

/*!
    Called when the KNX bus is connected or disconnected
    to test this, temporarily disconnect the KNX bus connection
    from the baos device
*/
void Serial::onBusConnectedEvent(bool busConnected)
{
  _out.printInfo("Info: KNX bus connection state event: " + std::string(busConnected ? "connected" : "disconnected"));
  _stopped = !busConnected;
}

/*!
    Called when the TCP/IP connection is broken.
    To test this, simply remove the IP cable from the baos device.
    Note: it can take some time for the stream connection to indicate
    the the connection has been shutdown
*/
void Serial::onConnectorClosedEvent()
{
  _out.printInfo("Info: Baos Connector disconnected");
}


}

