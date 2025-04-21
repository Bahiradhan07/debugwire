#include <iostream>
#include<fcntl.h>
#include<unistd.h>
#include "vsomeipclient.hpp"
// #include "../src/i2c.cpp"
#include <cstring>
#include "../include/i2c.hpp"
using namespace std;
void response_frm_cl(const vector<uint8_t> &data)
{
    cout << "recieved data" << endl;
    for (auto hi : data)
    {
        cout << hex << static_cast<int>(hi) << endl;
    }
}

int main()
{
	uint8_t data=0;
    uint8_t arr[] = {23, 45, 3, 5, 21};
    //vsomeipclient client;
    I2CDevice i2c("/dev/i2c-1", 0x08);
  while(1)
  {
    i2c.readByte(0,data);
	sleep(1);
	cout<<hex<<"data"<<static_cast<int>(data)<<endl;
  }               
    //i2c.writeByte(0, 0x10);
    //i2c.writeBytes(0x00, arr, 5);
   // client.register_callback12(response_frm_cl);
    //client.start(0x1234, 0x5678, 0x0421);
}
