/* linker.ld */
OUTPUT_FORMAT(elf64-x86-64)
ENTRY(kernel_entry)

SECTIONS
{
    . = 0x100000;                /* 加载地址，可根据需要调整 */
    .text : {
        *(.text*)
    }
    .data : {
        *(.data*)
    }
    .bss : {
        *(.bss*)
    }
    /DISCARD/ : {
        *(.comment)
        *(.eh_frame)
    }
}