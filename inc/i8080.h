#include "MKE04Z1284.h"
#include "port.h"
#include "pin.h"

#ifndef I8080_H
#define I8080_H

namespace i8080def
{
	//Data bus 0-7
	const Gpio::Port lowDataPort = Gpio::Port::D;
	const uint8_t lowDataPins = 0xFF;
	//Data bus 8-15
	const Gpio::Port highDataPort = Gpio::Port::D;
	const uint8_t highDataPins = 0xFF;
	//CS
	const Gpio::Port csPort = Gpio::Port::E;
	const uint8_t csPin = 1;
	//RS
	const Gpio::Port rsPort = Gpio::Port::C;
	const uint8_t rsPin = 5;
	//WR
	const Gpio::Port wrPort = Gpio::Port::E;
	const uint8_t wrPin = 0;
	//RD
	const Gpio::Port rdPort = Gpio::Port::E;
	const uint8_t rdPin = 0;
	//RST
	const Gpio::Port rstPort = Gpio::Port::E;
	const uint8_t rstPin = 0;
}


class I8080
{
//variables
public:

protected:
	Port high, low;
	Pin rs, rst, wr, rd, cs;
private:



//functions
public:
	I8080();

protected:
	void index(uint16_t indx);
	void data(uint16_t dta);
	void putData (uint16_t dta);
	void wReg (uint16_t indx , uint16_t dta);
	void strob ();
private:


};

#endif
