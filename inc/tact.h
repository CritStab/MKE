#include "MK02F12810.h"                // Device header

#ifndef TACT_H
#define TACT_H



class Tact
{
  //variables
public:
  enum mode {FEI,FEE,FBI,FBE,BLPI,BLPE};
  enum cpu_div {cpu_d1, cpu_d2, cpu_d3, cpu_d4, cpu_d5, cpu_d6, cpu_d7, cpu_d8, cpu_d9, cpu_d10,
	  cpu_d11, cpu_d12, cpu_d13, cpu_d14, cpu_d15, cpu_d16};
  enum bus_div {bus_d1,bus_d2,bus_d3,bus_d4,bus_d5,bus_d6,bus_d7,bus_d8,bus_d9,bus_d10,
	 bus_d11,bus_d12,bus_d13,bus_d14,bus_d15,bus_d16};
 private:
  static uint8_t cpu_clock;
  static uint8_t bus_clock;
  static uint8_t flash_clock;
  static uint8_t mcgir_clock;
  uint8_t src;
  //functions
public:
  Tact ();
  Tact (mode m, uint8_t frq);
  static uint8_t & get_frq_cpu (){return cpu_clock;};
  static uint8_t & get_frq_bus (){return bus_clock;};
  static uint8_t & get_frq_flash (){return flash_clock;};
  static uint8_t & get_mcgir_clock (){return mcgir_clock;}
private:
  //===int_OSC===//
  //FLL engaged
  void init_FEI ();

  //bypass
  void init_FBI (uint8_t i);
  //
  void init_BLPI (cpu_div cpu_d=cpu_d1, bus_div bus_d=bus_d1);

  //===ext_OSC===//
  //FLL engaged
  void init_FEE (cpu_div cpu_d=cpu_d1, bus_div bus_d=bus_d2);
  //bypass, FLL engaged
  void init_FBE ();
  //bypass
  void init_BLPE (uint8_t i);

  void Set_frq (uint8_t frq);

};

 
#endif

