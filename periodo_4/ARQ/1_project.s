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
    convert_decimal:
        modi r5, r3, 10
        divi r3, r3, 10
        ret
    print_digit:
        push sr, r6, r5, r3
        // r5 - Digito atual a ser exigibido
        // r3 - valor não convertido que ainda deve ser exibido

        // rotina para compara converção para decimal (0-9)
        call convert_decimal
        // comparação para verificar se tem digitos a esquerda
        // para exibir
        cmp r3, r0
        // se não existir mais digitos a esquerda para exibir
        // interronpe a recursão
        beq 1
        call print_digit
        // caso existam digitos a esquerda para exibit
        // pula a comparação "se o digito atual é zero"
        bgt 2
        // esse comparação acontece quando não exitem mais digitos
        // a esquerda para exibir, caso digito atual for zero
        // ele não será exibido
        cmp r5, r0
        // pula a exibição se digito atual for zero
        beq 2
        // converter o digito atual para ascii
        addi r5, r5, 48
        // imprimir
        s8 [r1], r5
        pop r3, r5, r6, sr
        ret

    print_number:
        // salvando registradores
        // r1 - ponteiro para o terminal
        // r2 - ponteiro da posição atual do vetor
        push r3, r5
        // R1 = Endereco do terminal
        l32 r1, [terminal]
        // ler byte para imprimir
        l32 r3, [r2]
        // rotina para exibir os digitos
        call print_digit
        // carrega contexto
        pop r5, r3
        // Retorno da funcao
        ret
    exibir_vetor:
        push r4, r3, r2
        // ponteiro do vetor
        mov r4, V1
        divi r4, r4, 4
        // Inicializando contador
        mov r3, 0
        // inicar interação
        cmp r3, r5
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
        addi r4, r4, 1
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
        // r4 - ponteiro do vetor = j
        // r5 - tamanho do vetor
        // r6 - v[j]
        // r7 - v[j + 1]
        // r3 - contador do loop
        // Pon5eir5 do vetor
        mov r4, V1
        divi r4, r4, 4
        // Inicializa contador
        subi r5, r5, 1
        mov r3, 0
        // compara se valor atual do contador é igual ao tamanho do vetor
        cmp r3, r5
        // interrnpe o loop se valor atual do contador é igual ao tamanho
        // do vetor
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
        // incremento do contador
        addi r3, r3, 1
        // reiniciar a recursão
        bun -13

        pop r7, r6, r4, r3, r5

        ret
    ordena:
        // salva na pilha o valor atual de r3
        push r3
        // r3 - contador
        // r5 - tamanho do vetor

        // inicializar contador
        mov r3, 0
        // compara o valor atual do contador com tamanho do vetor
        cmp r3, r5
        // interronpe o loop se valor do contador é igual ao tamanho do
        // vetor
        beq 3
        // rotina de substuição de valores
        call substitui
        // incremento do contador
        addi r3, r3, 1
        // reiniciar o loop
        bun -5
        // retoma o valor anterior de r3
        pop r3

        ret
    // Funcao principal
    main:
        // SP = 32 KiB
        mov sp, 0x7FFC
        mov r5, 7
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
        .4byte 4, 9, 1, 3, 2, 5, 7
    // Endereco do dispositivo (OUT)
    terminal:
        .4byte 0x8888888B
        
        // mov r3, 0
        // cmpi r3, 3
        // beq 2
        // addi r3, r3, 1
        // bun -4
        // <-> for i := 0; i == 3; i++