#include "MKE04Z1284.h"                // Device header
#include "tact.h"
#include "delay.h"
#include "ssd1289.h"



Tact frq;



extern "C" {
	void SysTick_Handler();
	void PIT_CH1_IRQHandler();
	void IRQ_IRQHandler();
}

const uint8_t ArialBlack_8_data[] = {
0x00,0x00,0x20,0x05,0x08,0x25,0xE2,0x30,0x0A,0x62};

int main()
{

	Ssd1289 display;
	Ssd1289::sFont lat;
	lat.height = 10;
	lat.width = 1;
	lat.shift = 0;
	lat.font = ArialBlack_8_data;
	display.fillScreen(ssd1289Color::GREEN);
	//display.drawArr(50,50,ssd1289Color::BLUE, ssd1289Color::RED, ArialBlack_8_data,2);
	display.symbol(100,100, ssd1289Color::BLUE, ssd1289Color::RED, 1, lat);
	display.horLine(10, 100, ssd1289Color::YELLOW, 200, 5);
	while (1)
	{

	}
}
