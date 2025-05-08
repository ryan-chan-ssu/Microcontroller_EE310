#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <xc.h>
#include "ssd1306.h"
#include "mcc_generated_files/i2c_host/i2c1.h"
#define _XTAL_FREQ 4000000  // Or whatever your actual Fosc is


#define SSD1306_ADDRESS 0x3C

static void SSD1306_WriteCommand(uint8_t cmd) {
    uint8_t packet[2] = {0x00, cmd};
    I2C1_Write(SSD1306_ADDRESS, packet, 2);
}

void SSD1306_WriteData(uint8_t* data, size_t len) {
    uint8_t packet[129];
    packet[0] = 0x40;
    memcpy(&packet[1], data, len);
    I2C1_Write(SSD1306_ADDRESS, packet, len + 1);
}

void SSD1306_Init(void) {
    __delay_ms(100);

    SSD1306_WriteCommand(0xAE); // display off
    SSD1306_WriteCommand(0xD5); SSD1306_WriteCommand(0x80);
    SSD1306_WriteCommand(0xA8); SSD1306_WriteCommand(0x3F);
    SSD1306_WriteCommand(0xD3); SSD1306_WriteCommand(0x00);
    SSD1306_WriteCommand(0x40);
    SSD1306_WriteCommand(0x8D); SSD1306_WriteCommand(0x14);
    SSD1306_WriteCommand(0x20); SSD1306_WriteCommand(0x00);
    SSD1306_WriteCommand(0xA1);
    SSD1306_WriteCommand(0xC8);
    SSD1306_WriteCommand(0xDA); SSD1306_WriteCommand(0x12);
    SSD1306_WriteCommand(0x81); SSD1306_WriteCommand(0xCF);
    SSD1306_WriteCommand(0xD9); SSD1306_WriteCommand(0xF1);
    SSD1306_WriteCommand(0xDB); SSD1306_WriteCommand(0x40);
    SSD1306_WriteCommand(0xA4);
    SSD1306_WriteCommand(0xA6);
    SSD1306_WriteCommand(0xAF); // display on
}

void SSD1306_SetCursor(uint8_t col, uint8_t page) {
    SSD1306_WriteCommand(0xB0 | page);                    // page address
    SSD1306_WriteCommand(0x00 | (col & 0x0F));            // lower column
    SSD1306_WriteCommand(0x10 | ((col >> 4) & 0x0F));     // upper column
}

void SSD1306_Clear(void) {
    uint8_t empty[128] = {0};
    for (uint8_t page = 0; page < 8; page++) {
        SSD1306_SetCursor(0, page);
        SSD1306_WriteData(empty, 128);
    }
}

void SSD1306_WriteString(const char* str) {
    SSD1306_SetCursor(0, 0);
    while (*str) {
        uint8_t data[5] = {0x00, 0x00, 0x00, 0x00, 0x00}; // Blank box
        SSD1306_WriteData(data, 5);  // placeholder glyphs
        str++;
    }
}
