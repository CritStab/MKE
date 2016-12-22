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



extern "C" {
	void SysTick_Handler();
	void PIT_CH1_IRQHandler();
	void FTM0_IRQHandler ();
}



const uint16_t speedVal=8;


const uint16_t coolingSpeed1 = 920;



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

Tact frq;
Hd44780 lcd;


Button buttonEncoder (Gpio::Port::C, 7);
Pin tilt (Gpio::Port::A, 0, Gpio::PP::PullDown);
Buffer value;
Senc encoder (Gpio::Port::C, 6, Gpio::Port::E, 2);
Pit updateLcd (Pit::channel::ch1, 100, Pit::mode::ms);
Ftm ftm1 (Ftm::nFtm::FTM_1, Ftm::division::div128, 37500);
Ftm ftm2 (Ftm::nFtm::FTM_2, Ftm::division::div8, 1000);
Pwm heater (ftm1, Ftm::channel::ch0, Pwm::mode::EdgePwm, Pwm::pulseMode::highPulse);
Pwm fan (ftm2, Ftm::channel::ch3, Pwm::mode::EdgePwm, Pwm::pulseMode::highPulse);
Pwm beeper (ftm2, Ftm::channel::ch2, Pwm::mode::EdgePwm, Pwm::pulseMode::highPulse);



typedef void (*PtrF)();

struct flags
{
  unsigned encLongPress : 1;
  unsigned encShortPress : 2;
  unsigned encReady : 1;
  unsigned beeper : 2;
  unsigned tilt : 1;
}flag;

struct position
{
  uint8_t row;
  uint8_t coloumn;
}speedCursor, heaterCursor;

struct data
{
  uint16_t value;
  uint16_t lowLimit;
  uint16_t highLimit;
  uint8_t grade;
  position pos;	
}speed, heaterPwm;

position * ScreenCursor [2] = {&speedCursor, &heaterCursor};

enum newChar {celsius, cursor};

data * ScreenVal [2]= {&speed, &heaterPwm};

uint16_t speedV [10] = {700, 760, 800, 840, 880, 920, 940, 960, 980, 1000};

void mainScreen ();

//button action
void changeLpFlag ();
void changeSpFlag ();
void initHeater ();
void initFun ();
void scan_enc ();
void initPosition ();
void initDataPosition ();
void clearCursors ();
void clearCursor ();
void initIrq ();
void initPwm ();

void PIT_CH1_IRQHandler()
{
	updateLcd.clear_flag();
	//update screen
	if (!tilt.state())
	{
		fan.setValue(coolingSpeed1);
		heater.setValue(4000);
	}
	else
	{
		//update fan speed
		fan.setValue(speedV [speed.value]);
		//update heater value
		heater.setValue(heaterPwm.value);
	}

	buttonEncoder.scanAction();
	//update value
	if (flag.encLongPress) ScreenVal [flag.encShortPress]->value = encoder.getValue ();

	//add beeper
	if (!flag.beeper) beeper.setValue(0);
	else
	{
		beeper.setValue(1000);
		flag.beeper = 0;
	}

	//===draw value===//
	//speed
	lcd.setPosition (speed.pos.row, speed.pos.coloumn);
	value.parsDec16 (speed.value, 3);
	lcd.sendString (value.getElement(2));
	//heater
	lcd.setPosition(heaterPwm.pos.row, heaterPwm.pos.coloumn);
	value.parsDec16 (heaterPwm.value, 4);
	lcd.sendString (value.getElement(1));
}

void SysTick_Handler()
{
    buttonEncoder.scanButton ();
    if (flag.encLongPress)encoder.scan ();
}

int main()
{
  mainScreen ();
  buttonEncoder.setLongLimit (4000);
  buttonEncoder.setShortLimit (40);
  
  buttonEncoder.setlongPressAction (changeLpFlag);
  buttonEncoder.setshortPressAction (changeSpFlag);
  value.setFont (Array_char);

  initPosition ();
  initDataPosition ();


  //map pwm pins
  initPwm ();
  ftm2.start();
  ftm1.start();

  updateLcd.interrupt_enable();
  updateLcd.start();

  Systimer mainLoop (Systimer::mode::us, 250);
  
  while (1)
  {
  }
}


void initPwm ()
{
	//maped ftm1 ch0 at H2
	SIM->PINSEL |= SIM_PINSEL_FTM1PS0_MASK;


	//maped ftm2 ch2 at D0, ch3 at D1
	SIM->PINSEL1 &= ~(SIM_PINSEL1_FTM2PS2_MASK | SIM_PINSEL1_FTM2PS3_MASK);
	SIM->PINSEL1 |= SIM_PINSEL1_FTM2PS3(0x01) | SIM_PINSEL1_FTM2PS2(0x01);
}

void initPosition ()
{
  speedCursor.coloumn = 8;
  speedCursor.row = 0;
  heaterCursor.coloumn = 0;
  heaterCursor.row = 1;
}

void initDataPosition ()
{
  speed.value = speedVal;
  speed.highLimit = 10;
  speed.lowLimit = 0;
  speed.pos.coloumn = 11;
  speed.pos.row = 0;
  speed.grade = 1;
  heaterPwm.value = 7500;
  heaterPwm.highLimit = 9900;
  heaterPwm.lowLimit = 3000;
  heaterPwm.pos.coloumn = 7;
  heaterPwm.pos.row = 1;
  heaterPwm.grade = 100;
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
  lcd.setPosition (1, 1);
  lcd.sendString ("Heater");
  speedCursor.row = 0;
  speedCursor.coloumn = 9;
  heaterCursor.row = 1;
  heaterCursor.coloumn = 0;
}


void changeLpFlag ()
{
  if (flag.encLongPress) 
  {
    flag.encLongPress = 0;
    clearCursor ();
  }
  else 
  {
    flag.encLongPress = 1;
    flag.encShortPress = 0;

    //set encoder value + limits
    encoder.setValue (ScreenVal [flag.encShortPress]->value);
    encoder.setHigh (ScreenVal[flag.encShortPress]->highLimit);
    encoder.setLow (ScreenVal[flag.encShortPress]->lowLimit);
    encoder.setGrade(ScreenVal[flag.encShortPress]->grade);

    lcd.setPosition (ScreenCursor[flag.encShortPress]->row, ScreenCursor[flag.encShortPress]->coloumn);
    lcd.data (cursor);
  }
}

void changeSpFlag ()
{
  if (flag.encLongPress)
  {
	  flag.beeper = 1;
	  ScreenVal [flag.encShortPress]->value = encoder.getValue ();
	  clearCursor ();
	  flag.encShortPress ^= 1;
	  //update cursor position
	  lcd.setPosition (ScreenCursor[flag.encShortPress]->row, ScreenCursor[flag.encShortPress]->coloumn);
	  lcd.data (cursor);

	  //update encoder value
	  encoder.setValue (ScreenVal[flag.encShortPress]->value);
	  encoder.setHigh (ScreenVal [flag.encShortPress]->highLimit);
	  encoder.setLow (ScreenVal [flag.encShortPress]->lowLimit);
	  encoder.setGrade(ScreenVal[flag.encShortPress]->grade);
  }
}
	

void clearCursor ()
{
	lcd.setPosition (ScreenCursor[flag.encShortPress]->row, ScreenCursor[flag.encShortPress]->coloumn);
	lcd.data (' ');
}

