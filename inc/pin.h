#include "MKE04Z1284.h"                 // Device header
#include "gpio.h"

#ifndef PIN_H
#define PIN_H



class Pin: protected Gpio
{
//variables
public:

private:
	uint8_t pin_;
//functions
public:
	Pin (Port port, uint8_t p , out o);
	Pin (Port port, uint8_t p , PP m);
	Pin (Port port, uint8_t p);
	void setIn (PP pp_);
	void setOut (out o=out::PushPull);
	void set();
	void clear ();
	void togle ();
	bool state ();
};

#endif



