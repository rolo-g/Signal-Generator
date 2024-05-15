// GPIO IP Example
// GPIO IP Library (gpio_ip.c)
// Jason Losh

//-----------------------------------------------------------------------------
// Hardware Target
//-----------------------------------------------------------------------------

// Target Platform: Xilinx XUP Blackboard

// AXI4-Lite interface: 
//   Mapped to offset of 0
//
// IRQ interface:
//   IRQ_F2P[2] is used as the interrupt interface to the PS
//
// GPIO interface:
//   GPIO[31-0] is used as a general purpose GPIO p

//-----------------------------------------------------------------------------


#include <stdint.h>          // C99 integer types -- uint32_t
#include <stdbool.h>         // bool
#include <stdio.h>           // printf  REMOVE ON FINAL
#include <fcntl.h>           // open
#include <sys/mman.h>        // mmap
#include <unistd.h>          // close
#include "../address_map.h"  // address map
#include "wg_ip.h"         // gpio
#include "wg_regs.h"       // registers

//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------

uint32_t *base = NULL;

//-----------------------------------------------------------------------------
// Subroutines
//-----------------------------------------------------------------------------

bool wgOpen()
{
    // Open /dev/mem
    int file = open("/dev/mem", O_RDWR | O_SYNC);
    bool bOK = (file >= 0);
    if (bOK)
    {
        // Create a map from the physical memory location of
        // /dev/mem at an offset to LW avalon interface
        // with an aperature of SPAN_IN_BYTES bytes
        // to any location in the virtual 32-bit memory space of the process
        base = mmap(NULL, SPAN_IN_BYTES, PROT_READ | PROT_WRITE, MAP_SHARED,
                    file, AXI4_LITE_BASE + WG_BASE_OFFSET);
        bOK = (base != MAP_FAILED);

        // Close /dev/mem
        close(file);
    }
    return bOK;
}

void setMode(uint8_t mode, bool field)
{   
    uint8_t shift = field ? OFS_MODE_B_FIELD : OFS_MODE_A_FIELD;
    *(base+OFS_MODE_REG) &= ~(0x7 << shift);
    *(base+OFS_MODE_REG) |= (mode << shift);
}

void setRun(bool running, bool field)
{
    uint8_t shift = field ? OFS_RUN_B_FIELD : OFS_RUN_A_FIELD; 
    *(base+OFS_RUN_REG) &= ~(0x1 << shift);
    *(base+OFS_RUN_REG) |= (running << shift);
}

void setFreqA(uint32_t frequency)
{
    *(base+OFS_FREQ_A_REG) = frequency;
}

void setFreqB(uint32_t frequency)
{
    *(base+OFS_FREQ_B_REG) = frequency;
}

void setOffset(int16_t offset, bool field)
{
    uint8_t shift = field ? OFS_OFFSET_B_FIELD : OFS_OFFSET_A_FIELD;
    *(base+OFS_OFFSET_REG) &= ~(0xFFFF << shift);
    *(base+OFS_OFFSET_REG) |= (offset << shift);
}

void setAmplitude(int16_t amplitude, bool field)
{
    uint8_t shift = field ? OFS_AMPL_B_FIELD : OFS_AMPL_A_FIELD;
    *(base+OFS_AMPLITUDE_REG) &= ~(0xFFFF << shift);
    *(base+OFS_AMPLITUDE_REG) |= (amplitude << shift);
}

void setDutyCycle(uint16_t dtycyc, bool field)
{
    uint8_t shift = field ? OFS_DC_B_FIELD : OFS_DC_A_FIELD;
    *(base+OFS_DTYCYC_REG) &= ~(0xFFFF << shift);
    *(base+OFS_DTYCYC_REG) |= (dtycyc << shift);
}

void setCycles(int16_t cycles, bool field)
{
    uint8_t shift = field ? OFS_CYCLES_B_FIELD : OFS_CYCLES_A_FIELD;
    *(base+OFS_CYCLES_REG) &= ~(0xFFFF << shift);
    *(base+OFS_CYCLES_REG) |= (cycles << shift);
}
