#ifndef GD_H_
#define GD_H_

#define BAOS_FAMILY_ID 67
#define BAOS_FAMILY_NAME "KNX Baos"

#include <homegear-base/BaseLib.h>
#include "PhysicalInterfaces/Serial.h"
#include "Baos.h"

using namespace BaseLib;
using namespace BaseLib::DeviceDescription;

namespace Baos
{

//class Baos;
//class Serial;

class GD
{
public:
	virtual ~GD();

	static BaseLib::SharedObjects* bl;
	static Baos* family;
	static BaseLib::Output out;
    static std::map<std::string, std::shared_ptr<Serial>> physicalInterfaces;
private:
	GD();
};

}

#endif
