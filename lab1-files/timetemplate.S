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
	timstr:	.asciz "\0"
		.text
		.globl timetemplate, tick, time2string
	
	.macro	PUSH (%reg)
		addi	sp,sp,-4
		sw	%reg,0(sp) 
	.end_macro
	
	.macro	POP (%reg)
		lw	%reg,0(sp)
		addi	sp,sp,4
	.end_macro
	
		
	timetemplate:
		la	a0, timstr
		jal     display_string
		
		# wait a little
		li	a0, 5000 	# ms
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
	
	
	# Function for displaying a string with a newline at the end	
	display_string:	
		li a7,4
		ecall
		li a0, 10
		li a7,11
		ecall
		
		lb a0, 0(sp)
		ecall
		lb a0, 1(sp)
		ecall
		lb a0, 2(sp)
		ecall
		lb a0, 3(sp)
		ecall
		lb a0, 4(sp)
		ecall
		lb a0, 5(sp)
		ecall
		
		addi    sp, sp, 12
		
		jr ra
		
			
	# tick: update time pointed to by $a0
	tick:	
		lw	t0, 0(a0)	# get time
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
	tiend:	
		sw	t0, 0(a0)	# save updated result
		jr	ra		# return
	
	#########################################################
	# Place for your functions: time2string, hex2asc, delay.#
	#########################################################
	
	delay:
		li t0, 	125		#freq 1.25 GHz ÷ 2 operations
		mul t1, t0, a0		#multiply to get total amount of cycles
		li t0, 0		#set t0 to 0 to count loop
		
		loop: 
			addi t0, t0, 1		#add 1 to t0 loop counter
			ble t0, t1, loop	#keep looping until t0 equals t1	
		jr ra
		
	
	time2string:
	    addi    sp, sp, -4
	    sw      ra, 0(sp)
	    
	    addi    sp, sp, -8
	    
	    
	    # Extract minutes (upper 8 bits) and convert to ASCII'
	    li      t1, 0xF  		# Mask to get units place
	    slli    t1, t1, 12   	# Extract minutes
	    and     t1, a1, t1  	# Mask to get tens place
	    srli    a0, t1, 12   # Shift right to get digit
	    jal     hex2asc     # Convert to ASCII
	    sb      a0, 0(sp)   # Store ASCII character in the string
	    
	    li      t1, 0xF  		# Mask to get units place
	    slli    t1, t1, 8   	# Extract minutes
	    and     t1, a1, t1 	# Mask to get units place
	    srli    a0, t1, 8   # Shift right to get digit
	    jal     hex2asc     # Convert to ASCII
	    sb      a0, 1(sp)   # Store ASCII character in the string
	    
	    # Store colon character
	    li      a0, 0x3A    # ASCII ':'
	    sb      a0, 2(sp)   # Store in the string
	    
	    li      t1, 0xF  		
	    slli    t1, t1, 4   	
	    and     t1, a1, t1 
	    srli    a0, t1, 4   
	    jal     hex2asc     # Convert to ASCII
	    sb      a0, 3(sp)   # Store ASCII character in the string
	    
	    andi    a0, a1, 0x0F  # Mask to get units place
	    jal     hex2asc     # Convert to ASCII
	    sb      a0, 4(sp)   # Store ASCII character in the string
	    
	    # Store null terminator
	    li      a0, 0x00    # ASCII NUL
	    sb      a0, 5(sp)   # Store null terminator
	    
	    lw      ra, 8(sp)
	   
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
	    li    	t2, 0xF
	    and   	a0, a0, t2
	
	
	    li 		t0, 10               
	    blt 	a0, t0, convert_digit
	
	    addi 	a0, a0, 55        
	    andi    	a0, a0, 0x7F   # Ensure only 7 bits are set, clear upper bits
	    jr 		ra                  
	
	convert_digit:
	    addi 	a0, a0, 48         
	    andi    	a0, a0, 0x7F   # Ensure only 7 bits are set, clear upper bits
	    jr 		ra   
	
	
