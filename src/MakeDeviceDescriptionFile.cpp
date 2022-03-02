#include"MakeDeviceDescriptionFile.h"


namespace Baos
{
	MakeDeviceDescription::MakeDeviceDescription(const kdrive::baos::BaosDatapointDescriptions::Descriptions descriptions, const std::string serialNumber) {	
		try {
			createDirectories();

			std::shared_ptr<HomegearDevice> device = std::make_shared<HomegearDevice>(GD::bl);
			device->version = 1;
			PSupportedDevice supportedDevice = std::make_shared<SupportedDevice>(GD::bl);
			supportedDevice->id = serialNumber;
			supportedDevice->description = "Baos Device";
			supportedDevice->typeNumber = 1;
			device->supportedDevices.push_back(supportedDevice);

			PJsonPayload payloadRaw = std::make_shared<JsonPayload>(GD::bl);
			payloadRaw->key = "RAW";
			payloadRaw->parameterId = "RAW";

			{
				PPacket devicePacket = std::make_shared<Packet>(GD::bl);
				devicePacket->id = "ON.DPT_1";
				device->packetsById[devicePacket->id] = devicePacket;
				devicePacket->function1 = "onDpt1Packet";
				devicePacket->direction = Packet::Direction::Enum::toCentral;
				devicePacket->channel = -1;
				devicePacket->jsonPayloads.push_back(payloadRaw);
				{
					PJsonPayload payload = std::make_shared<JsonPayload>(GD::bl);
					devicePacket->jsonPayloads.push_back(payload);
					payload->key = "DPT_1:STATE";
					payload->parameterId = "STATE";
				}			
			}
			{
				PPacket devicePacket = std::make_shared<Packet>(GD::bl);
				devicePacket->id = "SET.DPT_1";
				device->packetsById[devicePacket->id] = devicePacket;
				devicePacket->function1 = "setDpt1Packet";
				devicePacket->direction = Packet::Direction::Enum::fromCentral;
				devicePacket->channel = -1;
				{
					PJsonPayload payload = std::make_shared<JsonPayload>(GD::bl);
					devicePacket->jsonPayloads.push_back(payload);
					payload->key = "DPT_1:STATE";
					payload->parameterId = "STATE";
				}
			}

			{
				PPacket devicePacket = std::make_shared<Packet>(GD::bl);
				devicePacket->id = "ON.DPT_2";
				device->packetsById[devicePacket->id] = devicePacket;
				devicePacket->function1 = "onDpt2Packet";
				devicePacket->direction = Packet::Direction::Enum::toCentral;
				devicePacket->channel = -1;
				devicePacket->jsonPayloads.push_back(payloadRaw);
				{
					PJsonPayload payload = std::make_shared<JsonPayload>(GD::bl);
					devicePacket->jsonPayloads.push_back(payload);
					payload->key = "DPT_2:STATE";
					payload->parameterId = "STATE";
				}
				{
					PJsonPayload payload = std::make_shared<JsonPayload>(GD::bl);
					devicePacket->jsonPayloads.push_back(payload);
					payload->key = "DPT_2:CONTROL";
					payload->parameterId = "CONTROL";
				}
			}
			{
				PPacket devicePacket = std::make_shared<Packet>(GD::bl);
				devicePacket->id = "SET.DPT_2";
				device->packetsById[devicePacket->id] = devicePacket;
				devicePacket->function1 = "setDpt2Packet";
				devicePacket->direction = Packet::Direction::Enum::fromCentral;
				devicePacket->channel = -1;
				{
					PJsonPayload payload = std::make_shared<JsonPayload>(GD::bl);
					devicePacket->jsonPayloads.push_back(payload);
					payload->key = "DPT_2:STATE";
					payload->parameterId = "STATE";
				}
				{
					PJsonPayload payload = std::make_shared<JsonPayload>(GD::bl);
					devicePacket->jsonPayloads.push_back(payload);
					payload->key = "DPT_2:CONTROL";
					payload->parameterId = "CONTROL";
				}
			}

			{
				PPacket devicePacket = std::make_shared<Packet>(GD::bl);
				devicePacket->id = "ON.DPT_3";
				device->packetsById[devicePacket->id] = devicePacket;
				devicePacket->function1 = "onDpt3Packet";
				devicePacket->direction = Packet::Direction::Enum::toCentral;
				devicePacket->channel = -1;
				devicePacket->jsonPayloads.push_back(payloadRaw);
				{
					PJsonPayload payload = std::make_shared<JsonPayload>(GD::bl);
					devicePacket->jsonPayloads.push_back(payload);
					payload->key = "DPT_3:STATE";
					payload->parameterId = "STATE";
				}
				{
					PJsonPayload payload = std::make_shared<JsonPayload>(GD::bl);
					devicePacket->jsonPayloads.push_back(payload);
					payload->key = "DPT_3:CONTROL";
					payload->parameterId = "CONTROL";
				}
			}
			{
				PPacket devicePacket = std::make_shared<Packet>(GD::bl);
				devicePacket->id = "SET.DPT_3";
				device->packetsById[devicePacket->id] = devicePacket;
				devicePacket->function1 = "setDpt3Packet";
				devicePacket->direction = Packet::Direction::Enum::fromCentral;
				devicePacket->channel = -1;
				{
					PJsonPayload payload = std::make_shared<JsonPayload>(GD::bl);
					devicePacket->jsonPayloads.push_back(payload);
					payload->key = "DPT_3:STATE";
					payload->parameterId = "STATE";
				}
				{
					PJsonPayload payload = std::make_shared<JsonPayload>(GD::bl);
					devicePacket->jsonPayloads.push_back(payload);
					payload->key = "DPT_3:CONTROL";
					payload->parameterId = "CONTROL";
				}
			}

			{
				PPacket devicePacket = std::make_shared<Packet>(GD::bl);
				devicePacket->id = "ON.DPT_4";
				device->packetsById[devicePacket->id] = devicePacket;
				devicePacket->function1 = "onDpt4Packet";
				devicePacket->direction = Packet::Direction::Enum::toCentral;
				devicePacket->channel = -1;
				devicePacket->jsonPayloads.push_back(payloadRaw);
				{
					PJsonPayload payload = std::make_shared<JsonPayload>(GD::bl);
					devicePacket->jsonPayloads.push_back(payload);
					payload->key = "DPT_4:CHARACTER_SET";
					payload->parameterId = "VALUE";
				}
			}
			{
				PPacket devicePacket = std::make_shared<Packet>(GD::bl);
				devicePacket->id = "SET.DPT_4";
				device->packetsById[devicePacket->id] = devicePacket;
				devicePacket->function1 = "setDpt4Packet";
				devicePacket->direction = Packet::Direction::Enum::fromCentral;
				devicePacket->channel = -1;
				{
					PJsonPayload payload = std::make_shared<JsonPayload>(GD::bl);
					devicePacket->jsonPayloads.push_back(payload);
					payload->key = "DPT_4:CHARACTER_SET";
					payload->parameterId = "VALUE";
				}
			}

			{
				PPacket devicePacket = std::make_shared<Packet>(GD::bl);
				devicePacket->id = "ON.DPT_5";
				device->packetsById[devicePacket->id] = devicePacket;
				devicePacket->function1 = "onDpt5Packet";
				devicePacket->direction = Packet::Direction::Enum::toCentral;
				devicePacket->channel = -1;
				devicePacket->jsonPayloads.push_back(payloadRaw);
				{
					PJsonPayload payload = std::make_shared<JsonPayload>(GD::bl);
					devicePacket->jsonPayloads.push_back(payload);
					payload->key = "DPT_5:UNSIGNED_VALUE";
					payload->parameterId = "VALUE";
				}
			}
			{
				PPacket devicePacket = std::make_shared<Packet>(GD::bl);
				devicePacket->id = "SET.DPT_5";
				device->packetsById[devicePacket->id] = devicePacket;
				devicePacket->function1 = "setDpt5Packet";
				devicePacket->direction = Packet::Direction::Enum::fromCentral;
				devicePacket->channel = -1;
				{
					PJsonPayload payload = std::make_shared<JsonPayload>(GD::bl);
					devicePacket->jsonPayloads.push_back(payload);
					payload->key = "DPT_5:UNSIGNED_VALUE";
					payload->parameterId = "VALUE";
				}
			}

			{
				PPacket devicePacket = std::make_shared<Packet>(GD::bl);
				devicePacket->id = "ON.DPT_6";
				device->packetsById[devicePacket->id] = devicePacket;
				devicePacket->function1 = "onDpt6Packet";
				devicePacket->direction = Packet::Direction::Enum::toCentral;
				devicePacket->channel = -1;
				devicePacket->jsonPayloads.push_back(payloadRaw);
				{
					PJsonPayload payload = std::make_shared<JsonPayload>(GD::bl);
					devicePacket->jsonPayloads.push_back(payload);
					payload->key = "DPT_6:SIGNED_VALUE";
					payload->parameterId = "VALUE";
				}
			}
			{
				PPacket devicePacket = std::make_shared<Packet>(GD::bl);
				devicePacket->id = "SET.DPT_6";
				device->packetsById[devicePacket->id] = devicePacket;
				devicePacket->function1 = "setDpt6Packet";
				devicePacket->direction = Packet::Direction::Enum::fromCentral;
				devicePacket->channel = -1;
				{
					PJsonPayload payload = std::make_shared<JsonPayload>(GD::bl);
					devicePacket->jsonPayloads.push_back(payload);
					payload->key = "DPT_6:SIGNED_VALUE";
					payload->parameterId = "VALUE";
				}
			}

			{
				PPacket devicePacket = std::make_shared<Packet>(GD::bl);
				devicePacket->id = "ON.DPT_7";
				device->packetsById[devicePacket->id] = devicePacket;
				devicePacket->function1 = "onDpt7Packet";
				devicePacket->direction = Packet::Direction::Enum::toCentral;
				devicePacket->channel = -1;
				devicePacket->jsonPayloads.push_back(payloadRaw);
				{
					PJsonPayload payload = std::make_shared<JsonPayload>(GD::bl);
					devicePacket->jsonPayloads.push_back(payload);
					payload->key = "DPT_7:UNSIGNED_VALUE";
					payload->parameterId = "VALUE";
				}
			}
			{
				PPacket devicePacket = std::make_shared<Packet>(GD::bl);
				devicePacket->id = "SET.DPT_7";
				device->packetsById[devicePacket->id] = devicePacket;
				devicePacket->function1 = "setDpt7Packet";
				devicePacket->direction = Packet::Direction::Enum::fromCentral;
				devicePacket->channel = -1;
				{
					PJsonPayload payload = std::make_shared<JsonPayload>(GD::bl);
					devicePacket->jsonPayloads.push_back(payload);
					payload->key = "DPT_7:UNSIGNED_VALUE";
					payload->parameterId = "VALUE";
				}
			}

			{
				PPacket devicePacket = std::make_shared<Packet>(GD::bl);
				devicePacket->id = "ON.DPT_8";
				device->packetsById[devicePacket->id] = devicePacket;
				devicePacket->function1 = "onDpt8Packet";
				devicePacket->direction = Packet::Direction::Enum::toCentral;
				devicePacket->channel = -1;
				devicePacket->jsonPayloads.push_back(payloadRaw);
				{
					PJsonPayload payload = std::make_shared<JsonPayload>(GD::bl);
					devicePacket->jsonPayloads.push_back(payload);
					payload->key = "DPT_8:SIGNED_VALUE";
					payload->parameterId = "VALUE";
				}
			}
			{
				PPacket devicePacket = std::make_shared<Packet>(GD::bl);
				devicePacket->id = "SET.DPT_8";
				device->packetsById[devicePacket->id] = devicePacket;
				devicePacket->function1 = "setDpt8Packet";
				devicePacket->direction = Packet::Direction::Enum::fromCentral;
				devicePacket->channel = -1;
				{
					PJsonPayload payload = std::make_shared<JsonPayload>(GD::bl);
					devicePacket->jsonPayloads.push_back(payload);
					payload->key = "DPT_8:SIGNED_VALUE";
					payload->parameterId = "VALUE";
				}
			}

			{
				PPacket devicePacket = std::make_shared<Packet>(GD::bl);
				devicePacket->id = "ON.DPT_9";
				device->packetsById[devicePacket->id] = devicePacket;
				devicePacket->function1 = "onDpt9Packet";
				devicePacket->direction = Packet::Direction::Enum::toCentral;
				devicePacket->channel = -1;
				devicePacket->jsonPayloads.push_back(payloadRaw);
				{
					PJsonPayload payload = std::make_shared<JsonPayload>(GD::bl);
					devicePacket->jsonPayloads.push_back(payload);
					payload->key = "DPT_9:FLOAT";
					payload->parameterId = "VALUE";
				}
				{
					PJsonPayload payload = std::make_shared<JsonPayload>(GD::bl);
					devicePacket->jsonPayloads.push_back(payload);
					payload->key = "DPT_9:FLOAT_MSB";
					payload->parameterId = "MSB";
				}
				{
					PJsonPayload payload = std::make_shared<JsonPayload>(GD::bl);
					devicePacket->jsonPayloads.push_back(payload);
					payload->key = "DPT_9:FLOAT_LSB";
					payload->parameterId = "LSB";
				}
			}
			{
				PPacket devicePacket = std::make_shared<Packet>(GD::bl);
				devicePacket->id = "SET.DPT_9";
				device->packetsById[devicePacket->id] = devicePacket;
				devicePacket->function1 = "setDpt9Packet";
				devicePacket->direction = Packet::Direction::Enum::fromCentral;
				devicePacket->channel = -1;
				{
					PJsonPayload payload = std::make_shared<JsonPayload>(GD::bl);
					devicePacket->jsonPayloads.push_back(payload);
					payload->key = "DPT_9:FLOAT";
					payload->parameterId = "VALUE";
				}
				{
					PJsonPayload payload = std::make_shared<JsonPayload>(GD::bl);
					devicePacket->jsonPayloads.push_back(payload);
					payload->key = "DPT_9:FLOAT_MSB";
					payload->parameterId = "MSB";
				}
				{
					PJsonPayload payload = std::make_shared<JsonPayload>(GD::bl);
					devicePacket->jsonPayloads.push_back(payload);
					payload->key = "DPT_9:FLOAT_LSB";
					payload->parameterId = "LSB";
				}
			}

			{
				PPacket devicePacket = std::make_shared<Packet>(GD::bl);
				devicePacket->id = "ON.DPT_10";
				device->packetsById[devicePacket->id] = devicePacket;
				devicePacket->function1 = "onDpt10Packet";
				devicePacket->direction = Packet::Direction::Enum::toCentral;
				devicePacket->channel = -1;
				devicePacket->jsonPayloads.push_back(payloadRaw);
				{
					PJsonPayload payload = std::make_shared<JsonPayload>(GD::bl);
					devicePacket->jsonPayloads.push_back(payload);
					payload->key = "DPT_10:TIME-DAY";
					payload->parameterId = "DAY_OF_WEEK";
				}
				{
					PJsonPayload payload = std::make_shared<JsonPayload>(GD::bl);
					devicePacket->jsonPayloads.push_back(payload);
					payload->key = "DPT_10:TIME-HOUR";
					payload->parameterId = "HOUR";
				}
				{
					PJsonPayload payload = std::make_shared<JsonPayload>(GD::bl);
					devicePacket->jsonPayloads.push_back(payload);
					payload->key = "DPT_10:TIME-MINUTE";
					payload->parameterId = "MINUTE";
				}
				{
					PJsonPayload payload = std::make_shared<JsonPayload>(GD::bl);
					devicePacket->jsonPayloads.push_back(payload);
					payload->key = "DPT_10:TIME-SECOND";
					payload->parameterId = "SECOND";
				}
			}
			{
				PPacket devicePacket = std::make_shared<Packet>(GD::bl);
				devicePacket->id = "SET.DPT_10";
				device->packetsById[devicePacket->id] = devicePacket;
				devicePacket->function1 = "setDpt10Packet";
				devicePacket->direction = Packet::Direction::Enum::fromCentral;
				devicePacket->channel = -1;
				{
					PJsonPayload payload = std::make_shared<JsonPayload>(GD::bl);
					devicePacket->jsonPayloads.push_back(payload);
					payload->key = "DPT_10:TIME-DAY";
					payload->parameterId = "DAY_OF_WEEK";
				}
				{
					PJsonPayload payload = std::make_shared<JsonPayload>(GD::bl);
					devicePacket->jsonPayloads.push_back(payload);
					payload->key = "DPT_10:TIME-HOUR";
					payload->parameterId = "HOUR";
				}
				{
					PJsonPayload payload = std::make_shared<JsonPayload>(GD::bl);
					devicePacket->jsonPayloads.push_back(payload);
					payload->key = "DPT_10:TIME-MINUTE";
					payload->parameterId = "MINUTE";
				}
				{
					PJsonPayload payload = std::make_shared<JsonPayload>(GD::bl);
					devicePacket->jsonPayloads.push_back(payload);
					payload->key = "DPT_10:TIME-SECOND";
					payload->parameterId = "SECOND";
				}
			}

			{
				PPacket devicePacket = std::make_shared<Packet>(GD::bl);
				devicePacket->id = "ON.DPT_11";
				device->packetsById[devicePacket->id] = devicePacket;
				devicePacket->function1 = "onDpt11Packet";
				devicePacket->direction = Packet::Direction::Enum::toCentral;
				devicePacket->channel = -1;
				devicePacket->jsonPayloads.push_back(payloadRaw);
				{
					PJsonPayload payload = std::make_shared<JsonPayload>(GD::bl);
					devicePacket->jsonPayloads.push_back(payload);
					payload->key = "DPT_11:DATE-DAY";
					payload->parameterId = "DAY";
				}
				{
					PJsonPayload payload = std::make_shared<JsonPayload>(GD::bl);
					devicePacket->jsonPayloads.push_back(payload);
					payload->key = "DPT_11:DATE-MONTH";
					payload->parameterId = "MONTH";
				}
				{
					PJsonPayload payload = std::make_shared<JsonPayload>(GD::bl);
					devicePacket->jsonPayloads.push_back(payload);
					payload->key = "DPT_11:DATE-YEAR";
					payload->parameterId = "YEAR";
				}
			}
			{
				PPacket devicePacket = std::make_shared<Packet>(GD::bl);
				devicePacket->id = "SET.DPT_11";
				device->packetsById[devicePacket->id] = devicePacket;
				devicePacket->function1 = "setDpt11Packet";
				devicePacket->direction = Packet::Direction::Enum::fromCentral;
				devicePacket->channel = -1;
				{
					PJsonPayload payload = std::make_shared<JsonPayload>(GD::bl);
					devicePacket->jsonPayloads.push_back(payload);
					payload->key = "DPT_11:DATE-DAY";
					payload->parameterId = "DAY";
				}
				{
					PJsonPayload payload = std::make_shared<JsonPayload>(GD::bl);
					devicePacket->jsonPayloads.push_back(payload);
					payload->key = "DPT_11:DATE-MONTH";
					payload->parameterId = "MONTH";
				}
				{
					PJsonPayload payload = std::make_shared<JsonPayload>(GD::bl);
					devicePacket->jsonPayloads.push_back(payload);
					payload->key = "DPT_11:DATE-YEAR";
					payload->parameterId = "YEAR";
				}
			}

			{
				PPacket devicePacket = std::make_shared<Packet>(GD::bl);
				devicePacket->id = "ON.DPT_12";
				device->packetsById[devicePacket->id] = devicePacket;
				devicePacket->function1 = "onDpt12Packet";
				devicePacket->direction = Packet::Direction::Enum::toCentral;
				devicePacket->channel = -1;
				devicePacket->jsonPayloads.push_back(payloadRaw);
				{
					PJsonPayload payload = std::make_shared<JsonPayload>(GD::bl);
					devicePacket->jsonPayloads.push_back(payload);
					payload->key = "DPT_12:UNSIGNED_VALUE";
					payload->parameterId = "VALUE";
				}
			}
			{
				PPacket devicePacket = std::make_shared<Packet>(GD::bl);
				devicePacket->id = "SET.DPT_12";
				device->packetsById[devicePacket->id] = devicePacket;
				devicePacket->function1 = "setDpt12Packet";
				devicePacket->direction = Packet::Direction::Enum::fromCentral;
				devicePacket->channel = -1;
				{
					PJsonPayload payload = std::make_shared<JsonPayload>(GD::bl);
					devicePacket->jsonPayloads.push_back(payload);
					payload->key = "DPT_12:UNSIGNED_VALUE";
					payload->parameterId = "VALUE";
				}
			}

			{
				PPacket devicePacket = std::make_shared<Packet>(GD::bl);
				devicePacket->id = "ON.DPT_13";
				device->packetsById[devicePacket->id] = devicePacket;
				devicePacket->function1 = "onDpt13Packet";
				devicePacket->direction = Packet::Direction::Enum::toCentral;
				devicePacket->channel = -1;
				devicePacket->jsonPayloads.push_back(payloadRaw);
				{
					PJsonPayload payload = std::make_shared<JsonPayload>(GD::bl);
					devicePacket->jsonPayloads.push_back(payload);
					payload->key = "DPT_13:SIGNED_VALUE";
					payload->parameterId = "VALUE";
				}
			}
			{
				PPacket devicePacket = std::make_shared<Packet>(GD::bl);
				devicePacket->id = "set.DPT_13";
				device->packetsById[devicePacket->id] = devicePacket;
				devicePacket->function1 = "setDpt13Packet";
				devicePacket->direction = Packet::Direction::Enum::fromCentral;
				devicePacket->channel = -1;
				{
					PJsonPayload payload = std::make_shared<JsonPayload>(GD::bl);
					devicePacket->jsonPayloads.push_back(payload);
					payload->key = "DPT_13:SIGNED_VALUE";
					payload->parameterId = "VALUE";
				}
			}

			{
				PPacket devicePacket = std::make_shared<Packet>(GD::bl);
				devicePacket->id = "ON.DPT_14";
				device->packetsById[devicePacket->id] = devicePacket;
				devicePacket->function1 = "onDpt14Packet";
				devicePacket->direction = Packet::Direction::Enum::toCentral;
				devicePacket->channel = -1;
				devicePacket->jsonPayloads.push_back(payloadRaw);
				{
					PJsonPayload payload = std::make_shared<JsonPayload>(GD::bl);
					devicePacket->jsonPayloads.push_back(payload);
					payload->key = "DPT_14:FLOAT";
					payload->parameterId = "VALUE";
				}
			}
			{
				PPacket devicePacket = std::make_shared<Packet>(GD::bl);
				devicePacket->id = "SET.DPT_14";
				device->packetsById[devicePacket->id] = devicePacket;
				devicePacket->function1 = "setDpt14Packet";
				devicePacket->direction = Packet::Direction::Enum::fromCentral;
				devicePacket->channel = -1;
				{
					PJsonPayload payload = std::make_shared<JsonPayload>(GD::bl);
					devicePacket->jsonPayloads.push_back(payload);
					payload->key = "DPT_14:FLOAT";
					payload->parameterId = "VALUE";
				}
			}

			{
				PPacket devicePacket = std::make_shared<Packet>(GD::bl);
				devicePacket->id = "ON.DPT_16";
				device->packetsById[devicePacket->id] = devicePacket;
				devicePacket->function1 = "onDpt16Packet";
				devicePacket->direction = Packet::Direction::Enum::toCentral;
				devicePacket->channel = -1;
				devicePacket->jsonPayloads.push_back(payloadRaw);
				{
					PJsonPayload payload = std::make_shared<JsonPayload>(GD::bl);
					devicePacket->jsonPayloads.push_back(payload);
					payload->key = "DPT_16:TEXT";
					payload->parameterId = "TEXT";
				}
			}
			{
				PPacket devicePacket = std::make_shared<Packet>(GD::bl);
				devicePacket->id = "SET.DPT_16";
				device->packetsById[devicePacket->id] = devicePacket;
				devicePacket->function1 = "setDpt16Packet";
				devicePacket->direction = Packet::Direction::Enum::fromCentral;
				devicePacket->channel = -1;
				{
					PJsonPayload payload = std::make_shared<JsonPayload>(GD::bl);
					devicePacket->jsonPayloads.push_back(payload);
					payload->key = "DPT_16:TEXT";
					payload->parameterId = "TEXT";
				}
			}

			{
				PPacket devicePacket = std::make_shared<Packet>(GD::bl);
				devicePacket->id = "ON.DPT_18";
				device->packetsById[devicePacket->id] = devicePacket;
				devicePacket->function1 = "onDpt18Packet";
				devicePacket->direction = Packet::Direction::Enum::toCentral;
				devicePacket->channel = -1;
				devicePacket->jsonPayloads.push_back(payloadRaw);
				{
					PJsonPayload payload = std::make_shared<JsonPayload>(GD::bl);
					devicePacket->jsonPayloads.push_back(payload);
					payload->key = "DPT_18:SCENE_CONTROL";
					payload->parameterId = "SCENE_CONTROL";
				}
			}
			{
				PPacket devicePacket = std::make_shared<Packet>(GD::bl);
				devicePacket->id = "SET.DPT_18";
				device->packetsById[devicePacket->id] = devicePacket;
				devicePacket->function1 = "setDpt18Packet";
				devicePacket->direction = Packet::Direction::Enum::fromCentral;
				devicePacket->channel = -1;
				{
					PJsonPayload payload = std::make_shared<JsonPayload>(GD::bl);
					devicePacket->jsonPayloads.push_back(payload);
					payload->key = "DPT_18:SCENE_CONTROL";
					payload->parameterId = "SCENE_CONTROL";
				}
			}
			// {{{ Channel 0
			{
				PFunction function(new Function(GD::bl));
				function->channel = 0;
				function->type = "KNX_BAOS_MAINTENANCE";
				function->variablesId = "maintenance_values";
				device->functions[function->channel] = function;

				PParameter parameter(new Parameter(GD::bl, function->variables));
				parameter->id = "UNREACH";
				function->variables->parametersOrdered.push_back(parameter);
				function->variables->parameters[parameter->id] = parameter;
				parameter->writeable = false;
				parameter->service = true;
				parameter->logical = std::make_shared<LogicalBoolean>(GD::bl);;
				parameter->physical = std::make_shared<PhysicalInteger>(GD::bl);
				parameter->physical->groupId = parameter->id;
				parameter->physical->operationType = IPhysical::OperationType::internal;
			}
			// }}}

			//{{{ channel 1++
			{
				for (const auto& item : descriptions)
				{
					const kdrive::baos::BaosDatapointDescription& d = item.second;
					const int id = d.getId();
					const int dpt = d.getDatapointType();
					const int bits = d.getValueTypeSizeBits();
					const int bytes = d.getValueTypeSizeBytes();

					const std::string size = d.isBitType() ? std::to_string(bits) + " Bits" : std::to_string(bytes) + " Bytes";
					GD::out.printInfo("Id: " + std::to_string(id) + ", Datapoint type " + std::to_string(dpt) + ", Size: " + size);

					PFunction function = std::make_shared<Function>(GD::bl);
					function->channel = id;
					function->variablesId = "values_" + std::to_string(id);
					device->functions[function->channel] = function;

					switch (dpt)
					{
					case kdrive::baos::BaosDatapointTypes::DatapointDisabled: /*!< Datapoint disabled */
						break;
					case kdrive::baos::BaosDatapointTypes::DPT_1: /*!< DPT 1 (1 Bit, Boolean) */
						function->type = "[" + std::to_string(id) + "]" + "DPT_1:1Bit_Boolean";
					{
						auto logical = std::make_shared<LogicalBoolean>(GD::bl);
						logical->defaultValue = false;
						makeParameter(function, "STATE", true, true, logical, std::make_shared<PhysicalNone>(GD::bl), "DPT_1");
					}
					break;
					case kdrive::baos::BaosDatapointTypes::DPT_2: /*!< DPT 2 (2 Bit, Control) */
						function->type = "[" + std::to_string(id) + "]" + "DPT_2:2Bit_Control";
					{
						auto logical = std::make_shared<LogicalBoolean>(GD::bl);
						logical->defaultValue = false;
						makeParameter(function, "STATE", true, true, logical, std::make_shared<PhysicalNone>(GD::bl), "DPT_2");
						makeParameter(function, "CONTROL", true, true, logical, std::make_shared<PhysicalNone>(GD::bl), "DPT_2");
					}
					break;
					case kdrive::baos::BaosDatapointTypes::DPT_3: /*!< DPT 3 (4 Bit, Dimming, Blinds) */
						function->type = "[" + std::to_string(id) + "]" + "DPT_3:3Bit_Control";
					{
						auto logical = std::make_shared<LogicalBoolean>(GD::bl);
						logical->defaultValue = false;
						makeParameter(function, "STATE", true, true, logical, std::make_shared<PhysicalNone>(GD::bl), "DPT_3");
					}
					{
						auto logical = std::make_shared<LogicalInteger>(GD::bl);
						logical->defaultValue = 0;
						logical->minimumValue = 0;
						logical->maximumValue = 7;
						makeParameter(function, "CONTROL", true, true, logical, std::make_shared<PhysicalNone>(GD::bl), "DPT_3");
					}
					break;
					case kdrive::baos::BaosDatapointTypes::DPT_4: /*!< DPT 4 (8 Bit, Character Set) */
						function->type = "[" + std::to_string(id) + "]" + "DPT_4:8Bit_CharacterSet";
					{
						auto logical = std::make_shared<LogicalInteger>(GD::bl);
						logical->defaultValue = 0;
						logical->minimumValue = 0;
						logical->maximumValue = 255;
						makeParameter(function, "VALUE", true, true, logical, std::make_shared<PhysicalNone>(GD::bl), "DPT_4");
					}
					break;
					case kdrive::baos::BaosDatapointTypes::DPT_5: /*!< DPT 5 (8 Bit, Unsigned Value) */
						function->type = "[" + std::to_string(id) + "]" + "DPT_5:8Bit_UnsignedValue";
					{
						auto logical = std::make_shared<LogicalInteger>(GD::bl);
						logical->defaultValue = 0;
						logical->minimumValue = 0;
						logical->maximumValue = 255;
						makeParameter(function, "VALUE", true, true, logical, std::make_shared<PhysicalNone>(GD::bl), "DPT_5");
					}
					break;
					case kdrive::baos::BaosDatapointTypes::DPT_6: /*!< DPT 6 (8 Bit, Signed Value) */
						function->type = "[" + std::to_string(id) + "]" + "DPT_6:8Bit_SignedValue";
					{
						auto logical = std::make_shared<LogicalInteger>(GD::bl);
						logical->defaultValue = 0;
						logical->minimumValue = -128;
						logical->maximumValue = 127;
						makeParameter(function, "VALUE", true, true, logical, std::make_shared<PhysicalNone>(GD::bl), "DPT_6");
					}
					break;
					case kdrive::baos::BaosDatapointTypes::DPT_7: /*!< DPT 7 (2 Byte, Unsigned Value) */
						function->type = "[" + std::to_string(id) + "]" + "DPT_7:2BByte_UnsignedValue";
					{
						auto logical = std::make_shared<LogicalInteger>(GD::bl);
						logical->defaultValue = 0;
						logical->minimumValue = 0;
						logical->maximumValue = 65535;
						makeParameter(function, "VALUE", true, true, logical, std::make_shared<PhysicalNone>(GD::bl), "DPT_7");
					}
					break;
					case kdrive::baos::BaosDatapointTypes::DPT_8: /*!< DPT 8 (2 Byte, Signed Value) */
						function->type = "[" + std::to_string(id) + "]" + "DPT_8:2Byte_SignedValue";
					{
						auto logical = std::make_shared<LogicalInteger>(GD::bl);
						logical->defaultValue = 0;
						logical->minimumValue = -32768;
						logical->maximumValue = 32767;
						makeParameter(function, "VALUE", true, true, logical, std::make_shared<PhysicalNone>(GD::bl), "DPT_8");
					}
					break;
					case kdrive::baos::BaosDatapointTypes::DPT_9: /*!< DPT 9 (2 Byte, Float Value) */
						function->type = "[" + std::to_string(id) + "]" + "DPT_9:2Byte_FloatValue";
					{
						auto logical = std::make_shared<LogicalDecimal>(GD::bl);
						logical->defaultValue = 0;
						logical->minimumValue = -670760;
						logical->maximumValue = 670760;
						makeParameter(function, "VALUE", true, true, logical, std::make_shared<PhysicalNone>(GD::bl), "DPT_9");
					}
					{
						auto logical = std::make_shared<LogicalInteger>(GD::bl);
						logical->defaultValue = 0;
						logical->minimumValue = 0;
						logical->maximumValue = 255;
						makeParameter(function, "MSB", true, false, logical, std::make_shared<PhysicalNone>(GD::bl), "DPT_9");
					}
					{
						auto logical = std::make_shared<LogicalInteger>(GD::bl);
						logical->defaultValue = 0;
						logical->minimumValue = 0;
						logical->maximumValue = 255;
						makeParameter(function, "LSB", true, false, logical, std::make_shared<PhysicalNone>(GD::bl), "DPT_9");
					}
					break;
					case kdrive::baos::BaosDatapointTypes::DPT_10: /*!< DPT 10 (3 Byte, Time) */
						function->type = "[" + std::to_string(id) + "]" + "DPT_10:3Byte_Time";
					{
						auto logical = std::make_shared<LogicalInteger>(GD::bl);
						logical->defaultValue = 0;
						logical->minimumValue = 0;
						logical->maximumValue = 7;
						makeParameter(function, "DAY_OF_WEEK", true, true, logical, std::make_shared<PhysicalNone>(GD::bl), "DPT_10");
					}
					{
						auto logical = std::make_shared<LogicalInteger>(GD::bl);
						logical->defaultValue = 0;
						logical->minimumValue = 0;
						logical->maximumValue = 24;
						makeParameter(function, "HOUR", true, true, logical, std::make_shared<PhysicalNone>(GD::bl), "DPT_10");
					}
					{
						auto logical = std::make_shared<LogicalInteger>(GD::bl);
						logical->defaultValue = 0;
						logical->minimumValue = 0;
						logical->maximumValue = 60;
						makeParameter(function, "MINUTE", true, true, logical, std::make_shared<PhysicalNone>(GD::bl), "DPT_10");
					}
					{
						auto logical = std::make_shared<LogicalInteger>(GD::bl);
						logical->defaultValue = 0;
						logical->minimumValue = 0;
						logical->maximumValue = 60;
						makeParameter(function, "SECOND", true, true, logical, std::make_shared<PhysicalNone>(GD::bl), "DPT_10");
					}
					break;
					case kdrive::baos::BaosDatapointTypes::DPT_11: /*!< DPT 11 (3 Byte, Date) */
						function->type = "[" + std::to_string(id) + "]" + "DPT_11:3Byte_Date";
					{
						auto logical = std::make_shared<LogicalInteger>(GD::bl);
						logical->defaultValue = 0;
						logical->minimumValue = 0;
						logical->maximumValue = 31;
						makeParameter(function, "DAY", true, true, logical, std::make_shared<PhysicalNone>(GD::bl), "DPT_11");
					}
					{
						auto logical = std::make_shared<LogicalInteger>(GD::bl);
						logical->defaultValue = 0;
						logical->minimumValue = 0;
						logical->maximumValue = 12;
						makeParameter(function, "MONTH", true, true, logical, std::make_shared<PhysicalInteger>(GD::bl), "DPT_11");
					}
					{
						auto logical = std::make_shared<LogicalInteger>(GD::bl);
						logical->defaultValue = 0;
						logical->minimumValue = 0;
						logical->maximumValue = 255;
						makeParameter(function, "YEAR", true, true, logical, std::make_shared<PhysicalInteger>(GD::bl), "DPT_11");
					}
					break;
					case kdrive::baos::BaosDatapointTypes::DPT_12: /*!< DPT 12 (4 Byte, Unsigned Value) */
						function->type = "[" + std::to_string(id) + "]" + "DPT_12:4Byte_UnsignedValue";
					{
						auto logical = std::make_shared<LogicalInteger64>(GD::bl);
						logical->defaultValue = 0;
						logical->minimumValue = 0;
						logical->maximumValue = 4294967295;
						makeParameter(function, "VALUE", true, true, logical, std::make_shared<PhysicalNone>(GD::bl), "DPT_12");
					}
					break;
					case kdrive::baos::BaosDatapointTypes::DPT_13: /*!< DPT 13 (4 Byte, Signed Value) */
						function->type = "[" + std::to_string(id) + "]" + "DPT_13:4Byte_SignedValue";
					{
						auto logical = std::make_shared<LogicalInteger>(GD::bl);
						logical->defaultValue = 0;
						logical->minimumValue = -2147483648;
						logical->maximumValue = 2147483647;
						makeParameter(function, "VALUE", true, true, logical, std::make_shared<PhysicalNone>(GD::bl), "DPT_13");
					}
					break;
					case kdrive::baos::BaosDatapointTypes::DPT_14: /*!< DPT 14 (4 Byte, Float Value) */
						function->type = "[" + std::to_string(id) + "]" + "DPT_14:4Byte_FloatValue";
					{
						auto logical = std::make_shared<LogicalDecimal>(GD::bl);
						logical->defaultValue = 0;
						makeParameter(function, "VALUE", true, true, logical, std::make_shared<PhysicalNone>(GD::bl), "DPT_14");
					}
					break;
					case kdrive::baos::BaosDatapointTypes::DPT_15: /*!< DPT 15 (4 Byte, Access) */
						break;
					case kdrive::baos::BaosDatapointTypes::DPT_16: /*!< DPT 16 (14 Byte, String) */
						function->type = "[" + std::to_string(id) + "]" + "DPT_16:14Byte_String";
					{
						auto logical = std::make_shared<LogicalString>(GD::bl);
						logical->defaultValue = "";
						makeParameter(function, "TEXT", true, true, logical, std::make_shared<PhysicalNone>(GD::bl), "DPT_16");
					}
					break;
					case kdrive::baos::BaosDatapointTypes::DPT_17: /*!< DPT 17 (1 Byte, Scene Number) */
						break;
					case kdrive::baos::BaosDatapointTypes::DPT_18: /*!< DPT 18 (1 Byte, Scene Control) */
						function->type = "[" + std::to_string(id) + "]" + "DPT_18:1Byte_SceneControl";
					{
						auto logical = std::make_shared<LogicalInteger>(GD::bl);
						logical->defaultValue = 0;
						logical->minimumValue = 0;
						logical->maximumValue = 63;
						makeParameter(function, "SCENE_CONTROL", true, true, logical, std::make_shared<PhysicalNone>(GD::bl), "DPT_18");
					}
					break;
					case kdrive::baos::BaosDatapointTypes::DPT_20: /*!< DPT 20 (1 Byte) */
						break;
					case kdrive::baos::BaosDatapointTypes::DPT_232: /*!< DPT 232 (3-byte colour RGB) */
						break;
					case kdrive::baos::BaosDatapointTypes::Unknown: /*!< Unknown DPT */
						break;
					default:
						break;
					}
				}
			}

			std::string filename = _xmlPath + serialNumber + ".xml";
			device->save(filename);
		}
		catch (const std::exception& ex) {
			GD::out.printEx(__FILE__, __LINE__, __PRETTY_FUNCTION__, ex.what());
		}
	}

	void MakeDeviceDescription::createDirectories() {
		try {
			uid_t localUserId = BaseLib::HelperFunctions::userId(GD::bl->settings.dataPathUser());
			gid_t localGroupId = BaseLib::HelperFunctions::groupId(GD::bl->settings.dataPathGroup());
			if (((int32_t)localUserId) == -1 || ((int32_t)localGroupId) == -1) {
				localUserId = GD::bl->userId;
				localGroupId = GD::bl->groupId;
			}

			std::string path1 = GD::bl->settings.familyDataPath();
			std::string path2 = path1 + std::to_string(GD::family->getFamily()) + "/";
			_xmlPath = path2 + "desc/";
			if (!BaseLib::Io::directoryExists(path1)) BaseLib::Io::createDirectory(path1, GD::bl->settings.dataPathPermissions());
			if (localUserId != 0 || localGroupId != 0) {
				if (chown(path1.c_str(), localUserId, localGroupId) == -1) GD::out.printWarning("Could not set owner on " + path1);
				if (chmod(path1.c_str(), GD::bl->settings.dataPathPermissions()) == -1) GD::out.printWarning("Could not set permissions on " + path1);
			}
			if (!BaseLib::Io::directoryExists(path2)) BaseLib::Io::createDirectory(path2, GD::bl->settings.dataPathPermissions());
			if (localUserId != 0 || localGroupId != 0) {
				if (chown(path2.c_str(), localUserId, localGroupId) == -1) GD::out.printWarning("Could not set owner on " + path2);
				if (chmod(path2.c_str(), GD::bl->settings.dataPathPermissions()) == -1) GD::out.printWarning("Could not set permissions on " + path2);
			}
			if (!BaseLib::Io::directoryExists(_xmlPath)) BaseLib::Io::createDirectory(_xmlPath, GD::bl->settings.dataPathPermissions());
			if (localUserId != 0 || localGroupId != 0) {
				if (chown(_xmlPath.c_str(), localUserId, localGroupId) == -1) GD::out.printWarning("Could not set owner on " + _xmlPath);
				if (chmod(_xmlPath.c_str(), GD::bl->settings.dataPathPermissions()) == -1) GD::out.printWarning("Could not set permissions on " + _xmlPath);
			}
		}
		catch (const std::exception& ex) {
			GD::out.printEx(__FILE__, __LINE__, __PRETTY_FUNCTION__, ex.what());
		}
	}

	void MakeDeviceDescription::makeParameter(BaseLib::DeviceDescription::PFunction function, std::string id, bool readable, bool writable, std::shared_ptr<BaseLib::DeviceDescription::ILogical> logical, std::shared_ptr<BaseLib::DeviceDescription::IPhysical> physical, std::string packetId) {
		if (!function)return;

		if(id != "RAW" && function->variables->parameters.find("RAW") == function->variables->parameters.end())
			makeParameter(function, "RAW", true, false, std::make_shared<LogicalString>(GD::bl), std::make_shared<PhysicalNone>(GD::bl), packetId);
				
		PParameter parameter(new Parameter(GD::bl, function->variables));
		if (!parameter) return;
		parameter->id = id;

		function->variables->parametersOrdered.push_back(parameter);
		function->variables->parameters[parameter->id] = parameter;
		
		parameter->writeable = writable;
		parameter->readable = readable;

		ParameterCast::PRpcBinary cast = std::make_shared<ParameterCast::RpcBinary>(GD::bl);
		parameter->casts.push_back(cast);
		
		parameter->logical = logical;
		parameter->physical = physical;
		parameter->physical->groupId = parameter->id;
		parameter->physical->operationType = IPhysical::OperationType::command;
		if(readable)
		{
			std::shared_ptr<Parameter::Packet> eventPacket = std::make_shared<Parameter::Packet>();
			parameter->eventPackets.push_back(eventPacket);
			eventPacket->type = Parameter::Packet::Type::Enum::event;
			eventPacket->id = "ON." + packetId;
		}
		if(writable)
		{
			std::shared_ptr<Parameter::Packet> eventPacket = std::make_shared<Parameter::Packet>();
			parameter->eventPackets.push_back(eventPacket);
			eventPacket->type = Parameter::Packet::Type::Enum::set;
			eventPacket->id = "SET." + packetId;
		}
	}
}