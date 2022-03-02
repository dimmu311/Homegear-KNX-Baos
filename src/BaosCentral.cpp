#include "BaosCentral.h"
#include "PhysicalInterfaces/Serial.h"
#include "Baos.h"
#include "GD.h"
#include "MakeDeviceDescriptionFile.h"

#include <iomanip>

namespace Baos
{

BaosCentral::BaosCentral(ICentralEventSink* eventHandler) : BaseLib::Systems::ICentral(BAOS_FAMILY_ID, GD::bl, eventHandler)
{
	init();
}

BaosCentral::BaosCentral(uint32_t deviceID, std::string serialNumber, int32_t address, ICentralEventSink* eventHandler) : BaseLib::Systems::ICentral(BAOS_FAMILY_ID, GD::bl, deviceID, serialNumber, address, eventHandler)
{
	init();
}

void BaosCentral::init()
{
  if(_initialized) return; //Prevent running init two times
  _initialized = true;
	_searching = false;

  for(auto& physicalInterface : GD::physicalInterfaces)
  {
    _physicalInterfaceEventhandlers[physicalInterface.first] = physicalInterface.second->addEventHandler((BaseLib::Systems::IPhysicalInterface::IPhysicalInterfaceEventSink*)this);
  }
}

BaosCentral::~BaosCentral()
{
	dispose();
}

void BaosCentral::dispose(bool wait)
{
	try
	{
		if(_disposing) return;
		_disposing = true;
		GD::out.printDebug("Removing device " + std::to_string(_deviceId) + " from physical device's event queue...");
    for(auto& physicalInterface : GD::physicalInterfaces)
    {
        //Just to make sure cycle through all physical devices. If event handler is not removed => segfault
        physicalInterface.second->removeEventHandler(_physicalInterfaceEventhandlers[physicalInterface.first]);
    }
	}
  catch(const std::exception& ex)
  {
    GD::out.printEx(__FILE__, __LINE__, __PRETTY_FUNCTION__, ex.what());
  }
}

bool BaosCentral::onPacketReceived(std::string& senderId, std::shared_ptr<BaseLib::Systems::Packet> packet)
{
  try
  {
    if(_disposing) return false;
    PBaosPacket baosPacket(std::dynamic_pointer_cast<BaosPacket>(packet));
    if(!baosPacket) return false;
		std::string serialNumber = baosPacket->getSerialNumber();
		if (serialNumber.empty()) return false;

    if(_bl->debugLevel >= 4) _bl->out.printInfo(BaseLib::HelperFunctions::getTimeString(baosPacket->getTimeReceived()) + " Baos packet received (" + senderId + "): " + BaseLib::HelperFunctions::getHexString(baosPacket->getRawData()));
		auto peer = getPeer(serialNumber);
    if(peer)
    {
      peer->packetReceived(baosPacket);
      return true;
    }
  }
  catch(const std::exception& ex)
  {
    GD::out.printEx(__FILE__, __LINE__, __PRETTY_FUNCTION__, ex.what());
  }
  return false;
}

void BaosCentral::loadPeers()
{
	try
	{
		std::shared_ptr<BaseLib::Database::DataTable> rows = _bl->db->getPeers(_deviceId);
		for(BaseLib::Database::DataTable::iterator row = rows->begin(); row != rows->end(); ++row)
		{
			int32_t peerId = row->second.at(0)->intValue;
			GD::out.printMessage("Loading peer " + std::to_string(peerId));
			size_t nodeId = row->second.at(2)->intValue;
			auto peer = std::make_shared<BaosPeer>(peerId, nodeId, row->second.at(3)->textValue, _deviceId, this);
			if(!peer->load(this)) continue;
			if(!peer->getRpcDevice()) continue;
			std::lock_guard<std::mutex> peersGuard(_peersMutex);
			if(!peer->getSerialNumber().empty()) _peersBySerial[peer->getSerialNumber()] = peer;
			_peersById[peerId] = peer;
		}
	}
	catch(const std::exception& ex)
  {
    GD::out.printEx(__FILE__, __LINE__, __PRETTY_FUNCTION__, ex.what());
  }
}

void BaosCentral::savePeers(bool full)
{
	try
	{
		std::lock_guard<std::mutex> peersGuard(_peersMutex);
		for(auto i = _peersById.begin(); i != _peersById.end(); ++i)
		{
			//Necessary, because peers can be assigned to multiple virtual devices
			if(i->second->getParentID() != _deviceId) continue;
			//We are always printing this, because the init script needs it
			GD::out.printMessage("(Shutdown) => Saving peer " + std::to_string(i->second->getID()));
			i->second->save(full, full, full);
		}
	}
	catch(const std::exception& ex)
  {
    GD::out.printEx(__FILE__, __LINE__, __PRETTY_FUNCTION__, ex.what());
  }
}

std::shared_ptr<BaosPeer> BaosCentral::getPeer(uint64_t id)
{
	try
	{
		std::lock_guard<std::mutex> peersGuard(_peersMutex);
		auto peersByIdIterator = _peersById.find(id);
		if(peersByIdIterator != _peersById.end()) return std::dynamic_pointer_cast<BaosPeer>(peersByIdIterator->second);
	}
	catch(const std::exception& ex)
  {
    GD::out.printEx(__FILE__, __LINE__, __PRETTY_FUNCTION__, ex.what());
  }
  return std::shared_ptr<BaosPeer>();
}

std::shared_ptr<BaosPeer> BaosCentral::getPeer(const std::string& serialNumber)
{
	try
	{
		std::lock_guard<std::mutex> peersGuard(_peersMutex);
		auto peersBySerialIterator = _peersBySerial.find(serialNumber);
		if(peersBySerialIterator != _peersBySerial.end()) return std::dynamic_pointer_cast<BaosPeer>(peersBySerialIterator->second);
	}
	catch(const std::exception& ex)
  {
		GD::out.printEx(__FILE__, __LINE__, __PRETTY_FUNCTION__, ex.what());
  }
  return std::shared_ptr<BaosPeer>();
}

void BaosCentral::deletePeer(uint64_t id)
{
	try
	{
		std::shared_ptr<BaosPeer> peer(getPeer(id));
		if(!peer) return;

		peer->deleting = true;
		PVariable deviceAddresses(new Variable(VariableType::tArray));
		deviceAddresses->arrayValue->push_back(PVariable(new Variable(peer->getSerialNumber())));

		PVariable deviceInfo(new Variable(VariableType::tStruct));
		deviceInfo->structValue->insert(StructElement("ID", PVariable(new Variable((int32_t)peer->getID()))));
		PVariable channels(new Variable(VariableType::tArray));
		deviceInfo->structValue->insert(StructElement("CHANNELS", channels));

		std::shared_ptr<HomegearDevice> rpcDevice = peer->getRpcDevice();
		for(Functions::iterator i = rpcDevice->functions.begin(); i != rpcDevice->functions.end(); ++i)
		{
			deviceAddresses->arrayValue->push_back(PVariable(new Variable(peer->getSerialNumber() + ":" + std::to_string(i->first))));
			channels->arrayValue->push_back(PVariable(new Variable(i->first)));
		}

    std::vector<uint64_t> deletedIds{ id };
		raiseRPCDeleteDevices(deletedIds, deviceAddresses, deviceInfo);

		{
			std::lock_guard<std::mutex> peersGuard(_peersMutex);
			_peersBySerial.erase(peer->getSerialNumber());
			_peersById.erase(id);
		}

    int32_t i = 0;
    while(peer.use_count() > 1 && i < 600)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        i++;
    }
    if(i == 600) GD::out.printError("Error: Peer deletion took too long.");

		peer->deleteFromDatabase();
		GD::out.printMessage("Removed peer " + std::to_string(peer->getID()));
	}
	catch(const std::exception& ex)
    {
        GD::out.printEx(__FILE__, __LINE__, __PRETTY_FUNCTION__, ex.what());
    }
}

std::string BaosCentral::handleCliCommand(std::string command)
{
	try
	{
		std::ostringstream stringStream;
		if(command == "help" || command == "h")
		{
			stringStream << "List of commands (shortcut in brackets):" << std::endl << std::endl;
			stringStream << "For more information about the individual command type: COMMAND help" << std::endl << std::endl;
			stringStream << "peers list (ls)\t\tList all peers" << std::endl;
			stringStream << "peers remove (prm)\tRemove a peer (without unpairing)" << std::endl;
			stringStream << "peers select (ps)\tSelect a peer" << std::endl;
			stringStream << "peers setname (pn)\tName a peer" << std::endl;
			stringStream << "search (sp)\t\tSearches for new devices" << std::endl;
			stringStream << "unselect (u)\t\tUnselect this device" << std::endl;
			return stringStream.str();
		}
		if(command.compare(0, 12, "peers remove") == 0 || command.compare(0, 3, "prm") == 0)
		{
			uint64_t peerID = 0;

			std::stringstream stream(command);
			std::string element;
			int32_t offset = (command.at(1) == 'r') ? 0 : 1;
			int32_t index = 0;
			while(std::getline(stream, element, ' '))
			{
				if(index < 1 + offset)
				{
					index++;
					continue;
				}
				else if(index == 1 + offset)
				{
					if(element == "help") break;
					peerID = BaseLib::Math::getNumber(element, false);
					if(peerID == 0) return "Invalid id.\n";
				}
				index++;
			}
			if(index == 1 + offset)
			{
				stringStream << "Description: This command removes a peer without trying to unpair it first." << std::endl;
				stringStream << "Usage: peers remove PEERID" << std::endl << std::endl;
				stringStream << "Parameters:" << std::endl;
				stringStream << "  PEERID:\tThe id of the peer to remove. Example: 513" << std::endl;
				return stringStream.str();
			}

			if(!peerExists(peerID)) stringStream << "This peer is not paired to this central." << std::endl;
			else
			{
				deletePeer(peerID);
				stringStream << "Removed peer " << std::to_string(peerID) << "." << std::endl;
			}
			return stringStream.str();
		}
		else if(command.compare(0, 10, "peers list") == 0 || command.compare(0, 2, "pl") == 0 || command.compare(0, 2, "ls") == 0)
		{
			try
			{
				std::string filterType;
				std::string filterValue;

				std::stringstream stream(command);
				std::string element;
				int32_t offset = (command.at(1) == 'l' || command.at(1) == 's') ? 0 : 1;
				int32_t index = 0;
				while(std::getline(stream, element, ' '))
				{
					if(index < 1 + offset)
					{
						index++;
						continue;
					}
					else if(index == 1 + offset)
					{
						if(element == "help")
						{
							index = -1;
							break;
						}
						filterType = BaseLib::HelperFunctions::toLower(element);
					}
					else if(index == 2 + offset)
					{
						filterValue = element;
						if(filterType == "name") BaseLib::HelperFunctions::toLower(filterValue);
					}
					index++;
				}
				if(index == -1)
				{
					stringStream << "Description: This command lists information about all peers." << std::endl;
					stringStream << "Usage: peers list [FILTERTYPE] [FILTERVALUE]" << std::endl << std::endl;
					stringStream << "Parameters:" << std::endl;
					stringStream << "  FILTERTYPE:\tSee filter types below." << std::endl;
					stringStream << "  FILTERVALUE:\tDepends on the filter type. If a number is required, it has to be in hexadecimal format." << std::endl << std::endl;
					stringStream << "Filter types:" << std::endl;
					stringStream << "  ID: Filter by id." << std::endl;
					stringStream << "      FILTERVALUE: The id of the peer to filter (e. g. 513)." << std::endl;
					stringStream << "  SERIAL: Filter by serial number." << std::endl;
					stringStream << "      FILTERVALUE: The serial number of the peer to filter (e. g. JEQ0554309)." << std::endl;
					stringStream << "  NAME: Filter by name." << std::endl;
					stringStream << "      FILTERVALUE: The part of the name to search for (e. g. \"1st floor\")." << std::endl;
					stringStream << "  TYPE: Filter by device type." << std::endl;
					stringStream << "      FILTERVALUE: The 2 byte device type in hexadecimal format." << std::endl;
					return stringStream.str();
				}

				if(_peersById.empty())
				{
					stringStream << "No peers are paired to this central." << std::endl;
					return stringStream.str();
				}
				std::string bar(" │ ");
				const int32_t idWidth = 11;
				const int32_t nameWidth = 25;
				const int32_t serialWidth = 17;
				const int32_t typeWidth1 = 4;
				const int32_t typeWidth2 = 25;
				std::string nameHeader("Name");
				nameHeader.resize(nameWidth, ' ');
				std::string typeStringHeader("Type String");
				typeStringHeader.resize(typeWidth2, ' ');
				stringStream << std::setfill(' ')
					<< std::setw(idWidth) << "ID" << bar
					<< nameHeader << bar
					<< std::setw(serialWidth) << "Serial Number" << bar
					<< std::setw(typeWidth1) << "Type" << bar
					<< typeStringHeader << bar
					<< std::endl;
				stringStream << "────────────┼───────────────────────────┼───────────────────┼──────┼───────────────────────────" << std::endl;
				stringStream << std::setfill(' ')
					<< std::setw(idWidth) << " " << bar
					<< std::setw(nameWidth) << " " << bar
					<< std::setw(serialWidth) << " " << bar
					<< std::setw(typeWidth1) << " " << bar
					<< std::setw(typeWidth2) << " " << bar
					<< std::endl;
				_peersMutex.lock();
				for(std::map<uint64_t, std::shared_ptr<BaseLib::Systems::Peer>>::iterator i = _peersById.begin(); i != _peersById.end(); ++i)
				{
					std::shared_ptr<BaosPeer> peer(std::dynamic_pointer_cast<BaosPeer>(i->second));
					if(filterType == "id")
					{
						uint64_t id = BaseLib::Math::getNumber(filterValue, false);
						if(i->second->getID() != id) continue;
					}
					else if(filterType == "name")
					{
						std::string name = i->second->getName();
						if((signed)BaseLib::HelperFunctions::toLower(name).find(filterValue) == (signed)std::string::npos) continue;
					}
					else if(filterType == "serial")
					{
						if(i->second->getSerialNumber() != filterValue) continue;
					}
					else if(filterType == "type")
					{
						int32_t deviceType = BaseLib::Math::getNumber(filterValue, true);
						if((int32_t)i->second->getDeviceType() != deviceType) continue;
					}

					uint64_t currentID = i->second->getID();
					std::string idString = (currentID > 999999) ? "0x" + BaseLib::HelperFunctions::getHexString(currentID, 8) : std::to_string(currentID);
					stringStream << std::setw(idWidth) << std::setfill(' ') << idString << bar;
					std::string name = i->second->getName();
					size_t nameSize = BaseLib::HelperFunctions::utf8StringSize(name);
					if(nameSize > (unsigned)nameWidth)
					{
						name = BaseLib::HelperFunctions::utf8Substring(name, 0, nameWidth - 3);
						name += "...";
					}
					else name.resize(nameWidth + (name.size() - nameSize), ' ');
					stringStream << name << bar
						<< std::setw(serialWidth) << i->second->getSerialNumber() << bar
						<< std::setw(typeWidth1) << BaseLib::HelperFunctions::getHexString(i->second->getDeviceType(), 4) << bar;
					if (i->second->getRpcDevice())
					{
						PSupportedDevice type = i->second->getRpcDevice()->getType(i->second->getDeviceType(), i->second->getFirmwareVersion());
						std::string typeID;
						if (type) typeID = type->id;
						if (typeID.size() > (unsigned)typeWidth2)
						{
							typeID.resize(typeWidth2 - 3);
							typeID += "...";
						}
						else typeID.resize(typeWidth2, ' ');
						stringStream << typeID << bar;
					}
					else stringStream << std::setw(typeWidth2) << " " << bar;
					stringStream << std::endl << std::dec;
				}
				_peersMutex.unlock();
				stringStream << "────────────┴───────────────────────────┴───────────────────┴──────┴───────────────────────────" << std::endl;
				
				return stringStream.str();
			}
			catch(const std::exception& ex)
			{
				_peersMutex.unlock();
				GD::out.printEx(__FILE__, __LINE__, __PRETTY_FUNCTION__, ex.what());
			}
		}
		else if(command.compare(0, 13, "peers setname") == 0 || command.compare(0, 2, "pn") == 0)
		{
			uint64_t peerID = 0;
			std::string name;

			std::stringstream stream(command);
			std::string element;
			int32_t offset = (command.at(1) == 'n') ? 0 : 1;
			int32_t index = 0;
			while(std::getline(stream, element, ' '))
			{
				if(index < 1 + offset)
				{
					index++;
					continue;
				}
				else if(index == 1 + offset)
				{
					if(element == "help") break;
					else
					{
						peerID = BaseLib::Math::getNumber(element, false);
						if(peerID == 0) return "Invalid id.\n";
					}
				}
				else if(index == 2 + offset) name = element;
				else name += ' ' + element;
				index++;
			}
			if(index == 1 + offset)
			{
				stringStream << "Description: This command sets or changes the name of a peer to identify it more easily." << std::endl;
				stringStream << "Usage: peers setname PEERID NAME" << std::endl << std::endl;
				stringStream << "Parameters:" << std::endl;
				stringStream << "  PEERID:\tThe id of the peer to set the name for. Example: 513" << std::endl;
				stringStream << "  NAME:\tThe name to set. Example: \"1st floor light switch\"." << std::endl;
				return stringStream.str();
			}

			if(!peerExists(peerID)) stringStream << "This peer is not paired to this central." << std::endl;
			else
			{
				std::shared_ptr<BaosPeer> peer = getPeer(peerID);
				peer->setName(name);
				stringStream << "Name set to \"" << name << "\"." << std::endl;
			}
			return stringStream.str();
		}
		else if(command.compare(0, 6, "search") == 0 || command.compare(0, 2, "sp") == 0)
		{
			std::stringstream stream(command);
			std::string element;
			int32_t offset = (command.at(1) == 'p') ? 0 : 1;
			int32_t index = 0;
			while(std::getline(stream, element, ' '))
			{
				if(index < 1 + offset)
				{
					index++;
					continue;
				}
				else if(index == 1 + offset)
				{
					if(element == "help")
					{
						stringStream << "Description: This command searches for new devices." << std::endl;
						stringStream << "Usage: search" << std::endl << std::endl;
						stringStream << "Parameters:" << std::endl;
						stringStream << "  There are no parameters." << std::endl;
						return stringStream.str();
					}
				}
				index++;
			}

			auto result = searchDevices(nullptr, "");
			stringStream << "Search completed. Found " << result->integerValue64 << " new peers." << std::endl;
			return stringStream.str();
		}
		else return "Unknown command.\n";
	}
	catch(const std::exception& ex)
    {
        GD::out.printEx(__FILE__, __LINE__, __PRETTY_FUNCTION__, ex.what());
    }
    return "Error executing command. See log file for more details.\n";
}

std::shared_ptr<BaosPeer> BaosCentral::createPeer(uint32_t deviceType, const std::string& serialNumber, std::shared_ptr<Serial> interface, bool save)
{
	try
	{
		std::shared_ptr<BaosPeer> peer(new BaosPeer(_deviceId, this));
		peer->setDeviceType(deviceType);
		peer->setSerialNumber(serialNumber);
		peer->setRpcDevice(GD::family->getRpcDevices()->find(deviceType, 0, -1));
		if(!peer->getRpcDevice()) return std::shared_ptr<BaosPeer>();
		if(save) peer->save(true, true, false); //Save and create peerID
		peer->setPhysicalInterfaceId(interface->getID());
		return peer;
	}
  catch(const std::exception& ex)
  {
    GD::out.printEx(__FILE__, __LINE__, __PRETTY_FUNCTION__, ex.what());
  }
  return std::shared_ptr<BaosPeer>();
}

void BaosCentral::homegearShuttingDown()
{
}

//RPC functions
PVariable BaosCentral::deleteDevice(BaseLib::PRpcClientInfo clientInfo, std::string serialNumber, int32_t flags)
{
	try
	{
		if(serialNumber.empty()) return Variable::createError(-2, "Unknown device.");

    uint64_t peerId = 0;

    {
        std::shared_ptr<BaosPeer> peer = getPeer(serialNumber);
        if(!peer) return Variable::createError(-2, "Unknown device.");
        peerId = peer->getID();
    }

		return deleteDevice(clientInfo, peerId, flags);
	}
	catch(const std::exception& ex)
  {
      GD::out.printEx(__FILE__, __LINE__, __PRETTY_FUNCTION__, ex.what());
  }
  return Variable::createError(-32500, "Unknown application error.");
}

PVariable BaosCentral::deleteDevice(BaseLib::PRpcClientInfo clientInfo, uint64_t peerId, int32_t flags)
{
	try
	{
		if(peerId == 0) return Variable::createError(-2, "Unknown device.");
		if(peerId >= 0x40000000) return Variable::createError(-2, "Cannot delete virtual device.");

    {
        std::shared_ptr<BaosPeer> peer = getPeer(peerId);
        if(!peer) return Variable::createError(-2, "Unknown device.");
    }

		deletePeer(peerId);

		return PVariable(new Variable(VariableType::tVoid));
	}
	catch(const std::exception& ex)
    {
        GD::out.printEx(__FILE__, __LINE__, __PRETTY_FUNCTION__, ex.what());
    }
    return Variable::createError(-32500, "Unknown application error.");
}

PVariable BaosCentral::searchDevices(BaseLib::PRpcClientInfo clientInfo, const std::string& interfaceId)
{
	try
	{
    std::lock_guard<std::mutex> searchDevicesGuard(_searchDevicesMutex);
    std::unordered_set<std::shared_ptr<BaosPeer>> newPeers;
    for(auto& interface : GD::physicalInterfaces)
    {
      if(!interfaceId.empty() && interface.first != interfaceId) continue;
			auto descriptions = interface.second->searchPeers();
			auto serialNumber = interface.second->getSerialNumber();
			MakeDeviceDescription makeDeviceDescription(descriptions, serialNumber);			
			GD::family->reloadRpcDevices();

			if (_peersBySerial.find(serialNumber) == _peersBySerial.end()) {

				std::shared_ptr<BaosPeer> peer = createPeer(1, serialNumber, interface.second, true);
				if (!peer || !peer->getRpcDevice()) {
					GD::out.printError("Error: Could not add device with type " + BaseLib::HelperFunctions::getHexString(1) + ". No matching XML file was found.");
					continue;
				}
				peer->setName("KNX Baos " + serialNumber);

				std::lock_guard<std::mutex> peersGuard(_peersMutex);
				_peersBySerial[peer->getSerialNumber()] = peer;
				_peersById[peer->getID()] = peer;

				peer->initializeCentralConfig();
				GD::out.printMessage("Added peer " + std::to_string(peer->getID()) + ".");
				newPeers.emplace(std::move(peer));
			}
			else {
				_peersBySerial[serialNumber]->setRpcDevice(GD::family->getRpcDevices()->find(1, 0, -1));
				_peersBySerial[serialNumber]->initializeCentralConfig();
				raiseRPCUpdateDevice(_peersBySerial[serialNumber]->getID(), 0, _peersBySerial[serialNumber]->getSerialNumber(), 0);
			}
    }

    if(!newPeers.empty())
    {
      std::vector<uint64_t> newIds;
      newIds.reserve(newPeers.size());
      PVariable deviceDescriptions = std::make_shared<BaseLib::Variable>(BaseLib::VariableType::tArray);
      deviceDescriptions->arrayValue->reserve(100);
      for(auto& newPeer : newPeers)
      {
        std::shared_ptr<std::vector<PVariable>> descriptions = newPeer->getDeviceDescriptions(clientInfo, true, std::map<std::string, bool>());
        if(!descriptions) continue;
        newIds.push_back(newPeer->getID());
        for(auto& description : *descriptions)
        {
            if(deviceDescriptions->arrayValue->size() + 1 > deviceDescriptions->arrayValue->capacity()) deviceDescriptions->arrayValue->reserve(deviceDescriptions->arrayValue->size() + 100);
            deviceDescriptions->arrayValue->push_back(description);
        }

        {
            auto pairingState = std::make_shared<PairingState>();
            pairingState->peerId = newPeer->getID();
            pairingState->state = "success";
            std::lock_guard<std::mutex> newPeersGuard(_newPeersMutex);
            _newPeers[BaseLib::HelperFunctions::getTime()].emplace_back(std::move(pairingState));
        }
      }
        raiseRPCNewDevices(newIds, deviceDescriptions);
    }
    return std::make_shared<BaseLib::Variable>(newPeers.size());
	}
	catch(const std::exception& ex)
	{
		GD::out.printEx(__FILE__, __LINE__, __PRETTY_FUNCTION__, ex.what());
	}
	return Variable::createError(-32500, "Unknown application error.");
}
//End RPC functions
}
