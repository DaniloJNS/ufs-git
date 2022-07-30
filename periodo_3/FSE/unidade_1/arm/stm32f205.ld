FLASH_init = 0x08000000;
FLASH_size = 128K;
SRAM_init  = 0x20000000;
SRAM_size  = 48K;
_estack    = SRAM_init + SRAM_size;
MEMORY {
    FLASH : ORIGIN = FLASH_init, LENGTH = FLASH_size
    SRAM  : ORIGIN = SRAM_init, LENGTH = SRAM_size
}

SECTIONS {
    .text   : { *(.text.vector_table) *(.text.start) *(.text*) } > FLASH
    .rodata : { *(.rodata*) } > FLASH
    .data   : { *(.data*) } > SRAM AT > FLASH
    .bss    : { *(.bss*) } > SRAM
} 
