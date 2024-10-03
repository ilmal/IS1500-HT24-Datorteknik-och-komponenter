	# timetemplate.S
	# Written 2015 by F Lundevall
	# Ported 2024/06 by W Szczerek (from MIPS to RISC-V)
	# Copyright abandonded - this file is in the public domain.
	
	#############################################################
	# Choose the macro syntax for RARS or DTEK-V board.         #
	# The syntax for RARS is probably due to its MIPS heritage. #
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
	#.macro	PUSH reg
	#	addi sp,sp,-4
	#	sw \reg,0(sp) 
	#.endm
	
	#.macro	POP reg
	#	lw	\reg,0(sp)
	#	addi	sp,sp,4
	#.endm
	#############################################################
	
		.data
		.align 2
	mytime:	.word 	0x5957
	timstr:	.asciz "\0"	# null string
		.text
		.globl timetemplate, tick, time2string, delay, display_string
	
	timetemplate:
		la	a0, timstr	# load adress timstr into a0
		jal     display_string	# call display_string func
		
		# wait a little
		li	a0, 1000 		# ms (5000 ms = 5s)
		jal	delay		# call delay func
		
		# call tick
		la 	a0, mytime	# load adress mytime into a0 (current time)
		jal	tick		# call tick func
		
		# call your function time2string
		la	a0, timstr	# load adress timstr into a0 again
		la	t0, mytime	# load mytime into t0
		lw	a1, 0(t0)	# load t0 content into a1 without offset
		
		jal	time2string	# call time2string
			
		
		# go back and do it all again
		j	timetemplate
	
	
	# Function for displaying a string with a newline at the end	
	display_string:
	        li a7,4
	        ecall
	        li a0, 10
	        li a7,11
	        ecall
	        jr ra

			
	# tick: update time pointed to by $a0
	tick:	
		lw	t0, 0(a0)	# get time
		addi	t0, t0, 1	# increase time with 1
		andi	t1, t0, 0xf	# check lowest digit (f = 1111)
		sltiu	t2, t1, 0xa	# if digit < 10, okay
		bnez	t2, tiend	# if t2 not equal to 0, goto tiend
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
	tiend:	
		sw	t0, 0(a0)	# save updated result
		jr	ra		# return

	delay:
		li t0, 	5000		# random test number
		li t0, 	15000		# random test number
		mul t1, t0, a0		# multiply to get total amount of cycles
		li t0, 0		# set t0 to 0 to count loop
		
		loop: 
			addi t0, t0, 1		# add 1 to t0 loop counter
			ble t0, t1, loop	# keep looping until t0 equals t1	
		jr ra			# return
		
	
	time2string:
	    addi    sp, sp, -12		# adjust stack pointer 
	    sw      ra, 8(sp)		# save return adress
	    
	    sw 		s0, 4(sp)		# save s0 which is caller saved
	   
	    sw		s1, 0(sp)		# save s1 which is caller saved

	    add 	s0, a0, zero	# save a0 to s0
		add 	s1, sp, zero	# save sp to s1
	    
	    
	    # Extract minutes (upper 8 bits) and convert to ASCII'
	    li      t1, 0xF  		# Mask to get units place (1111)
	    slli    t1, t1, 12   	# Extract minutes (shift left 12 times -> 1111 0000 0000 0000)
	    and     t1, a1, t1  	# Mask to get tens place (only 1 and 1 equals T for and-operation)
	    srli    a0, t1, 12   # Shift right to get digit (0000 0000 0000 xxxx)
	    jal     hex2asc     # Convert to ASCII character
	    sb      a0, 0(s0)   # Store ASCII character returned from hex2asc in the string stored in s0
	    
	    li      t1, 0xF  		# Mask to get units place
	    slli    t1, t1, 8   	# Extract minutes (0000 1111 0000 0000)
	    and     t1, a1, t1 	# Mask to get units place
	    srli    a0, t1, 8   # Shift right to get digit
	    jal     hex2asc     # Convert to ASCII
	    sb      a0, 1(s0)   # Store ASCII character next in the string
	    
	    # Store colon character
	    li      a0, 0x3A    # ASCII for ':'
	    sb      a0, 2(s0)   # Store next in the string
	    
	    li      t1, 0xF  		
	    slli    t1, t1, 4   # extract seconds (0000 0000 1111 0000)	
	    and     t1, a1, t1  
	    srli    a0, t1, 4   
	    jal     hex2asc     # Convert to ASCII
	    sb      a0, 3(s0)   # Store ASCII character next in the string
	    
	    andi    a0, a1, 0x0F  # Mask to get units place (0000 0000 0000 1111)
	    jal     hex2asc     # Convert to ASCII
	    sb      a0, 4(s0)   # Store ASCII character in the string
	    
	    add 	a0, s0, zero	# restore a0 from s0 (address to the string memory)
	   	add		sp, s1, zero	# restore sp
	    lw		s1, 0(sp)
	    lw		s0, 4(sp)   # restore s0
	    lw      ra, 8(sp)	# load return adress and restore stack pointer
	   
	    jr      ra
		
	hex2asc:
		addi    sp,sp,-4
		sw      ra,0(sp)
		
		jal	hexasc		# call hexasc
	
		lw      ra,0(sp)
		addi    sp,sp,4
		jr      ra	
	
	  # You can write your own code for hexasc here
	  #
	  
	hexasc:
	
	    #apply a mask to remove the 4 most signifigant bits
	    li    	t2, 0xF		# 1111 in binary
	    and   	a0, a0, t2
	
	
	    li 		t0, 10               
	    blt 	a0, t0, convert_digit	# if number is less than 10 it's a digit char
	
	    addi 	a0, a0, 55     # add 55 for correct ascii if it's a letter    
	    andi    	a0, a0, 0x7F   # Ensure only 7 bits are set, clear upper bits (01111111)
	    jr 		ra                  
	
	convert_digit:
	    addi 	a0, a0, 48     # add 48 if if it's a digit    
	    andi    	a0, a0, 0x7F   # Ensure only 7 bits are set, clear upper bits
	    jr 		ra   
	
	
