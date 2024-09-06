  # hexmain.S
  # Written 2015-09-04 by F Lundevall
  # Copyright abandonded - this file is in the public domain.
  
	.text
	.globl hex2asc

hex2asc:
	li	a0, 17		# test number (from 0 to 15)
	
	addi    sp,sp,-4
	sw      ra,0(sp)
	
	jal	hexasc		# call hexasc
	
	li	a7, 11	# write a0 to stdout
	ecall

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
    

