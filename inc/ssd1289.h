#include "MKE04Z1284.h"
#include "delay.h"
#include "i8080.h"

#ifndef SSD1289_H
#define SSD1289_H

class Ssd1289 : protected I8080
{
//variables
public:
	struct sFont
	{
		const uint8_t * font;
		uint8_t width;
		uint8_t height;
		uint8_t shift;
	};
protected:
private:

//functions
public:
	Ssd1289();
	void pixel (uint16_t x , uint16_t y, const uint16_t color);
	void fillScreen (uint16_t color);
	void symbol (uint16_t x, uint16_t y, const uint16_t color, const uint16_t fon, const uint8_t ch, sFont & s);
	void string (uint16_t x, uint16_t y, const uint16_t color, const uint16_t fon, const char *str, sFont &f, uint8_t interval);
	void setCursor (uint16_t x , uint16_t y);
	void drawArr (uint16_t x , uint16_t y, const uint16_t color, const uint16_t fon, const uint8_t *arr, uint8_t l);
	void drawPic (uint16_t x , uint16_t y, const uint16_t *arr, uint8_t length, uint16_t width);
	void horLine (uint16_t x, uint16_t y, uint16_t color, uint16_t length, uint8_t thick);
	void verLine (uint16_t x, uint16_t y, uint16_t color, uint16_t length, uint8_t thick);
	void line (uint16_t x, uint16_t y, uint16_t color, uint16_t length, uint8_t thick);
	void rectangle (uint16_t x, uint16_t y, uint16_t color, uint16_t length, uint8_t width, uint8_t thick);
protected:
private:
	void init();

}; //ssd1289

#endif //__SSD1289_H__
