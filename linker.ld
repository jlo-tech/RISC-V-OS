ENTRY(_start)

MEMORY { ram : ORIGIN = 0x80000000, LENGTH = 256M}

SECTIONS
{
  . = 0x80000000;

  kernel_start = .;

  kernel_mapping_start = .;

  .text.start : {
    src/boot.o( .text._start )
    src/boot.o( .text.* )
  }
  .text : ALIGN(0x1000) {
    *(.text);
  }
  .data : ALIGN(0x1000) {
    *(.data);
  }

  kernel_mapping_end = .;

  .bss : ALIGN(0x1000) {
    *(.bss);
  }

  . = ALIGN(0x1000);
  kernel_end = .;
}
