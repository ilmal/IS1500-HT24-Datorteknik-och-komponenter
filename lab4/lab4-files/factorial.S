.text

	addi t0, zero, 3	# input: factorial of 0, 3, and 8, ... 
	beq t0, zero, h_zero	# handles 0!
	add tp, zero, t0		# stores result of n(n-1)
	add t1, zero, zero		# set counter
	
multiply:
	addi t1, t1, 1			# add 1 to counter
	beq t1, t0, m_end	# check if counter equals t0
	add t2, tp, t2			# add result with value of tp
	beq zero, zero, multiply	# jump back to start of multiply
	
	
	
m_end: 
	 add t1, zero, zero		# reset counter 
	 addi t0, t0, -1		# remove 1 from t0 so n -> n-1
	 beq t0, zero, f_end	# if t0 reach 0, end
	 add tp, zero, t2		# set tp to current result to keep add
	 add t2, zero, zero		# reset t2
	 beq zero, zero, multiply	# go back to multiply
	
f_end:
	add t2, zero, tp		# add value of tp to t2
	beq zero, zero, stop		# jump to stop 

h_zero:
	addi t2, zero, 1		# add 1 to t2 as res since 0! = 1
	beq zero, zero, stop		# go to stop

stop:
	beq zero, zero, stop		# infinite stop loop
