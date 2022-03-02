#ifndef BAOSPEER_H_
#define BAOSPEER_H_

#include "BaosPacket.h"

#include <homegear-base/BaseLib.h>

#include <list>

using namespace BaseLib;
using namespace BaseLib::DeviceDescription;
using namespace BaseLib::Systems;

namespace Baos
{
class BaosCentral;
class Serial;

class BaosPeer : public BaseLib::Systems::Peer
{
public:
	BaosPeer(uint32_t parentID, IPeerEventSink* eventHandler);
	BaosPeer(int32_t id, int32_t address, std::string serialNumber, uint32_t parentID, IPeerEventSink* eventHandler);
	virtual ~BaosPeer();

	//Features
	virtual bool wireless() { return false; }
	//End features

	//{{{ In table variables
	std::string getPhysicalInterfaceId() { return _physicalInterfaceId; }
	void setPhysicalInterfaceId(std::string);
	//}}}
	std::shared_ptr<Serial>& getPhysicalInterface() { return _physicalInterface; }

	virtual std::string handleCliCommand(std::string command);

	virtual bool load(BaseLib::Systems::ICentral* central);
	virtual void save(bool savePeer, bool saveVariables, bool saveCentralConfig);
	virtual void savePeers() {};
	virtual int32_t getChannelGroupedWith(int32_t channel) { return -1; }
	virtual int32_t getNewFirmwareVersion() { return 0; }
	virtual std::string getFirmwareVersionString(int32_t firmwareVersion) { return "1.0"; }
	virtual bool firmwareUpdateAvailable() { return false; }
	
	void packetReceived(std::shared_ptr<BaosPacket> packet);

	//RPC methods
	/**
	 * {@inheritDoc}
	 */
  virtual PVariable getDeviceInfo(BaseLib::PRpcClientInfo clientInfo, std::map<std::string, bool> fields);

  /**
	* {@inheritDoc}
	*/
	virtual PVariable getParamsetDescription(BaseLib::PRpcClientInfo clientInfo, int32_t channel, ParameterGroup::Type::Enum type, uint64_t remoteID, int32_t remoteChannel, bool checkAcls);

	/**
	 * {@inheritDoc}
	 */
	virtual PVariable getParamset(BaseLib::PRpcClientInfo clientInfo, int32_t channel, ParameterGroup::Type::Enum type, uint64_t remoteID, int32_t remoteChannel, bool checkAcls);

	/**
	 * {@inheritDoc}
	 */
	virtual PVariable putParamset(BaseLib::PRpcClientInfo clientInfo, int32_t channel, ParameterGroup::Type::Enum type, uint64_t remoteID, int32_t remoteChannel, PVariable variables, bool checkAcls, bool onlyPushing = false);

	/**
	 * {@inheritDoc}
	 */
	virtual PVariable setValue(BaseLib::PRpcClientInfo clientInfo, uint32_t channel, std::string valueKey, PVariable value, bool wait);
	//End RPC methods
protected:
  struct FrameValue
  {
    std::list<uint32_t> channels;
    std::vector<uint8_t> value;
  };

  struct FrameValues
  {
    std::string frameID;
    std::list<uint32_t> paramsetChannels;
    ParameterGroup::Type::Enum parameterSetType;
    std::map<std::string, FrameValue> values;
  };

	//In table variables:
	std::string _physicalInterfaceId;
	//End

	std::shared_ptr<BaseLib::Rpc::RpcEncoder> _binaryEncoder;
	std::shared_ptr<BaseLib::Rpc::RpcDecoder> _binaryDecoder;

	std::shared_ptr<Serial> _physicalInterface;

	virtual void setPhysicalInterface(std::shared_ptr<Serial> interface);

	virtual std::shared_ptr<BaseLib::Systems::ICentral> getCentral();

	void getValuesFromPacket(PBaosPacket packet, std::vector<FrameValues>& frameValue);

	virtual PParameterGroup getParameterSet(int32_t channel, ParameterGroup::Type::Enum type);

	virtual void loadVariables(BaseLib::Systems::ICentral* central, std::shared_ptr<BaseLib::Database::DataTable>& rows);
	virtual void saveVariables();

	std::string printConfig();

};

}

#endif
