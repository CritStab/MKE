#include "pin.h"

Pin::Pin (Port port, uint8_t p , out o)
:Gpio(port)
{
	pin_ = p;
	GpioBase [arrPort [prt]]->PDDR |= 1 << (pin_+arrPinShift[prt]);
	PORT->PUE0 |= (uint8_t)o << (pin_+arrPinShift[prt]);
}

Pin::Pin (Port port, uint8_t p , PP m)
:Gpio(port)
{
	pin_ = p;
	GpioBase [arrPort [prt]]->PDDR &= ~( 1 << (pin_+arrPinShift[prt]));
	PORT->PUE0 |= (uint8_t)m << (pin_+arrPinShift[prt]);
}

Pin::Pin (Port port, uint8_t p)
:Gpio(port)
{
	pin_ = p;
}

void Pin::setIn (PP pp_)
{
	GpioBase [arrPort [prt]]->PDDR &= ~( 1 << (pin_+arrPinShift[prt]));
	PORT->PUE0 |= (uint8_t)pp_ << (pin_+arrPinShift[prt]);
}

void Pin::setOut (out o)
{
	GpioBase [arrPort [prt]]->PDDR |= 1 << (pin_+arrPinShift[prt]);
	PORT->PUE0 |= (uint8_t)o << (pin_+arrPinShift[prt]);
}

void Pin::set()
{
	GpioBase [arrPort [prt]]->PSOR |= 1 << (pin_+arrPinShift[prt]);
}

void Pin::clear ()
{
	GpioBase [arrPort [prt]]->PCOR |= 1 << (pin_+arrPinShift[prt]);
}

void Pin::togle ()
{
	GpioBase [arrPort [prt]]->PTOR |= 1 << (pin_+arrPinShift[prt]);
}

bool Pin::state ()
{
	return (GpioBase [arrPort [prt]]->PDIR&(1 << (pin_+arrPinShift[prt])));
}
