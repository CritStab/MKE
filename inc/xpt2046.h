#include "MK02F12810.h"                 // Device header
#include "spi.h"
#include "delay.h"

namespace Xpt2046Def
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

//MOSI
const Gpio::Port MosiPort = Gpio::E;
const uint8_t MosiPin = 18;

const Spi::CTAR_number CtarNumber = Spi::CTAR0;
}

#ifndef XPT2046_H
#define XPT2046_H


const uint8_t chy = 0x90;
const uint8_t chx = 0xD0;

class Xpt2046
{
//variables
public:

private:
	Spi * mod;
	uint16_t x,y;

//functions
public:
	Xpt2046 (Spi &s);
	void setMode ();
	void getXY ();
	uint16_t & getX (){return x;}
	uint16_t & getY (){return y;}
private:
	void transmitt (uint8_t val);
	uint16_t getData (uint8_t adress);
};

#endif
