
# Makefile

AS = riscv64-unknown-elf-as
CC = riscv64-unknown-elf-gcc
LD = riscv64-unknown-elf-ld

ASFLAGS = -g -c # -march=rv64imafdc -mabi=lp64d
CCFLAGS = -g -c -ffreestanding -nostdlib -nostartfiles -fno-builtin -fno-stack-check -fno-stack-protector -I include -march=rv64imafdc -mabi=lp64d -fPIC -mno-relax -Wall -Wextra # -ffunction-sections -mcmodel=medany
LDFLAGS = -T linker.ld -nostdlib

OBJC = $(patsubst %.c, %.o, $(wildcard src/*.c))
OBJS = $(patsubst %.S, %.o, $(wildcard src/*.S))

QEMU = qemu-system-riscv64
QFLAGS = -bios none -machine virt -smp 4 -m 300M -nographic -serial mon:stdio -kernel

GDB = riscv64-unknown-elf-gdb

NC='\033[0m'		# No color
RED='\033[0;31m'
BLUE='\033[0;34m'
GREEN='\033[0;32m'
PURPLE='\033[0;35m'
ORANGE='\033[0;33m'

qemu: kernel.elf
	@ echo "[QEMU] ..."
	@ $(QEMU) $(QFLAGS) kernel.elf

qemu-debug: kernel.elf
	@ echo "[QEMU] ..."
	@ $(QEMU) -s -S $(QFLAGS) kernel.elf

gdb: kernel.elf
	$(GDB) \
		-ex "file kernel.elf" \
		-ex "target remote localhost:1234"

kernel.elf: $(OBJC) $(OBJS)
	@ $(LD) $(LDFLAGS) -o $@ $^
	@ echo [ ${GREEN}LD${NC} ] $@ $^

src/%.o: src/%.c
	@ $(CC) $(CCFLAGS) -o $@ $^
	@ echo [ ${BLUE}CC${NC} ] $@ $^

src/%.o: src/%.S
	@ $(AS) $(ASFLAGS) -o $@ $^
	@ echo [ ${ORANGE}AS${NC} ] $@ $^

clean:
	@ rm -r src/*.o
	@ echo [ ${RED}RM${NC} ] src/*.o

	@ rm -r kernel.elf
	@ echo [ ${RED}RM${NC} ] kernel.elf

.PHONY = clean
