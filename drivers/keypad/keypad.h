#ifndef KEYPAD_H
#define KEYPAD_H

#include <stdint.h>

/**
 * @file keypad.h
 * @brief 3x4 Matrix Keypad Driver.
 */

/**
 * @brief Initialize the keypad GPIO pins.
 */
void Keypad_Init(void);

/**
 * @brief Scan the keypad and return the pressed key.
 * 
 * @return The character of the pressed key (e.g. '1', '*', etc), or 0 if nothing is pressed.
 */
char Keypad_GetKey(void);

#endif // KEYPAD_H
