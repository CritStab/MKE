#include "horline.h"

Horline::Horline(uint16_t x_, uint16_t y_, uint16_t color_, uint16_t length_, uint8_t thick_)
:x(x_), y(y_), color (color_), length (length_), thick (thick_)
{

}

void Horline::draw () const
{
	driver->horLine (x, y, color, length, thick);
}
