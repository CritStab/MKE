#include "MKE04Z1284.h"                // Device header
#include "tact.h"
#include "delay.h"
#include "ssd1289.h"
#include "shape.h"
#include "colors16bit.h"

Tact frq;


Shape * layer1[]= {};

extern "C" {
	void SysTick_Handler();
	void PIT_CH1_IRQHandler();
	void IRQ_IRQHandler();
}

const uint8_t ArialBlack_8_data[] = {
0x00,0x00,0x20,0x05,0x08,0x25,0xE2,0x30,0x0A,0x62};


void mainScreen (Ssd1289 &);


int main()
{
	Ssd1289 display;
	Ssd1289::sFont lat;
	lat.height = 10;
	lat.width = 1;
	lat.shift = 0;
	lat.font = ArialBlack_8_data;
	display.fillScreen(colors16bit::BLUE);
	//display.drawArr(50,50,ssd1289Color::BLUE, ssd1289Color::RED, ArialBlack_8_data,2);
	//display.symbol(100,100, ssd1289Color::BLUE, ssd1289Color::RED, 1, lat);
	//display.horLine(10, 100, ssd1289Color::YELLOW, 200, 5);
	//mainScreen(display);
	while (1)
	{

	}
}

void mainScreen (Ssd1289 & d)
{
	d.fillScreen(colors16bit::SILVER);

	//
	d.verLine(160, 0, colors16bit::BLACK, 240, 1);
	d.horLine(0, 120, colors16bit::BLACK, 320, 1);

	//gradient
	d.rectangle(5,5, colors16bit::BLACK,150, 110, 1);
	d.rectangle(165,5, colors16bit::BLACK,150, 110, 1);
	d.rectangle(5,125, colors16bit::BLACK,150, 110, 1);
	d.rectangle(165,125, colors16bit::BLACK,150, 110, 1);
}




