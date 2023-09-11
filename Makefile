
# Makefile

AS = riscv64-unknown-elf-as
CC = riscv64-unknown-elf-gcc
LD = riscv64-unknown-elf-ld

ASFLAGS = -g -c # -march=rv64imafdc -mabi=lp64d
CCFLAGS = -g -c -ffreestanding -nostdlib -nostartfiles -fno-builtin -fno-stack-check -fno-stack-protector -I include -Wall -Wextra -march=rv64imafdc -mabi=lp64d -fPIC -mno-relax # -ffunction-sections -mcmodel=medany
LDFLAGS = -T linker.ld -nostdlib

OBJC = $(patsubst %.c, %.o, $(wildcard src/*.c))
OBJS = $(patsubst %.S, %.o, $(wildcard src/*.S))

QEMU = qemu-system-riscv64
QFLAGS = -bios none -machine virt -smp 4 -m 256M -nographic -serial mon:stdio -kernel

GDB = riscv64-unknown-elf-gdb

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
	@ echo -e "[\e[92m LD \e[39m]" $@ $^
	@ rm -r src/*.o
	@ echo -e "[\e[91m RM \e[39m] src/*.o"

src/%.o: src/%.c
	@ $(CC) $(CCFLAGS) -o $@ $^
	@ echo -e "[\e[94m CC \e[39m]" $@ $^

src/%.o: src/%.S
	@ $(AS) $(ASFLAGS) -o $@ $^
	@ echo -e "[\e[93m AS \e[39m]" $@ $^

clean:
	@ rm -r kernel.elf
	@ echo -e "[\e[91m RM \e[39m] kernel.elf"

.PHONY = clean
