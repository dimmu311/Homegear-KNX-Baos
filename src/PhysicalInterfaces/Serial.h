#ifndef Serial_H
#define Serial_H

#include "../BaosPacket.h"

#include <kdrive/baos/Baos.h>

namespace Baos
{

class Serial : public BaseLib::Systems::IPhysicalInterface
{
public:
    explicit Serial(std::shared_ptr<BaseLib::Systems::PhysicalInterfaceSettings> settings);
    ~Serial() override;
    void startListening() override;
    void stopListening() override;
    void sendPacket(std::shared_ptr<BaseLib::Systems::Packet> packet) override;

    kdrive::baos::BaosDatapointDescriptions::Descriptions searchPeers();
    std::string getSerialNumber();

    bool isOpen() override { return !_stopped; }
 
protected:
    BaseLib::Output _out;
    kdrive::baos::BaosEvent _baosEvent;
    kdrive::baos::BaosConnector::Ptr _connector;
    std::string _device;
    std::string _serialNumber;

    std::atomic<uint16_t> _messageCounter{ 0 };

    void listen();
    
    void onDatapointEvent(kdrive::baos::BaosDatapoint& baosDatapoint);
    void onBusConnectedEvent(bool busConnected);
    void onConnectorClosedEvent();
};

}
#endif
