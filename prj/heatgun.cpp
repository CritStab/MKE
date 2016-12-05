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
	void IRQ_IRQHandler();
}

//pid value

const double p  = 9.0;
const double i  = 2.0;
const double d  = 3.0;

const uint8_t buttEncPin = 0;
const uint8_t tiltPin = 2;

const uint8_t encAPin = 4;
const uint8_t encBPin = 5;

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
//Senc encoder (Gpio::Port::B, encAPin, Gpio::Port::B, encBPin, 100);
Pit updateLcd (Pit::channel::ch1, 100, Pit::mode::ms);
Adc sensor (Adc::channel::SE10, Adc::resolution::bit_12, Adc::buffer::buffer8);
Ftm ftm1 (Ftm::nFtm::FTM_1, Ftm::division::div32, 150);
Ftm ftm2 (Ftm::nFtm::FTM_2, Ftm::division::div128, 37500);
//Pwm heater (ftm2, Ftm::channel::ch3, Pwm::mode::EdgePwm, Pwm::pulseMode::highPulse);
//Pwm fan (ftm1, Ftm::channel::ch0, Pwm::mode::EdgePwm, Pwm::pulseMode::highPulse);
//Pwm beeper (ftm1, Ftm::channel::ch1, Pwm::mode::EdgePwm, Pwm::pulseMode::highPulse);
Filter filters;


typedef void (*PtrF)();

struct period_
{
  uint8_t lcd;
  uint8_t adc;
  uint8_t pid;
}period = {10, 40, 100};

struct encdr
{
  uint8_t state;
  uint16_t count;
}encod;

struct flags
{
  unsigned encLongPress : 1;
  unsigned encShortPress : 2;
  unsigned encReady : 1;
  unsigned screens :1;
  unsigned shift :2;
}flag;

struct position
{
  uint8_t row;
  uint8_t coloumn;
}speedCursor, tempCursor, pCursor, iCursor, dCursor;

struct data
{
  uint16_t value;
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
}


void SysTick_Handler()
{
    buttonEncoder.scanButton ();
    
 /* //опрос энкодера при длительном нажатии кнопки
  if (flag.encLongPress)encoder.scan ();
  if (flag.encLongPress)
  {
	  clearCursors ();
      lcd.setPosition (ScreenCursor[flag.screens][flag.encShortPress]->row, ScreenCursor[flag.screens][flag.encShortPress]->coloumn);
      lcd.data (cursor);
      ScreenVal [flag.screens][flag.encShortPress]->value = encoder.getValue ();
  }*/
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

	  currTemp.value = tempAdc >> 3;

//update PID
	regulator.setP (pVal.value);
	regulator.setI (iVal.value);
	regulator.setD (dVal.value);

	//calculate PID
	pidVal.value = regulator.compute (currTemp.value);
	/*heater.setValue(pidVal.value);

	//update fan speed
	fan.setValue(speed.value);
*/

}

void initIrq ();
void initPwm ();


int main()
{
  mainScreen ();
  pidScreen ();
  buttonEncoder.setLongLimit (1000);
  buttonEncoder.setShortLimit (10);
  
  buttonEncoder.setlongPressAction (changeLpFlag);
  buttonEncoder.setshortPressAction (changeSpFlag);
  value.setFont (Array_char);
  //init pwm
  initPosition ();
  initDataPosition ();

  filters.setFltDiv2 (Filter::busclkDivision::div4096);
  filters.setFilter(Filter::sourceFilter::IRQ_, Filter::clkFilter::busclkHigh);
 /* filters.setFltDiv3(Filter::lpoclkDivision::div16);
  //set 5.8kHz for encoder

  //set 62.5hz==16ms for button and tilt sensor
  filters.setFilter(Filter::sourceFilter::PTA, Filter::clkFilter::lpoclk);
  filters.setFilter(Filter::sourceFilter::PTE, Filter::clkFilter::lpoclk);
*/

  sensor.interrupt(true);

  updateLcd.interrupt_enable();
  updateLcd.start();
  //adcTrigger.start();

  Systimer mainLoop (Systimer::mode::ms, 1);
  
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
  speed.pos.coloumn = 11;
  speed.pos.row = 0;
  currTemp.value = 0;
  currTemp.pos.coloumn = 3;
  currTemp.pos.row = 1;
  setTemp.value = TsetVal;
  setTemp.pos.coloumn = 12;
  setTemp.pos.row = 1;
  pVal.value = regulator.getP();
  pVal.pos.coloumn = 18;
  pVal.pos.row = 0;
  iVal.value = regulator.getI ();
  iVal.pos.coloumn = 23;
  iVal.pos.row = 0;
  dVal.value = regulator.getD ();
  dVal.pos.coloumn = 28;
  dVal.pos.row = 0;
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
    //flag.encShortPress = flag.screens;
    clearCursors ();
  }
  else 
  {
    flag.encLongPress = 1;
    flag.encShortPress = 0;
    //encoder.setValue (ScreenVal [flag.screens][flag.encShortPress]->value);
    lcd.setPosition (ScreenCursor[flag.screens][flag.encShortPress]->row, ScreenCursor[flag.screens][flag.encShortPress]->coloumn);
    lcd.data (cursor);
   
  }
}

void changeSpFlag ()
{
  if (!flag.encLongPress) flag.screens ^= 1;
	
  else if (flag.encLongPress&&flag.screens)//screenPid
  {
    //ScreenVal [flag.screens][flag.encShortPress]->value = encoder.getValue ();
    flag.encShortPress++;
    if (flag.encShortPress>2) flag.encShortPress = 0;
    //encoder.setValue (ScreenVal [flag.screens][flag.encShortPress]->value);
  }
  else if (flag.encLongPress&&!flag.screens) //mainScreen
  {
    //ScreenVal [flag.screens][flag.encShortPress]->value = encoder.getValue ();
    if (flag.encShortPress) flag.encShortPress = 0;
    else flag.encShortPress = 1;
    //encoder.setValue (ScreenVal [flag.screens][flag.encShortPress]->value);
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
