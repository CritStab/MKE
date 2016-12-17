#include "MKE04Z1284.h"                 // Device header
#include "gpio.h"

#ifndef PORT_H
#define PORT_H



class Port: protected Gpio
{
	//uint8_t val;
//functions
public:
	Port (Gpio::Port port, uint8_t value);
	void set(uint8_t value);
	void clear (uint8_t value);
	void togle (uint8_t value);
};

#endif



