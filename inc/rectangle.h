#include "MKE04Z1284.h"                // Device header
#include "shape.h"
#include "colors16bit.h"

#ifndef RECTANGLE_H
#define RECTANGLE_H

class Rectangle : public Shape
{
private:
	uint16_t x, y, color, length;
	uint8_t width, thick;

public:
	Rectangle (uint16_t x_, uint16_t y_, uint16_t color_, uint16_t length_, uint8_t width_, uint8_t thick_);
	void draw () const override;
};

#endif
