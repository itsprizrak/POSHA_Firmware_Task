#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>

#define F_CPU 16000000UL
#define BAUD_RATE 2400
#define UBRR_VALUE ((F_CPU / 16 / BAUD_RATE) - 1)

volatile uint16_t eeprom_addr = 0, data_count = 0;
volatile uint32_t start_time = 0, end_time = 0;
volatile float transmission_speed = 0;

void USART_Init() {
    UBRR0H = (uint8_t)(UBRR_VALUE >> 8);
    UBRR0L = (uint8_t)UBRR_VALUE;
    UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void USART_Transmit(char data) {
    while (!(UCSR0A & (1 << UDRE0)));
    UDR0 = data;
}

void USART_TransmitString(const char *str) {
    while (*str) USART_Transmit(*str++);
}

void USART_TransmitFloat(float value) {
    char buffer[20];
    dtostrf(value, 10, 2, buffer);
    USART_TransmitString(buffer);
}

void Timer1_Init() {
    TCCR1B |= (1 << CS11);
}

uint32_t micros() {
    return ((uint32_t)TCNT1) * 4;
}

ISR(USART_RX_vect) {
    char received_data = UDR0;

    if (data_count == 0) start_time = micros();

    eeprom_write_byte((uint8_t *)eeprom_addr++, received_data);
    data_count++;

    if (data_count >= 1007) {
        end_time = micros();
        transmission_speed = ((float)data_count * 8) / ((end_time - start_time) / 1000000);
    }
}

void transmit_stored_data() {
    for (uint16_t i = 0; i < 1007; i++) {
        USART_Transmit(eeprom_read_byte((uint8_t *)i));
        _delay_ms(1);
    }
}

int main(void) {
    USART_Init();
    Timer1_Init();
    sei();

    while (data_count < 1007);

    transmit_stored_data();
    
    while (1);
}
