#include "MKE04Z1284.h"                // Device header
#include "tact.h"
#include "delay.h"
#include "hd44780.h"
#include "buffer.h"
#include "pit.h"
#include "pin.h"
#include "senc.h"
#include "systimer.h"
//#include "button.h"
#include "filter.h"



Tact frq;
Hd44780 lcd;
Buffer val;
Pit pit1 (Pit::channel::ch1, 1000, Pit::mode::ms);
Pin pin1 (Gpio::Port::H, 0, Gpio::out::PushPull);
Pin button (Gpio::Port::I, 4, Gpio::PP::PullDown);
Senc encoder (Gpio::Port::B, 4, Gpio::Port::B, 5, 1000);
//Button butt (Gpio::Port::I, 4);
Filter filters;


extern "C" {
	void SysTick_Handler();
	void PIT_CH1_IRQHandler();
	void IRQ_IRQHandler();
}


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

void action ();

void SysTick_Handler()
{
	pin1.togle();
}

void PIT_CH1_IRQHandler()
{
	pit1.clear_flag();
	/*if (!button.state())
	encoder.scan();*/

}


void initIrq ();

void IRQ_IRQHandler()
{
	IRQ->SC |= IRQ_SC_IRQACK_MASK;
	pin1.set();
}

int main()
{

	lcd.sendString("Hello!!!");
	lcd.setPosition(1, 0);
	val.setFont (Array_char);

	encoder.setValue(50);


	filters.setFltDiv2 (Filter::busclkDivision::div4096);
	filters.setFltDiv3(Filter::lpoclkDivision::div32);
	//set 5.8kHz for encoder
	//filters.setFilter(Filter::sourceFilter::IRQ_, Filter::clkFilter::busclkHigh);
	//set 62.5hz==16ms for button and tilt sensor
	//filters.setFilter(Filter::sourceFilter::PTI_, Filter::clkFilter::lpoclk);
	PORT->IOFLT1 |= 0x03;
	//filters.setFilter(Filter::sourceFilter::PTE, Filter::clkFilter::lpoclk);
	//initIrq ();

	pit1.interrupt_enable();
	pit1.start();
/*
	//SysTick_BASE

	 SysTick->LOAD=0xFFFFFF;		// Загрузка значения
	 SysTick->VAL=48000;		// Обнуляем таймеры и флаги. Записью, помните?

	 SysTick->CTRL=	SysTick_CTRL_CLKSOURCE_Msk |
	                SysTick_CTRL_TICKINT_Msk   |
	                SysTick_CTRL_ENABLE_Msk;*/
	 Systimer (Systimer::mode::us, 250);


	while (1)
	{
		//encoder.scan();
		lcd.setPosition(1, 0);
		val.parsDec16(encoder.getValue());
		lcd.sendString(val.getContent());
		delay_ms(1);
	}
}

void action ()
{
	pin1.togle();
}


void initIrq ()
{
	Pin irqPin (Gpio::Port::I, 4, Gpio::PP::PullUp);
	SIM->SCGC |= SIM_SCGC_IRQ_MASK;
	IRQ->SC &= ~(IRQ_SC_IRQEDG_MASK|IRQ_SC_IRQMOD_MASK|IRQ_SC_IRQPDD_MASK) ;
	IRQ->SC |= IRQ_SC_IRQPE_MASK| IRQ_SC_IRQIE_MASK;
	NVIC_EnableIRQ(IRQ_IRQn);
}

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
/*
void initIrq ()
{


	PORT->IOFLT0 &=~ PORT_IOFLT0_FLTDIV2_MASK;
	PORT->IOFLT0 |=PORT_IOFLT0_FLTDIV2(2);
	PORT->IOFLT1 &= ~PORT_IOFLT1_FLTIRQ_MASK;
	PORT->IOFLT1 |= PORT_IOFLT1_FLTIRQ(2);

	//falling edge
	IRQ->SC &= ~ (IRQ_SC_IRQEDG_MASK | IRQ_SC_IRQMOD_MASK);
	IRQ->SC |= IRQ_SC_IRQPDD_MASK | IRQ_SC_IRQPE_MASK;
}*/

void initFilter (Gpio &pin, uint8_t pin_)
{
	pin.settingPin(pin_, Gpio::mode::Input);

}
