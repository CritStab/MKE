#include "picture.h"

Picture::Picture (uint16_t x_, uint16_t y_, const uint16_t *arr, uint8_t length_, uint16_t width_)
:x(x_), y(y_), ptr (arr), length (length_), width (width_)
{

}

void Picture::draw () const
{
	driver->drawPic(x, y, ptr, length, width);
}

