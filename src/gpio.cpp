#include "gpio.h"

GPIO_Type * Gpio::GpioBase [3] = {GPIOA , GPIOB , GPIOC};
PORT_Type * Gpio::PortBase [3] = {};

uint8_t Gpio::arrPort [9] = {0,0,0,0,1,1,1,1,2};
uint8_t Gpio::arrPinShift [9] = {0,8,16,24,0,8,16,24,0};

Gpio::Gpio ()
{

}

Gpio::Gpio (Port p)
{
	prt = (uint8_t)p;
}

void Gpio::settingPinPort (Port p)
{
	prt = (uint8_t)p;
}

void Gpio::settingPin (uint8_t pin , mode m)
{
	GpioBase [arrPort [prt]]->PDDR &=~ 1 << (pin+arrPinShift[prt]);
	GpioBase [arrPort [prt]]->PDDR |= (uint8_t)m << (pin+arrPinShift[prt]);
}

void Gpio::PuPdPin (uint8_t pin, state s, PP p)
{
	PORT->PUE0 &=~ (1 << (pin+arrPinShift[prt]*prt));
	PORT->PUE0 |= (bool)s << (pin+arrPinShift[prt]*prt);
}


void Gpio::setPin (uint8_t pin )
{
	GpioBase [arrPort [prt]]->PSOR |= 1 << (pin+arrPinShift[prt]);
}

void Gpio::clearPin (unsigned char pin)
{
	GpioBase [arrPort [prt]]->PCOR |= 1 << (pin+arrPinShift[prt]);
}

void Gpio::toglePin (uint8_t pin)
{
	GpioBase [arrPort [prt]]->PTOR |= 1 << (pin+arrPinShift[prt]);
}

void Gpio::setPinState (uint8_t pin , bool state)
{
	clearPin (pin);
	GpioBase [arrPort [prt]]->PDOR = state << (pin+arrPinShift[prt]);
}

bool Gpio::pinState (uint8_t pin)
{
	return (GpioBase [arrPort [prt]]->PDIR&(1 << (pin+arrPinShift[prt])));
}

void Gpio::settingPort (uint8_t value, mode m )
{
	GpioBase [arrPort [prt]]->PDDR &=~ (value << arrPinShift[prt]);
	GpioBase [arrPort [prt]]->PDDR |= value << arrPinShift[prt];
}

void Gpio::setValPort (uint8_t value)
{
	GpioBase [arrPort [prt]]->PSOR |= value << arrPinShift[prt];
}

void Gpio::clearValPort (uint8_t value)
{
	GpioBase [arrPort [prt]]->PCOR |= value << arrPinShift[prt];
}
