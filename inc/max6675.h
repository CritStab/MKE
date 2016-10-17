#include "MK02F12810.h"                 // Device header
#include "spi.h"
#include "delay.h"

namespace max6675def
{

const Gpio::Port CsPort = Gpio::E;
const uint8_t CsPin = 16;
const Spi::CS_number CsNumber = Spi::CS0;

//SCK
const Gpio::Port SckPort = Gpio::E;
const uint8_t SckPin = 17;

//MISO
const Gpio::Port MisoPort = Gpio::E;
const uint8_t MisoPin = 19;

const Spi::CTAR_number CtarNumber = Spi::CTAR0;
}

#ifndef MAX6675_H
#define MAX6675_H





class Max6675
{
//variables
public:
	char buffer_value[2];
	static char number [10];
private:
	Spi * mod;
	uint16_t byte_code;
//functions
public:
	Max6675 (Spi &s);
	void setMode ();
	bool readCelsius();
	uint16_t & getTemp ();
};

#endif
