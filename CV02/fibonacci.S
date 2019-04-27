# fibonacci.S
# 
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

# Definitions of used registers and values
#define NULL $0
#define MAX_COUNT 50
#define ITEM_A $4
#define ITEM_B $5
#define COUNT $3
#define MAX_CONDITION $6
#define MAX_COUNT_REG $7
#define RESULTS $8
#define TMP $9
#define RESULT $10
#define SAVE $11



.globl _start
.set noat
.set noreorder

.text

_start:
main:
	add		COUNT, NULL, NULL		# COUNT = NULL + NULL
	addi	ITEM_A, NULL, 1			# ITEM_A = NULL + 1
	addi	ITEM_B, NULL, 1			# ITEM_B = NULL + 1

	la		RESULTS, results		# *RESULTS = &results
	add		SAVE, RESULTS, NULL		# SAVE = RESULTS + NULL
	
	

	addi	MAX_COUNT_REG, NULL, MAX_COUNT			# MAX_COUNT_REG = NULL + MAX_COUNT
	
	slt		MAX_CONDITION, COUNT, MAX_COUNT_REG		# MAX_CONDITION = (COUNT < MAX_COUNT_REG)
	beq		MAX_CONDITION, NULL, end	# if MAX_CONDITION == NULL then end

	# Fibonacci computing

	# At first, store one
	addi	RESULT, $0, 1			# RESULT = $0 + 1
	sw		RESULT, (SAVE)		# 
	addi	SAVE, SAVE, 4			# SAVE = SAVE + 4
	sw		RESULT, (SAVE)		# 
	addi	SAVE, SAVE, 4			# SAVE = SAVE + 4
	addi	COUNT, COUNT, 2			# COUNT = COUNT + 2
	
	addi	ITEM_A, NULL, 1			# ITEM_A = NULL + 1
	addi	ITEM_B, NULL, 1

	while:
	add		RESULT, ITEM_A, ITEM_B		# RESULT = ITEM_A + ITEM_B
	sw		RESULT, (SAVE)		# 
	addi	COUNT, COUNT, 1			# COUNT = COUNT + 1
	
	addi	SAVE, SAVE, 4			# SAVE = SAVE + 4
	add		ITEM_B, NULL, ITEM_A		# ITEM_B = NULL + ITEM_A
	add		ITEM_A, NULL, RESULT		# ITEM_A = NULL + RESULT
	

	slt		MAX_CONDITION, COUNT, MAX_COUNT_REG		# MAX_CONDITION = (COUNT < MAX_COUNT_REG)
	bne		MAX_CONDITION, NULL, while	# if !MAX_CONDITION == NULL then while
	
	
	
	

end:
	nop		# do nothing
	break
	

.data

results:
.skip MAX_COUNT