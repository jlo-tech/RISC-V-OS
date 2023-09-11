.global _start

.global stack_start
.global stack_end

.section .text

_start:
	# verify that only hart 0 startsup
    	csrr t0, mhartid

    	# multiply by 4096
	slli t0, t0 , 12

	# set up stack
	la sp, stack_start
	# add offset to stack base
	add sp, sp, t0

	# jump to main
	j kmain


.section .bss

stack_end:
.fill 16384, 0
stack_start:
