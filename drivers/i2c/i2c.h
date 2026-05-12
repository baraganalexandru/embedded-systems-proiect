#ifndef I2C_H
#define I2C_H

#include <stdint.h>

/**
 * @file i2c.h
 * @brief Hardware I2C (TWI) Master Driver.
 */

/**
 * @brief Initialize the I2C interface.
 * 
 * @param scl_freq Desired SCL frequency in Hz (e.g., 100000 for 100kHz).
 */
void I2C_Init(uint32_t scl_freq);

/**
 * @brief Send a START condition and the slave address.
 * 
 * @param addr Slave address (7-bit) shifted left by 1, OR'ed with Read(1) or Write(0) bit.
 */
void I2C_Start(uint8_t addr);

/**
 * @brief Send a STOP condition.
 */
void I2C_Stop(void);

/**
 * @brief Write a byte over I2C.
 * 
 * @param data Byte to write.
 */
void I2C_Write(uint8_t data);

#endif // I2C_H
