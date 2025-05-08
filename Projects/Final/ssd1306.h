#ifndef SSD1306_H
#define SSD1306_H

#include <stdint.h>
#include <stddef.h>

#define SSD1306_I2C_ADDRESS 0x3C

void SSD1306_Init(void);
void SSD1306_Clear(void);
void SSD1306_SetCursor(uint8_t col, uint8_t page);
void SSD1306_WriteChar(char c);
void SSD1306_WriteString(const char* str);
void SSD1306_WriteData(uint8_t* data, size_t len);

#endif
