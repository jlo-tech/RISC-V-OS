.global handle_traps

.set context_size, 264

.macro store_context base

	# store regsiters
	sd x1, -248(\base)
	sd x2, -240(\base)
	sd x3, -232(\base)
	sd x4, -224(\base)
	sd x5, -216(\base)
	sd x6, -208(\base)
	sd x7, -200(\base)
	sd x8, -192(\base)
	sd x9, -184(\base)
	sd x10, -176(\base)
	sd x11, -168(\base)
	sd x12, -160(\base)
	sd x13, -152(\base)
	sd x14, -144(\base)
	sd x15, -136(\base)
	sd x16, -128(\base)
	sd x17, -120(\base)
	sd x18, -112(\base)
	sd x19, -104(\base)
	sd x20, -96(\base)
	sd x21, -88(\base)
	sd x22, -80(\base)
	sd x23, -72(\base)
	sd x24, -64(\base)
	sd x25, -56(\base)
	sd x26, -48(\base)
	sd x27, -40(\base)
	sd x28, -32(\base)
	sd x29, -24(\base)
	sd x30, -16(\base)
	sd x31, -8(\base)

	# save privilege
	csrr t0, mstatus
	srli t0, t0, 11
	andi t0, t0, 3
	sb t0, -264(\base)

	# store pc
	csrr t0, mepc
	sd t0, -256(\base)

.endm

.macro load_context base

	# load privilege
	lb t0, -264(\base)	# load priv from cpu_t struct
	andi t0, t0, 3		# reconstruct value
	slli t0, t0, 11		# ...

	li t1, 3			# create bit mask
	slli t1, t1, 11

	csrr t2, mstatus	# set mpp bits to zero in t2(mstatus)
	or t2, t2, t1
	xor t2, t2, t1

	or t2, t2, t0		# set bits from load privileges
	csrw mstatus, t2	# write back mstatus

	# load pc
	ld t0, -256(\base)
	csrw mepc, t0

	# load registers
	ld x1, -248(\base)
	ld x2, -240(\base)
	ld x3, -232(\base)
	ld x4, -224(\base)
	ld x5, -216(\base)
	ld x6, -208(\base)
	ld x7, -200(\base)
	ld x8, -192(\base)
	ld x9, -184(\base)
	ld x10, -176(\base)
	ld x11, -168(\base)
	ld x12, -160(\base)
	ld x13, -152(\base)
	ld x14, -144(\base)
	ld x15, -136(\base)
	ld x16, -128(\base)
	ld x17, -120(\base)
	ld x18, -112(\base)
	ld x19, -104(\base)
	ld x20, -96(\base)
	ld x21, -88(\base)
	ld x22, -80(\base)
	ld x23, -72(\base)
	ld x24, -64(\base)
	ld x25, -56(\base)
	ld x26, -48(\base)
	ld x27, -40(\base)
	ld x28, -32(\base)
	ld x29, -24(\base)
	ld x30, -16(\base)
	ld x31, -8(\base)

.endm

.section .text

handle_traps:

	# save registers
	store_context sp

	# put sp on the right position
	addi sp, sp, -context_size

	# first arg is pointer to cpu state
	mv a0, sp

	# second arg is the reason for the trap
	csrr a1, mcause

	# third arg are additional information
	csrr a2, mtval

	# call trap request handler
	jal ra, handle_trq

	# make returned task active here

	# align a0 to right position and store it in t6
	addi t6, a0, context_size

	# restore cpu state
	load_context t6

	# jump back
	mret


.global _switch_task

# pass control immediatly to task
_switch_task:

	# align pointer
	addi t6, a0, context_size

	# we have to take t6, cause its the last register(x31) that is restored
	load_context t6

	# jump into new task
	mret
