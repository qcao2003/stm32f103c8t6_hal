#include "oled.h"
#include "oledfont.h"

const unsigned char bmp_data[]=
{
0x00,0x00,0x00,0x80,0x80,0x80,0x80,0x00,0x80,0x80,0x60,0x40,0x80,0x80,0xC0,0xE0,
0xE0,0x40,0x20,0x10,0x50,0xB0,0xE0,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x1C,0x1F,0x09,0x18,0x18,0x10,0x10,0x01,0x1F,0x1F,0x1F,0x1F,0x09,0x01,0x11,0x15,
0x10,0x11,0x11,0x1F,0x1D,0x1C,0x1F,0x17,0x12,0x18,0x10,0x10,0x11,0x18,0x1F,0x1F
};

BMP bmp=
{
	32,	//������
	13,	//������
	bmp_data
};

void SSD1306_WriteByte(uint8_t dat,uint8_t cmd)
{
	uint8_t buffer[2];
	buffer[0]=cmd;
	buffer[1]=dat;
	HAL_I2C_Master_Transmit(&hi2c1,0x78,buffer,2,0xffff);
}

void OLED_Clear(void)
{
  uint8_t i,n;		    
	for(i=0;i<8;i++)  
	{  
		SSD1306_WriteByte (0xb0+i,OLED_CMD);    //����ҳ��ַ��0~7��
		SSD1306_WriteByte (0x00,OLED_CMD);      //������ʾλ�á��е͵�ַ
		SSD1306_WriteByte (0x10,OLED_CMD);      //������ʾλ�á��иߵ�ַ   
		for(n=0;n<128;n++)
			SSD1306_WriteByte(0,OLED_DATA); //write 0x00;
	} //������ʾ
}

void OLED_Set_Pos(unsigned char x, unsigned char y) 
{ 	
	SSD1306_WriteByte(0xb0+y,OLED_CMD);						//page y
	SSD1306_WriteByte(((x&0xf0)>>4)|0x10,OLED_CMD);			//high 4 address
	SSD1306_WriteByte((x&0x0f),OLED_CMD); 					//low 4 address
} 

void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr,uint8_t Char_Size)
{      	
	unsigned char c=0,i=0;	
		c=chr-' ';//�õ�ƫ�ƺ��ֵ			
		if(x>Max_Column-1){x=0;y=y+2;}
		if(Char_Size ==16)
			{
				OLED_Set_Pos(x,y);	
				for(i=0;i<8;i++)
				SSD1306_WriteByte(F8X16[c*16+i],OLED_DATA);
				OLED_Set_Pos(x,y+1);
				for(i=0;i<8;i++)
				SSD1306_WriteByte(F8X16[c*16+i+8],OLED_DATA);
			}
		else 
			{	
				OLED_Set_Pos(x,y);
				for(i=0;i<6;i++)
				SSD1306_WriteByte(F6x8[c][i],OLED_DATA);
			}
}

void OLED_ShowString(uint8_t x,uint8_t y,uint8_t *chr,uint8_t Char_Size)
{
	unsigned char j=0;
	while (chr[j]!='\0')
	{		OLED_ShowChar(x,y,chr[j],Char_Size);
			x+=8;
		if(x>120){x=0;y+=2;}
			j++;
	}
}

void OLED_ShowPic(uint8_t x,uint8_t y,BMP bmp)
{
	uint8_t xsize=bmp.xSize;
	uint8_t ysize=bmp.ySize;
	OLED_Set_Pos(x,y);
	//������ظ߶Ȳ���8�ı��������������1����
	if(ysize%8!=0)
		ysize=ysize+8;
	uint8_t i,j;
	for(i=0;i<ysize/8;i++)
	{
		OLED_Set_Pos(x,y+i);
		for(j=0;j<xsize;j++)
		SSD1306_WriteByte(*(bmp.p+i*xsize+j),OLED_DATA);
	}
}


//m^n����
uint32_t oled_pow(uint8_t m,uint8_t n)
{
	uint32_t result=1;	 
	while(n--)result*=m;    
	return result;
}				


//��ʾ2������
//x,y :�������	 
//len :���ֵ�λ��
//size:�����С
//mode:ģʽ	0,���ģʽ;1,����ģʽ
//num:��ֵ(0~4294967295);	 		  
void OLED_ShowNum(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size2)
{         	
	uint8_t t,temp;
	uint8_t enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/oled_pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar(x+(size2/2)*t,y,' ',size2);
				continue;
			}else enshow=1; 
		 	 
		}
	 	OLED_ShowChar(x+(size2/2)*t,y,temp+'0',size2); 
	}
} 

                        
void OLED_Init(void)
{
	HAL_Delay(100);
	SSD1306_WriteByte(0xAE,OLED_CMD); //display off
	
	SSD1306_WriteByte(0x20,OLED_CMD); //Set Memory Addressing Mode   
	SSD1306_WriteByte(0x10,OLED_CMD); //00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
	
	SSD1306_WriteByte(0x00,OLED_CMD); //---set low column address
	SSD1306_WriteByte(0x10,OLED_CMD); //---set high column address
	
	SSD1306_WriteByte(0xB0,OLED_CMD); //Set Page Start Address for Page Addressing Mode,0-7
	SSD1306_WriteByte(0xC8,OLED_CMD); //Set COM Output Scan Direction
 
	SSD1306_WriteByte(0x40,OLED_CMD); //--set start line address
	
	SSD1306_WriteByte(0x81,OLED_CMD); //--set contrast control register
	SSD1306_WriteByte(0xff,OLED_CMD); //�Աȶ�����
	
	SSD1306_WriteByte(0xA1,OLED_CMD); //--set segment re-map 0 to 127
	SSD1306_WriteByte(0xC8,OLED_CMD); //--set segment re-map 0 to 127
	
	SSD1306_WriteByte(0xA6,OLED_CMD); //--set normal display
	
	SSD1306_WriteByte(0xA8,OLED_CMD); //--set multiplex ratio(1 to 64)
	SSD1306_WriteByte(0x3F,OLED_CMD); //
	
	SSD1306_WriteByte(0xA4,OLED_CMD); //0xa4,Output follows RAM content;0xa5,Output ignores RAM content
	
	SSD1306_WriteByte(0xD3,OLED_CMD); //-set display offset
	SSD1306_WriteByte(0x00,OLED_CMD); //-not offset
	
	SSD1306_WriteByte(0xD5,OLED_CMD); //--set display clock divide ratio/oscillator frequency
	SSD1306_WriteByte(0xF0,OLED_CMD); //--set divide ratio
	
	SSD1306_WriteByte(0xD9,OLED_CMD); //--set pre-charge period
	SSD1306_WriteByte(0x22,OLED_CMD); //
	
	SSD1306_WriteByte(0xDA,OLED_CMD); //--set com pins hardware configuration
	SSD1306_WriteByte(0x12,OLED_CMD);
	
	SSD1306_WriteByte(0xDB,OLED_CMD); //--set vcomh
	SSD1306_WriteByte(0x20,OLED_CMD); //0x20,0.77xVcc
	
	SSD1306_WriteByte(0x8D,OLED_CMD); //--set DC-DC enable
	SSD1306_WriteByte(0x14,OLED_CMD); //
	SSD1306_WriteByte(0xAF,OLED_CMD); //--turn on SSD1306 panel
}

