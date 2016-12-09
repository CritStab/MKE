#include "MKE04Z1284.h"                // Device header
#include "shape.h"
#include "colors16bit.h"

#ifndef PIXEL_H
#define PIXEL_H

class Pixel : public Shape
{
private:
	uint16_t x, y, color;
public:

	Pixel (uint16_t x, uint16_t y, const uint16_t color);
	void draw () const override;

};

#endif
