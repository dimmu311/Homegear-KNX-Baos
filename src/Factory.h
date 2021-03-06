#ifndef FACTORY_H
#define FACTORY_H

#include <homegear-base/BaseLib.h>
#include "Baos.h"

class BaosFactory : BaseLib::Systems::SystemFactory
{
public:
	virtual BaseLib::Systems::DeviceFamily* createDeviceFamily(BaseLib::SharedObjects* bl, BaseLib::Systems::IFamilyEventSink* eventHandler);
};

extern "C" std::string getVersion();
extern "C" int32_t getFamilyId();
extern "C" std::string getFamilyName();
extern "C" BaseLib::Systems::SystemFactory* getFactory();

#endif
