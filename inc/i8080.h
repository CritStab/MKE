#include "MKE04Z1284.h"
#include "port.h"
#include "pin.h"

#ifndef I8080_H
#define I8080_H

namespace i8080def
{
	//Data bus 0-7
	const Gpio::Port lowDataPort = Gpio::Port::G;
	const uint8_t lowDataPins = 0xFF;
	//Data bus 8-15
	const Gpio::Port highDataPort = Gpio::Port::F;
	const uint8_t highDataPins = 0xFF;
	//CS
	const Gpio::Port csPort = Gpio::Port::D;
	const uint8_t csPin = 3;
	//RS
	const Gpio::Port rsPort = Gpio::Port::D;
	const uint8_t rsPin = 0;
	//WR
	const Gpio::Port wrPort = Gpio::Port::D;
	const uint8_t wrPin = 1;
	//RD
	const Gpio::Port rdPort = Gpio::Port::D;
	const uint8_t rdPin = 2;
	//RST
	const Gpio::Port rstPort = Gpio::Port::D;
	const uint8_t rstPin = 4;
}


class I8080
{
//variables
public:
	Port high, low;
	Pin rs, rst, wr, rd, cs;
protected:

private:



//functions
public:
	I8080();
	void index(uint16_t indx);
	void data(uint16_t dta);
	void putData (uint16_t dta);
	void wReg (uint16_t indx , uint16_t dta);
	void strob ();
protected:

private:


};

#endif
