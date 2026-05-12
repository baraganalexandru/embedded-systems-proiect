#include "i2c.h"
#include <avr/io.h>
#include <util/twi.h>

void I2C_Init(uint32_t scl_freq) {
    // Calculate TWBR based on F_CPU and scl_freq.
    // TWSR prescaler is set to 1 (bits 0:1 = 00).
    TWSR = 0x00;
    TWBR = (uint8_t)(((F_CPU / scl_freq) - 16) / 2);
    TWCR = (1 << TWEN); // Enable TWI
}

void I2C_Start(uint8_t addr) {
    // Send START condition
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
    
    // Wait for TWINT flag set (START transmitted)
    while (!(TWCR & (1 << TWINT)));

    // Load slave address into TWDR
    TWDR = addr;
    
    // Clear TWINT bit to start transmission of address
    TWCR = (1 << TWINT) | (1 << TWEN);
    
    // Wait for TWINT flag set (Address transmitted)
    while (!(TWCR & (1 << TWINT)));
}

void I2C_Stop(void) {
    // Transmit STOP condition
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
    
    // Wait for STOP to complete
    while (TWCR & (1 << TWSTO));
}

void I2C_Write(uint8_t data) {
    // Load data into TWDR
    TWDR = data;
    
    // Clear TWINT bit to start transmission
    TWCR = (1 << TWINT) | (1 << TWEN);
    
    // Wait for TWINT flag set (Data transmitted)
    while (!(TWCR & (1 << TWINT)));
}
