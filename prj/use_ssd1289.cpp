#include "MKE04Z1284.h"                // Device header
#include "tact.h"
#include "delay.h"
#include "ssd1289.h"
#include "shape.h"
#include "pixel.h"
#include "verline.h"
#include "horline.h"
#include "rectangle.h"
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

void screen ();
void drawScreen (Shape *);


int main()
{
	Ssd1289 display;
	Ssd1289::sFont lat;
	lat.height = 10;
	lat.width = 1;
	lat.shift = 0;
	lat.font = ArialBlack_8_data;
	display.fillScreen(colors16bit::BLUE);

	Shape::driver = &display;
	delay_ms(2000);
	mainScreen(display);

	while (1)
	{

	}
}

void mainScreen (Ssd1289 & d)
{
	d.fillScreen(colors16bit::SILVER);

	d.verLine(160, 0, colors16bit::BLACK, 240, 1);
	d.horLine(0, 120, colors16bit::BLACK, 320, 1);


	//gradient
	d.rectangle(5,5, colors16bit::BLACK,150, 110, 1);
	d.rectangle(165,5, colors16bit::BLACK,150, 110, 1);
	d.rectangle(5,125, colors16bit::BLACK,150, 110, 1);
	d.rectangle(165,125, colors16bit::BLACK,150, 110, 1);
}

void screen ()
{
	Horline screen (0, 0, colors16bit::BLACK, 320, 240);
	Horline hLine (0, 120, colors16bit::WHITE, 320, 1);
	Verline vline (0, 120, colors16bit::WHITE, 320, 1);

	layer1[0] = &screen;
	layer1[1] = &hLine;
	layer1[2] = &vline;

}

void drawScreen (Shape * scr)
{
	uint8_t n = sizeof scr/sizeof scr[0];
	for (uint8_t i=0;i<n;++i)
	{
		scr->draw();
		scr++;
	}
}




