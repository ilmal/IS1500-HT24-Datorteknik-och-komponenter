  # hexmain.S
  # Written 2015-09-04 by F Lundevall
  # Copyright abandonded - this file is in the public domain.
  
	.text
	.globl hex2asc

hex2asc:
	li	a0, 17		# test number (from 0 to 15)
				# 17 in decimal = 0001 0001 in hexadecimal 
	
	addi    sp,sp,-4	# move down stackpointer
	sw      ra,0(sp)	#store return adress
	
	jal	hexasc		# call hexasc
	
	li	a7, 11		# write a0 to stdout
	ecall

	lw      ra,0(sp)	# load return adress		
	addi    sp,sp,4		# return stack pointer to original position
	jr      ra		# jump back to return adress

  # You can write your own code for hexasc here
  
hexasc:

    #apply a mask to remove the 4 most signifigant bits
    li    	t2, 0xF		# load 15 in decimal to t2
    and   	a0, a0, t2	# perform AND so that only the 4 lsb are stored in a0
    				# F in hexadecimal is 00001111 in binary


    li 		t0, 10       		# load 10 into t0        
    blt 	a0, t0, convert_digit	# if a0 is less than t0, it should convert to a digit 0-9
					# taken branch
    addi 	a0, a0, 55     # If a0 should be a letter, add 55 to retrieve correct ascii  
    andi    	a0, a0, 0x7F   # Ensure only 7 bits are set, clear upper bits
    jr 		ra                  

convert_digit:
    addi 	a0, a0, 48     # Add 48 to a0 to retrieve correct ascii        
    andi    	a0, a0, 0x7F   # Ensure only 7 bits are set, clear upper bits (7F = 01111111)
    jr 		ra                
    

