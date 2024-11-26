.text
	addi t0, zero, 8	# which factorial
	beq t0, zero, handle_zero
	add t2, zero, t0
	add t1, zero, t0
		
multiply:
	addi t1, t1, -1
	beq zero, t1, stop
	mul t2, t1, t2
	beq zero, zero, multiply
	
handle_zero:
	addi t2, zero, 1
	
stop:
	beq zero, zero, stop	
	
	