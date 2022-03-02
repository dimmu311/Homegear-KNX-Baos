#include "GD.h"
#include "PhysicalInterfaces/Serial.h"
#include "Baos.h"

namespace Baos
{
	BaseLib::SharedObjects* GD::bl = nullptr;
	Baos* GD::family = nullptr;
	BaseLib::Output GD::out;
    std::map<std::string, std::shared_ptr<Serial>> GD::physicalInterfaces;
}
