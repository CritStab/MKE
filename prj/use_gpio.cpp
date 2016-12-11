#include "MKE04Z1284.h"                 // Device header
#include "tact.h"
#include "pin.h"
#include "port.h"
#include "delay.h"
#include "ftm.h"
#include "pwm.h"

Tact frq;


int main ()
{

	Ftm ftm1 (Ftm::nFtm::FTM_1, Ftm::division::div128, 37500);
	//maped ftm2 ch2 at D0, ch3 at D1
	SIM->PINSEL1 &= ~(SIM_PINSEL1_FTM2PS2_MASK|SIM_PINSEL1_FTM2PS3_MASK);
	SIM->PINSEL1 |= SIM_PINSEL1_FTM2PS3(0x01)|SIM_PINSEL1_FTM2PS2(0x01);
	SIM->PINSEL |= SIM_PINSEL_FTM1PS0_MASK;
	Ftm ftm2 (Ftm::nFtm::FTM_2, Ftm::division::div32, 100);

	Pwm heater (ftm1, Ftm::channel::ch0, Pwm::mode::EdgePwm, Pwm::pulseMode::highPulse);
	Pwm fan (ftm2, Ftm::channel::ch3, Pwm::mode::EdgePwm, Pwm::pulseMode::highPulse);
	Pwm beeper (ftm2, Ftm::channel::ch2, Pwm::mode::EdgePwm, Pwm::pulseMode::highPulse);

	ftm2.start();
	ftm1.start();
	fan.setValue(50);
	heater.setValue(10000);
	Pin p (Gpio::Port::B, 4, Gpio::out::PushPull);
	p.set();

	while (1)
	{

	}
}
