# 1 "./bubble-sort.S"
# 1 "<built-in>"
# 1 "<command-line>"
# 31 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 32 "<command-line>" 2
# 1 "./bubble-sort.S"
# bubble-sort.S
# MIPS assembly program to sort array of words using bubble sort algorithm

# Copyright 2019 Jiri Skoda <skodaji4@fel.cvut.cz>
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at

# http:

# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.



# definitions of used registers
# 37 "./bubble-sort.S"
.globl _start
.set noreorder

.text

_start:
main:

 #set controlling registers to 0
 addi $3, $0, 0 # $3 = $0 + 0
 addi $5, $0, 0 # $5 = $0 + 0

 #get reference to first item in array
 la $8, array # $8 = *array
 add $11, $8, $0 # $11 = $8 + $0


 while_a:
  #reset contorlling registe
  addi $5, $0, 0 # $5 = $0 + 0

  #compute maximal index of cycle as length of array minus upper cycle controlling register minus one
  addi $7, $0, 5 # $7 = $0 + 5
  sub $7, $7, $3 # $7 = $7 - $3
  addi $12, $0, 1 # $12 = $0 + 1
  sub $7, $7, $12 # $7 = $7 - $12

  while_b:
   # set pointer to begin of array
   add $11, $0, $8 # $11 = $0 + $8

   #set pointer to actually processing items (start of array plus controlling index multiplied by 4)
   add $12, $5, $0 # $12 = $5 + $0
   sll $12, $12, 2 # $12 = $12 << 2 ($12 = 4 * $12)
   add $11, $11, $12 # $11 = $11 + $12

   # load actually processing item
   lw $9, ($11) # $9 = *$11

   # load next item
   addi $12, $11, 4 # $12 = *$11 + 4
   lw $10, ($12) # $10 = *$12

   # check if one item is smaller than other one
   # - if yes, continue and swap them
   # - if not, skip swaping (jump to end of swapping section)
   slt $12, $9, $10 # $12 = ($9 < $10)
   bne $12, $0, end_swap # if $12 != $0 then end_swap

   # load item into temporary register
   add $12, $0, $10 # $12 = $0 + $9

   # set values of items
   add $10, $0, $9 # $10 = $0 + $12
   add $9, $0, $12 # $9 = $0 + $12

   # save values to array
   sw $9, ($11) # *$11 = $9
   addi $12, $11, 4 # $12 = $11 + 4
   sw $10, ($12) # *$12 = $10

   end_swap:


   # increment controlling register
   addi $5, $5, 1 # $5 = $5 + 1

   # check if controlling register is smaller than maximal allowed value of this register
   # - if not, jump out of cycle
   # - if yes, jump to the start of the cycle
   slt $6, $5, $7 # $6 = ($5 < $7)
   beq $6, 0, done_b # if $6 == 0 then done_b
   j while_b # jump to while_b

  done_b:

  # increment controlling register
  addi $3, $3, 1 # $3 = $3 + 1

  # check if controlling register is smaller than maximal allowed value of this register
  # - if not, jump out of cycle
  # - if yes, jump to the start of the cycle
  slt $4, $3, 5 # $4 = ($3 < 5)
  beq $4, 0, done_a # if $4 == 1 then done_a
  j while_a # jump to while_a

 done_a:

end:
nop

.data

array:
 .word 5,3,4,1,2
