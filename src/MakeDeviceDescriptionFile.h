#ifndef MakeDeviceDescriptionFile_H
#define MakeDeviceDescriptionFile_H

#include <kdrive/baos/Baos.h>
#include <sys/stat.h>
#include <homegear-base/BaseLib.h>
#include "GD.h"

namespace Baos
{

class MakeDeviceDescription
{
public:
	MakeDeviceDescription(const kdrive::baos::BaosDatapointDescriptions::Descriptions descriptions, const std::string serialNumber);
private:
	void createDirectories();
	std::string _xmlPath;
	
	void makeParameter(BaseLib::DeviceDescription::PFunction function, std::string id, bool readable, bool writable, std::shared_ptr<BaseLib::DeviceDescription::ILogical> logical, std::shared_ptr<BaseLib::DeviceDescription::IPhysical> physical, std::string packetId);
};

}
#endif