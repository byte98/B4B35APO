#define SPILED_REG_BASE    0xffffc100

#define SPILED_REG_LED_LINE_o           0x004
#define SPILED_REG_LED_RGB1_o           0x010
#define SPILED_REG_LED_RGB2_o           0x014
#define SPILED_REG_LED_KBDWR_DIRECT_o   0x018

#define SPILED_REG_KBDRD_KNOBS_DIRECT_o 0x020
#define SPILED_REG_KNOBS_8BIT_o         0x024

#define RED $1
#define GREEN $2
#define BLUE $3

#define KNOBS_ADDRESS $4
#define KNOBS_VALUE $5
#define MEM_BASE $6
#define KNOBS_VAL_NEG $7

.globl _start
.set noat
.set noreorder

.text

_start:
main:    

loop:
    li		MEM_BASE,  SPILED_REG_BASE		# MEM_BASE = SPILED_REG_BASE     
    lw		KNOBS_VALUE, SPILED_REG_KNOBS_8BIT_o(MEM_BASE)		#
    nop
    nor     KNOBS_VAL_NEG, $0, KNOBS_VALUE
    sw		KNOBS_VAL_NEG, SPILED_REG_LED_RGB1_o(MEM_BASE)		# 
    sw		KNOBS_VALUE, SPILED_REG_LED_RGB2_o(MEM_BASE)
    nop
    nop
    nop
    sw		KNOBS_VALUE, SPILED_REG_LED_RGB1_o(MEM_BASE)		# 
    sw		KNOBS_VAL_NEG, SPILED_REG_LED_RGB2_o(MEM_BASE)
    nop
    j		loop				# jump to loop
    
        
.data
