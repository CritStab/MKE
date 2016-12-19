#include "color.h"

Color::Color (uint32_t val, colorDepth c)
{
	tempcolor.code = val;

}
Color::Color ()
{

}

uint16_t Color::rgb24toRgb16 (uint32_t val)
{
	tempcolor.code = val;
	rgb16 = (tempcolor.color [(uint8_t)rgb::red]>>3) << 11;
	rgb16 |= (tempcolor.color [(uint8_t)rgb::green]>>2) << 5;
	rgb16 |= tempcolor.color [(uint8_t)rgb::blue] >> 3;
	return rgb16;
}
void Color::rgb16toRgb24 (uint16_t val)
{

}

uint32_t & Color::getRgb24 ()
{
	return rgb24;
}

uint16_t & Color::getRgb16 ()
{
	return rgb16;
}
