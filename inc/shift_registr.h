#include "MKE04Z1284.h"                 // Device header
#include "spi.h"
#include "gpio.h"

namespace ShiftDef
{
//===Defenitions===//
//CS
const Gpio::Port CsPort = Gpio::C;
const uint8_t CsPin = 0;
}

#ifndef SHIFT_REGISTR_H
#define SHIFT_REGISTR_H

class Shift
{
private:
	using ptrMode = void (Shift::*)();
	using ptrSend = void (Shift::*)(uint8_t);
	Spi::Mode mode_;
	Gpio CS;
	Spi * mod;
	static ptrMode modeFunc [2];
	static ptrSend sendFunc [2];

//functions
public:
	Shift (Spi &);
	void send (uint8_t data);

private:
	void setModeHw ();
	void setModeSw ();
	void sendHw (uint8_t data);
	void sendSw (uint8_t data);
};
#endif
