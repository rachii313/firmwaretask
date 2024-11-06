#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#define F_CPU 16000000UL //clock frequency

 //Baud rate configuration (2400 baud rate)
#define MY_UBRR F_CPU/16/2400-1
// EEPROM memory size: Define enough space for the text data
#define EEPROM_START_ADDR 0x00
// Function to initialize UART
void uart_init(unsigned int ubrr) {
    UBRR0H = (unsigned char)(ubrr >> 8);
    UBRR0L = (unsigned char)(ubrr);
    UCSR0B = (1 << RXEN0) | (1 << TXEN0);  // Enable receiver and transmitter
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // 8 data bits, 1 stop bit
}
// Function to send a byte over UART
void uart_send(unsigned char data) {
    while (!(UCSR0A & (1 << UDRE0))); // Wait for the transmit buffer to be empty
    UDR0 = data; // Transmit data
}
// Function to receive a byte over UART
unsigned char uart_receive(void) {
    while (!(UCSR0A & (1 << RXC0))); // Wait for data to be received
    return UDR0; // Get and return received data from the UDR register
}
// Function to store data in EEPROM
void eeprom_write(unsigned int addr, unsigned char data) {
    while (EECR & (1 << EEPE)); // Wait for completion of previous write
    EEAR = addr;               // Set EEPROM address
    EEDR = data;               // Set EEPROM data register
    EECR |= (1 << EEMPE);      // Enable EEPROM write
    EECR |= (1 << EEPE);       // Start the write
}
// Function to retrieve data from EEPROM
unsigned char eeprom_read(unsigned int addr) {
    while (EECR & (1 << EEPE)); // Wait for completion of previous write
    EEAR = addr;               // Set EEPROM address
    EECR |= (1 << EERE);       // Start EEPROM read
    return EEDR;               // Return the read data
}
void delay_ms(unsigned int ms) {
    while (ms--) {
        _delay_ms(1);
    }
}

int main(void) {
    unsigned int ubrr = MY_UBRR;
    uart_init(ubrr); // Initialize UART with the calculated baud rate

    unsigned char byte_received;
    unsigned int data_index = 0;
    
    // Wait for the PC to send data
    while (1) {
        byte_received = uart_receive(); // Receive byte from PC
        eeprom_write(data_index++, byte_received); // Store byte in EEPROM

        // Assume we receive a special character (e.g., '\0') to signify the end of transmission
        if (byte_received == '\0') {
            break;
        }
    }

    // Now, transmit the data back to the PC from EEPROM
    data_index = 0;
    while (1) {
        byte_received = eeprom_read(data_index++);
        uart_send(byte_received); // Send the byte to PC
        delay_ms(1); // Optional: slow down the transfer for reliable testing

        // Assume we send a special character (e.g., '\0') to signify end of transmission
        if (byte_received == '\0') {
            break;
        }
    }

    return 0;
}
