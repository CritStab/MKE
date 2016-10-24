#include "MKE04Z1284.h"                // Device header
#include "gpio.h"
#include "tact.h"
#include "delay.h"
#include "hd44780.h"
#include "buffer.h"
#include "pit.h"
#include "filter.h"


Tact frq;
Pit Stimer(Pit::ch1, 2000, Pit::ms);

Buffer temp_iron(3);
Buffer temp_heat_gun(3);
Buffer adcVal (3);
Gpio A (Gpio::Port::A);
Filter irqFilter;



const uint8_t led = 4;
const uint16_t val_iron = 230;
const uint16_t val_gun = 260;
const uint8_t adcPin = 1;

extern "C" {
	void SysTick_Handler();
	void PIT_CH1_IRQHandler();
}
/*
void PIT_CH1_IRQHandler()
{
	static bool flag;
	Stimer.clear_flag();
	if (flag)
	{
		display.Shift(Hd44780::Window, Hd44780::Left, 16);
		flag = 0;
	}
	else
	{
		display.command(clear_counter);
		flag = 1;
	}
}*/



uint8_t new_char0[8]
{ 0x1F,
0x1F,
0x1F,
0x1F,
0x1F,
0x1F,
0x1F,
0x1F,
};

const char * menu_iron = "Solder Iron";
const char * menu_heat_gun = "Heat Gun";

void menu();
void initAdc (uint8_t n);
uint16_t getAdc (uint8_t n);
void initIrq ();
void initFilter (Gpio &, uint8_t pin_);


int main()
{
	/*irqFilter.setFltDiv2(Filter::busclkDivision::div128);
	irqFilter.setFilter(Filter::sourceFilter::IRQ_, Filter::clkFilter::busclkHigh);
	initIrq();
	initAdc (adcPin);
	temp_iron.pars(val_iron);
	temp_heat_gun.pars(val_gun);*/
	//menu();
	/*Stimer.interrupt_enable();
	Stimer.start();*/
	Hd44780 display (Gpio::Port::C);
	display.set_position(0,0);
	display.send_string("Hello");
	while (1)
	{
		adcVal.pars(getAdc (adcPin));
		delay_ms(10);
	}
}
/*
void menu()
{
	display.set_position(0, 1);
	display.send_string(menu_iron);
	display.set_position(1, 1);
	display.send_string(temp_iron.getArray());
	display.set_position(0, 17);
	display.send_string(menu_heat_gun);
	display.set_position(1, 17);
	display.send_string(temp_heat_gun.getArray());
}*/

void initAdc (uint8_t n)
{
	SIM->SCGC |= SIM_SCGC_ADC_MASK|ADC_SC2_ADTRG_MASK;
	ADC->SC2 &= ~ ADC_SC2_REFSEL_MASK;
	//ADC->SC2 |=  ADC_SC2_REFSEL(1);

	ADC->SC3 &=  ~ (ADC_SC3_ADICLK_MASK|ADC_SC3_ADIV_MASK|ADC_SC3_MODE_MASK);
	ADC->SC3 |= ADC_SC3_ADIV(3) | ADC_SC3_MODE(2);
	ADC->APCTL1 |= 1 << n;
}

uint16_t getAdc (uint8_t n)
{
	ADC->SC1 = ADC_SC1_ADCH(n);
	while (!ADC->SC1&ADC_SC1_COCO_MASK);
	return ADC->R;
}

void initIrq ()
{
	SIM->SCGC |= SIM_SCGC_IRQ_MASK;

	PORT->IOFLT0 &=~ PORT_IOFLT0_FLTDIV2_MASK;
	PORT->IOFLT0 |=PORT_IOFLT0_FLTDIV2(2);
	PORT->IOFLT1 &= ~PORT_IOFLT1_FLTIRQ_MASK;
	PORT->IOFLT1 |= PORT_IOFLT1_FLTIRQ(2);

	//falling edge
	IRQ->SC &= ~ (IRQ_SC_IRQEDG_MASK | IRQ_SC_IRQMOD_MASK);
	IRQ->SC |= IRQ_SC_IRQPDD_MASK | IRQ_SC_IRQPE_MASK;
}

void initFilter (Gpio &pin, uint8_t pin_)
{
	pin.settingPin(pin_, Gpio::mode::Input);

}
