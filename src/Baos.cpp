#include <memory>
#include "Baos.h"
#include "Interfaces.h"
#include "BaosCentral.h"
#include "GD.h"

#include <iomanip>

namespace Baos
{

Baos::Baos(BaseLib::SharedObjects* bl, BaseLib::Systems::IFamilyEventSink* eventHandler) : BaseLib::Systems::DeviceFamily(bl, eventHandler, BAOS_FAMILY_ID, BAOS_FAMILY_NAME)
{
	GD::bl = _bl;
	GD::family = this;
	GD::out.init(bl);
	GD::out.setPrefix("Module Knx Baos: ");
	GD::out.printDebug("Debug: Loading module...");
	_physicalInterfaces.reset(new Interfaces(bl, _settings->getPhysicalInterfaceSettings()));
}

bool Baos::init() {
	_bl->out.printInfo("Loading XML RPC devices...");
	BaseLib::Io io;
	io.init(_bl);
	std::string xmlPath = _bl->settings.familyDataPath() + std::to_string(GD::family->getFamily()) + "/desc/";
	if (io.directoryExists(xmlPath) && !io.getFiles(xmlPath).empty()) _rpcDevices->load(xmlPath);
	return true;
}

void Baos::reloadRpcDevices() {
	_bl->out.printInfo("Reloading XML RPC devices...");
	BaseLib::Io io;
	io.init(_bl);
	std::string xmlPath = _bl->settings.familyDataPath() + std::to_string(GD::family->getFamily()) + "/desc/";
	if (io.directoryExists(xmlPath) && !io.getFiles(xmlPath).empty()) _rpcDevices->load(xmlPath);
}

void Baos::dispose()
{
	if(_disposed) return;
	DeviceFamily::dispose();

	_central.reset();
}

std::shared_ptr<BaseLib::Systems::ICentral> Baos::initializeCentral(uint32_t deviceId, int32_t address, std::string serialNumber)
{
	return std::make_shared<BaosCentral>(deviceId, serialNumber, 1, this);
}

void Baos::createCentral()
{
	try
	{
		if(_central) return;

		int32_t seedNumber = BaseLib::HelperFunctions::getRandomNumber(1, 999999);
		std::ostringstream stringstream;
		stringstream << "BAOS" << std::setw(6) << std::setfill('0') << std::dec << seedNumber;
		std::string serialNumber(stringstream.str());

		_central.reset(new BaosCentral(0, serialNumber, 1, this));
		GD::out.printMessage("Created central with id " + std::to_string(_central->getId()) + ", address 0x" + BaseLib::HelperFunctions::getHexString(1, 6) + " and serial number " + serialNumber);
	}
	catch(const std::exception& ex)
    {
    	GD::out.printEx(__FILE__, __LINE__, __PRETTY_FUNCTION__, ex.what());
    }
}

PVariable Baos::getPairingInfo()
{
	try
	{
		if(!_central) return std::make_shared<BaseLib::Variable>(BaseLib::VariableType::tStruct);
		PVariable info = std::make_shared<BaseLib::Variable>(BaseLib::VariableType::tStruct);

		//{{{ General
		info->structValue->emplace("searchInterfaces", std::make_shared<BaseLib::Variable>(false));
		//}}}

		//{{{ Family settings
		PVariable familySettings = std::make_shared<BaseLib::Variable>(BaseLib::VariableType::tStruct);
		info->structValue->emplace("familySettings", familySettings);
		//}}}

		//{{{ Pairing methods
		PVariable pairingMethods = std::make_shared<BaseLib::Variable>(BaseLib::VariableType::tStruct);
		pairingMethods->structValue->emplace("searchDevices", std::make_shared<BaseLib::Variable>(BaseLib::VariableType::tStruct));
		info->structValue->emplace("pairingMethods", pairingMethods);
		//}}}

		//{{{ interfaces
		PVariable interfaces = std::make_shared<BaseLib::Variable>(BaseLib::VariableType::tStruct);

		PVariable interface = std::make_shared<BaseLib::Variable>(BaseLib::VariableType::tStruct);
		interface->structValue->emplace("name", std::make_shared<BaseLib::Variable>(std::string("serial")));
		interface->structValue->emplace("ipDevice", std::make_shared<BaseLib::Variable>(false));

		auto field = std::make_shared<BaseLib::Variable>(BaseLib::VariableType::tStruct);
		field->structValue->emplace("pos", std::make_shared<BaseLib::Variable>(0));
		field->structValue->emplace("label", std::make_shared<BaseLib::Variable>(std::string("l10n.knxBaos.pairingInfo.id")));
		field->structValue->emplace("type", std::make_shared<BaseLib::Variable>(std::string("string")));
		interface->structValue->emplace("id", field);

		field = std::make_shared<BaseLib::Variable>(BaseLib::VariableType::tStruct);
		field->structValue->emplace("label", std::make_shared<BaseLib::Variable>(std::string("l10n.knxBaos.pairingInfo.device")));
		field->structValue->emplace("type", std::make_shared<BaseLib::Variable>(std::string("string")));
		interface->structValue->emplace("device", field);

		interfaces->structValue->emplace("knxbaos", interface);

		info->structValue->emplace("interfaces", interfaces);
		//}}}

		return info;
	}
	catch(const std::exception& ex)
	{
		GD::out.printEx(__FILE__, __LINE__, __PRETTY_FUNCTION__, ex.what());
	}
	return Variable::createError(-32500, "Unknown application error.");
}
}
