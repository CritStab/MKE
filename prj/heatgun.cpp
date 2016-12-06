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
#include "filter.h"

extern "C" {
	void SysTick_Handler();
	void ADC_IRQHandler();
	void PIT_CH1_IRQHandler();
}

//pid value

const double p  = 9.0;
const double i  = 2.0;
const double d  = 3.0;

const uint8_t buttEncPin = 0;
const uint8_t tiltPin = 2;

const uint8_t encAPin = 6;
const uint8_t encBPin = 7;

const uint16_t TsetVal=250;
const uint16_t speedVal=60;

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


Button buttonEncoder (Gpio::Port::A, buttEncPin);
//Button tilt (Gpio::Port::A, tiltPin);
Buffer value;
Pid regulator (p, i, d, TsetVal);
Senc encoder (Gpio::Port::C, encAPin, Gpio::Port::C, encBPin, 100);
Pit updateLcd (Pit::channel::ch1, 100, Pit::mode::ms);
Adc sensor (Adc::channel::SE10, Adc::resolution::bit_12, Adc::buffer::buffer8);
Ftm ftm1 (Ftm::nFtm::FTM_1, Ftm::division::div128, 37500);
Ftm ftm2 (Ftm::nFtm::FTM_2, Ftm::division::div32, 150);
Pwm heater (ftm1, Ftm::channel::ch1, Pwm::mode::EdgePwm, Pwm::pulseMode::highPulse);
Pwm fan (ftm2, Ftm::channel::ch0, Pwm::mode::EdgePwm, Pwm::pulseMode::highPulse);
Pwm beeper (ftm2, Ftm::channel::ch1, Pwm::mode::EdgePwm, Pwm::pulseMode::highPulse);
Filter filters;


typedef void (*PtrF)();

struct flags
{
  unsigned encLongPress : 1;
  unsigned encShortPress : 2;
  unsigned encReady : 1;
  unsigned screens :1;
  unsigned shift :2;
  unsigned beeper : 2;
}flag;

struct position
{
  uint8_t row;
  uint8_t coloumn;
}speedCursor, tempCursor, pCursor, iCursor, dCursor;

struct data
{
  uint16_t value;
  uint16_t lowLimit;
  uint16_t highLimit;
  position pos;	
}speed, currTemp, setTemp, pVal, iVal, dVal, pidVal;

position * ScreenCursor [2][3] = {
{&speedCursor, &tempCursor}, 
{&pCursor, &iCursor, &dCursor}
};

enum newChar {celsius, cursor};


data * ScreenVal [2] [4]= {
{&speed, &setTemp, &currTemp},
{&pVal, &iVal, &dVal, &pidVal}
};


void mainScreen ();
void pidScreen ();
void changeScreen ();
void getMainScreen ();
void getPidScreen ();
PtrF screenF [2] = {&getMainScreen, &getPidScreen};

//button action
void changeLpFlag ();
void changeSpFlag ();


void initHeater ();
void initFun ();
void scan_enc ();
void initPosition ();
void initDataPosition ();
void clearCursors ();

void PIT_CH1_IRQHandler()
{
	updateLcd.clear_flag();
	//update screen

	sensor.convertBuffer();
	buttonEncoder.scanAction();
	screenF [flag.screens]();

	//===draw value===//
	//draw main screen
	data **tempPtr = &ScreenVal[0][0];
	for (uint8_t i=0;i<3;++i)
	{
		lcd.setPosition ((*tempPtr)->pos.row, (*tempPtr)->pos.coloumn);
	    value.parsDec16 ((*tempPtr)->value, 3);
	    lcd.sendString (value.getElement(2));
	    *tempPtr++;
	}

	//draw pid screen
	tempPtr = &ScreenVal[1][0];
	for (uint8_t i=0;i<3;++i)
	{
		lcd.setPosition ((*tempPtr)->pos.row, (*tempPtr)->pos.coloumn);
	    value.parsFloat ((*tempPtr)->value);
	    lcd.sendString (value.getElement(2));
	    *tempPtr++;
	}

	//draw pid Value
	lcd.setPosition(pidVal.pos.row, pidVal.pos.coloumn);
	value.parsDec16 (pidVal.value);
	lcd.sendString (value.getContent());

	//update value
	if (flag.encLongPress) ScreenVal [flag.screens][flag.encShortPress]->value = encoder.getValue ();

	//add beeper
	if (!flag.beeper) beeper.setValue(0);
	else
	{
		beeper.setValue(50);
		flag.beeper = 0;
	}
}


void SysTick_Handler()
{
    buttonEncoder.scanButton ();
    if (flag.encLongPress)encoder.scan ();
}

void IRQ_IRQHandler()
{
	IRQ->SC |= IRQ_SC_IRQACK_MASK;
}

void ADC_IRQHandler()
{
uint16_t tempAdc = 0;
    for (uint8_t i=0;i<8;++i)
    {
      tempAdc += (sensor.getData() >> 2);
    }

	currTemp.value = tempAdc >> 4;

	//update PID
	regulator.setP (pVal.value);
	regulator.setI (iVal.value);
	regulator.setD (dVal.value);

	//calculate PID
	pidVal.value = regulator.compute (currTemp.value);
	fan.setValue(speed.value);

	//update heater value
	heater.setValue(pidVal.value);

}

void initIrq ();
void initPwm ();


int main()
{
  mainScreen ();
  pidScreen ();
  buttonEncoder.setLongLimit (4000);
  buttonEncoder.setShortLimit (40);
  
  buttonEncoder.setlongPressAction (changeLpFlag);
  buttonEncoder.setshortPressAction (changeSpFlag);
  value.setFont (Array_char);

  initPosition ();
  initDataPosition ();

  //interrupt ADC switch on
  sensor.interrupt(true);

  //set High value for PID regulator
  regulator.setPidLimit(37500);

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

void initIrq ()
{
	Pin irqPin (Gpio::Port::I, 4, Gpio::PP::PullUp);
	SIM->SCGC |= SIM_SCGC_IRQ_MASK;
	SIM->PINSEL &= ~ SIM_PINSEL_IRQPS_MASK;
	SIM->PINSEL |= SIM_PINSEL_IRQPS(0x05);
	IRQ->SC &= ~(IRQ_SC_IRQEDG_MASK|IRQ_SC_IRQMOD_MASK) ;
	IRQ->SC |= IRQ_SC_IRQPE_MASK| IRQ_SC_IRQIE_MASK|IRQ_SC_IRQPDD_MASK;
	NVIC_EnableIRQ(IRQ_IRQn);
}

void initPwm ()
{
	//maped ftm1 ch1 at E7
	SIM->PINSEL |= SIM_PINSEL_FTM1PS1_MASK;

	//maped ftm2 ch1 at c1, ch0 at c0
	SIM->PINSEL1 &= ~(SIM_PINSEL1_FTM2PS0_MASK|SIM_PINSEL1_FTM2PS1_MASK);
}

void initPosition ()
{
  speedCursor.coloumn = 8;
  speedCursor.row = 0;
  tempCursor.coloumn = 8;
  tempCursor.row = 1;
  pCursor.coloumn = 16;
  pCursor.row = 0;
  iCursor.coloumn = 21;
  iCursor.row = 0;
  dCursor.coloumn = 26;
  dCursor.row = 0;
}

void initDataPosition ()
{
  speed.value = speedVal;
  speed.highLimit = 100;
  speed.lowLimit = 0;
  speed.pos.coloumn = 11;
  speed.pos.row = 0;
  currTemp.value = 0;
  currTemp.pos.coloumn = 3;
  currTemp.pos.row = 1;
  setTemp.value = TsetVal;
  setTemp.highLimit = 450;
  setTemp.lowLimit = 0;
  setTemp.pos.coloumn = 12;
  setTemp.pos.row = 1;
  pVal.value = regulator.getP();
  pVal.highLimit = 99;
  pVal.lowLimit = 0;
  pVal.pos.coloumn = 18;
  pVal.pos.row = 0;
  iVal.value = regulator.getI ();
  iVal.highLimit = 99;
  iVal.lowLimit = 0;
  iVal.pos.coloumn = 23;
  iVal.pos.row = 0;
  dVal.value = regulator.getD ();
  dVal.highLimit = 99;
  dVal.lowLimit = 0;
  dVal.pos.coloumn = 28;
  dVal.pos.row = 0;
  pidVal.value = 0;
  pidVal.pos.coloumn = 21;
  pidVal.pos.row = 1;
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

void getMainScreen ()
{
  lcd.command (clear_counter);
  flag.shift = 0;
}

void getPidScreen ()
{
  if (!flag.shift)
  {
    lcd.setShiftPosition(16);
    flag.shift = 1;
  }
}

void changeLpFlag ()
{
  if (flag.encLongPress) 
  {
    flag.encLongPress = 0;
    clearCursors ();
  }
  else 
  {
    flag.encLongPress = 1;
    flag.encShortPress = 0;

    //set encoder value + limits
    encoder.setValue (ScreenVal [flag.screens][flag.encShortPress]->value);
    encoder.setHigh (ScreenVal [flag.screens][flag.encShortPress]->highLimit);
    encoder.setLow (ScreenVal [flag.screens][flag.encShortPress]->lowLimit);

    lcd.setPosition (ScreenCursor[flag.screens][flag.encShortPress]->row, ScreenCursor[flag.screens][flag.encShortPress]->coloumn);
    lcd.data (cursor);
  }
}

void changeSpFlag ()
{
  if (!flag.encLongPress)
  {
	  flag.screens ^= 1;
	  flag.beeper = 1;
  }

	
  else
  {
	   flag.beeper = 1;
	  ScreenVal [flag.screens][flag.encShortPress]->value = encoder.getValue ();
	  if (flag.screens)
	  {
		  flag.encShortPress++;
		  if (flag.encShortPress>2) flag.encShortPress = 0;
	  }
	  else
	  {
		  if (flag.encShortPress) flag.encShortPress = 0;
		  else flag.encShortPress = 1;
	  }
	  //update cursor position
	  clearCursors ();
	  lcd.setPosition (ScreenCursor[flag.screens][flag.encShortPress]->row, ScreenCursor[flag.screens][flag.encShortPress]->coloumn);
	  lcd.data (cursor);

	  //update encoder value
	  encoder.setValue (ScreenVal [flag.screens][flag.encShortPress]->value);
	  encoder.setHigh (ScreenVal [flag.screens][flag.encShortPress]->highLimit);
	  encoder.setLow (ScreenVal [flag.screens][flag.encShortPress]->lowLimit);
  }
}
	
void clearCursors ()
{
      //clear all cursors
      position **tPtr = &ScreenCursor[0][0];
      for (uint8_t i=0;i<2;++i)
      {
        lcd.setPosition ((*tPtr)->row, (*tPtr)->coloumn);
        lcd.data (' ');
        *tPtr++;
      }
      tPtr = &ScreenCursor[1][0];
       for (uint8_t i=0;i<3;++i)
      {
        lcd.setPosition ((*tPtr)->row, (*tPtr)->coloumn);
        lcd.data (' ');
        *tPtr++;
      }  
}
