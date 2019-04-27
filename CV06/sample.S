.globl _start
.set noat
.set noreorder

.text
_start:

main:

    addi  $2,  $0, 10
    add   $11, $0, $2   // A : $11<-$2
    add   $12, $0, $2   // B : $12<-$2
    add   $13, $0, $2   // C : $13<-$2

    // la $5, varx  // $5 = (byte*) &varx; 
    // The macro-instruction la is compiled as two following instructions:
    lui   $5, %hi(varx) // load the upper part of address
    ori   $5, $5, %lo(varx) // append the lower part of address

    lw    $1, 0($5)     // $1 = *((int*)$5);
    add   $15, $0, $1   // D : $15<-$1
    add   $16, $0, $1   // E : $16<-$1
    add   $17, $0, $1   // F : $17<-$1
loop:
    break
    beq    $0, $0, loop
    nop

.data

varx:
	.word  1