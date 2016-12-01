#include "MKE04Z1284.h"                // Device header



#ifndef IRQ_H
#define IRQ_H

class Irq
{
public:
	enum class sense : uint8_t {onlyEdge, edgeLevel};
	enum class pullUp: uint8_t {enable, disable};
	enum class edge :uint8_t {falling, rising};

private:


public:
	//constructor with soft trigger
	Irq ();


};

#endif
