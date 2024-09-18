 # pointers.S
 # By David Broman, 2015-09-14
 # Ported 2024/??/?? by W Szczerek (from MIPS to RISC-V)
 # This file is in the public domain


# Choose the syntax for RARS or DTEK-V board.
# The syntax for RARS is probably due to its MIPS heritage.
###################
# Macros for RARS #
###################
.macro	PUSH (%reg)
	addi	sp,sp,-4
	sw	%reg,0(sp) 
.end_macro

.macro	POP (%reg)
	lw	%reg,0(sp)
	addi	sp,sp,4
.end_macro
###################
# Macros for DTEK #
###################
#.macro	PUSH reg
#	addi sp,sp,-4
#	sw \reg,0(sp) 
#.endm

#.macro	POP reg
#	lw	\reg,0(sp)
#	addi	sp,sp,4
#.endm
###################

	.data
text1: 	  .asciz "This is a string."
text2:	  .asciz "Yet another thing."

	.align 2
list1: 	.space 80
list2: 	.space 80
counter:.word  0

	.text
	.globl main
	.type main, @function
main:
	jal	work
stop:	j	stop

# function work()
work:
	PUSH	ra
	la 	a0,text1 # load phrase 1
	la	a1,list1 # load memory location 1 
	la	a2,counter # counts the amount of characters in total
	jal	copycodes 
	
	la 	a0,text2 # load phrase 2 
	la	a1,list2 # load memory location 2
	la	a2,counter # counts the amount of characters in total
	jal	copycodes
	POP	ra
	jr	ra
	
	
# function copycodes()
copycodes:
loop:
	lb	t0,0(a0)	 # load first character from phrase
	beqz	t0,done	 # if the character is "zero" == end of phrase => jump to end 
	sw	t0,0(a1) 	 # store the first character to memory  

	addi	a0,a0,1  # add one to the character index to move on to the next character in word 
	addi	a1,a1,4  # move stack by 4 to make space for next store of character in memory
	
	lw	t1,0(a2) 	 # load the counter
	addi	t1,t1,1  # add one to the counter (the counter that counts the total number of characters)
	sw	t1,0(a2) 	 # store the counter
	j	loop
done:
	jr	ra 
		



