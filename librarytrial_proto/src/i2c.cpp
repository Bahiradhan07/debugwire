#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <iostream>
#include "i2c.hpp"
using namespace std;
I2CDevice::I2CDevice(const std::string &devicePath, uint8_t slaveAddr)
    : devicePath(devicePath), slaveAddress(slaveAddr)
{
    fd = open(devicePath.c_str(), O_RDWR);
    if (fd < 0)
    {
        perror("Failed to open I2C device");
        return;
    }

    if (ioctl(fd, I2C_SLAVE, slaveAddress) < 0)
    {
        perror("Failed to set I2C address");
        close(fd);
        fd = -1;
    }
}

I2CDevice::~I2CDevice()
{
    if (fd >= 0)
    {
        close(fd);
    }
}

bool I2CDevice::isOpen() const
{
    return fd >= 0;
}

bool I2CDevice::writeByte(uint8_t reg, uint8_t data)
{
    uint8_t buffer[2] = {reg, data};
    return write(fd, buffer, 2) == 2;
}

bool I2CDevice::readByte(uint8_t reg, uint8_t &data)
{
//    if (write(fd, &reg, 1) != 1)
//{
//cout<<"failed"<<endl;
//        return false;
//}
//	char mass[1];
   if(read(fd, &data, 1) != 1)
	{
cout<<"error"<<endl;
}
else
{
//printf("recieved data %d\n",mass[0]);
cout<<"inside_data"<<dec<<static_cast<int>(data)<<endl;
}
return 1;
}

bool I2CDevice::writeBytes(uint8_t reg, const uint8_t *data, size_t length)
{
    uint8_t buffer[length + 1];
    buffer[0] = reg;
    std::copy(data, data + length, buffer + 1);
    return write(fd, buffer, length + 1) == (ssize_t)(length + 1);
}

bool I2CDevice::readBytes(uint8_t reg, uint8_t *data, size_t length)
{
    if (write(fd, &reg, 1) != 1)
        return false;
    return read(fd, data, length) == (ssize_t)length;
}
