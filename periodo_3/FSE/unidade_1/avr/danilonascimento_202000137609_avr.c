// Definições de E/S 
#include <avr/io.h>
// Biblioteca padrão de E/S
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <ctype.h>
// Definição do clock do Sistema
#define F_CPU 16000000UL
// Definição da taxa de transmissão
#define BAUD 9600
// Biblioteca auxiliar para comunicação serial
#include <util/setbaud.h>
// Biblioteca de inteiros
#include <stdint.h>
// Procedimento de inicialização da UART 0 
void inicializar_uart0() {
    // Configurando a taxa de transmissão
    UBRR0H = UBRRH_VALUE;
    UBRR0L = UBRRL_VALUE;
    // Habilitando recepção e transmissão
    UCSR0B = (1 << RXEN0) | (1 << TXEN0);
    // Modo assincrono  e sem paridade
    // Quadro de 8 bits com 1 bit de parada
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}
// Função para envio de dado pela UART 0
int enviar_dado_uart0(char dado, FILE* fluxo) {
    // Checando por quebra de linha
    if(dado == '\n')
        enviar_dado_uart0('\r', fluxo);
    // Esperando por envio pedente
    while(!(UCSR0A & (1 << UDRE0)));
    // Enviando dado
    UDR0 = dado;
    // Retornado sem error
    return 0;
}
// Função de recebimento de dados pelo UART 0
int receber_dado_uart0(FILE* fluxo) {
    // Esperando por recebimento
    while(!(UCSR0A & (1 << RXC0)));
    // Retornando registrador de dado
    return UDR0;
}

// Criando fluxos de E/S serial
FILE stdin_uart0 = FDEV_SETUP_STREAM(NULL, receber_dado_uart0, _FDEV_SETUP_READ);
FILE stdout_uart0 = FDEV_SETUP_STREAM(enviar_dado_uart0, NULL, _FDEV_SETUP_WRITE);

// Caracteres de texto
char texto[100] = { 0 };
char morse[36][6] = {
    "-----",  // 48 - 0
    ".----",  // 49 - 1
    "..---",  // 50 - 2
    "...--",  // 51 - 3
    "....-",  // 52 - 4
    ".....",  // 53 - 5
    "-....",  // 54 - 6
    "--...",  // 55 - 7
    "---..",  // 56 - 8
    "----.",  // 57 - 9
    ".-",     // 65 - A
    "-...",   // 66 - B
    "-.-.",   // 67 - C
    "-..",    // 68 - D
    ".",      // 69 - E
    "..-.",   // 70 - F
    "--.",    // 71 - G
    "....",   // 72 - H
    "..",     // 73 - I
    ".---",   // 74 - J
    "-.-",    // 75 - K
    ".-..",   // 76 - L
    "--",     // 77 - M
    "-.",     // 78 - N
    "---",    // 79 - O
    ".--.",   // 80 - P
    "--.-",   // 81 - Q
    ".-.",    // 82 - R
    "...",    // 83 - S
    "-",      // 84 - T
    "..-",    // 85 - U
    "...-",   // 86 - V
    ".--",    // 87 - W
    "-..-",   // 88 - X
    "-.--",   // 89 - Y
    "--..",   // 90 - Z
};

int main()
{
    // Inicializando UART 0
    inicializar_uart0();
    // Reconfigurando E/S padrão
    stdin = &stdin_uart0;
    stdout = &stdout_uart0;
    // Laço infinito
    int number;
    char letter;
    while (1) {
        // Recebendo e enviando dados
        gets(texto);
        int texto_len = strlen(texto);

        for (int i = 0; i < texto_len - 1; ++i) {
            letter = texto[i];

            if(isdigit(letter) == 0 && isupper(letter) == 0)
                letter = texto[i] & ~(0x20);

            number = (int)letter;

            if (number >= 65)
                number -= 7;
            number -= 48;

            printf("%s", morse[number]);

            if(i < texto_len -2)
                printf(" ");
        }
        printf("\n");
    }
    
    return 0;
}
