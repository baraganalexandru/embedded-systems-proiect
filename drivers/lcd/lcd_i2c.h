#ifndef LCD_I2C_H
#define LCD_I2C_H

#include <stdint.h>

/**
 * @file lcd_i2c.h
 * @brief I2C LCD Driver for HD44780 with PCF8574 backpack.
 */

// Common I2C Addresses for PCF8574
#define LCD_I2C_ADDR 0x27

/**
 * @brief Initialize the LCD.
 * 
 * @param i2c_addr The I2C address of the LCD backpack (usually 0x27).
 */
void LCD_Init(uint8_t i2c_addr);

/**
 * @brief Clear the LCD screen.
 */
void LCD_Clear(void);

/**
 * @brief Set the cursor position.
 * 
 * @param col Column (0-15).
 * @param row Row (0-1).
 */
void LCD_SetCursor(uint8_t col, uint8_t row);

/**
 * @brief Print a string to the LCD.
 * 
 * @param str Null-terminated string.
 */
void LCD_Print(const char *str);

#endif // LCD_I2C_H
