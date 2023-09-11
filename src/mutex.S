.global lock
.global unlock

.section .text

lock:
	li t0, 1
spin:
	amoswap.d.aq t1, t0, (a0)	# a0 hold lock value
	bnez t1, spin
	ret

unlock:
	amoswap.d.rl x0, x0, (a0)
	ret
