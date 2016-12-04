#include "MKE04Z1284.h"                 // Device header
#include "tact.h"
#include "pin.h"
#include "delay.h"




Tact frq;


const uint8_t ledPin = 3;



int main ()
{
	SIM->SOPT0 &= ~SIM_SOPT0_NMIE_MASK;
	Pin led (Gpio::Port::A, ledPin, Gpio::out::PushPull);

	while (1)
	{
		led.togle();
		delay_ms(1000);
	}
}
