#ifndef I2CDEVICE_H
#define I2CDEVICE_H

#include <string>
#include <cstdint>

class I2CDevice
{
public:
    I2CDevice(const std::string &devicePath, uint8_t slaveAddr);
    ~I2CDevice();

    bool writeByte(uint8_t reg, uint8_t data);
    bool readByte(uint8_t reg, uint8_t &data);
    bool writeBytes(uint8_t reg, const uint8_t *data, size_t length);
    bool readBytes(uint8_t reg, uint8_t *data, size_t length);
    bool isOpen() const;

private:
    int fd;
    std::string devicePath;
    uint8_t slaveAddress;

    bool setRegister(uint8_t reg);
};

#endif
