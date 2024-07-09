#ifndef __OLED_H
#define __OLED_H	

#include "stdlib.h"	   
#include "i2c.h"

#define OLED_CMD  0X00 //д����
#define OLED_DATA 0X40 //д����
 
#define SSD1306_I2C_ADDR         0x78
#define SSD1306_WIDTH            128
#define SSD1306_HEIGHT           6
 
#define SIZE 16
#define XLevelL		0x02
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xFF 
#define X_WIDTH 	128
#define Y_WIDTH 	64

typedef struct
{
	uint8_t xSize;
	uint8_t ySize;
	const unsigned char *p;
}BMP;

extern BMP bmp;

void SSD1306_WriteByte(uint8_t dat,uint8_t cmd);
void OLED_Clear(void);
void OLED_Set_Pos(unsigned char x, unsigned char y);
void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr,uint8_t Char_Size);
void OLED_ShowString(uint8_t x,uint8_t y,uint8_t *chr,uint8_t Char_Size);
void OLED_ShowPic(uint8_t x,uint8_t y,BMP bmp);
void OLED_ShowNum(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size1);
void OLED_Init(void);

#endif


