#include "Factory.h"
#include "../config.h"
#include "GD.h"

BaseLib::Systems::DeviceFamily* BaosFactory::createDeviceFamily(BaseLib::SharedObjects* bl, BaseLib::Systems::IFamilyEventSink* eventHandler)
{
	return new Baos::Baos(bl, eventHandler);
}

std::string getVersion()
{
	return VERSION;
}

int32_t getFamilyId()
{
	return BAOS_FAMILY_ID;
}

std::string getFamilyName()
{
	return BAOS_FAMILY_NAME;
}

BaseLib::Systems::SystemFactory* getFactory()
{
	return (BaseLib::Systems::SystemFactory*)(new BaosFactory);
}
