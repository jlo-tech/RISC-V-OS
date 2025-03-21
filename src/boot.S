.global _start

.global stack_start
.global stack_end

.section .text

_start:
	# read hart id
    csrr t0, mhartid

    # multiply by 8192
	slli t0, t0 , 13

	# set up stack
	la sp, stack_start
	# add offset to stack base
	add sp, sp, t0

	# jump to main
	j kmain


.section .bss

stack_end:
.fill 32768, 0
stack_start:
