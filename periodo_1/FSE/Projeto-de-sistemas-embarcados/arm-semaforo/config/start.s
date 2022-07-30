.syntax unified
.cpu    cortex-m3
.fpu    softvfp
.thumb

.section .text.vector_table
.global _vector_table
_vector_table:
    // Topo da pilha
    .word _estack
    // Rotina de inicialização
    .word _start

.section .text.start
.thumn_func
_start:
    bl main
    b  .