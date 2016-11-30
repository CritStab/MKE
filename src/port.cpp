#include "port.h"

Port::Port (Gpio::Port port, uint8_t value)
:Gpio(port)
{
	GpioBase [arrPort [prt]]->PDDR |= value << arrPinShift[prt];
}


void Port::set(uint8_t value)
{
	clear (value);
	GpioBase [arrPort [prt]]->PSOR |= value << arrPinShift[prt];
}

void Port::clear (uint8_t value)
{
	GpioBase [arrPort [prt]]->PCOR |= value << arrPinShift[prt];
}

void Port::togle (uint8_t value)
{
	GpioBase [arrPort [prt]]->PTOR |= value << arrPinShift[prt];
}
