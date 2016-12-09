#include "MKE04Z1284.h"                // Device header
#include "ssd1289.h"


#ifndef SHAPE_H
#define SHAPE_H

class Shape
{
public:

	static Ssd1289 * driver;
public:
	virtual void draw () const = 0;
	virtual ~Shape(){}
};




#endif
