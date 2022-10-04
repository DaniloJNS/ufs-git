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
        l32 r1, [terminal_out]
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
    print_digit:
        push sr, r6, r5, r4, r3
        // r5 - Digito atual a ser exigibido
        // r3 - valor não convertido que ainda deve ser exibido

        // rotina para compara converção para decimal (0-9)
        mov r4, 10
        div r5, r3, r3, r4
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
        pop r3, r4, r5, r6, sr
        ret

    print_number:
        // salvando registradores
        // r1 - ponteiro para o terminal
        // r2 - ponteiro da posição atual do vetor
        push r3, r5
        // R1 = Endereco do terminal
        l32 r1, [terminal_out]
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
        
    insertion:
        mov r4, V1
        divi r4, r4, 4
        ret

    substitui:
        push r1, r2, r3, r4, r5
        push r6, r7, r8, r9
        // r4 - ponteiro do vetor = j
        // r5 - tamanho do vetor
        // r6 - v[j]
        // r7 - v[j + 1]
        // r3 - contador do loop i 
        // r2 - contador do loop j
        // Pon5eir5 do vetor
        call insertion
        // do vetor
        // {{ key = arr[i];
            add r7, r4, r3
            l32 r6, [r7]
        // }}
        // j = i - 1
        subi r2, r3, 1
        // j >= 0
        cmpi r2, -1
        // devia se for falso
        beq 8

        // {{ r8 = arr[j]
            add r7, r4, r2
            addi r9, r7, 1
            l32 r8, [r7]
        // }}
        cmp r8, r6
        // arr[j + 1] = arr[j]
        // desvia ser for falso
        bbe 3
        // arr[j + 1] = arr[j]
        s32 [r9], r8
        // decremetno do contador
        subi r2, r2, 1
        bun -10

        // {{ arr[j+1] = key
            add r7, r4, r2
            addi r9, r7, 1
            s32 [r9], r6
        // }}
        pop r9, r8, r7, r6
        pop r5, r4, r3, r2, r1

        ret
    ordena:
        // salva na pilha o valor atual de r3
        push r3
        // r3 - contador
        // r5 - tamanho do vetor

        // inicializar contador
        mov r3, 1
        // compara o valor atual do contador com tamanho do vetor
        cmp r3, r5
        // interronpe o loop se valor do contador é igual ao tamanho do
        // vetor
        bgt 3
        // rotina de substuição de valores
        call substitui
        // Incrementa contador
        addi r3, r3, 1
        // reiniciar o loop
        bun -5
        
        // retoma o valor anterior de r3
        pop r3

        ret
    ler_entrada:
        // salva contexto
        push r1, r2, r3, r4, r5
        // ler ponteiro para entrada do terminal
        l32 r2, [terminal_in]
        // ponteiro do vector
        mov r1, V1
        // converte o tamanho do vetor para 8 bits
        muli r5, r5, 4
        // inicializa o contador
        mov r4, 0
        // compara o contador o tamanho do vetor
        cmp r4, r5
        // interrompe a a recursão se valor do contador é igual ao tamanho do vetor
        beq 5
        // ler bit da entrada do terminal
        l8 r3, [r2]
        // armazenar valor lido do terminal
        s8 [r1], r3
        // incrementar ponteiro do vetor
        addi r1, r1, 1
        // incrementa contador
        addi r4, r4, 1
        // reiniciar loop
        bun -7
        // retoma o contexto anterior
        pop r5, r4, r3, r2, r1
        // retorna função
        ret
    // Funcao principal
    main:
        // SP = 32 KiB
        mov sp, 0x7FFC
        // r5 - tamanho do vetor
        mov r5, 7
        // R2 = ponteiro da string
        mov r2, input_message
        call printf
        // ler entrada do terminal
        call ler_entrada
        // exibit todos os valores do vetor de forma sequancial
        call exibir_vetor
        // R2 = ponteiro da string
        mov r2, sorted_message
        // printf
        call printf
        // rotina de ordenação de vetores
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
        .fill 7, 4, 1
    // Endereco do dispositivo (OUT)
    terminal_in:
        .4byte 0x8888888A
    terminal_out:
        .4byte 0x8888888B