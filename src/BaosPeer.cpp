#include "BaosPeer.h"
#include "BaosCentral.h"
#include "Baos.h"
#include "PhysicalInterfaces/Serial.h"
#include "GD.h"
#include <sstream>
#include <iomanip>

namespace Baos
{
std::shared_ptr<BaseLib::Systems::ICentral> BaosPeer::getCentral()
{
	try
	{
		if(_central) return _central;
		_central = GD::family->getCentral();
		return _central;
	}
	catch(const std::exception& ex)
	{
		GD::out.printEx(__FILE__, __LINE__, __PRETTY_FUNCTION__, ex.what());
	}
	return std::shared_ptr<BaseLib::Systems::ICentral>();
}

BaosPeer::BaosPeer(uint32_t parentID, IPeerEventSink* eventHandler) : Peer(GD::bl, parentID, eventHandler)
{
  _binaryEncoder.reset(new BaseLib::Rpc::RpcEncoder(GD::bl));
  _binaryDecoder.reset(new BaseLib::Rpc::RpcDecoder(GD::bl));
}

BaosPeer::BaosPeer(int32_t id, int32_t address, std::string serialNumber, uint32_t parentID, IPeerEventSink* eventHandler) : Peer(GD::bl, id, address, serialNumber, parentID, eventHandler)
{
  _binaryEncoder.reset(new BaseLib::Rpc::RpcEncoder(GD::bl));
  _binaryDecoder.reset(new BaseLib::Rpc::RpcDecoder(GD::bl));
}

BaosPeer::~BaosPeer()
{
	dispose();
}

std::string BaosPeer::handleCliCommand(std::string command)
{
	try
	{
		std::ostringstream stringStream;

		if(command == "help")
		{
			stringStream << "List of commands:" << std::endl << std::endl;
			stringStream << "For more information about the individual command type: COMMAND help" << std::endl << std::endl;
			stringStream << "unselect\t\tUnselect this peer" << std::endl;
      stringStream << "channel count\t\tPrint the number of channels of this peer" << std::endl;
      stringStream << "config print\t\tPrints all configuration parameters and their values" << std::endl;
			return stringStream.str();
		}
    if (command.compare(0, 13, "channel count") == 0)
    {
      std::stringstream stream(command);
      std::string element;
      int32_t index = 0;
      while (std::getline(stream, element, ' '))
      {
        if (index < 2)
        {
          index++;
          continue;
        }
        else if (index == 2)
        {
          if (element == "help")
          {
            stringStream << "Description: This command prints this peer's number of channels." << std::endl;
            stringStream << "Usage: channel count" << std::endl << std::endl;
            stringStream << "Parameters:" << std::endl;
            stringStream << "  There are no parameters." << std::endl;
            return stringStream.str();
          }
        }
        index++;
      }

      stringStream << "Peer has " << _rpcDevice->functions.size() << " channels." << std::endl;
      return stringStream.str();
    }
    else if (command.compare(0, 12, "config print") == 0) {
      std::stringstream stream(command);
      std::string element;
      int32_t index = 0;
      while (std::getline(stream, element, ' ')) {
        if (index < 2) {
          index++;
          continue;
        }
        else if (index == 2) {
          if (element == "help") {
            stringStream
              << "Description: This command prints all configuration parameters of this peer. The values are in BidCoS packet format."
              << std::endl;
            stringStream << "Usage: config print" << std::endl << std::endl;
            stringStream << "Parameters:" << std::endl;
            stringStream << "  There are no parameters." << std::endl;
            return stringStream.str();
          }
        }
        index++;
      }
      return printConfig();
    }
		return "Unknown command.\n";
	}
	catch(const std::exception& ex)
    {
        GD::out.printEx(__FILE__, __LINE__, __PRETTY_FUNCTION__, ex.what());
    }
    return "Error executing command. See log file for more details.\n";
}

std::string BaosPeer::printConfig() {
  try {
    std::ostringstream stringStream;
    stringStream << "MASTER" << std::endl;
    stringStream << "{" << std::endl;
    for (std::unordered_map<uint32_t, std::unordered_map<std::string, BaseLib::Systems::RpcConfigurationParameter>>::iterator i = configCentral.begin();
      i != configCentral.end(); ++i) {
      stringStream << "\t" << "Channel: " << std::dec << i->first << std::endl;
      stringStream << "\t{" << std::endl;
      for (std::unordered_map<std::string, BaseLib::Systems::RpcConfigurationParameter>::iterator j = i->second.begin();
        j != i->second.end(); ++j) {
        stringStream << "\t\t[" << j->first << "]: ";
        if (!j->second.rpcParameter) stringStream << "(No RPC parameter) ";
        std::vector<uint8_t> parameterData = j->second.getBinaryData();
        for (std::vector<uint8_t>::const_iterator k = parameterData.begin();
          k != parameterData.end(); ++k) {
          stringStream << std::hex << std::setfill('0') << std::setw(2) << (int32_t)*k << " ";
        }
        stringStream << std::endl;
      }
      stringStream << "\t}" << std::endl;
    }
    stringStream << "}" << std::endl << std::endl;

    stringStream << "VALUES" << std::endl;
    stringStream << "{" << std::endl;
    for (std::unordered_map<uint32_t, std::unordered_map<std::string, BaseLib::Systems::RpcConfigurationParameter>>::iterator i = valuesCentral.begin();
      i != valuesCentral.end(); ++i) {
      stringStream << "\t" << "Channel: " << std::dec << i->first << std::endl;
      stringStream << "\t{" << std::endl;
      for (std::unordered_map<std::string, BaseLib::Systems::RpcConfigurationParameter>::iterator j = i->second.begin();
        j != i->second.end(); ++j) {
        stringStream << "\t\t[" << j->first << "]: ";
        if (!j->second.rpcParameter) stringStream << "(No RPC parameter) ";
        std::vector<uint8_t> parameterData = j->second.getBinaryData();
        for (std::vector<uint8_t>::const_iterator k = parameterData.begin();
          k != parameterData.end(); ++k) {
          stringStream << std::hex << std::setfill('0') << std::setw(2) << (int32_t)*k << " ";
        }
        stringStream << std::endl;
      }
      stringStream << "\t}" << std::endl;
    }
    stringStream << "}" << std::endl << std::endl;

    return stringStream.str();
  }
  catch (const std::exception& ex) {
    GD::out.printEx(__FILE__, __LINE__, __PRETTY_FUNCTION__, ex.what());
  }
  return "";
}

void BaosPeer::save(bool savePeer, bool variables, bool centralConfig)
{
	try
	{
		Peer::save(savePeer, variables, centralConfig);
	}
	catch(const std::exception& ex)
    {
    	GD::out.printEx(__FILE__, __LINE__, __PRETTY_FUNCTION__, ex.what());
    }
}

void BaosPeer::setPhysicalInterfaceId(std::string id)
{
    if(id.empty() || (GD::physicalInterfaces.find(id) != GD::physicalInterfaces.end() && GD::physicalInterfaces.at(id)))
    {
        _physicalInterfaceId = id;
        setPhysicalInterface(GD::physicalInterfaces.at(_physicalInterfaceId));
        saveVariable(19, _physicalInterfaceId);
    }
}

void BaosPeer::setPhysicalInterface(std::shared_ptr<Serial> interface)
{
	try
	{
		if(!interface) return;
		_physicalInterface = interface;
	}
	catch(const std::exception& ex)
    {
    	GD::out.printEx(__FILE__, __LINE__, __PRETTY_FUNCTION__, ex.what());
    }
}

void BaosPeer::loadVariables(BaseLib::Systems::ICentral* central, std::shared_ptr<BaseLib::Database::DataTable>& rows)
{
  try {
    if (!rows) rows = _bl->db->getPeerVariables(_peerID);
    Peer::loadVariables(central, rows);

    for (BaseLib::Database::DataTable::iterator row = rows->begin(); row != rows->end(); ++row) {
      switch (row->second.at(2)->intValue) {
      case 19:
        _physicalInterfaceId = row->second.at(4)->textValue;
        setPhysicalInterface(GD::physicalInterfaces.at(_physicalInterfaceId));
        break;
      }
    }
  }
  catch (const std::exception& ex)
  {
    GD::out.printEx(__FILE__, __LINE__, __PRETTY_FUNCTION__, ex.what());
  }
}


void BaosPeer::saveVariables()
{
  try {
    if (_peerID == 0) return;
    Peer::saveVariables();

    saveVariable(19, _physicalInterfaceId);
  }
  catch (const std::exception& ex)
  {
    GD::out.printEx(__FILE__, __LINE__, __PRETTY_FUNCTION__, ex.what());
  }
}

bool BaosPeer::load(BaseLib::Systems::ICentral* central)
{
	try
	{
		std::shared_ptr<BaseLib::Database::DataTable> rows;
    if (!rows) rows = _bl->db->getPeerVariables(_peerID);
		loadVariables(central, rows);

		_rpcDevice = GD::family->getRpcDevices()->find(_deviceType, _firmwareVersion, -1);
		if(!_rpcDevice)
		{
			GD::out.printError("Error loading peer " + std::to_string(_peerID) + ": Device type not found: 0x" + BaseLib::HelperFunctions::getHexString(_deviceType) + " Firmware version: " + std::to_string(_firmwareVersion));
			return false;
		}
		initializeTypeString();
		loadConfig();
		initializeCentralConfig();

		serviceMessages.reset(new BaseLib::Systems::ServiceMessages(_bl, _peerID, _serialNumber, this));
		serviceMessages->load();

		return true;
	}
	catch(const std::exception& ex)
    {
    	GD::out.printEx(__FILE__, __LINE__, __PRETTY_FUNCTION__, ex.what());
    }
    return false;
}

PParameterGroup BaosPeer::getParameterSet(int32_t channel, ParameterGroup::Type::Enum type)
{
	try
	{
		PParameterGroup parameterGroup = _rpcDevice->functions.at(channel)->getParameterGroup(type);
		if(!parameterGroup || parameterGroup->parameters.empty())
		{
			GD::out.printDebug("Debug: Parameter set of type " + std::to_string(type) + " not found for channel " + std::to_string(channel));
			return PParameterGroup();
		}
		return parameterGroup;
	}
	catch(const std::exception& ex)
	{
		GD::out.printEx(__FILE__, __LINE__, __PRETTY_FUNCTION__, ex.what());
	}
	return PParameterGroup();
}

void BaosPeer::getValuesFromPacket(PBaosPacket packet, std::vector<FrameValues>& frameValues)
{
  try
  {
    if(!_rpcDevice) return;
    if (_rpcDevice->packetsByFunction1.find(packet->getMethod()) == _rpcDevice->packetsByFunction1.end()) return;
    std::pair<PacketsByFunction::iterator, PacketsByFunction::iterator> range = _rpcDevice->packetsByFunction1.equal_range(packet->getMethod());
    if (range.first == _rpcDevice->packetsByFunction1.end()) return;
    PacketsById::iterator i = range.first;
    do
    {
      FrameValues currentFrameValues;
      PPacket frame(i->second);
      if(!frame) continue;
      if (frame->direction != BaseLib::DeviceDescription::Packet::Direction::Enum::toCentral) continue;
      int32_t channel = -1;
      channel = packet->getDatapointId();
      if (frame->channel > -1) channel = frame->channel;
      currentFrameValues.frameID = frame->id;

      for (JsonPayloads::iterator j = frame->jsonPayloads.begin(); j != frame->jsonPayloads.end(); ++j)
      {
        BaseLib::PVariable currentJson = packet->getJson();
        auto keyIterator = currentJson->structValue->find((*j)->key);
        if (keyIterator == currentJson->structValue->end()) continue;
        currentJson = keyIterator->second;
        if (!(*j)->subkey.empty())
        {
          auto subkeyIterator = currentJson->structValue->find((*j)->subkey);
          if (subkeyIterator == currentJson->structValue->end()) continue;
          currentJson = subkeyIterator->second;
          if (!(*j)->subsubkey.empty())
          {
            auto subsubkeyIterator = currentJson->structValue->find((*j)->subsubkey);
            if (subsubkeyIterator == currentJson->structValue->end()) continue;
            currentJson = subsubkeyIterator->second;
          }
        }

        for (std::vector<PParameter>::iterator k = frame->associatedVariables.begin(); k != frame->associatedVariables.end(); ++k)
        {
          if ((*k)->physical->groupId != (*j)->parameterId) continue;
          currentFrameValues.parameterSetType = (*k)->parent()->type();

          bool setValues = false;

          if (currentFrameValues.paramsetChannels.empty()) //Fill paramsetChannels
          {
            int32_t startChannel = (channel < 0) ? 0 : channel;
            int32_t endChannel;
            //When fixedChannel is -2 (means '*') cycle through all channels
            if (frame->channel == -2)
            {
              startChannel = 0;
              endChannel = _rpcDevice->functions.rbegin()->first;
            }
            else endChannel = startChannel;

            for (int32_t l = startChannel; l <= endChannel; l++)
            {
              Functions::iterator functionIterator = _rpcDevice->functions.find(l);
              if (functionIterator == _rpcDevice->functions.end()) continue;
              PParameterGroup parameterGroup = functionIterator->second->getParameterGroup(currentFrameValues.parameterSetType);

              if (!parameterGroup) continue;
              if (!parameterGroup || parameterGroup->parameters.find((*k)->id) == parameterGroup->parameters.end()) continue;
              currentFrameValues.paramsetChannels.push_back(l);
              currentFrameValues.values[(*k)->id].channels.push_back(l);
              setValues = true;
            }
          }
          else //Use paramsetChannels
          {
            for (std::list<uint32_t>::const_iterator l = currentFrameValues.paramsetChannels.begin(); l != currentFrameValues.paramsetChannels.end(); ++l)
            {
              Functions::iterator functionIterator = _rpcDevice->functions.find(*l);
              if (functionIterator == _rpcDevice->functions.end()) continue;
              PParameterGroup parameterGroup = functionIterator->second->getParameterGroup(currentFrameValues.parameterSetType);
              if (!parameterGroup || parameterGroup->parameters.find((*k)->id) == parameterGroup->parameters.end()) continue;
              currentFrameValues.values[(*k)->id].channels.push_back(*l);
              setValues = true;
            }
          }

          if (setValues)
          {
            //This is a little nasty and costs a lot of resources, but we need to run the data through the packet converter
            std::vector<uint8_t> encodedData;
            _binaryEncoder->encodeResponse(currentJson, encodedData);
            PVariable data = (*k)->convertFromPacket(encodedData, Role(), true);
            (*k)->convertToPacket(data, Role(), currentFrameValues.values[(*k)->id].value);
          }
        }
      }
      if (!currentFrameValues.values.empty()) frameValues.push_back(currentFrameValues);
    } while(++i != range.second && i != _rpcDevice->packetsByFunction1.end());
  }
  catch(const std::exception& ex)
  {
    GD::out.printEx(__FILE__, __LINE__, __PRETTY_FUNCTION__, ex.what());
  }
}

void BaosPeer::packetReceived(std::shared_ptr<BaosPacket> packet)
{
  try
  {
    if(_disposing || !packet || !_rpcDevice) return;
    auto central = std::dynamic_pointer_cast<BaosCentral>(getCentral());
    if(!central) return;
    setLastPacketReceived();
    serviceMessages->endUnreach();

    std::vector<FrameValues> frameValues;
    getValuesFromPacket(packet, frameValues);
    std::map<uint32_t, std::shared_ptr<std::vector<std::string>>> valueKeys;
    std::map<uint32_t, std::shared_ptr<std::vector<PVariable>>> rpcValues;

    //Loop through all matching frames
    for(std::vector<FrameValues>::iterator a = frameValues.begin(); a != frameValues.end(); ++a)
    {
      PPacket frame;
      if(!a->frameID.empty()) frame = _rpcDevice->packetsById.at(a->frameID);
      if(!frame) continue;

      for(std::map<std::string, FrameValue>::iterator i = a->values.begin(); i != a->values.end(); ++i)
      {
        for(std::list<uint32_t>::const_iterator j = a->paramsetChannels.begin(); j != a->paramsetChannels.end(); ++j)
        {
          if(std::find(i->second.channels.begin(), i->second.channels.end(), *j) == i->second.channels.end()) continue;
          if (*j != packet->getDatapointId()) continue;
          if(!valueKeys[*j] || !rpcValues[*j])
          {
            valueKeys[*j].reset(new std::vector<std::string>());
            rpcValues[*j].reset(new std::vector<PVariable>());
          }

          BaseLib::Systems::RpcConfigurationParameter& parameter = valuesCentral[*j][i->first];
          parameter.setBinaryData(i->second.value);
          if(parameter.databaseId > 0) saveParameter(parameter.databaseId, i->second.value);
          else saveParameter(0, ParameterGroup::Type::Enum::variables, *j, i->first, i->second.value);
          if(_bl->debugLevel >= 4) GD::out.printInfo("Info: " + i->first + " on channel " + std::to_string(*j) + " of peer " + std::to_string(_peerID) + " with serial number " + _serialNumber  + " was set to 0x" + BaseLib::HelperFunctions::getHexString(i->second.value) + ".");

          if(parameter.rpcParameter)
          {
            //Process service messages
            if(parameter.rpcParameter->service && !i->second.value.empty())
            {
              if(parameter.rpcParameter->logical->type == ILogical::Type::Enum::tEnum)
              {
                serviceMessages->set(i->first, i->second.value.at(0), *j);
              }
              else if(parameter.rpcParameter->logical->type == ILogical::Type::Enum::tBoolean)
              {
                serviceMessages->set(i->first, parameter.rpcParameter->convertFromPacket(i->second.value, parameter.mainRole(), true)->booleanValue);
              }
            }

            valueKeys[*j]->push_back(i->first);
            rpcValues[*j]->push_back(parameter.rpcParameter->convertFromPacket(i->second.value, parameter.mainRole(), true));
          }
        }
      }
    }

    if(!rpcValues.empty())
    {
      for(std::map<uint32_t, std::shared_ptr<std::vector<std::string>>>::iterator j = valueKeys.begin(); j != valueKeys.end(); ++j)
      {
        if(j->second->empty()) continue;
        std::string eventSource = "device-" + std::to_string(_peerID);
        std::string address(_serialNumber + ":" + std::to_string(j->first));
        raiseEvent(eventSource, _peerID, j->first, j->second, rpcValues.at(j->first));
        raiseRPCEvent(eventSource, _peerID, j->first, address, j->second, rpcValues.at(j->first));
      }
    }
  }
  catch(const std::exception& ex)
  {
    GD::out.printEx(__FILE__, __LINE__, __PRETTY_FUNCTION__, ex.what());
  }
}

//RPC Methods
PVariable BaosPeer::getDeviceInfo(BaseLib::PRpcClientInfo clientInfo, std::map<std::string, bool> fields)
{
	try
	{
		PVariable info(Peer::getDeviceInfo(clientInfo, fields));
		if(info->errorStruct) return info;

		if(fields.empty() || fields.find("INTERFACE") != fields.end()) info->structValue->insert(StructElement("INTERFACE", PVariable(new Variable(_physicalInterface->getID()))));

		return info;
	}
	catch(const std::exception& ex)
    {
    	GD::out.printEx(__FILE__, __LINE__, __PRETTY_FUNCTION__, ex.what());
    }
    return PVariable();
}

PVariable BaosPeer::getParamsetDescription(BaseLib::PRpcClientInfo clientInfo, int32_t channel, ParameterGroup::Type::Enum type, uint64_t remoteID, int32_t remoteChannel, bool checkAcls)
{
	try
	{
		if(_disposing) return Variable::createError(-32500, "Peer is disposing.");
		if(channel < 0) channel = 0;
		Functions::iterator functionIterator = _rpcDevice->functions.find(channel);
		if(functionIterator == _rpcDevice->functions.end()) return Variable::createError(-2, "Unknown channel");
		PParameterGroup parameterGroup = functionIterator->second->getParameterGroup(type);
		if(!parameterGroup) return Variable::createError(-3, "Unknown parameter set");

		return Peer::getParamsetDescription(clientInfo, channel, parameterGroup, checkAcls);
	}
	catch(const std::exception& ex)
    {
        GD::out.printEx(__FILE__, __LINE__, __PRETTY_FUNCTION__, ex.what());
    }
    return Variable::createError(-32500, "Unknown application error.");
}

PVariable BaosPeer::putParamset(BaseLib::PRpcClientInfo clientInfo, int32_t channel, ParameterGroup::Type::Enum type, uint64_t remoteID, int32_t remoteChannel, PVariable variables, bool checkAcls, bool onlyPushing)
{
	try
	{
		if(_disposing) return Variable::createError(-32500, "Peer is disposing.");
		if(channel < 0) channel = 0;
		Functions::iterator functionIterator = _rpcDevice->functions.find(channel);
		if(functionIterator == _rpcDevice->functions.end()) return Variable::createError(-2, "Unknown channel");
		PParameterGroup parameterGroup = functionIterator->second->getParameterGroup(type);
		if(!parameterGroup) return Variable::createError(-3, "Unknown parameter set");
		if(variables->structValue->empty()) return PVariable(new Variable(VariableType::tVoid));

		auto central = getCentral();
		if(!central) return Variable::createError(-32500, "Could not get central.");

		if(type == ParameterGroup::Type::Enum::variables)
		{
			for(Struct::iterator i = variables->structValue->begin(); i != variables->structValue->end(); ++i)
			{
				if(i->first.empty() || !i->second) continue;

				if(checkAcls && !clientInfo->acls->checkVariableWriteAccess(central->getPeer(_peerID), channel, i->first)) continue;

				setValue(clientInfo, channel, i->first, i->second, true);
			}
		}
		else
		{
			return Variable::createError(-3, "Parameter set type is not supported.");
		}
		return std::make_shared<Variable>(VariableType::tVoid);
	}
	catch(const std::exception& ex)
    {
        GD::out.printEx(__FILE__, __LINE__, __PRETTY_FUNCTION__, ex.what());
    }
    return Variable::createError(-32500, "Unknown application error.");
}

PVariable BaosPeer::getParamset(BaseLib::PRpcClientInfo clientInfo, int32_t channel, ParameterGroup::Type::Enum type, uint64_t remoteID, int32_t remoteChannel, bool checkAcls)
{
	try
	{
		if(_disposing) return Variable::createError(-32500, "Peer is disposing.");
		if(channel < 0) channel = 0;
		if(remoteChannel < 0) remoteChannel = 0;
		Functions::iterator functionIterator = _rpcDevice->functions.find(channel);
		if(functionIterator == _rpcDevice->functions.end()) return Variable::createError(-2, "Unknown channel");
		PParameterGroup parameterGroup = functionIterator->second->getParameterGroup(type);
		if(!parameterGroup) return Variable::createError(-3, "Unknown parameter set");
		PVariable variables(new Variable(VariableType::tStruct));

		auto central = getCentral();
		if(!central) return Variable::createError(-32500, "Could not get central.");

		for(Parameters::iterator i = parameterGroup->parameters.begin(); i != parameterGroup->parameters.end(); ++i)
		{
			if(i->second->id.empty()) continue;
			if(!i->second->visible && !i->second->service && !i->second->internal && !i->second->transform)
			{
				GD::out.printDebug("Debug: Omitting parameter " + i->second->id + " because of it's ui flag.");
				continue;
			}
			PVariable element;
			if(type == ParameterGroup::Type::Enum::variables)
			{
				if(checkAcls && !clientInfo->acls->checkVariableReadAccess(central->getPeer(_peerID), channel, i->first)) continue;
				if(!i->second->readable) continue;
				if(valuesCentral.find(channel) == valuesCentral.end()) continue;
				if(valuesCentral[channel].find(i->second->id) == valuesCentral[channel].end()) continue;
				auto& parameter = valuesCentral[channel][i->second->id];
				std::vector<uint8_t> parameterData = parameter.getBinaryData();
				element = i->second->convertFromPacket(parameterData, parameter.mainRole(), false);
			}

			if(!element) continue;
			if(element->type == VariableType::tVoid) continue;
			variables->structValue->insert(StructElement(i->second->id, element));
		}
		return variables;
	}
	catch(const std::exception& ex)
    {
        GD::out.printEx(__FILE__, __LINE__, __PRETTY_FUNCTION__, ex.what());
    }
    return Variable::createError(-32500, "Unknown application error.");
}

PVariable BaosPeer::setValue(BaseLib::PRpcClientInfo clientInfo, uint32_t channel, std::string valueKey, PVariable value, bool wait)
{
    try
    {
        if(_disposing) return Variable::createError(-32500, "Peer is disposing.");
        if(!value) return Variable::createError(-32500, "value is nullptr.");
        Peer::setValue(clientInfo, channel, valueKey, value, wait); //Ignore result, otherwise setHomegerValue might not be executed
        std::shared_ptr<BaosCentral> central = std::dynamic_pointer_cast<BaosCentral>(getCentral());
        if(!central) return Variable::createError(-32500, "Could not get central object.");;
        if(valueKey.empty()) return Variable::createError(-5, "Value key is empty.");
        if(channel == 0 && serviceMessages->set(valueKey, value->booleanValue)) return PVariable(new Variable(VariableType::tVoid));
        std::unordered_map<uint32_t, std::unordered_map<std::string, BaseLib::Systems::RpcConfigurationParameter>>::iterator channelIterator = valuesCentral.find(channel);
        if(channelIterator == valuesCentral.end()) return Variable::createError(-2, "Unknown channel.");
        std::unordered_map<std::string, BaseLib::Systems::RpcConfigurationParameter>::iterator parameterIterator = channelIterator->second.find(valueKey);
        if(parameterIterator == valuesCentral[channel].end()) return Variable::createError(-5, "Unknown parameter.");
        PParameter rpcParameter = parameterIterator->second.rpcParameter;
        if(!rpcParameter) return Variable::createError(-5, "Unknown parameter.");
        BaseLib::Systems::RpcConfigurationParameter& parameter = valuesCentral[channel][valueKey];
        std::shared_ptr<std::vector<std::string>> valueKeys(new std::vector<std::string>());
        std::shared_ptr<std::vector<PVariable>> values(new std::vector<PVariable>());

        if(rpcParameter->physical->operationType == IPhysical::OperationType::Enum::store)
        {
            std::vector<uint8_t> parameterData;
            rpcParameter->convertToPacket(value, parameter.mainRole(), parameterData);
            parameter.setBinaryData(parameterData);
            if(parameter.databaseId > 0) saveParameter(parameter.databaseId, parameterData);
            else saveParameter(0, ParameterGroup::Type::Enum::variables, channel, valueKey, parameterData);

            if(rpcParameter->readable)
            {
                valueKeys->push_back(valueKey);
                values->push_back(rpcParameter->convertFromPacket(parameterData, parameter.mainRole(), true));
            }

            if(!valueKeys->empty())
            {
                std::string address(_serialNumber + ":" + std::to_string(channel));
                raiseEvent(clientInfo->initInterfaceId, _peerID, channel, valueKeys, values);
                raiseRPCEvent(clientInfo->initInterfaceId, _peerID, channel, address, valueKeys, values);
            }
            return std::make_shared<Variable>(VariableType::tVoid);
        }
        else if(rpcParameter->physical->operationType != IPhysical::OperationType::Enum::command) return Variable::createError(-6, "Parameter is not settable.");
        if(rpcParameter->setPackets.empty() && !rpcParameter->writeable) return Variable::createError(-6, "parameter is read only");
        std::vector<std::shared_ptr<Parameter::Packet>> setRequests;
        if(!rpcParameter->setPackets.empty())
        {
            for(std::vector<std::shared_ptr<Parameter::Packet>>::iterator i = rpcParameter->setPackets.begin(); i != rpcParameter->setPackets.end(); ++i)
            {
                if((*i)->conditionOperator != Parameter::Packet::ConditionOperator::none)
                {
                    int32_t intValue = value->integerValue;
                    if(parameter.rpcParameter->logical->type == BaseLib::DeviceDescription::ILogical::Type::Enum::tBoolean) intValue = value->booleanValue;
                    if(!(*i)->checkCondition(intValue)) continue;
                }
                setRequests.push_back(*i);
            }
        }

        std::vector<uint8_t> parameterData;
        rpcParameter->convertToPacket(value, parameter.mainRole(), parameterData);
        parameter.setBinaryData(parameterData);
        if(parameter.databaseId > 0) saveParameter(parameter.databaseId, parameterData);
        else saveParameter(0, ParameterGroup::Type::Enum::variables, channel, valueKey, parameterData);
        if(_bl->debugLevel >= 4) GD::out.printInfo("Info: " + valueKey + " of peer " + std::to_string(_peerID) + " with serial number " + _serialNumber + ":" + std::to_string(channel) + " was set to 0x" + BaseLib::HelperFunctions::getHexString(parameterData) + ".");

        if(rpcParameter->readable)
        {
            valueKeys->push_back(valueKey);
            values->push_back(rpcParameter->convertFromPacket(parameterData, parameter.mainRole(), true));
        }

        for(std::shared_ptr<Parameter::Packet> setRequest : setRequests)
        {
            PacketsById::iterator packetIterator = _rpcDevice->packetsById.find(setRequest->id);
            if(packetIterator == _rpcDevice->packetsById.end()) return Variable::createError(-6, "No frame was found for parameter " + valueKey);
            PPacket frame = packetIterator->second;

            std::shared_ptr<std::map<std::string, PVariable>> values(new std::map<std::string, PVariable>());
            for (JsonPayloads::iterator i = frame->jsonPayloads.begin(); i != frame->jsonPayloads.end(); ++i)
            {
              bool paramFound = false;
              for (std::unordered_map<std::string, BaseLib::Systems::RpcConfigurationParameter>::iterator j = valuesCentral[channel].begin(); j != valuesCentral[channel].end(); ++j)
              {
                if (!j->second.rpcParameter) continue;
                if ((*i)->parameterId == j->second.rpcParameter->physical->groupId)
                {
                  if ((*i)->key.empty()) continue;
                  std::vector<uint8_t> parameterData = j->second.getBinaryData();
                  values->emplace(std::pair<std::string, PVariable>((*i)->key, _binaryDecoder->decodeResponse(parameterData)));
                  paramFound = true;
                  break;
                }
              }
              if (!paramFound) GD::out.printError("Error constructing packet. param \"" + (*i)->parameterId + "\" not found. Peer: " + std::to_string(_peerID) + " Serial number: " + _serialNumber + " Frame: " + frame->id);

            }
            auto packet = std::make_shared<BaosPacket>(channel, frame->function1, values);
            if (!_physicalInterface) continue;
            _physicalInterface->sendPacket(packet);
        }

        if(!valueKeys->empty())
        {
            std::string address(_serialNumber + ":" + std::to_string(channel));
            raiseEvent(clientInfo->initInterfaceId, _peerID, channel, valueKeys, values);
            raiseRPCEvent(clientInfo->initInterfaceId, _peerID, channel, address, valueKeys, values);
        }

        return std::make_shared<Variable>(VariableType::tVoid);
    }
    catch(const std::exception& ex)
    {
        GD::out.printEx(__FILE__, __LINE__, __PRETTY_FUNCTION__, ex.what());
    }
    return Variable::createError(-32500, "Unknown application error. See error log for more details.");
}
//End RPC methods
}
