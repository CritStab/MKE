#include "MKE04Z1284.h"                 // Device header
#include "tact.h"
#include "gpio.h"
#include "delay.h"


Tact frq;
Gpio B (Gpio::D);

const uint8_t led = 7;

int main ()
{
	B.settingPin(led);
	//SIM->SOPT0 &= ~SIM_SOPT0_NMIE_MASK;


	while (1)
	{
		B.toglePin(led);
		delay_ms(1000);
	}
}
