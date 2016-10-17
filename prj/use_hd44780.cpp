#include "MK02F12810.h"                 // Device header
#include "gpio.h"
#include "tact.h"
#include "delay.h"
#include "hd44780.h"
#include "buffer.h"
#include "pit.h"


Tact frq;
Pit Stimer(Pit::ch1, 2000, Pit::ms);
Hd44780 display;
Buffer temp_iron(3);
Buffer temp_heat_gun(3);


const uint8_t led = 4;
const uint16_t val_iron = 230;
const uint16_t val_gun = 260;

extern "C" {
	void SysTick_Handler();
	void PIT1_IRQHandler();
}

void PIT1_IRQHandler()
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

const char * menu_iron = "Solder Iron";
const char * menu_heat_gun = "Heat Gun";

void menu();


int main()
{
	temp_iron.pars(val_iron);
	temp_heat_gun.pars(val_gun);
	menu();
	Stimer.interrupt_enable();
	Stimer.start();

	while (1)
	{
	}
}

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
}


