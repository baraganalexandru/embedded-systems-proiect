#include "keypad.h"
#include "bsp/nano.h"
#include "drivers/gpio/gpio.h"
#include "utils/delay.h"

// Define Rows (Inputs, with Pull-ups)
// Mapped based on Diagnostic_Keypad output
#define R1_PORT GPIO_PORTB
#define R1_PIN  0  // D8
#define R2_PORT GPIO_PORTD
#define R2_PIN  7  // D7
#define R3_PORT GPIO_PORTD
#define R3_PIN  6  // D6
#define R4_PORT GPIO_PORTD
#define R4_PIN  5  // D5

// Define Cols (Outputs)
#define C1_PORT GPIO_PORTD
#define C1_PIN  4  // D4
#define C2_PORT GPIO_PORTD
#define C2_PIN  3  // D3
#define C3_PORT GPIO_PORTD
#define C3_PIN  2  // D2

static const char keys[4][3] = {
    {'1', '2', '3'},
    {'4', '5', '6'},
    {'7', '8', '9'},
    {'*', '0', '#'}
};

void Keypad_Init(void) {
    // Setup Rows as Input
    GPIO_Init(R1_PORT, R1_PIN, GPIO_INPUT);
    GPIO_Init(R2_PORT, R2_PIN, GPIO_INPUT);
    GPIO_Init(R3_PORT, R3_PIN, GPIO_INPUT);
    GPIO_Init(R4_PORT, R4_PIN, GPIO_INPUT);
    
    // Enable Pull-ups on Rows
    GPIO_Write(R1_PORT, R1_PIN, GPIO_HIGH);
    GPIO_Write(R2_PORT, R2_PIN, GPIO_HIGH);
    GPIO_Write(R3_PORT, R3_PIN, GPIO_HIGH);
    GPIO_Write(R4_PORT, R4_PIN, GPIO_HIGH);
    
    // Setup Cols as Output
    GPIO_Init(C1_PORT, C1_PIN, GPIO_OUTPUT);
    GPIO_Init(C2_PORT, C2_PIN, GPIO_OUTPUT);
    GPIO_Init(C3_PORT, C3_PIN, GPIO_OUTPUT);
    
    // Set Cols HIGH by default
    GPIO_Write(C1_PORT, C1_PIN, GPIO_HIGH);
    GPIO_Write(C2_PORT, C2_PIN, GPIO_HIGH);
    GPIO_Write(C3_PORT, C3_PIN, GPIO_HIGH);
}

char Keypad_GetKey(void) {
    char pressed_key = 0;
    
    for (uint8_t c = 0; c < 3; c++) {
        // Set current column LOW
        if (c == 0) GPIO_Write(C1_PORT, C1_PIN, GPIO_LOW);
        else if (c == 1) GPIO_Write(C2_PORT, C2_PIN, GPIO_LOW);
        else if (c == 2) GPIO_Write(C3_PORT, C3_PIN, GPIO_LOW);
        
        Delay(2); // Small delay to allow voltage to settle
        
        // Read rows
        if (GPIO_Read(R1_PORT, R1_PIN) == GPIO_LOW) pressed_key = keys[0][c];
        else if (GPIO_Read(R2_PORT, R2_PIN) == GPIO_LOW) pressed_key = keys[1][c];
        else if (GPIO_Read(R3_PORT, R3_PIN) == GPIO_LOW) pressed_key = keys[2][c];
        else if (GPIO_Read(R4_PORT, R4_PIN) == GPIO_LOW) pressed_key = keys[3][c];
        
        // Set current column HIGH again
        if (c == 0) GPIO_Write(C1_PORT, C1_PIN, GPIO_HIGH);
        else if (c == 1) GPIO_Write(C2_PORT, C2_PIN, GPIO_HIGH);
        else if (c == 2) GPIO_Write(C3_PORT, C3_PIN, GPIO_HIGH);
        
        if (pressed_key != 0) {
            break;
        }
    }
    
    return pressed_key;
}