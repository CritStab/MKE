#include "MK02F12810.h"                  // Device header
#include "gpio.h"
#include "delay.h"
#include "pit.h"
#include "tact.h"
#include "adc.h"
#include "buffer.h"
#include "hd44780.h"

Tact frq;
const char led = 0;
Gpio E(Gpio::E);
Pit del(Pit::ch0, 1, Pit::ms);
Buffer val (4);
Hd44780 display;

const char adc_ch = 1;
uint16_t calibr;


bool init_adc ();
uint16_t conv_adc(uint8_t pin);

struct flags
{
	unsigned led_indicator_delay : 1;
}flag;


extern "C"
{
	void PIT_IRQHandler();
}

void PIT_IRQHandler()
{
	del.clear_flag();
}

uint16_t result [4];

int main()
{
	init_adc ();
	display.set_position(0,2);
	display.send_string("Adc value:");
	while (1)
	{
		display.set_position(1,0);
		val.pars(conv_adc(adc_ch));
		display.send_string(4, val.getArray());
		delay_ms(200);
	}
}

bool init_adc ()
{
	//tact ADC0
	SIM->SCGC6 |= SIM_SCGC6_ADC0_MASK;
	//Set busclock
	ADC0->CFG1 &= ~ADC_CFG1_ADICLK_MASK;
	//Set divider - 8
	ADC0->CFG1 |= ADC_CFG1_ADIV(3);
	ADC0->SC3 |= ADC_SC3_AVGE_MASK|ADC_SC3_AVGS(3);
	ADC0->SC2 &= ~ ADC_SC2_ADTRG_MASK;
	ADC0->SC3 |= ADC_SC3_CAL_MASK ;
	while (!(ADC0->SC1[0]&ADC_SC1_COCO_MASK));
	if (ADC0->SC3&ADC_SC3_CALF_MASK) return false;
	calibr = ADC0->CLP0+ADC0->CLP1+ADC0->CLP2+ADC0->CLP3+ADC0->CLP4+ADC0->CLPS;
	calibr >>=1;
	ADC0->CFG1|= ADC_CFG1_ADLSMP_MASK|ADC_CFG1_MODE(2);
	return true;

}

uint16_t conv_adc(uint8_t pin)
{
	//Select 4 channal and start conversation
	ADC0->SC1[0] = ADC_SC1_ADCH(pin);
	while (!(ADC0->SC1[0]&ADC_SC1_COCO_MASK));
	return ADC0->R[0];
}
