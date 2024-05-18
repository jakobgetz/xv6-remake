OUTPUT_ARCH("riscv")
ENTRY( _entry ) /* not needed, I think this is just where
the _start function would jump to in a user level program.
So we could change the entry point from a C program from
main to entry or something */

SECTIONS {
    . = 0x80000000;

    .text : {
        *(.text .text.*)
        . = ALIGN(0x1000)
        _trampoline = .;
        *(trampsec)
        . = ALIGN(0x1000);
        ASSERT(. - _trampoline == 0x1000, "erorr: trampoline larger than one page")
    }

    .rodata : {
        *(.srodata .srodata.*)
        *(.rodata .rodata.*)
    }

    .data : {
        *(.sdata .sdata.*)
        *(.data .data.*)
    }

    .bss : {
        *(.sbss .sbss.*)
        *(.bss .bss.*)
    }
}