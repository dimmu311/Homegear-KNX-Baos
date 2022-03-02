#ifndef BAOSCENTRAL_H_
#define BAOSCENTRAL_H_

#include <homegear-base/BaseLib.h>
#include "BaosPeer.h"
#include "BaosPacket.h"

#include <memory>
#include <mutex>
#include <string>

namespace Baos
{

class Serial;

class BaosCentral : public BaseLib::Systems::ICentral
{
public:
	//In table variables
	int32_t getFirmwareVersion() { return _firmwareVersion; }
	void setFirmwareVersion(int32_t value) { _firmwareVersion = value; saveVariable(0, value); }
	//End

	BaosCentral(ICentralEventSink* eventHandler);
	BaosCentral(uint32_t deviceType, std::string serialNumber, int32_t address, ICentralEventSink* eventHandler);
	virtual ~BaosCentral();
	virtual void dispose(bool wait = true);

	virtual void loadVariables() {};
	virtual void saveVariables() {};
	virtual void loadPeers();
	virtual void savePeers(bool full);

	virtual void homegearShuttingDown();

	virtual bool onPacketReceived(std::string& senderId, std::shared_ptr<BaseLib::Systems::Packet> packet);
	virtual std::string handleCliCommand(std::string command);

	std::shared_ptr<BaosPeer> getPeer(uint64_t id);
	std::shared_ptr<BaosPeer> getPeer(const std::string& serialNumber);

	virtual PVariable deleteDevice(BaseLib::PRpcClientInfo clientInfo, std::string serialNumber, int32_t flags);
	virtual PVariable deleteDevice(BaseLib::PRpcClientInfo clientInfo, uint64_t peerID, int32_t flags);
  virtual PVariable searchDevices(BaseLib::PRpcClientInfo clientInfo, const std::string& interfaceId);
protected:
	//In table variables
	int32_t _firmwareVersion = 0;
	//End

	std::atomic_bool _searching{false};

	std::mutex _searchDevicesMutex;

	//std::unordered_map<std::string, std::unordered_map<size_t, std::shared_ptr<BaosPeer>>> _peersByInterface;

	/**
	 * Creates a new peer. The method does not add the peer to the peer arrays.
	 *
	 * @param deviceType The device type.
	 * @param serialNumber The serial number.
	 * @param save (default true) Set to "true" to save the peer in the database.
	 * @return Returns a pointer to the newly created peer on success. If the creation was not successful, a nullptr is returned.
	 */
	std::shared_ptr<BaosPeer> createPeer(uint32_t deviceType, const std::string& serialNumber, std::shared_ptr<Serial> interface, bool save = true);
	void deletePeer(uint64_t id);

	void init();
};

}

#endif
