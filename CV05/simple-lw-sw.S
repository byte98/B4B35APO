.globl _start
.set noat

.text

_start:
main:


loop:
	la	   $6, vect_a
	la	   $7, vect_b
	la     $8, vect_c	
	addi   $9, $0, 16
	
vect_next:
	// load the word from absolute address
	lw	   $2, 0($6)
	lw     $3, 0($7)
	add    $2, $2, $3
	// store the word to absolute address
	sw     $2, 0($8)
	addi   $6, $6, 4
	addi   $7, $7, 4
	addi   $8, $8, 4	
	addi   $9, $9, -1
	bne    $9, $0, vect_next
	
	cache 0, 0
	break

	// stop execution wait for debugger/user
	// break
	// ensure that continuation does not
	// interpret random data
	beq    $0, $0, loop
	nop

.data

vect_a:
	.word  0x0
	.word  0x1
	.word  0x2
	.word  0x3
	.word  0x4
	.word  0x5
	.word  0x6
	.word  0x7
	.word  0x8
	.word  0x9
	.word  0xa
	.word  0xb
	.word  0xc
	.word  0xd
	.word  0xe
	.word  0xf
vect_b:
	.word  0x0
	.word  0x1
	.word  0x2
	.word  0x3
	.word  0x4
	.word  0x5
	.word  0x6
	.word  0x7
	.word  0x8
	.word  0x9
	.word  0xa
	.word  0xb
	.word  0xc
	.word  0xd
	.word  0xe
	.word  0xf

.skip 16

vect_c: 	
	.skip 64
