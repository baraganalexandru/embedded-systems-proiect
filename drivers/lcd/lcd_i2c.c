#include "lcd_i2c.h"
#include "drivers/i2c/i2c.h"
#include "utils/delay.h"

// LCD Commands
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

// Flags for display on/off control
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

// Flags for function set
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00

// PCF8574 bit definitions
#define EN 0x04 // Enable bit
#define RW 0x02 // Read/Write bit
#define RS 0x01 // Register select bit
#define BL 0x08 // Backlight bit

static uint8_t _i2c_addr;
static uint8_t _displaycontrol;

static void I2C_WriteExpander(uint8_t data) {
    I2C_Start((_i2c_addr << 1) | 0); // Write mode
    I2C_Write(data | BL); // Keep backlight on
    I2C_Stop();
}

static void LCD_PulseEnable(uint8_t data) {
    I2C_WriteExpander(data | EN); // En HIGH
    Delay(1); // Enable pulse must be >450ns
    
    I2C_WriteExpander(data & ~EN); // En LOW
    Delay(1); // Commands need >37us to settle
}

static void LCD_Write4Bits(uint8_t value) {
    I2C_WriteExpander(value);
    LCD_PulseEnable(value);
}

static void LCD_SendCommand(uint8_t value) {
    uint8_t highnib = value & 0xF0;
    uint8_t lownib = (value << 4) & 0xF0;
    
    LCD_Write4Bits(highnib | 0); // RS = 0
    LCD_Write4Bits(lownib | 0);  // RS = 0
}

static void LCD_SendData(uint8_t value) {
    uint8_t highnib = value & 0xF0;
    uint8_t lownib = (value << 4) & 0xF0;
    
    LCD_Write4Bits(highnib | RS); // RS = 1
    LCD_Write4Bits(lownib | RS);  // RS = 1
}

void LCD_Init(uint8_t i2c_addr) {
    _i2c_addr = i2c_addr;
    
    // SEE PAGE 45/46 FOR INITIALIZATION SPECIFICATION!
    // according to datasheet, we need at least 40ms after power rises above 2.7V
    // before sending commands. Arduino can turn on way befer 4.5V so we'll wait 50
    Delay(50);
    
    // Now we pull both RS and R/W low to begin commands
    I2C_WriteExpander(0);
    Delay(1000);
    
    // Put the LCD into 4 bit mode
    // this is according to the hitachi HD44780 datasheet
    // figure 24, pg 46
    
    // we start in 8bit mode, try to set 4 bit mode
    LCD_Write4Bits(0x30);
    Delay(5); // wait min 4.1ms
    
    // second try
    LCD_Write4Bits(0x30);
    Delay(5); // wait min 4.1ms
    
    // third go!
    LCD_Write4Bits(0x30); 
    Delay(1);
    
    // finally, set to 4-bit interface
    LCD_Write4Bits(0x20); 
    
    // set # lines, font size, etc.
    LCD_SendCommand(LCD_FUNCTIONSET | LCD_4BITMODE | LCD_2LINE | LCD_5x8DOTS);  
    
    // turn the display on with no cursor or blinking default
    _displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
    LCD_SendCommand(LCD_DISPLAYCONTROL | _displaycontrol);
    
    // clear it off
    LCD_Clear();
    
    // Initialize to default text direction (for roman languages)
    LCD_SendCommand(LCD_ENTRYMODESET | 0x02); // Left to right
}

void LCD_Clear(void) {
    LCD_SendCommand(LCD_CLEARDISPLAY);
    Delay(2); // this command takes a long time!
}

void LCD_SetCursor(uint8_t col, uint8_t row) {
    int row_offsets[] = { 0x00, 0x40, 0x14, 0x54 };
    if (row > 1) {
        row = 1; // we only support 2 lines right now safely
    }
    LCD_SendCommand(LCD_SETDDRAMADDR | (col + row_offsets[row]));
}

void LCD_Print(const char *str) {
    while (*str) {
        LCD_SendData(*str++);
    }
}
