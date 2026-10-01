# timetemplate.S
# Written 2015 by F Lundevall
# Ported 2024/06 by W Szczerek (from MIPS to RISC-V)
# Copyright abandonded - this file is in the public domain.

#############################################################
# PUSH / POP macros.                                        #
#                                                           #
# RARS and the GNU assembler (used for the DTEK-V board)    #
# use slightly different macro syntax. The file ships with  #
# the RARS-style block active, because Assignments 1-4 are  #
# done in RARS. When you port to the board in Assignment 5, #
# comment out the RARS block below and uncomment the DTEK   #
# block beneath it.                                         #
#############################################################
###################
# Macros for RARS #
###################
#.macro	PUSH (%reg)
#	addi	sp,sp,-4
#	sw	%reg,0(sp)
#.end_macro
#
#.macro	POP (%reg)
#	lw	%reg,0(sp)
#	addi	sp,sp,4
#.end_macro
###################
# Macros for DTEK #
###################
.macro	PUSH reg
	addi sp,sp,-4
	sw \reg,0(sp)
.endm

.macro	POP reg
	lw	\reg,0(sp)
	addi	sp,sp,4
.endm
#############################################################

	.data
	.align 2
mytime:	.word 	0x5957
timstr:	.asciz 	"text more text lots of text\0"
	.text
	# delay is declared global so the auto-grader's test harness (which
	# calls delay directly from a small labmain.S) can resolve the symbol.
	.globl display_string, tick, time2string, delay

# Function for displaying a string with a newline at the end	
display_string:	
	li a7,4
	ecall
	li a0, 10
	li a7,11
	ecall
	jr ra
	
timetemplate:
	la	a0, timstr
	jal     display_string
	
	# wait a little
	li	a0, 1000	# ms
	jal	delay
	
	# call tick
	la 	a0, mytime
	jal	tick
	
	# call your function time2string
	la	a0, timstr
	la	t0, mytime
	lw	a1, 0(t0)
	jal	time2string
	
	# go back and do it all again
	j	timetemplate

	
# tick: update time pointed to by $a0
tick:	lw	t0, 0(a0)	# get time
	addi	t0, t0, 1	# increase
	andi	t1, t0, 0xf	# check lowest digit
	sltiu	t2, t1, 0xa	# if digit < a, okay
	bnez	t2, tiend
	addi	t0, t0, 0x6	# adjust lowest digit
	
	andi	t1, t0, 0xf0	# check next digit
	sltiu	t2, t1, 0x60	# if digit < 6, okay
	bnez	t2, tiend
	addi	t0, t0, 0xa0	# adjust digit
	
	li	t3, 0xF
	slli	t3, t3, 0x8
	and	t1, t0, t3	# check minute digit
	addi	t3, x0, 0xA
	slli	t3, t3, 0x8
	slt	t2, t1, t3	# if digit < a, okay
	bnez	t2, tiend
	addi	t0, t0, 0x600	# adjust digit - this one's okay, it's lower than 0x7FF 
	
	li	t3, 0xF
	slli	t3, t3, 0xC
	and	t1, t0, t3	# check last digit
	addi	t3, x0, 0x6
	slli	t3, t3, 0xC
	slt	t2, t1, t3	# if digit < 6, okay
	bnez	t2, tiend
	
	li	t3, 0xA
	slli	t3, t3, 0xC
	add	t0, t0, t3	# adjust last digit
tiend:	sw	t0,0(a0)	# save updated result
	jr	ra		# return

#########################################################
# Place for your functions: time2string, hexasc, delay. #
#########################################################
time2string:			# Since little endian, 59:57 in hex should be expected like this: 0(a0) = 0x353A3935, 4(a0) = 0x00000037
	PUSH 	ra		# Save ra
	PUSH 	a0		# Save timstr address	
	li	t1, 0		# Main register where the NBCD is translated to
	li	t2, 0		# Register to keep track of loop iteration
	li	t3, 4		# A constant used in loop to branch off from loop
	li	t4, 1		# A constant used in loop to branch to saveword
	li	t5, 2		# A constant used in loop to branch to colon
loop:
	bge	t2, t3, end	# for(t2 = 0; t2 < 4; t2++);
	add	a0, zero, a1	# Move a1 to a0
	jal 	hexasc		# a0 is converted to hex in hexasc
	srli	a1, a1, 4	# Shift right 4 bits. Ex, 1254 => X125
	slli	t1, t1, 8	# Shift left 8 bits. Ex, XXXX => XX00
	or	t1, t1, a0	# Places the value from a0 to t1
	addi	t2, t2, 1	# t2++
	beq	t2, t4, saveword	# if(t2 == 1), jump to saveword
	beq	t2, t5, colon		# if(t2 == 2), jump to colon
	j	loop
saveword:			
	POP	a0
	sw	t1, 4(a0)	# Last digit in the NBCD is stored at 4(a0) because of little endian
	PUSH	a0
	j	loop		# Continue loop
colon:				# Adds a colon (:)
	slli	t1, t1, 8	# Makes room for 0x3A (:)
	ori	t1, t1, 0x3A	# Adds 0x3A to t1
	j	loop		# Continue loop
end:
	POP	a0
	POP	ra
	sw	t1, 0(a0)	# Save to timstr
	jr	ra		# Return from time2string

hexasc:
	andi	a0, a0, 0x0000000F
	addi 	t0, zero, 10	# Load a constant 10 to t0
	ble	t0, a0, letter	# Compare if t0 <= a0
	addi	a0, a0, 0x30	# Converts to ASCII digits 0-9
	jr 	ra
	
letter:
	addi 	a0, a0, 0x37	# Converts to ASCII letters A-F
	jr	ra

delay :
	li 	t1, 10000	# Upper condition in the for-loop
while:
	li	t0, 0		# Reset the i variable in the for loop to 0
	ble	a0, zero, delayend	# while(ms > 0);
	addi	a0, a0, -1	# ms = ms - 1
for:
	bge	t0, t1, while	# for(i < t1), otherwise end for-loop and continue the while-loop
	addi	t0, t0, 1	# i++
	j	for		# Continue for-loop
delayend:
	jr	ra		# Return from delay
