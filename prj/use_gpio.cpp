#include "MKE04Z1284.h"                 // Device header
#include "tact.h"
#include "gpio.h"

Tact frq;
Gpio B (Gpio::B);

const uint8_t led = 4;

int main ()
{
	B.settingPin(led);
	B.setPin(led);

	while (1)
	{

	}
}
