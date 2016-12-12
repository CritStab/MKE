#include "MKE04Z1284.h"                // Device header
#include "shape.h"
#include "colors16bit.h"

#ifndef PIXEL_H
#define PIXEL_H

class Picture : public Shape
{
private:
	uint16_t x, y, length, width;
	const uint16_t * ptr;
public:

	Picture (uint16_t x , uint16_t y, const uint16_t *arr, uint8_t length, uint16_t width);
	void draw () const override;

};

#endif
