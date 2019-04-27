# bubble-sort.S
# MIPS assembly program to sort array of words using bubble sort algorithm
#
# Copyright 2019 Jiri Skoda <skodaji4@fel.cvut.cz>
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
# 
#     http://www.apache.org/licenses/LICENSE-2.0
# 
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.



# definitions of used registers

#define ARRAY_COUNT 16

#define WHILE_A_CONTROL $3
#define WHILE_A_CONDITION $4

#define WHILE_B_CONTROL $5
#define WHILE_B_CONDITION $6
#define WHILE_B_MAX $7

#define ARRAY $8
#define ITEM_A $9
#define ITEM_B $10
#define ARRAY_PTR $11

#define TMP $12

.globl _start
.set noreorder

.text

_start:
main:

	#set controlling registers to 0
	addi	WHILE_A_CONTROL, $0, 0			# WHILE_A_CONTROL = $0 + 0
	addi	WHILE_B_CONTROL, $0, 0			# WHILE_B_CONTROL = $0 + 0

	#get reference to first item in array
	la		ARRAY, array		# ARRAY = *array
	add		ARRAY_PTR, ARRAY, $0		# ARRAY_PTR = ARRAY + $0
	

	while_a:
		#reset contorlling registe
		addi	WHILE_B_CONTROL, $0, 0			# WHILE_B_CONTROL = $0 + 0	

		#compute maximal index of cycle as length of array minus upper cycle controlling register minus one
		addi	WHILE_B_MAX, $0, ARRAY_COUNT			# WHILE_B_MAX = $0 + ARRAY_COUNT
		sub		WHILE_B_MAX, WHILE_B_MAX, WHILE_A_CONTROL		# WHILE_B_MAX = WHILE_B_MAX - WHILE_A_CONTROL
		addi	TMP, $0, 1			# TMP = $0 + 1
		sub		WHILE_B_MAX, WHILE_B_MAX, TMP		# WHILE_B_MAX = WHILE_B_MAX - TMP

		while_b:
			# set pointer to begin of array
			add		ARRAY_PTR, $0, ARRAY		# ARRAY_PTR = $0 + ARRAY

			#set pointer to actually processing items (start of array plus controlling index multiplied by 4)
			add		TMP, WHILE_B_CONTROL, $0		# TMP = WHILE_B_CONTROL + $0
			sll		TMP, TMP, 2					# TMP = TMP << 2 (TMP = 4 * TMP)
			add		ARRAY_PTR, ARRAY_PTR, TMP	# ARRAY_PTR = ARRAY_PTR + TMP	

			# load actually processing item				
			lw		ITEM_A, (ARRAY_PTR)		# ITEM_A = *ARRAY_PTR

			# load next item
			addi	TMP, ARRAY_PTR, 4			# TMP = *ARRAY_PTR + 4
			lw		ITEM_B, (TMP)		# ITEM_B = *TMP

			# check if one item is smaller than other one
			#  - if yes, continue and swap them
			#  - if not, skip swaping (jump to end of swapping section)
			slt 	TMP, ITEM_A, ITEM_B  # TMP = (ITEM_A < ITEM_B)
			bne		TMP, $0, end_swap	# if TMP != $0 then end_swap	

			# load item into temporary register
			add		TMP, $0, ITEM_B		# TMP = $0 + ITEM_A

			# set values of items
			add		ITEM_B, $0, ITEM_A		# ITEM_B = $0 + TMP
			add		ITEM_A, $0, TMP		# ITEM_A = $0 + TMP

			# save values to array
			sw		ITEM_A, (ARRAY_PTR)		# *ARRAY_PTR = ITEM_A
			addi	TMP, ARRAY_PTR, 4			# TMP = ARRAY_PTR + 4
			sw		ITEM_B, (TMP)		# *TMP = ITEM_B

			end_swap:		
			

			# increment controlling register
			addi	WHILE_B_CONTROL, WHILE_B_CONTROL, 1		# WHILE_B_CONTROL = WHILE_B_CONTROL + 1

			# check if controlling register is smaller than maximal allowed value of this register
			#  - if not, jump out of cycle
			#  - if yes, jump to the start of the cycle
			slt		WHILE_B_CONDITION, WHILE_B_CONTROL, WHILE_B_MAX # WHILE_B_CONDITION = (WHILE_B_CONTROL < WHILE_B_MAX)
			beq		WHILE_B_CONDITION, 0, done_b	# if WHILE_B_CONDITION == 0 then done_b
			j		while_b				# jump to while_b		

		done_b:

		# increment controlling register
		addi	WHILE_A_CONTROL, WHILE_A_CONTROL, 1			# WHILE_A_CONTROL = WHILE_A_CONTROL + 1

		# check if controlling register is smaller than maximal allowed value of this register
		#  - if not, jump out of cycle
		#  - if yes, jump to the start of the cycle
		slt 	WHILE_A_CONDITION, WHILE_A_CONTROL, ARRAY_COUNT # WHILE_A_CONDITION = (WHILE_A_CONTROL < ARRAY_COUNT)
		beq		WHILE_A_CONDITION, 0, done_a	# if WHILE_A_CONDITION == 1 then done_a
		j		while_a				# jump to while_a

	done_a:

end:
nop

.data

array:
	.word 15,5,3,4,1,2,8,14,0,11,9,10,13,12,7,6
