#include "MK02F12810.h"                 // Device header
#include "spi.h"
#include "tact.h"
#include "delay.h"
#include "pit.h"
#include "max6675.h"
#include "buffer.h"
#include "hd44780.h"
#include "shift_registr.h"




Tact frq;
Spi spi0;
Pit pit1 (Pit::ch1, 10, Pit::ms);
Max6675 sensor (spi0);
Shift reg (spi0);

Hd44780 display;
Buffer temp (4);

const char char_celsium [8]=
{
	0x18,
	0x18,
	0x00,
	0x06,
	0x09,
	0x08,
	0x09,
	0x06
};

extern "C" {
	void PIT1_IRQHandler();
}

void led_run ();

void PIT1_IRQHandler()
{
	pit1.clear_flag();
	sensor.readCelsius();
	temp.pars(sensor.getTemp());
	display.set_position(1, 1);
	display.send_string(3,temp.getArray());
	led_run ();
}



int main ()
{
	display.set_position(0, 1);
	display.send_string("Heat Gun");
	display.newChar(char_celsium, 0);
	display.set_position(1, 4);
	display.data(0);
	pit1.interrupt_enable();
	pit1.start();

	while (1)
	{

	}
}

void led_run ()
{
	for (int8_t i=0;i<8;++i)
	{
		reg.send(1 << i);
		delay_ms(100);
	}
	for (int8_t i=6;i>0;--i)
	{
		reg.send(1 << i);
		delay_ms(100);
	}
}

