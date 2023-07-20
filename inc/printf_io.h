
#ifndef __PRINTF_IO_H
#define __PRINTF_IO_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32l1xx.h"
#include "stdio.h"

#define LCD_RST_AHB  RCC_AHBPeriph_GPIOC
#define LCD_RST_PORT GPIOC
#define LCD_RST_PIN  GPIO_Pin_5

#define LCD_ENB_AHB  RCC_AHBPeriph_GPIOB
#define LCD_ENB_PORT GPIOB
#define LCD_ENB_PIN  GPIO_Pin_13

#define LCD_CMD_AHB  RCC_AHBPeriph_GPIOB
#define LCD_CMD_PORT GPIOB
#define LCD_CMD_PIN  GPIO_Pin_1

#define LCD_CLK_AHB  RCC_AHBPeriph_GPIOB
#define LCD_CLK_PORT GPIOB
#define LCD_CLK_PIN  GPIO_Pin_2

#define LCD_DTA_AHB  RCC_AHBPeriph_GPIOB
#define LCD_DTA_PORT GPIOB
#define LCD_DTA_PIN  GPIO_Pin_14

#define LCD_BL_AHB   RCC_AHBPeriph_GPIOB
#define LCD_BL_PORT  GPIOB
#define LCD_BL_PIN   GPIO_Pin_0

const uint8_t init_params[] =
{
    0xF1, 0x0F, 0xF8, 0x01, 0x06, 0xF0, 0x00, 0x2D, 0xA8, 0xA3, 0x81, 0x8, 0xFF
};

const uint8_t char_table[111][6] =
{
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // SPACE
    0x00, 0x00, 0x5F, 0x00, 0x00, 0x00, // !
    0x00, 0x03, 0x00, 0x03, 0x00, 0x00, // "
    0x14, 0x3E, 0x14, 0x3E, 0x14, 0x00, // #
    0x24, 0x2A, 0x7F, 0x2A, 0x12, 0x00, // $
    0x43, 0x33, 0x08, 0x66, 0x61, 0x00, // %
    0x36, 0x49, 0x55, 0x22, 0x50, 0x00, // &
    0x00, 0x05, 0x03, 0x00, 0x00, 0x00, // '
    0x00, 0x1C, 0x22, 0x41, 0x00, 0x00, // (
    0x00, 0x41, 0x22, 0x1C, 0x00, 0x00, // )
    0x14, 0x08, 0x3E, 0x08, 0x14, 0x00, // *
    0x08, 0x08, 0x3E, 0x08, 0x08, 0x00, // +
    0x00, 0x50, 0x30, 0x00, 0x00, 0x00, // ,
    0x08, 0x08, 0x08, 0x08, 0x08, 0x00, // -
    0x00, 0x60, 0x60, 0x00, 0x00, 0x00, // .
    0x20, 0x10, 0x08, 0x04, 0x02, 0x00, // /
    0x3E, 0x51, 0x49, 0x45, 0x3E, 0x00, // 0
    0x04, 0x02, 0x7F, 0x00, 0x00, 0x00, // 1
    0x42, 0x61, 0x51, 0x49, 0x46, 0x00, // 2
    0x22, 0x41, 0x49, 0x49, 0x36, 0x00, // 3
    0x18, 0x14, 0x12, 0x7F, 0x10, 0x00, // 4
    0x27, 0x45, 0x45, 0x45, 0x39, 0x00, // 5
    0x3E, 0x49, 0x49, 0x49, 0x32, 0x00, // 6
    0x01, 0x01, 0x71, 0x09, 0x07, 0x00, // 7
    0x36, 0x49, 0x49, 0x49, 0x36, 0x00, // 8
    0x26, 0x49, 0x49, 0x49, 0x3E, 0x00, // 9
    0x00, 0x36, 0x36, 0x00, 0x00, 0x00, // :
    0x00, 0x56, 0x36, 0x00, 0x00, 0x00, // ;
    0x08, 0x14, 0x22, 0x41, 0x00, 0x00, // <
    0x14, 0x14, 0x14, 0x14, 0x14, 0x00, // =
    0x00, 0x41, 0x22, 0x14, 0x08, 0x00, // >
    0x02, 0x01, 0x51, 0x09, 0x06, 0x00, // ?
    0x00, 0x06, 0x09, 0x09, 0x06, 0x00, // @
    0x7E, 0x09, 0x09, 0x09, 0x7E, 0x00, // A
    0x7F, 0x49, 0x49, 0x49, 0x36, 0x00, // B
    0x3E, 0x41, 0x41, 0x41, 0x22, 0x00, // C
    0x7F, 0x41, 0x41, 0x41, 0x3E, 0x00, // D
    0x7F, 0x49, 0x49, 0x49, 0x41, 0x00, // E
    0x7F, 0x09, 0x09, 0x09, 0x01, 0x00, // F
    0x3E, 0x41, 0x41, 0x49, 0x3A, 0x00, // G
    0x7F, 0x08, 0x08, 0x08, 0x7F, 0x00, // H
    0x00, 0x41, 0x7F, 0x41, 0x00, 0x00, // I
    0x30, 0x40, 0x40, 0x40, 0x3F, 0x00, // J
    0x7F, 0x08, 0x14, 0x22, 0x41, 0x00, // K
    0x7F, 0x40, 0x40, 0x40, 0x40, 0x00, // L
    0x7F, 0x02, 0x0C, 0x02, 0x7F, 0x00, // M
    0x7F, 0x02, 0x04, 0x08, 0x7F, 0x00, // N
    0x3E, 0x41, 0x41, 0x41, 0x3E, 0x00, // O
    0x7F, 0x09, 0x09, 0x09, 0x06, 0x00, // P
    0x1E, 0x21, 0x21, 0x21, 0x5E, 0x00, // Q
    0x7F, 0x09, 0x09, 0x09, 0x76, 0x00, // R
    0x26, 0x49, 0x49, 0x49, 0x32, 0x00, // S
    0x01, 0x01, 0x7F, 0x01, 0x01, 0x00, // T
    0x3F, 0x40, 0x40, 0x40, 0x3F, 0x00, // U
    0x1F, 0x20, 0x40, 0x20, 0x1F, 0x00, // V
    0x7F, 0x20, 0x10, 0x20, 0x7F, 0x00, // W
    0x63, 0x14, 0x08, 0x14, 0x63, 0x00, // X
    0x07, 0x08, 0x70, 0x08, 0x07, 0x00, // Y
    0x61, 0x51, 0x49, 0x45, 0x43, 0x00, // Z
    0x00, 0x7F, 0x41, 0x00, 0x00, 0x00, // [
    0x02, 0x04, 0x08, 0x10, 0x20, 0x00, //
    0x00, 0x00, 0x41, 0x7F, 0x00, 0x00, // ]
    0x00, 0x06, 0x09, 0x09, 0x06, 0x00, // ^ -> degree
    0x40, 0x40, 0x40, 0x40, 0x40, 0x00, // _
    0x00, 0x01, 0x02, 0x04, 0x00, 0x00, // `
    0x20, 0x54, 0x54, 0x54, 0x78, 0x00, // a
    0x7F, 0x44, 0x44, 0x44, 0x38, 0x00, // b
    0x38, 0x44, 0x44, 0x44, 0x44, 0x00, // c
    0x38, 0x44, 0x44, 0x44, 0x7F, 0x00, // d
    0x38, 0x54, 0x54, 0x54, 0x18, 0x00, // e
    0x04, 0x04, 0x7E, 0x05, 0x05, 0x00, // f
    0x08, 0x54, 0x54, 0x54, 0x3C, 0x00, // g
    0x7F, 0x08, 0x04, 0x04, 0x78, 0x00, // h
    0x00, 0x44, 0x7D, 0x40, 0x00, 0x00, // i
    0x20, 0x40, 0x44, 0x3D, 0x00, 0x00, // j
    0x7F, 0x10, 0x28, 0x44, 0x00, 0x00, // k
    0x00, 0x41, 0x7F, 0x40, 0x00, 0x00, // l
    0x7C, 0x04, 0x78, 0x04, 0x78, 0x00, // m
    0x7C, 0x08, 0x04, 0x04, 0x78, 0x00, // n
    0x38, 0x44, 0x44, 0x44, 0x38, 0x00, // o
    0x7C, 0x14, 0x14, 0x14, 0x08, 0x00, // p
    0x08, 0x14, 0x14, 0x14, 0x7C, 0x00, // q
    0x00, 0x7C, 0x08, 0x04, 0x04, 0x00, // r
    0x48, 0x54, 0x54, 0x54, 0x20, 0x00, // s
    0x04, 0x04, 0x3F, 0x44, 0x44, 0x00, // t
    0x3C, 0x40, 0x40, 0x20, 0x7C, 0x00, // u
    0x1C, 0x20, 0x40, 0x20, 0x1C, 0x00, // v
    0x3C, 0x40, 0x30, 0x40, 0x3C, 0x00, // w
    0x44, 0x28, 0x10, 0x28, 0x44, 0x00, // x
    0x0C, 0x50, 0x50, 0x50, 0x3C, 0x00, // y
    0x44, 0x64, 0x54, 0x4C, 0x44, 0x00, // z
    0x00, 0x08, 0x36, 0x41, 0x41, 0x00, // {
    0x00, 0x00, 0x7F, 0x00, 0x00, 0x00, // |
    0x41, 0x41, 0x36, 0x08, 0x00, 0x00, // }
    0x02, 0x01, 0x02, 0x04, 0x02, 0x00, // ~
    0x30, 0x48, 0xC8, 0x48, 0x00, 0x00, //ç
    0x10, 0xAA, 0xAA, 0xA8, 0x70, 0x00, //ğ
    0x00, 0x00, 0x7C, 0x00, 0x00, 0x00, //ı
    0x38, 0x45, 0x44, 0x45, 0x38, 0x00, //ö
    0x58, 0x54, 0xD4, 0x54, 0x30, 0x00, //ş
    0x38, 0x41, 0x40, 0x41, 0x38, 0x00, //ü
    0x3E, 0x41, 0xC1, 0x41, 0x22, 0x00, //Ç
    0x3C, 0x43, 0x53, 0x53, 0x34, 0x00, //Ğ
    0x00, 0x00, 0x7D, 0x00, 0x00, 0x00, //İ
    0x3C, 0x43, 0x42, 0x43, 0x3C, 0x00, //Ö
    0x26, 0x49, 0xC9, 0x49, 0x32, 0x00, //Ş
    0x3C, 0x41, 0x40, 0x41, 0x3C, 0x00, //Ü
    0x00, 0x00, 0x11, 0x15, 0x0A, 0x00, //3
    0x10, 0x38, 0x7C, 0xFE, 0x38, 0x38, //<
    0x38, 0x38, 0xFE, 0x7C, 0x38, 0x10  //>
};

void printf_init(void);
int _write(int fd, char *pbuffer, int size);

#ifdef __cplusplus
}
#endif

#endif
