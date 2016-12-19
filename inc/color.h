#include "MKE04Z1284.h"                // Device header



#ifndef COLOR_H
#define COLOR_H

class Color
{
public:
	enum class colorDepth {bit16, bit24};
	enum class rgb {red, green, blue};
private:
	union
	{
		uint32_t code;
		uint8_t color [3];
	}tempcolor;
	uint32_t rgb24;
	uint16_t rgb16;

public:
	Color (uint32_t val, colorDepth c);
	Color ();
	uint16_t rgb24toRgb16 (uint32_t val);
	void rgb16toRgb24 (uint16_t val);
	uint32_t & getRgb24 ();
	uint16_t & getRgb16 ();

};

#endif
