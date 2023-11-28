// GPIO IP Library Registers
// Jason Losh

//-----------------------------------------------------------------------------
// Hardware Target
//-----------------------------------------------------------------------------

// Target Platform: DE1-SoC Board
// Target uC:       -
// System Clock:    -

// Hardware configuration:
// -

//-----------------------------------------------------------------------------
// Device includes, defines, and assembler directives
//-----------------------------------------------------------------------------

#ifndef WG_REGS_H_
#define WG_REGS_H_

// register offsets
#define OFS_MODE_REG 0
#define OFS_RUN_REG 1
#define OFS_FREQ_A_REG 2
#define OFS_FREQ_B_REG 3
#define OFS_OFFSET_REG 4
#define OFS_AMPLITUDE_REG 5
#define OFS_DTYCYC_REG 6
#define OFS_CYCLES_REG 7

// field offsets
#define OFS_MODE_A_FIELD 0
#define OFS_MODE_B_FIELD 3
#define OFS_RUN_A_FIELD 0
#define OFS_RUN_B_FIELD 1
#define OFS_OFFSET_A_FIELD 0
#define OFS_OFFSET_B_FIELD 16
#define OFS_AMPL_A_FIELD 0
#define OFS_AMPL_B_FIELD 16
#define OFS_DC_A_FIELD 0
#define OFS_DC_B_FIELD 16
#define OFS_CYCLES_A_FIELD 0
#define OFS_CYCLES_B_FIELD 16

#define SPAN_IN_BYTES 32

#endif
