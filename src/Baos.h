#ifndef BAOSSERIAL_H_
#define BAOSSERIAL_H_

#include <homegear-base/BaseLib.h>

using namespace BaseLib;

namespace Baos
{
class BaosCentral;

class Baos : public BaseLib::Systems::DeviceFamily
{
public:
	Baos(BaseLib::SharedObjects* bl, BaseLib::Systems::IFamilyEventSink* eventHandler);
    Baos(const Baos&) = delete;
    Baos& operator=(const Baos&) = delete;
	bool init() override;
	virtual ~Baos() = default;
	virtual void dispose();

	virtual bool hasPhysicalInterface() { return false; }
	virtual PVariable getPairingInfo();
	void reloadRpcDevices();
protected:
	virtual void createCentral();
	virtual std::shared_ptr<BaseLib::Systems::ICentral> initializeCentral(uint32_t deviceId, int32_t address, std::string serialNumber);
};

}

#endif
