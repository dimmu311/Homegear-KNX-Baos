#include "Interfaces.h"
#include "GD.h"
#include "PhysicalInterfaces/Serial.h"
#include "Baos.h"

namespace Baos
{

Interfaces::Interfaces(BaseLib::SharedObjects* bl, std::map<std::string, Systems::PPhysicalInterfaceSettings> physicalInterfaceSettings) : Systems::PhysicalInterfaces(bl, GD::family->getFamily(), physicalInterfaceSettings)
{
	create();
}

void Interfaces::create(){
	try{
		for(auto settings : _physicalInterfaceSettings){
            std::shared_ptr<Serial> device;
            if (!settings.second || settings.second->device.empty()) {
                GD::out.printDebug("Debug: Something is wrong in knxBaos.conf.");
                continue;
            }
            GD::out.printDebug("Debug: Creating physical device. Type defined in knxBaos.conf is: " + settings.second->type);
            if(settings.second->type == "serial") device.reset(new Serial(settings.second));
            else GD::out.printError("Error: Unsupported physical device type: " + settings.second->type);
            if(device){
                if(_physicalInterfaces.find(settings.second->id) != _physicalInterfaces.end()) GD::out.printError("Error: id used for two devices: " + settings.second->id);
                _physicalInterfaces[settings.second->id] = device;
                GD::physicalInterfaces[settings.second->id] = device;
                }
		}
    }catch(const std::exception& ex){
		GD::out.printEx(__FILE__, __LINE__, __PRETTY_FUNCTION__, ex.what());
	}
}
}
