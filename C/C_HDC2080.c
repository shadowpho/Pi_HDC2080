#include <stdio.h>
#include <time.h>
#include "C_HDC2080.h"
#include <unistd.h>				//Needed for I2C port
#include <fcntl.h>				//
#include <sys/ioctl.h>				//change slave
#include <linux/i2c-dev.h>				



#define MFCT_ID 0x5449
#define DEVICE_ID 0x07D0

static int file_i2c_handle =0;


//device address
#define HDC2080_ADDRESS 0x40

//registers addresses
#define HDC2080_TEMPERATURE_REGISTER           (0x00)
#define HDC2080_HUMIDITY_REGISTER              (0x02)
#define HDC2080_RESET_REGISTER                (0x0E)
#define HDC2080_CONFIG_REGISTER        (0x0F)
#define HDC2080_MANUFACTURERID_REGISTER        (0xFC)
#define HDC2080_DEVICEID_REGISTER         (0xFE)

//bit configuration we actually care about
#define HDC2080_RESET_RESET_BIT              (0x80)
#define HDC2080_RESET_HEATER_ENABLE           (0x8)
#define HDC2080_CONFIG_GO        (0x1)

#define NANO_SECOND_MULTIPLIER  1000000                // 1 millisecond = 1,000,000 Nanoseconds
const long INTERVAL_MS = 500 * NANO_SECOND_MULTIPLIER;

//private function, we have to write 1 byte (address) to read from that location
int read_from_address(uint8_t address, uint16_t* recv)
{
	uint8_t buff[4] = {};
	buff[0] = address;
	if(write(file_i2c_handle, buff, 1) != 1)
	{
		printf("Device failed to ACK the register address\n");
		return -1;
	}
	if(read(file_i2c_handle,buff,2) != 2)
	{
		printf("Device failed to ACK the read -- maybe you are reading invalid register?\n");
		return -2;
	}
	
	*recv = (buff[1]<<8) | buff[0]; // 2 8-bit value combine to 1 16-bit 
	return 0;
}



//negative on error, positive on success
int setup_hdc2080()
{
	//OPEN I2C communication 
	char *filename = (char*)"/dev/i2c-1";
	if ((file_i2c_handle = open(filename, O_RDWR)) < 0)
	{
		printf("Failed to open the i2c bus\n");
		return -1;
	}
	if (ioctl(file_i2c_handle, I2C_SLAVE,HDC2080_ADDRESS ) < 0)
	{
		printf("Failed to acquire bus access and/or talk to slave.\n");
		return -2;
	}

	//CHECK THAT WE ARE TALKING TO RIGHT DEVICE
	uint16_t response;
	if(read_from_address(HDC2080_MANUFACTURERID_REGISTER,&response )!=0)
	{
		printf("Unable to aquire manufacture ID\n");
		return -3;
	}
	if(response != MFCT_ID)
	{
		printf("Wrong MFCT ID. Wrong device?\n"); return -4;
	}	
	
	if(read_from_address(HDC2080_DEVICEID_REGISTER,&response )!=0)
	{
		printf("Unable to aquire device ID\n");
		return -3;
	}
	if(response != DEVICE_ID)
	{
		printf("Wrong device ID. Wrong device?\n"); return -4;
	}

	//RESET THE DEVICE
	//
	
	uint8_t buff[4] = {};
	buff[0] = HDC2080_RESET_REGISTER; //register address
	buff[1] = HDC2080_RESET_RESET_BIT;//set high bit enable to reset. 
	if(write(file_i2c_handle, buff, 2) != 2)
	{
		printf("Device failed to ACK the reset command\n");
		return -1;
	}
	//sleep for 100ms
	nanosleep((const struct timespec[]){{0, 100*INTERVAL_MS}}, NULL);

	return 0;
}
int read_from_hdc2080(float* temperature, float* humidity)
{
	uint8_t buff[4] = {};
	buff[0] = HDC2080_CONFIG_REGISTER; 
	buff[1] = HDC2080_CONFIG_GO; 
	if(write(file_i2c_handle, buff, 2) != 2)
	{
		printf("Device failed to ACK the GO command\n");
		return -1;
	}
	//sleep for 10ms to let it finish measuring
	nanosleep((const struct timespec[]){{0, 10*INTERVAL_MS}}, NULL);
	
	uint16_t temp_raw, humidity_raw;

	if(read_from_address(HDC2080_TEMPERATURE_REGISTER,&temp_raw )!=0)
	{
		printf("Unable to read temperature\n");
		return -2;
	}

	if(read_from_address(HDC2080_HUMIDITY_REGISTER,&humidity_raw )!=0)
	{
		printf("Unable to read temperature\n");
		return -3;
	}

	*temperature = (temp_raw/ 65536.0)  * 165.0-40.0;
       	*humidity = (humidity_raw/65536.0) * 100.0;	

	return 0;
}
