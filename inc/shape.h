#include "MKE04Z1284.h"                // Device header



#ifndef SHAPE_H
#define SHAPE_H

class Shape
{

public:
	Shape ();
	virtual void draw () const = 0;
	virtual ~Shape();
};

#endif
