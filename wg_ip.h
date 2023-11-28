// GPIO IP Example
// GPIO IP Library (gpio_ip.h)
// Jason Losh

//-----------------------------------------------------------------------------
// Hardware Target
//-----------------------------------------------------------------------------

// Target Platform: Xilinx XUP Blackboard

// Hardware configuration:
//
// AXI4-Lite interface:
//   Mapped to offset of 0
//
// IRQ interface:
//   IRQ_F2P[2] is used as the interrupt interface to the PS
//
// GPIO interface:
//   GPIO[31-0] is used as a general purpose GPIO port

//-----------------------------------------------------------------------------

#ifndef WG_H_
#define WG_H_

#include <stdint.h>
#include <stdbool.h>

//-----------------------------------------------------------------------------
// Subroutines
//-----------------------------------------------------------------------------

bool wgOpen(void);

void setMode(uint8_t mode, bool field);
void setRun(bool running, bool field);
void setFreqA(uint32_t frequency);
void setFreqB(uint32_t frequency);
void setOffset(int16_t offset, bool field);
void setAmplitude(int16_t amplitude, bool field);
void setDutyCycle(uint16_t dtycyc, bool field);
void setCycles(int16_t cycles, bool field);

#endif
