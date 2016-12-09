#include "MKE04Z1284.h"                // Device header
#include "ssd1289.h"


#ifndef SHAPE_H
#define SHAPE_H

class Shape
{
public:

	static Ssd1289 * driver;
public:
	Shape ();
	virtual void draw () const = 0;
	virtual ~Shape();
	friend void getDriver(Shape &, Ssd1289 &);
};

inline void getDriver(Shape &d, Ssd1289 & dr)
{
	d.driver = &dr;
}



#endif
