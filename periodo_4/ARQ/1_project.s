// Segmento de codigo
.text
    // Tabela de vetor de interrupcao
    init:
        bun main
        .align 5
    // Procedimento de impressao
    printf:
        push r2 , r3
        // R1 = endereco do terminal
        l32 r1, [terminal]
        // Leitura de byte da mensagem
        l8 r3, [r2]
        // Comparando com '\0'
        cmpi r3, 0
        // Finalizando se for caractere nulo
        beq 3
        // Escreve no terminal
        s8 [r1], r3
        // Incrementa o ponteiro da mensagem
        addi r2, r2, 1
        // Repete a iteracao
        bun -6

        pop r3, r2  
        // Retorno da funcao
        ret
    print_number:
        // salvando registradores
        push r3, r4
        // inicializa contador com zero
        mov r4, 0
        // Leitura de byte da mensagem
        // R1 = endereco do terminal
        l32 r1, [terminal]
        // Comparando com '3'
        cmpi r4, 4
        // Finalizando r3 é iqual 3
        beq 6
        // ler byte para imprimir
        l8 r3, [r2]
        // ascii conversão
        addi r3, r3, 48
        // Escreve no terminal
        s8 [r1], r3
        // Incrementa o ponteiro da mensagem
        addi r2, r2, 1
        // Incrementa o contador
        addi r4, r4, 1
        // Repete a iteracao
        bun -8
        // carrega contexto
        pop r4, r3
        // Retorno da funcao
        ret
    exibir_vetor:
        push r4, r3, r2
        // ponteiro do vetor
        mov r4, V1
        // Inicializando contador
        mov r3, 0
        // inicar interação
        cmpi r3, 4
        beq 7
        // R2 = ponteiro do vetor
        addi r2, r4, 0
        // printf
        call print_number
        // R2 = ponteiro da string
        mov r2, space_message
        // printf
        call printf
        // Incremento contador
        addi r3, r3, 1
        // Incremento ponteiro
        addi r4, r4, 4
        // reiniciar loop
        bun -9
        // R2 = ponteiro da string
        mov r2, broke_line_message
        // printf
        call printf
        // Carregando contexto
        pop r2, r3, r4
        // Retorno da funcao
        ret
    substitui:
        push r5, r3, r4, r6, r7
        // Pon5eir5 do vetor
        mov r4, V1
        divi r4, r4, 4
        // Inicializa contador
        subi r5, r5, 1
        mov r3, 0
        cmp r3, r5
        beq 11
        
        // r6 = v[j] ; r7 = v[j + 1]
        l32 r6, [r4]
        addi r4, r4, 1
        l32 r7, [r4]
        // r6 > r7 ? r6, r7 = r7, r6 : null
        cmp r6 ,r7
        blt 4
        subi r4, r4, 1
        s32 [r4], r7
        addi r4, r4, 1
        s32 [r4], r6

        addi r3, r3, 1
        bun -13

        pop r7, r6, r4, r3, r5

        ret
    ordena:
        push r3

        mov r3, 0
        cmp r3, r5
        beq 3

        call substitui

        addi r3, r3, 1
        bun -5
        
        pop r3

        ret
    // Funcao principal
    main:
        // SP = 32 KiB
        mov sp, 0x7FFC
        // R2 = ponteiro da string
        mov r2, input_message
        // printf
        call printf
        // exibit todos os valores do vetor
        call exibir_vetor
        // R2 = ponteiro da string
        mov r2, sorted_message
        // printf
        call printf
        
        mov r5, 7
        call ordena
        // exibit todos os valores do vetor
        call exibir_vetor
        // Finalizacao de execucao
        int 0
// Segmento de dados
.data
    // Mensagens de texto
    input_message:
        .asciz "Input numbers:\n"
    sorted_message:
        .asciz "Sorted numbers:\n"
    space_message:
        .asciz " "
    broke_line_message:
        .asciz "\n"
    // Inicializando array
    V1:
        .4byte 4, 9, 1, 41, 2, 11, 5
    // Endereco do dispositivo (OUT)
    terminal:
        .4byte 0x8888888B
        
        // mov r3, 0
        // cmpi r3, 3
        // beq 2
        // addi r3, r3, 1
        // bun -4
        // <-> for i := 0; i == 3; i++