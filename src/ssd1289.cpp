#include "ssd1289.h"


Ssd1289::Ssd1289()
{
	delay_ms(100);
	rst.set();
	wReg(0x00,0x0001);
	wReg(0x03,0xA8A4);
	wReg(0x0C,0x0000);
	wReg(0x0D,0x080C);
	wReg(0x0E,0x2B00);
	wReg(0x1E,0x00B7);
	wReg(0x01,0x2B3F);
	wReg(0x02,0x0600);
	wReg(0x10,0x0000);
	delay_ms(30);
	wReg(0x11,0x6058);
	wReg(0x05,0x0000);
	wReg(0x06,0x0000);
	wReg(0x16,0xEF1C);
	wReg(0x17,0x0003);
	wReg(0x07,0x0233);
	wReg(0x0B,0x0000);
	wReg(0x0F,0x0000);
	wReg(0x41,0x0000);
	wReg(0x42,0x0000);
	wReg(0x48,0x0000);
	wReg(0x49,0x013F);
	wReg(0x4A,0x0000);
	wReg(0x4B,0x0000);
	wReg(0x44,0xEF00);
	wReg(0x45,0x0000);
	wReg(0x46,0x013F);
	wReg(0x30,0x0707);
	wReg(0x31,0x0204);
	wReg(0x32,0x0204);
	wReg(0x33,0x0502);
	wReg(0x34,0x0507);
	wReg(0x35,0x0204);
	wReg(0x36,0x0204);
	wReg(0x37,0x0502);
	wReg(0x3A,0x0302);
	wReg(0x3B,0x0302);
	wReg(0x23,0x0000);
	wReg(0x24,0x0000);
	wReg(0x25,0x8000);
	wReg(0x4f,0x0000);
	wReg(0x4e,0x0000);
	index(0x22);
}

void Ssd1289::pixel (uint16_t x , uint16_t y, const uint16_t color)
{
	setCursor(x, y);
	index(0x0022);
	data (color);
}

void Ssd1289::fillScreen (uint16_t color)
{
	setCursor(0, 0);
	index(0x0022);
	cs.clear();
	//data
	rs.set();
	low.set(color&0x0044);
	high.set(color>>8);
	for (uint32_t i=0;i<76800;++i)
	{
		strob();
	}
	cs.set();
}

void Ssd1289::symbol (uint16_t x, uint16_t y, const uint16_t color, const uint16_t fon, const uint8_t ch, sFont & s)
{
	const uint8_t * ptrFont = s.font;
	uint16_t colors [2] = {fon, color};

	for (uint8_t i=0;i<s.height;++i, ++ptrFont)
	{
		cs.set();
		setCursor(x, y+i);
		index(0x0022);
		cs.clear();
		//data
		rs.set();
		for (uint8_t j=0;j<s.width;++j, ++ptrFont)
		{
			for (uint8_t k=0;k<8;++k) putData(colors [*ptrFont&(1 << (7-k))]);
		}
	}
}

void Ssd1289::string (uint16_t x, uint16_t y, const uint16_t color, const uint16_t fon, const char *str, sFont &s, uint8_t interval)
{
	const uint8_t * ptrFont = s.font;
	uint16_t colors [2] = {fon, color};
}

void Ssd1289::setCursor (uint16_t x , uint16_t y)
{
	wReg(0x004e, x);
	wReg(0x004f, y);
}

void Ssd1289::drawArr (uint16_t x , uint16_t y, uint16_t color, uint16_t fon, const uint8_t *arr, uint8_t l)
{
	uint16_t colors [2] = {fon, color};
	setCursor(x, y);
	index(0x0022);
	cs.clear();
	//data
	rs.set();
	for (uint8_t j=0;j<l;++j, ++arr)
	{
		for (uint8_t k=0;k<8;++k) putData(colors [*arr&(1 << (7-k))]);
	}
	cs.set();
}

void Ssd1289::horLine (uint16_t x, uint16_t y, uint16_t color, uint16_t length, uint8_t thick)
{
	for (uint8_t i=0;i<thick;++i)
	{
		cs.set();
		setCursor(x, y+i);
		index(0x0022);
		cs.clear();
		//data
		rs.set();
		low.set(color&0x0044);
		high.set(color>>8);
		for (uint8_t j=0;j<length;++j) strob();
	}
	cs.set();
}
