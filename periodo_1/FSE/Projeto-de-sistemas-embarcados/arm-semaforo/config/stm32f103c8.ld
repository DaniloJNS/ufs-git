FLASH_init = 0x08000000;
FLASH_size = 64K;
RAM_init   = 0x20000000;
RAM_size   = 20K;
_estack    = RAM_init + RAM_size;

MEMORY {
    FLASH  : ORIGIN = FLASH_init ,  LENGTH = FLASH_size
    RAM    : ORIIN  = RAM_init,     LENGTH = RAM_size
}

SECTIONS {
    .text   : { *(.text.vector_table) *(.text.start) *(.text*) } > FLASH
    .rodata : { *(.rodata) } > FLASH
    .data   : { *(.data*) } > FLASH
    .bss    : { *(.bss) } > RAM
}