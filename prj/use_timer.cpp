#include "MK02F12810.h"                 // Device header
#include "tact.h"
#include "delay.h"
#include "ftm.h"
#include "pwm.h"
#include "qd.h"
#include "hd44780.h"
#include "buffer.h"
#include "gpio.h"
#include "pit.h"

extern "C"
{
	void PIT1_IRQHandler();
}

Tact frq;
Pwm led_pwm (Pwm::FTM_0, Pwm::ch0, Pwm::EdgePwm, Pwm::highPulse);
//Qd encoder (1000);
Pit pit1 (Pit::ch1, 1);
Hd44780 display;
Buffer val (3);
Gpio B (Gpio::B);

const uint8_t period = 1000;
const uint8_t duty = 100;
uint16_t N;
const char encA = 1 ;
const char encB = 13;

struct encdr
{
	uint8_t state;
	uint16_t count;
}encod;

void scan_enc (Gpio &);
void PIT1_IRQHandler()
{
	pit1.clear_flag();/*
	scan_enc (B);
	display.set_position(1,0);
	display.send_string(3, val.getArray());
	led_pwm.setChannelValue(N);*/
}

void initPwm (Pwm &);
void encoder_hw ();
void encoder_sw (Gpio &);


int main ()
{
	Qd encoder (1000);
	//pit1.interrupt_enable();
	//pit1.start();
	//encod ();
	//encoder_hw ();
	//B.settingPin(encA, Gpio::GPIO);
	display.set_position(0, 0);
	display.send_string("CNT");
	display.set_position(1, 0);
	display.send_string("CNTIN");
	initPwm(led_pwm);
	while (1)
	{

		display.set_position(1,6);
		val.pars(FTM1->CNTIN);
		display.send_string(4, val.getArray());
		display.set_position(0,6);
		val.pars(FTM1->CNT>>2);
		display.send_string(4, val.getArray());
		led_pwm.setChannelValue(FTM1->CNT>>2);
		delay_ms(1);
	/*	B.changePinState(encA);
		delay_ms(1000);
		display.set_position(1,0);
		val.pars(encoder.getValue());
		display.send_string(3, val.getArray());
		led_pwm.setChannelValue(encoder.getValue());
		scan_enc ();
		display.set_position(1,0);
		display.send_string(3, val.getArray());
		led_pwm.setChannelValue(N);


		if (!A.pinState(encA))
		{


		for (uint8_t i=100;i>0;--i)
		{
			led_pwm.setChannelValue(i);
			delay_ms(10);
		}
		}*/
	}
}

void encoder_sw (Gpio & pins)
{
	pins.settingPin(encA, Gpio::GPIO, Gpio::Input);
	pins.settingPin(encB, Gpio::GPIO, Gpio::Input);
	pins.PuPdPin(encA, Gpio::On, Gpio::PullUp);
	pins.PuPdPin(encB, Gpio::On, Gpio::PullUp);
}


void initPwm (Pwm &p)
{
	p.setDivision(Pwm::div4);
	p.setPeriod(period);
	p.setChannelValue(duty);
	p.start();
}

void encoder_hw ()
{
	//SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;
	//enable the clock for FTM1
	  SIM_SCGC6 |= SIM_SCGC6_FTM1_MASK;
	  PORTB_PCR0 = PORT_PCR_MUX(6);
	  	  PORTB_PCR1 = PORT_PCR_MUX(6);

	/*//enable the counter
	  FTM1_MODE |= FTM_MODE_FTMEN_MASK;
	//enable the counter to run in the BDM mode
	  FTM1_CONF |= FTM_CONF_BDMMODE(3);
	//load the Modulo register and counter initial value
	  FTM1_MOD = 4095;
	  FTM1_CNTIN = 0;
	//configuring FTM for quadrature mode
	  FTM1_QDCTRL |= FTM_QDCTRL_QUADEN_MASK;
	// start the timer clock, source is the external clock
	FTM1_SC |= FTM_SC_CLKS(1);
	//configuring the input pins:
	  //PORTA_PCR12 = PORT_PCR_MUX(7); // FTM1 CH0
	  //PORTA_PCR13 = PORT_PCR_MUX(7); // FTM1 CH1*/

	  FTM1_SC=0x00;
	  FTM1_CNTIN=0x00;
	  FTM1_C0SC=0x00;
	  FTM1_C1SC=0x00;
	  //FTM1_MODE |= FTM_MODE_FTMEN_MASK;
	  //FTM1_CONF |= FTM_CONF_BDMMODE(3);
	  FTM1_QDCTRL|=0x01
			  //|FTM_QDCTRL_QUADMODE_MASK

			  ;
	  FTM1_MOD|=0xFFFF;
	  FTM1_QDCTRL|= FTM_QDCTRL_PHAFLTREN_MASK|FTM_QDCTRL_PHBFLTREN_MASK;
	  FTM1->FILTER = FTM_FILTER_CH0FVAL(2)|FTM_FILTER_CH1FVAL(2);
	  FTM1_SC |= FTM_SC_CLKS(2);

}

void scan_enc (Gpio & pins)
{
  uint8_t new_;
  new_ = (1 << pins.pinState(encA))|pins.pinState(encB);
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
    val.pars(N);

}
