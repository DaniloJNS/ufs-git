// Definições de E/S
#include <avr/io.h>
// Atraso de tempo
#include <util/delay.h>

// função principal
int main()
{
    DDRB = DDRB | (1 << DDB5);

    while (1) {
        PORTB = PORTB ^ (1 << PORTB5);
    }

    _delay_ms(1000);
    return 0;
}
