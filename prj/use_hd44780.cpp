#include "MKE04Z1284.h"                // Device header
#include "tact.h"
#include "delay.h"
#include "hd44780.h"
#include "buffer.h"
#include "pit.h"


Tact frq;

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

int main()
{
	Hd44780 lcd;
	lcd.send_string("Hello!!!");


	while (1)
	{
		delay_ms(1000);
	}
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
