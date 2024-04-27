/* Default linker script, for normal executables */
OUTPUT_FORMAT("elf32-avr","elf32-avr","elf32-avr")
OUTPUT_ARCH(avr:5)

/* Processor memory layout */
MEMORY
{
    ROM(rx)   : ORIGIN = 0x00000000, LENGTH = 32K
    RAM(rw!x) : ORIGIN = 0x00800100, LENGTH =  2K
}

/* The stack starts at the last 0 based index of RAM */
ld__init_sp = ORIGIN(RAM) + LENGTH(RAM) - 1;

SECTIONS
{
    /* ISR vector table */
    .ld__vector_table_section :
    {
        . = ALIGN(2);
        *(.ld__vector_table_section)
        . = ALIGN(2);
        KEEP(*(.ld__vector_table_section))
        . = ALIGN(2);
    } > ROM

    /* Program code (text), read-only data and static ctors */
    .text :
    {
        . = ALIGN(2);
        ld__ctors_begin = . ;
        *(.ctors)
        . = ALIGN(2);
        KEEP (*(SORT(.ctors)))
        ld__ctors_end = . ;
        *(.progmem*)
        . = ALIGN(2);
        *(.trampolines*)
        . = ALIGN(2);
        *(.text)
        . = ALIGN(2);
        *(.text*)
        . = ALIGN(2);
    } > ROM

    /* The ROM-to-RAM initialized data section */
    .data :
    {
        . = ALIGN(2);
        ld__data_begin = .;
        *(.data)
        . = ALIGN(2);
        KEEP (*(.data))
        *(.data*)
        . = ALIGN(2);
        KEEP (*(.data*))
        *(.rodata)
        . = ALIGN(2);
        KEEP (*(.rodata))
        *(.rodata*)
        . = ALIGN(2);
        KEEP (*(.rodata*))
        . = ALIGN(2);
        ld__data_end = .;
    } > RAM AT > ROM

    /* The uninitialized (zero-cleared) data section */
    .bss :
    {
        . = ALIGN(2);
        ld__bss_begin = .;
        *(.bss)
        . = ALIGN(2);
        KEEP (*(.bss))
        *(.bss*)
        . = ALIGN(2);
        KEEP (*(.bss*))
        . = ALIGN(2);
        ld__bss_end = .;
    } > RAM

    ld__data_load_begin = LOADADDR(.data);
}
