#include "MKE04Z1284.h"                // Device header
#include "tact.h"
#include "delay.h"
#include "ftm.h"
#include "pwm.h"
#include "gpio.h"
#include "pit.h"

extern "C"
{
	void PIT_CH1_IRQHandler();
}

Tact frq;

Pit pit1(Pit::ch1, 1);

const uint16_t period = 1000;
const uint8_t duty = 100;
uint16_t N;
const char encA = 1;
const char encB = 13;

struct encdr
{
	uint8_t state;
	uint16_t count;
}encod;

void scan_enc(Gpio &);
void PIT_CH1_IRQHandler()
{
	pit1.clear_flag();/*
					  scan_enc (B);
					  display.set_position(1,0);
					  display.send_string(3, val.getArray());
					  led_pwm.setChannelValue(N);*/
}

void initPwm(Pwm &);
void encoder_hw();
void encoder_sw(Gpio &);


int main()
{
	SIM->PINSEL |= SIM_PINSEL_FTM1PS1_MASK;
	Ftm timer1(Ftm::FTM_1, Ftm::div128);
	timer1.setPeriod(937);
	Pwm led_pwm(timer1, Ftm::ch1, Pwm::EdgePwm, Pwm::highPulse);
	//pit1.interrupt_enable();
	//pit1.start();
	//encod ();
	led_pwm.setValue(470);
	while (1)
	{
		/*
		for (uint16_t i=80;i<=100;i+=2)
		{
		led_pwm.setValue(i);
		delay_ms(3000);
		}
		for (uint16_t i=98;i>=80;i-=2)
		{
		led_pwm.setValue(i);
		delay_ms(3000);
		}*/
	}
}




void scan_enc(Gpio & pins)
{
	uint8_t new_;
	new_ = (1 << pins.pinState(encA)) | pins.pinState(encB);
	switch (encod.state)
	{
	case 2:
	{
		if (new_ == 3) encod.count++;
		if (new_ == 0) encod.count--;
		break;
	}
	case 0:
	{
		if (new_ == 2) encod.count++;
		if (new_ == 1) encod.count--;
		break;
	}
	case 1:
	{
		if (new_ == 0) encod.count++;
		if (new_ == 3) encod.count--;
		break;
	}
	case 3:
	{
		if (new_ == 1) encod.count++;
		if (new_ == 2) encod.count--;
		break;
	}
	}
	encod.state = new_;
	N = encod.count;
	//val.pars(N);

}
