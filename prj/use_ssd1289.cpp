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

enum class color: uint8_t {red, green, blue};

union rgb24
{
	uint32_t code;
	uint8_t color[3];
};


void mainScreen (Ssd1289 &);

void screen ();
void drawScreen (Shape *);

const uint16_t colors [] = {colors16bit::BLACK, colors16bit::RED, colors16bit::BLUE,  colors16bit::GREEN, colors16bit::CYAN, colors16bit::MAGENTA,  colors16bit::YELLOW, colors16bit::WHITE,
 colors16bit::GRAY, colors16bit::SKY, colors16bit::ORANGE,  colors16bit::PINK, colors16bit::BROWN,colors16bit::VIOLET, colors16bit::SILVER,
 colors16bit::GOLD,  colors16bit::BEGH, colors16bit::NAVY, colors16bit::DARK_GREEN, colors16bit::DARK_CYAN, colors16bit::MAROON,  colors16bit::PURPLE,
colors16bit::LIGHT_GREY,  colors16bit::DARK_GREY};


int main()
{
	rgb24 purple;
	purple.code = 0x800080;
	uint8_t red = purple.color [(uint8_t)color::red];
	Ssd1289 display;
	Ssd1289::sFont lat;
	lat.height = 10;
	lat.width = 1;
	lat.shift = 0;
	lat.font = ArialBlack_8_data;
	display.fillScreen (colors16bit::WHITE);
	display.fillScreen (colors16bit::LIGHT_GREY);
	//display.drawArr(50,50,colors16bit::BLACK, colors16bit::WHITE, ArialBlack_8_data, 10);
	display.horLine(10,10, colors16bit::RED, 100, 5);
	display.symbol(50,50, colors16bit::WHITE, colors16bit::BLACK, 0, lat);


	/*Shape::driver = &display;

	mainScreen(display);
	display.drawPic(50, 50 )*/

	while (1)
	{
		uint8_t n = sizeof colors/sizeof *colors;
		for (uint8_t i=0;i<n;++i)
		{
			display.fillScreen(colors [i]);
			delay_ms(1000);
		}

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
	uint8_t n = sizeof scr/sizeof *scr;
	for (uint8_t i=0;i<n;++i)
	{
		scr->draw();
		scr++;
	}
}




