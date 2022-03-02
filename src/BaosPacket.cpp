#include <utility>
#include "BaosPacket.h"
#include "GD.h"

namespace Baos
{

BaosPacket::BaosPacket(std::string serialNumber, kdrive::baos::BaosDatapoint& baosDatapoint)
{
  //packet Received
  _serialNumber = serialNumber;
  _rawData = baosDatapoint.getData();
  _datapointType = baosDatapoint.getDatapointDescription().getDatapointType();
  _datapointId = baosDatapoint.getId();

  //GD::out.printInfo("Info: Received datapoint value for id " + std::to_string(_datapointId) + " = " + BaseLib::HelperFunctions::getHexString(_rawData));
  
  _json = std::make_shared<Variable>(VariableType::tStruct);
  _json->structValue->operator[]("RAW") = PVariable(new Variable("0x" + BaseLib::HelperFunctions::getHexString(_rawData)));

  switch (_datapointType)
  {
  case kdrive::baos::BaosDatapointTypes::DatapointDisabled: /*!< Datapoint disabled */
    break;
  case kdrive::baos::BaosDatapointTypes::DPT_1: /*!< DPT 1 (1 Bit, Boolean) */
    _packetType = "onDpt1Packet";

    _json->structValue->operator[]("DPT_1:STATE") = PVariable(new Variable(baosDatapoint.getBoolean()));
  break;
  case kdrive::baos::BaosDatapointTypes::DPT_2: /*!< DPT 2 (2 Bit, Control) */
    _packetType = "onDpt2Packet";

    _json->structValue->operator[]("DPT_2:STATE") = PVariable(new Variable(baosDatapoint.get1BitControl()));
    _json->structValue->operator[]("DPT_2:CONTROL") = PVariable(new Variable(baosDatapoint.get1BitControlValue()));
    break;
  case kdrive::baos::BaosDatapointTypes::DPT_3: /*!< DPT 3 (4 Bit, Dimming, Blinds) */
    _packetType = "onDpt3Packet";

    _json->structValue->operator[]("DPT_3:STATE") = PVariable(new Variable(baosDatapoint.get3BitControl()));
    _json->structValue->operator[]("DPT_3:CONTROL") = PVariable(new Variable(baosDatapoint.get3BitControlValue()));
    break;
  case kdrive::baos::BaosDatapointTypes::DPT_4: /*!< DPT 4 (8 Bit, Character Set) */
    _packetType = "onDpt4Packet";

    _json->structValue->operator[]("DPT_4:CHARACTER_SET") = PVariable(new Variable(baosDatapoint.getCharacterSet()));
    break;
  case kdrive::baos::BaosDatapointTypes::DPT_5: /*!< DPT 5 (8 Bit, Unsigned Value) */
    _packetType = "onDpt5Packet";

    _json->structValue->operator[]("DPT_5:UNSIGNED_VALUE") = PVariable(new Variable(baosDatapoint.get8BitUnsigned()));
    break;
  case kdrive::baos::BaosDatapointTypes::DPT_6: /*!< DPT 6 (8 Bit, Signed Value) */
    _packetType = "onDpt6Packet";

    _json->structValue->operator[]("DPT_6:SIGNED_VALUE") = PVariable(new Variable((int8_t)baosDatapoint.get8BitSigned()));
    break;
  case kdrive::baos::BaosDatapointTypes::DPT_7: /*!< DPT 7 (2 Byte, Unsigned Value) */
    _packetType = "onDpt7Packet";

    _json->structValue->operator[]("DPT_7:UNSIGNED_VALUE") = PVariable(new Variable(baosDatapoint.get2OctetUnsigned()));
    break;
  case kdrive::baos::BaosDatapointTypes::DPT_8: /*!< DPT 8 (2 Byte, Signed Value) */
    _packetType = "onDpt8Packet";

    _json->structValue->operator[]("DPT_8:SIGNED_VALUE") = PVariable(new Variable(baosDatapoint.get2OctetSigned()));
    break;
  case kdrive::baos::BaosDatapointTypes::DPT_9: /*!< DPT 9 (2 Byte, Float Value) */
    _packetType = "onDpt9Packet";

    _json->structValue->operator[]("DPT_9:FLOAT") = PVariable(new Variable(baosDatapoint.get2OctetFloat()));
    _json->structValue->operator[]("DPT_9:FLOAT_LSB") = PVariable(new Variable(baosDatapoint.get2OctetFloatLsb()));
    _json->structValue->operator[]("DPT_9:FLOAT_MSB") = PVariable(new Variable(baosDatapoint.get2OctetFloatMsb()));
    break;
  case kdrive::baos::BaosDatapointTypes::DPT_10: /*!< DPT 10 (3 Byte, Time) */
    _packetType = "onDpt10Packet";

    _json->structValue->operator[]("DPT_10:TIME-DAY") = PVariable(new Variable(baosDatapoint.getTimeDay()));
    _json->structValue->operator[]("DPT_10:TIME-HOUR") = PVariable(new Variable(baosDatapoint.getTimeHour()));
    _json->structValue->operator[]("DPT_10:TIME-MINUTE") = PVariable(new Variable(baosDatapoint.getTimeMinute()));
    _json->structValue->operator[]("DPT_10:TIME-SECOND") = PVariable(new Variable(baosDatapoint.getTimeSecond()));
    break;
  case kdrive::baos::BaosDatapointTypes::DPT_11: /*!< DPT 11 (3 Byte, Date) */
    _packetType = "onDpt11Packet";

    _json->structValue->operator[]("DPT_11:DATE-DAY") = PVariable(new Variable(baosDatapoint.getDateDay()));
    _json->structValue->operator[]("DPT_11:DATE-MONTH") = PVariable(new Variable(baosDatapoint.getDateMonth()));
    _json->structValue->operator[]("DPT_11:DATE-YEAR") = PVariable(new Variable(baosDatapoint.getDateYear()));
    break;
  case kdrive::baos::BaosDatapointTypes::DPT_12: /*!< DPT 12 (4 Byte, Unsigned Value) */
    _packetType = "onDpt12Packet";
    
    _json->structValue->operator[]("DPT_12:UNSIGNED_VALUE") = PVariable(new Variable((uint64_t)baosDatapoint.get4OctetUnsigned()));
    break;
  case kdrive::baos::BaosDatapointTypes::DPT_13: /*!< DPT 13 (4 Byte, Signed Value) */
    _packetType = "onDpt13Packet";

    _json->structValue->operator[]("DPT_13:SIGNED_VALUE") = PVariable(new Variable(baosDatapoint.get4OctetSigned()));
    break;
  case kdrive::baos::BaosDatapointTypes::DPT_14: /*!< DPT 14 (4 Byte, Float Value) */
    _packetType = "onDpt14Packet";
    GD::out.printInfo("Info: " + std::to_string(baosDatapoint.get4OctetFloat()));
    _json->structValue->operator[]("DPT_14:FLOAT") = PVariable(new Variable(baosDatapoint.get4OctetFloat()));
    break;
  case kdrive::baos::BaosDatapointTypes::DPT_15: /*!< DPT 15 (4 Byte, Access) */
    /*not available in ets
    baosDatapoint.getAccessCode();
    baosDatapoint.getAccessDirection();
    baosDatapoint.getAccessEncrypted();
    baosDatapoint.getAccessError();
    baosDatapoint.getAccessIndex();
    baosDatapoint.getAccessPermission();
    */
    break;
  case kdrive::baos::BaosDatapointTypes::DPT_16: /*!< DPT 16 (14 Byte, String) */
    _packetType = "onDpt16Packet";
    _json->structValue->operator[]("DPT_16:TEXT") = PVariable(new Variable(baosDatapoint.getString()));
    break;
  case kdrive::baos::BaosDatapointTypes::DPT_17: /*!< DPT 17 (1 Byte, Scene Number) */
    break;
  case kdrive::baos::BaosDatapointTypes::DPT_18: /*!< DPT 18 (1 Byte, Scene Control) */
    _packetType = "onDpt18Packet";
    _json->structValue->operator[]("DPT_18:SCENE_CONTROL") = PVariable(new Variable(baosDatapoint.get8BitUnsigned()));
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

BaosPacket::BaosPacket(uint32_t datapointId, std::string packetType, std::shared_ptr<std::map<std::string, PVariable>> values)
{
  //send Packet
  _datapointId = datapointId;
  _packetType = packetType;
  _values = values;


  GD::out.printDebug("PacketType : " + packetType,5);
  for (auto i = values->begin(); i != values->end(); ++i) {
    GD::out.printDebug("Packet: " + i->first + " = " + i->second->toString(), 5);
  }
}

bool BaosPacket::setValuesToDatapoint(kdrive::baos::BaosDatapoint& baosDatapoint) {
  _datapointType = baosDatapoint.getDatapointDescription().getDatapointType();

  switch (_datapointType)
  {
  case kdrive::baos::BaosDatapointTypes::DatapointDisabled: /*!< Datapoint disabled */
    break;
  case kdrive::baos::BaosDatapointTypes::DPT_1: /*!< DPT 1 (1 Bit, Boolean) */
    if (_packetType != "setDpt1Packet") return false;
    if (!_values) return false;
    if (_values->find("DPT_1:STATE") == _values->end()) return false;
    if (_values->at("DPT_1:STATE")->type != VariableType::tBoolean) return false;

    baosDatapoint.setBoolean(_values->at("DPT_1:STATE")->booleanValue);
    return true;
  case kdrive::baos::BaosDatapointTypes::DPT_2: /*!< DPT 2 (2 Bit, Control) */
    if (_packetType != "setDpt2Packet") return false;
    if (!_values) return false;
    if (_values->find("DPT_2:STATE") == _values->end()) return false;
    if (_values->find("DPT_2:CONTROL") == _values->end()) return false;

    if (_values->at("DPT_2:STATE")->type != VariableType::tBoolean) return false;
    if (_values->at("DPT_2:CONTROL")->type != VariableType::tBoolean) return false;

    baosDatapoint.set1BitControl(_values->at("DPT_2:STATE")->booleanValue, _values->at("DPT_2:CONTROL")->booleanValue);
    return true;
  case kdrive::baos::BaosDatapointTypes::DPT_3: /*!< DPT 3 (4 Bit, Dimming, Blinds) */
    if(_packetType != "setDpt3Packet") return false;

    if (_values->find("DPT_3:STATE") == _values->end()) return false;
    if (_values->find("DPT_3:CONTROL") == _values->end()) return false;
    if (_values->at("DPT_3:STATE")->type != VariableType::tBoolean) return false;
    if (_values->at("DPT_3:CONTROL")->type != VariableType::tInteger) return false;

    baosDatapoint.set3BitControl(_values->at("DPT_3:STATE")->integerValue, _values->at("DPT_3:CONTROL")->booleanValue);
    return true;
  case kdrive::baos::BaosDatapointTypes::DPT_4: /*!< DPT 4 (8 Bit, Character Set) */
    if(_packetType != "setDpt4Packet") return false;
    if (!_values) return false;
    if (_values->find("DPT_4:CHARACTER_SET") == _values->end()) return false;
    if (_values->at("DPT_4:CHARACTER_SET")->type != VariableType::tInteger) return false;

    baosDatapoint.setCharacterSet(_values->at("DPT_4:CHARACTER_SET")->integerValue);
    return true;
  case kdrive::baos::BaosDatapointTypes::DPT_5: /*!< DPT 5 (8 Bit, Unsigned Value) */
    if(_packetType != "setDpt5Packet") return false;
    if (!_values) return false;
    if (_values->find("DPT_5:UNSIGNED_VALUE") == _values->end()) return false;
    if (_values->at("DPT_5:UNSIGNED_VALUE")->type != VariableType::tInteger) return false;

    baosDatapoint.set8BitUnsigned(_values->at("DPT_5:UNSIGNED_VALUE")->integerValue);
    return true;
  case kdrive::baos::BaosDatapointTypes::DPT_6: /*!< DPT 6 (8 Bit, Signed Value) */
    if(_packetType != "setDpt6Packet") return false;
    if (!_values) return false;
    if (_values->find("DPT_6:SIGNED_VALUE") == _values->end()) return false;
    if (_values->at("DPT_6:SIGNED_VALUE")->type != VariableType::tInteger) return false;

    baosDatapoint.set8BitSigned(_values->at("DPT_6:SIGNED_VALUE")->integerValue);
    return true;
  case kdrive::baos::BaosDatapointTypes::DPT_7: /*!< DPT 7 (2 Byte, Unsigned Value) */
    if(_packetType != "setDpt7Packet") return false;
    if (!_values) return false;
    if (_values->find("DPT_7:UNSIGNED_VALUE") == _values->end()) return false;
    if (_values->at("DPT_7:UNSIGNED_VALUE")->type != VariableType::tInteger) return false;

    baosDatapoint.set2OctetUnsigned(_values->at("DPT_7:UNSIGNED_VALUE")->integerValue);
    return true;
  case kdrive::baos::BaosDatapointTypes::DPT_8: /*!< DPT 8 (2 Byte, Signed Value) */
    if(_packetType != "setDpt8Packet") return false;
    if (!_values) return false;
    if (_values->find("DPT_8:SIGNED_VALUE") == _values->end()) return false;
    if (_values->at("DPT_8:SIGNED_VALUE")->type != VariableType::tInteger) return false;

    baosDatapoint.set2OctetSigned(_values->at("DPT_8:SIGNED_VALUE")->integerValue);
    return true;
  case kdrive::baos::BaosDatapointTypes::DPT_9: /*!< DPT 9 (2 Byte, Float Value) */
    if(_packetType != "setDpt9Packet") return false;
    if (!_values) return false;
    if (_values->find("DPT_9:FLOAT") == _values->end()) return false;
    if (_values->at("DPT_9:FLOAT")->type != VariableType::tFloat) return false;

    baosDatapoint.set2OctetFloat(_values->at("DPT_9:FLOAT")->floatValue);
    return true;
  case kdrive::baos::BaosDatapointTypes::DPT_10: /*!< DPT 10 (3 Byte, Time) */
    if(_packetType != "setDpt10Packet") return false;
    if (!_values) return false;
    if (_values->find("DPT_10:TIME-DAY") == _values->end()) return false;
    if (_values->find("DPT_10:TIME-HOUR") == _values->end()) return false;
    if (_values->find("DPT_10:TIME-MINUTE") == _values->end()) return false;
    if (_values->find("DPT_10:TIME-SECOND") == _values->end()) return false;
    if (_values->at("DPT_10:TIME-DAY")->type != VariableType::tInteger) return false;
    if (_values->at("DPT_10:TIME-HOUR")->type != VariableType::tInteger) return false;
    if (_values->at("DPT_10:TIME-MINUTE")->type != VariableType::tInteger) return false;
    if (_values->at("DPT_10:TIME-SECOND")->type != VariableType::tInteger) return false;

    baosDatapoint.setTime(_values->at("DPT_10:TIME-DAY")->integerValue, _values->at("DPT_10:TIME-HOUR")->integerValue, _values->at("DPT_10:TIME-MINUTE")->integerValue, _values->at("DPT_10:TIME-SECOND")->integerValue);
    return true;
  case kdrive::baos::BaosDatapointTypes::DPT_11: /*!< DPT 11 (3 Byte, Date) */
    if(_packetType != "setDpt11Packet") return false;
    if (!_values) return false;
    if (_values->find("DPT_11:DATE-DAY") == _values->end()) return false;
    if (_values->find("DPT_11:DATE-MONTH") == _values->end()) return false;
    if (_values->find("DPT_11:DATE-YEAR") == _values->end()) return false;
    if (_values->at("DPT_11:DATE-DAY")->type != VariableType::tInteger) return false;
    if (_values->at("DPT_11:DATE-MONTH")->type != VariableType::tInteger) return false;
    if (_values->at("DPT_11:DATE-YEAR")->type != VariableType::tInteger) return false;
    
    baosDatapoint.setDate(_values->at("DPT_11:DATE-DAY")->integerValue, _values->at("DPT_11:DATE-MONTH")->integerValue, _values->at("DPT_11:DATE-YEAR")->integerValue);
    return true;
  case kdrive::baos::BaosDatapointTypes::DPT_12: /*!< DPT 12 (4 Byte, Unsigned Value) */
    if(_packetType != "setDpt12Packet") return false;
    if (!_values) return false;
    if (_values->find("DPT_12:UNSIGNED_VALUE") == _values->end()) return false;
    if (_values->at("DPT_12:UNSIGNED_VALUE")->type != VariableType::tInteger64) return false;

    baosDatapoint.set4OctetUnsigned(_values->at("DPT_12:UNSIGNED_VALUE")->integerValue64);
    return true;
  case kdrive::baos::BaosDatapointTypes::DPT_13: /*!< DPT 13 (4 Byte, Signed Value) */
    if(_packetType != "setDpt13Packet")return false;
    if (!_values) return false;
    if (_values->find("DPT_13:SIGNED_VALUE") == _values->end()) return false;
    if (_values->at("DPT_13:SIGNED_VALUE")->type != VariableType::tInteger) return false;

    baosDatapoint.set4OctetSigned(_values->at("DPT_13:SIGNED_VALUE")->floatValue);
    return true;
  case kdrive::baos::BaosDatapointTypes::DPT_14: /*!< DPT 14 (4 Byte, Float Value) */
    if(_packetType != "setDpt14Packet")return false;
    if (!_values) return false;
    if (_values->find("DPT_14:FLOAT") == _values->end()) return false;
    if (_values->at("DPT_14:FLOAT")->type != VariableType::tFloat) return false;

    baosDatapoint.set4OctetFloat(_values->at("DPT_14:FLOAT")->floatValue);
    return true;
  case kdrive::baos::BaosDatapointTypes::DPT_15: /*!< DPT 15 (4 Byte, Access) */
    /*not available in ets
    baosDatapoint.setAccess();
    */
    return false;
  case kdrive::baos::BaosDatapointTypes::DPT_16: /*!< DPT 16 (14 Byte, String) */
    if(_packetType != "setDpt16Packet") return false;
    if (!_values) return false;
    if (_values->find("DPT_16:TEXT") == _values->end()) return false;
    if (_values->at("DPT_16:TEXT")->type != VariableType::tString) return false;

    baosDatapoint.setString(_values->at("DPT_16:TEXT")->stringValue);
    return true;
  case kdrive::baos::BaosDatapointTypes::DPT_17: /*!< DPT 17 (1 Byte, Scene Number) */
    return false;
  case kdrive::baos::BaosDatapointTypes::DPT_18: /*!< DPT 18 (1 Byte, Scene Control) */
    if(_packetType != "setDpt18Packet")return false;
    if (!_values) return false;
    if (_values->find("DPT_18:SCENE_CONTROL") == _values->end()) return false;
    if (_values->at("DPT_18:SCENE_CONTORL")->type != VariableType::tInteger) return false;

    baosDatapoint.set8BitUnsigned(_values->at("DPT_18:SCENE_CONTROL")->floatValue);
    return true;
  case kdrive::baos::BaosDatapointTypes::DPT_20: /*!< DPT 20 (1 Byte) */
    return false;
  case kdrive::baos::BaosDatapointTypes::DPT_232: /*!< DPT 232 (3-byte colour RGB) */
    return false;
  case kdrive::baos::BaosDatapointTypes::Unknown: /*!< Unknown DPT */
    return false;
  default:
    return false;
  }
  return false;
}

}
