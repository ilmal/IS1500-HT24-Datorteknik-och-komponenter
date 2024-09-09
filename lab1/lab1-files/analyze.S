# analyze.S
# Written 2015 by F Lundevall
# Ported 2024/07 by W Szczerek (from MIPS to RISC-V)
# Copyright abandonded - this file is in the public domain.

	.text
	.globl analyze
analyze:
	li	s0, 0x30			# load 30 hexadecimal (48 decimal) to s0
loop:
	mv	a0, s0				# copy from s0 to a0, since print is from a0
	
	li	a7, 11				# environment call with a7 = 11 will print out
	ecall					# one byte from a0 to the Run I/O window

	addi	s0, s0, 0x01			# what happens if the constant is changed?
	
	li	t0, 0x5A			# which number to stop at, 90 in decimal	
	ble	s0, t0, loop			# If current number is less than 90, loop
    	jr 	ra				# 90 is ascii for letter 'Z'	


