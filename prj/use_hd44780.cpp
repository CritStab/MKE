#include "MKE04Z1284.h"
#include "tact.h"
#include "delay.h"
#include "hd44780.h"
#include "senc.h"
#include "buffer.h"
#include "button.h"
#include "senc.h"
#include "pid.h"
#include "pit.h"
#include "systimer.h"
#include "adc.h"
#include "ftm.h"
#include "pwm.h"



Tact frq;
Hd44780 lcd;
Button buttonEncoder (Gpio::Port::C, 7);
Pin tilt (Gpio::Port::A, 0, Gpio::PP::PullDown);
Buffer value;
Pid regulator (9, 2, 3, 300);
Senc encoder (Gpio::Port::C, 6, Gpio::Port::E, 2);
Pit updateLcd (Pit::channel::ch1, 100, Pit::mode::ms);
Adc sensor (Adc::channel::SE1, Adc::resolution::bit_12, Adc::buffer::buffer8);
Ftm ftm1 (Ftm::nFtm::FTM_1, Ftm::division::div128, 37500);
Ftm ftm2 (Ftm::nFtm::FTM_2, Ftm::division::div32, 100);
Pwm heater (ftm1, Ftm::channel::ch0, Pwm::mode::EdgePwm, Pwm::pulseMode::highPulse);
//Pwm fan (ftm2, Ftm::channel::ch3, Pwm::mode::EdgePwm, Pwm::pulseMode::highPulse);
//Pwm beeper (ftm2, Ftm::channel::ch2, Pwm::mode::EdgePwm, Pwm::pulseMode::highPulse);


extern "C" {
	void SysTick_Handler();
	void PIT_CH1_IRQHandler();
	void IRQ_IRQHandler();
}


const char cursorChar[8] =
{
0x18,
0x1C,
0x1E,
0x1F,
0x1F,
0x1E,
0x1C,
0x18,
};

const char celsiusChar[8] =
{
0x18,
0x18,
0x00,
0x06,
0x09,
0x08,
0x09,
0x06,
};

enum newChar {celsius, cursor};

struct position
{
  uint8_t row;
  uint8_t coloumn;
}speedCursor, tempCursor, pCursor, iCursor, dCursor;




void mainScreen ();
void pidScreen ();


int main()
{

	mainScreen ();
	pidScreen ();


	while (1)
	{

	}
}


void mainScreen ()
{
  lcd.newChar (celsiusChar, celsius);
  lcd.newChar (cursorChar, cursor);
  lcd.setPosition (0, 0);
  lcd.sendString ("HeatGun");
  lcd.setPosition (0, 9);
  lcd.sendString ("F=");
  lcd.setPosition (0, 14);
  lcd.data ('%');
  lcd.setPosition (1, 0);
  lcd.sendString ("Tc=");
  lcd.setPosition (1, 6);
  lcd.data (0);
  lcd.setPosition (1, 9);
  lcd.sendString ("Ts=");
  lcd.setPosition (1, 15);
  lcd.data (0);
  lcd.setPosition (1, 7);
  lcd.data (0xFF);
  lcd.setPosition (0, 7);
  lcd.data (0xFF);
  speedCursor.row = 0;
  speedCursor.coloumn = 9;
  tempCursor.row = 1;
  tempCursor.coloumn = 8;
}

void pidScreen ()
{
  lcd.setPosition (0, 17);
  lcd.data ('P');
  lcd.setPosition (0, 19);
  lcd.data ('.');
  lcd.setPosition (0, 22);
  lcd.sendString ("I");
  lcd.setPosition (0, 24);
  lcd.data ('.');
  lcd.setPosition (0, 27);
  lcd.sendString ("D");
  lcd.setPosition (0, 29);
  lcd.data ('.');
  lcd.setPosition (1, 17);
  lcd.sendString("PID");
}

