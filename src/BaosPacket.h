#ifndef BAOSPACKET_H_
#define BAOSPACKET_H_

#include <homegear-base/BaseLib.h>

#include <map>
#include <kdrive/baos/Baos.h>

using namespace BaseLib;

namespace Baos
{

class InvalidBaosPacketException : public BaseLib::Exception
{
public:
    explicit InvalidBaosPacketException(const std::string& message) : Exception(message) {}
};

class BaosPacket : public BaseLib::Systems::Packet
{
public:
    BaosPacket() = default;
    explicit BaosPacket(std::string serialNumber, kdrive::baos::BaosDatapoint& baosDatapoint);
    BaosPacket(uint32_t datapointId, std::string packetType, std::shared_ptr<std::map<std::string, PVariable>> values);
    virtual ~BaosPacket() = default;

    std::vector<uint8_t> getRawData() { return _rawData; }
    PVariable getJson() { return _json; }
    std::string getSerialNumber() { return _serialNumber; }
    uint32_t getDatapointId() { return _datapointId; }
    std::string getMethod() { return _packetType; }

    bool setValuesToDatapoint(kdrive::baos::BaosDatapoint& baosDatapoint);
protected:
    PVariable _json;
    std::vector<uint8_t> _rawData;
    std::shared_ptr<std::map<std::string, PVariable>> _values;
    uint32_t _datapointId = 0;
    uint8_t _datapointType = 255;
    std::string _packetType;
    std::string _serialNumber;
};

typedef std::shared_ptr<BaosPacket> PBaosPacket;

}
#endif
