#include "filter.h"


Filter::Filter()
{}

void Filter::setFltDiv2 (busclkDivision div)
{
	PORT->IOFLT0 &= ~ PORT_IOFLT0_FLTDIV2_MASK;
	PORT->IOFLT0 |= PORT_IOFLT0_FLTDIV2(div);
}

void Filter::setFltDiv1 (busclkDivision div)
{
	PORT->IOFLT0 &= ~ PORT_IOFLT0_FLTDIV1_MASK;
	PORT->IOFLT0 |= PORT_IOFLT0_FLTDIV1(div);
}

void Filter::setFltDiv3 (lpoclkDivision div)
{
	PORT->IOFLT0 &= ~ PORT_IOFLT0_FLTDIV3_MASK;
	PORT->IOFLT0 |= PORT_IOFLT0_FLTDIV3(div);
}

void Filter::setFilter (sourceFilter s, clkFilter d)
{
	PORT->IOFLT0 &= ~( (uint8_t)d << (uint8_t)s);
	PORT->IOFLT0 |= (uint8_t)d << (uint8_t)s;
}



