#include "tact.h"

uint8_t Tact::cpu_clock;
uint8_t Tact::bus_clock;
uint8_t Tact::flash_clock;
uint8_t Tact::mcgir_clock;

Tact::Tact ()
{

	cpu_clock = 20;
	bus_clock = 20;
	init_FEI ();
}

void Tact::init_FEI ()
{
	//===Settings power modes
	/* SMC_PMPROT: AHSRUN=1 (Allow High speed run mode),
	 * AVLP=1 (Allow Very-Low-Power Modes),ALLS=0,??=0,AVLLS=0,??=0 */
	SMC->PMPROT = (SMC_PMPROT_AHSRUN_MASK | SMC_PMPROT_AVLP_MASK);

	//===Settings prescallers
	// SIM_CLKDIV1: OUTDIV1=0 (core clock-1),OUTDIV2=0 (bus clock - 1),
	// OUTDIV4=1 (flash clock - 1)
	SIM->CLKDIV1 = SIM_CLKDIV1_OUTDIV1(0x00) |
	              SIM_CLKDIV1_OUTDIV2(0x00) |
	              SIM_CLKDIV1_OUTDIV4(0x00);

	//===Select FLL as a clock source for various peripherals
	// SIM_SOPT2: PLLFLLSEL&=~1
	SIM->SOPT2 &= (uint32_t)~(uint32_t)(SIM_SOPT2_PLLFLLSEL(0x03));

	//===LPO 1kHz oscillator drives 32 kHz clock for various peripherals
	// SIM_SOPT1: OSC32KSEL=3
	SIM->SOPT1 |= SIM_SOPT1_OSC32KSEL(0x03);

	//=== Switch to FEI Mode
	/* MCG_C1: CLKS=0 (output is FLL),FRDIV=0,IREFS=1 ( slow internal reference clock is selected),
	 * IRCLKEN=1 (Internal Reference Clock Enable),IREFSTEN=0 */
	MCG->C1 = MCG_C1_CLKS(0x00)|MCG_C1_FRDIV(0x00)|MCG_C1_IREFS_MASK|MCG_C1_IRCLKEN_MASK;

	/* MCG_C2: LOCRE0=0 (Interrupt request on a loss external reference clock),
	 * RANGE=0 (Slow ,HGO=0,EREFS=1,LP=0,IRCS=0 */
	MCG->C2 = (uint8_t)((MCG_C2 & (uint8_t)~(uint8_t)(MCG_C2_LOCRE0_MASK|
	              MCG_C2_RANGE(0x11) |
	              MCG_C2_HGO_MASK |
	              MCG_C2_LP_MASK |
	              MCG_C2_IRCS_MASK
	             )) | (uint8_t)(
	              MCG_C2_EREFS_MASK
	             ));

	//===Settings multiplyier
	/* MCG_C4: DMX32=0 (31.25–39.0625 kHz),DRST_DRS=0 (FLL Factor - 640)*/
	MCG->C4 &= (uint8_t)~(uint8_t)((MCG_C4_DMX32_MASK | MCG_C4_DRST_DRS(0x03)));

	/* MCG_C7: OSCSEL=0 */
	//MCG_C7 &= (uint8_t)~(uint8_t)(MCG_C7_OSCSEL(0x03));

	//===Check that the source of the FLL reference clock is the internal reference clock.
	while((MCG->S & MCG_S_IREFST_MASK) == 0x00U) ;

	//===Wait until output of the FLL is selected
	while((MCG->S & MCG_S_CLKST_MASK) != 0x00U) ;
}


