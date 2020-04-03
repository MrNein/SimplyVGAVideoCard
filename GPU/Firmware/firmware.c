//----------------------------------------------------------------------------------------------------
//������� �����������
//----------------------------------------------------------------------------------------------------
#define F_CPU 16000000UL

#define bool unsigned char
#define true  1
#define false 0

//----------------------------------------------------------------------------------------------------
//����������
//----------------------------------------------------------------------------------------------------
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <avr/pgmspace.h>

//----------------------------------------------------------------------------------------------------
//���������� ����������
//----------------------------------------------------------------------------------------------------

const char Text_SD_Ok[] PROGMEM =                "SD-Card is ok.\0";
const char Text_SD_SPI_error[] PROGMEM =        "SD-Card: SPI error.\0";
const char Text_SD_No_response[] PROGMEM =        "SD-Card: No response.\0";
const char Text_SD_Size_error[] PROGMEM =        "SD-Card: Size error.\0";

const char Text_FAT_FAT_12_error[] PROGMEM =    "FAT: FAT12 - error.\0";
const char Text_FAT_FAT_32_error[] PROGMEM =    "FAT: FAT32 - error.\0";
const char Text_FAT_FAT_16_ok[] PROGMEM =        "FAT: FAT16 - ok.\0";
const char Text_FAT_No_file[] PROGMEM =            "FAT: No File.\0";
const char Text_FAT_Found_file[] PROGMEM =        "FAT: Found File.\0";

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//������ ��������
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
unsigned char symboltable[][5] PROGMEM =
{
 {0x00,0x00,0x00,0x00,0x00},                    //space   Start code ASCII-0x20
 {0x00,0x00,0x4f,0x00,0x00},                    //!
 {0x00,0x07,0x00,0x07,0x00},                    //"
 {0x14,0x7f,0x14,0x7f,0x14},                    //#
 {0x24,0x2a,0x7f,0x2a,0x12},                    //$
 {0x23,0x13,0x08,0x64,0x62},                    //%
 {0x36,0x49,0x55,0x22,0x40},                    //&
 {0x00,0x05,0x03,0x00,0x00},                    //,
 {0x00,0x1c,0x22,0x41,0x00},                    //(
 {0x00,0x41,0x22,0x1c,0x00},                    //)
 {0x14,0x08,0x3E,0x08,0x14},                    //*
 {0x08,0x08,0x3E,0x08,0x08},                    //+
 {0x00,0x50,0x30,0x00,0x00},                    //,
 {0x08,0x08,0x08,0x08,0x08},                    //-
 {0x00,0x60,0x60,0x00,0x00},                    //.
 {0x20,0x10,0x08,0x04,0x02},                    ///
 {0x3e,0x51,0x49,0x45,0x3e},                    //0
 {0x00,0x42,0x7f,0x40,0x00},                    //1
 {0x42,0x61,0x51,0x49,0x46},                    //2
 {0x21,0x41,0x45,0x4b,0x31},                    //3
 {0x18,0x14,0x12,0x7f,0x10},                    //4
 {0x27,0x45,0x45,0x45,0x39},                    //5
 {0x3c,0x4a,0x49,0x49,0x30},                    //6
 {0x01,0x71,0x09,0x05,0x03},                    //7
 {0x36,0x49,0x49,0x49,0x36},                    //8
 {0x06,0x49,0x49,0x29,0x1e},                    //9
 {0x00,0x36,0x36,0x00,0x00},                    //:
 {0x00,0x56,0x36,0x00,0x00},                    //;
 {0x08,0x14,0x22,0x41,0x00},                    //<
 {0x14,0x14,0x14,0x14,0x14},                    //=
 {0x00,0x41,0x22,0x14,0x08},                    //>
 {0x02,0x01,0x51,0x09,0x06},                    //?
 {0x32,0x49,0x71,0x41,0x3e},                    //@
 {0x7e,0x11,0x11,0x11,0x7e},                    //A
 {0x7f,0x49,0x49,0x49,0x36},                    //B
 {0x3e,0x41,0x41,0x41,0x22},                    //C
 {0x7f,0x41,0x41,0x22,0x1c},                    //D
 {0x7f,0x49,0x49,0x49,0x41},                    //E
 {0x7f,0x09,0x09,0x09,0x01},                    //F
 {0x3e,0x41,0x49,0x49,0x3a},                    //G
 {0x7f,0x08,0x08,0x08,0x7f},                    //H
 {0x00,0x41,0x7f,0x41,0x00},                    //I
 {0x20,0x40,0x41,0x3f,0x01},                    //J
 {0x7f,0x08,0x14,0x22,0x41},                    //K
 {0x7f,0x40,0x40,0x40,0x40},                    //L
 {0x7f,0x02,0x0c,0x02,0x7f},                    //M
 {0x7f,0x04,0x08,0x10,0x7f},                    //N
 {0x3e,0x41,0x41,0x41,0x3e},                    //O
 {0x7f,0x09,0x09,0x09,0x06},                    //P
 {0x3e,0x41,0x51,0x21,0x5e},                    //Q
 {0x7f,0x09,0x19,0x29,0x46},                    //R
 {0x46,0x49,0x49,0x49,0x31},                    //S
 {0x01,0x01,0x7f,0x01,0x01},                    //T
 {0x3f,0x40,0x40,0x40,0x3f},                    //U
 {0x1f,0x20,0x40,0x20,0x1f},                    //V
 {0x3f,0x40,0x30,0x40,0x3f},                    //W
 {0x63,0x14,0x08,0x14,0x63},                    //X
 {0x07,0x08,0x70,0x08,0x07},                    //Y
 {0x61,0x51,0x49,0x45,0x43},                    //Z
 {0x00,0x7F,0x41,0x41,0x00},                    //[
 {0x02,0x04,0x08,0x10,0x20},                    //slash
 {0x00,0x41,0x41,0x7F,0x00},                    //]
 {0x04,0x02,0x01,0x02,0x04},                    //^
 {0x40,0x40,0x40,0x40,0x40},                    //_
 {0x00,0x01,0x02,0x04,0x00},                    //'
 {0x20,0x54,0x54,0x54,0x78},                    //a
 {0x7F,0x48,0x44,0x44,0x38},                    //b
 {0x38,0x44,0x44,0x44,0x20},                    //c
 {0x38,0x44,0x44,0x48,0x7F},                    //d
 {0x38,0x54,0x54,0x54,0x18},                    //e
 {0x08,0x7E,0x09,0x01,0x02},                    //f
 {0x0C,0x52,0x52,0x52,0x3E},                    //g
 {0x7F,0x08,0x04,0x04,0x78},                    //h
 {0x00,0x44,0x7D,0x40,0x00},                    //i
 {0x20,0x40,0x44,0x3D,0x00},                    //j
 {0x7F,0x10,0x28,0x44,0x00},                    //k
 {0x00,0x41,0x7F,0x40,0x00},                    //l
 {0x7C,0x04,0x18,0x04,0x78},                    //m
 {0x7C,0x08,0x04,0x04,0x78},                    //n
 {0x38,0x44,0x44,0x44,0x38},                    //o
 {0x7C,0x14,0x14,0x14,0x08},                    //p
 {0x08,0x14,0x14,0x18,0x7C},                    //q
 {0x7C,0x08,0x04,0x04,0x08},                    //r
 {0x48,0x54,0x54,0x54,0x20},                    //s
 {0x04,0x3F,0x44,0x40,0x20},                    //t
 {0x3C,0x40,0x40,0x20,0x7C},                    //u
 {0x1C,0x20,0x40,0x20,0x1C},                    //v
 {0x3C,0x40,0x30,0x40,0x3C},                    //w
 {0x44,0x28,0x10,0x28,0x44},                    //x
 {0x0C,0x50,0x50,0x50,0x3C},                    //y
 {0x44,0x64,0x54,0x4C,0x44},                    //z
 {0x00,0x08,0x36,0x41,0x00},                    //{
 {0x00,0x00,0x7f,0x00,0x00},                    //|
 {0x00,0x41,0x36,0x08,0x00},                    //}
 {0x02,0x01,0x02,0x02,0x01},                    //~ END ASCII - 0x20;
 {0x00,0x00,0x00,0x00,0x00},                    //space
 {0x7e,0x11,0x11,0x11,0x7e},                    //A
 {0x7f,0x49,0x49,0x49,0x33},                    //�
 {0x7f,0x49,0x49,0x49,0x36},                    //�
 {0x7f,0x01,0x01,0x01,0x03},                    //�
 {0xe0,0x51,0x4f,0x41,0xff},                    //�
 {0x7f,0x49,0x49,0x49,0x41},                    //E
 {0x77,0x08,0x7f,0x08,0x77},                    //�
 {0x41,0x49,0x49,0x49,0x36},                    //�
 {0x7f,0x10,0x08,0x04,0x7f},                    //�
 {0x7c,0x21,0x12,0x09,0x7c},                    //�
 {0x7f,0x08,0x14,0x22,0x41},                    //K
 {0x20,0x41,0x3f,0x01,0x7f},                    //�
 {0x7f,0x02,0x0c,0x02,0x7f},                    //M
 {0x7f,0x08,0x08,0x08,0x7f},                    //H
 {0x3e,0x41,0x41,0x41,0x3e},                    //O
 {0x7f,0x01,0x01,0x01,0x7f},                    //�
 {0x7f,0x09,0x09,0x09,0x06},                    //P
 {0x3e,0x41,0x41,0x41,0x22},                    //C
 {0x01,0x01,0x7f,0x01,0x01},                    //T
 {0x47,0x28,0x10,0x08,0x07},                    //�
 {0x1c,0x22,0x7f,0x22,0x1c},                    //�
 {0x63,0x14,0x08,0x14,0x63},                    //X
 {0x7f,0x40,0x40,0x40,0xff},                    //�
 {0x07,0x08,0x08,0x08,0x7f},                    //�
 {0x7f,0x40,0x7f,0x40,0x7f},                    //�
 {0x7f,0x40,0x7f,0x40,0xff},                    //�
 {0x01,0x7f,0x48,0x48,0x30},                    //�
 {0x7f,0x48,0x30,0x00,0x7f},                    //�
 {0x00,0x7f,0x48,0x48,0x30},                    //�
 {0x22,0x41,0x49,0x49,0x3e},                    //�
 {0x7f,0x08,0x3e,0x41,0x3e},                    //�
 {0x46,0x29,0x19,0x09,0x7f},                    //�
 {0x20,0x54,0x54,0x54,0x78},                    //a
 {0x3c,0x4a,0x4a,0x49,0x31},                    //�
 {0x7c,0x54,0x54,0x28,0x00},                    //�
 {0x7c,0x04,0x04,0x04,0x0c},                    //�
 {0xe0,0x54,0x4c,0x44,0xfc},                    //�
 {0x38,0x54,0x54,0x54,0x18},                    //e
 {0x6c,0x10,0x7c,0x10,0x6c},                    //�
 {0x44,0x44,0x54,0x54,0x28},                    //�
 {0x7c,0x20,0x10,0x08,0x7c},                    //�
 {0x7c,0x41,0x22,0x11,0x7c},                    //�
 {0x7c,0x10,0x28,0x44,0x00},                    //�
 {0x20,0x44,0x3c,0x04,0x7c},                    //�
 {0x7c,0x08,0x10,0x08,0x7c},                    //�
 {0x7c,0x10,0x10,0x10,0x7c},                    //�
 {0x38,0x44,0x44,0x44,0x38},                    //o
 {0x7c,0x04,0x04,0x04,0x7c},                    //�
 {0x7C,0x14,0x14,0x14,0x08},                    //p
 {0x38,0x44,0x44,0x44,0x20},                    //c
 {0x04,0x04,0x7c,0x04,0x04},                    //�
 {0x0C,0x50,0x50,0x50,0x3C},                    //�
 {0x30,0x48,0xfc,0x48,0x30},                    //�
 {0x44,0x28,0x10,0x28,0x44},                    //x
 {0x7c,0x40,0x40,0x40,0xfc},                    //�
 {0x0c,0x10,0x10,0x10,0x7c},                    //�
 {0x7c,0x40,0x7c,0x40,0x7c},                    //�
 {0x7c,0x40,0x7c,0x40,0xfc},                    //�
 {0x04,0x7c,0x50,0x50,0x20},                    //�
 {0x7c,0x50,0x50,0x20,0x7c},                    //�
 {0x7c,0x50,0x50,0x20,0x00},                    //�
 {0x28,0x44,0x54,0x54,0x38},                    //�
 {0x7c,0x10,0x38,0x44,0x38},                    //�
 {0x08,0x54,0x34,0x14,0x7c},                    //�
};

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//����������������
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#define OE_DDR        DDRB
#define OE_PORT        PORTB
#define OE            0

#define SELECT_DDR    DDRB
#define SELECT_PORT    PORTB
#define SELECT        1

#define WE_DDR        DDRB
#define WE_PORT        PORTB
#define WE            2

#define BLANK_DDR     DDRB
#define BLANK_PIN     PINB
#define BLANK         3

#define ROW_DDR     DDRA
#define ROW_PORT     PORTA

#define COL_DDR     DDRC
#define COL_PORT     PORTC

#define DATA_DDR     DDRD
#define DATA_PORT     PORTD


//----------------------------------------------------------------------------------------------------
//��������� �������
//----------------------------------------------------------------------------------------------------
void InitAVR(void);//������������� �����������
void SetColor(unsigned char y,unsigned char x,unsigned char color);//���������� ������ � �����������
void PrintProgmem(unsigned char y,unsigned char x,unsigned char color,unsigned char bk_color,const char *string);//����� ������ �� ����������� ������ � �������
//----------------------------------------------------------------------------------------------------
//�������������� ����������
//----------------------------------------------------------------------------------------------------

#include "sd.h"
#include "fat.h"

//----------------------------------------------------------------------------------------------------
//�������� ������� ���������
//----------------------------------------------------------------------------------------------------
int main(void)
{
 int x;
 int y;
 InitAVR();
 //����������� ���������� �� ����� �����
 SELECT_PORT&=0xff^(1<<SELECT);
 OE_PORT&=0xff^(1<<OE);
 WE_PORT|=1<<WE;
 //������� �����
 for(x=0;x<255;x++)
 {
  for(y=0;y<255;y++) SetColor(y,x,0);
 }
 _delay_ms(5000);//��� ������ �� ����� SD-�����
 int print_y=0;
 enum SD_RETURN_CODE sd_c=SD_Init();

 if (sd_c!=SD_OK)
 {
  if (sd_c==SD_SPI_ERROR)
  {
   PrintProgmem(print_y,0,255,0,Text_SD_SPI_error);
   print_y+=8;
  }
  if (sd_c==SD_RESPONSE_ERROR)
  {
   PrintProgmem(print_y,0,255,0,Text_SD_No_response);
   print_y+=8;
  }
  if (sd_c==SD_SIZE_ERROR)
  {
   PrintProgmem(print_y,0,255,0,Text_SD_Size_error);
   print_y+=8;
  }
  while(1);
 }
 PrintProgmem(print_y,0,255,0,Text_SD_Ok);
 print_y+=8;
 //�������������� FAT
 enum FAT_RETURN_CODE fat_c=FAT_Init();
 if (fat_c!=FAT_16_OK)
 {
  if (fat_c==FAT_12_ERROR)
  {
   PrintProgmem(print_y,0,255,0,Text_FAT_FAT_12_error);
   print_y+=8;
  }
  if (fat_c==FAT_32_ERROR)
  {
   PrintProgmem(print_y,0,255,0,Text_FAT_FAT_32_error);
   print_y+=8;
  }
  while(1);
 }
 PrintProgmem(print_y,0,255,0,Text_FAT_FAT_16_ok);
 print_y+=8;

 while(1)
 {
  //���� ������ ���������� ���� � ���������� ���
  if (FAT_BeginFileSearch()==true)
  {
   //PrintProgmem(print_y,0,255,0,Text_FAT_Found_file);
   //print_y+=8;
  }
  else
  {
   PrintProgmem(print_y,0,255,0,Text_FAT_No_file);
   print_y+=8;
   while(1);
  }
  while(1)
  {
   FAT_OutputFile();
   _delay_ms(1000);
   _delay_ms(1000);
   _delay_ms(1000);
   _delay_ms(1000);
   _delay_ms(1000);
   if (FAT_NextFileSearch()==false) break;
  }
 }
/*
*/
 while(1);
 return(0);
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//����� �������
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


//----------------------------------------------------------------------------------------------------
//������������� �����������
//----------------------------------------------------------------------------------------------------
void InitAVR(void)
{
 //����������� �����
 DDRA=0;
 DDRB=0;
 DDRD=0;
 DDRC=0;

 OE_DDR|=(1<<OE);
 WE_DDR|=(1<<WE);
 SELECT_DDR|=(1<<SELECT);
 BLANK_DDR&=0xff^(1<<BLANK);

 ROW_DDR=0xff;
 COL_DDR=0xff;
 DATA_DDR=0;

 //����� ��������� ������
 PORTA=0;
 PORTB=0xff;
 PORTD=0;
 PORTC=0;
}
//----------------------------------------------------------------------------------------------------
//���������� ������ � �����������
//----------------------------------------------------------------------------------------------------
void SetColor(unsigned char y,unsigned char x,unsigned char color)
{
 color=0xff^color;//����������� ����� (��� ���������� ��-�� ������ �� ����� ���)
 while(1)
 {
  if (BLANK_PIN&(1<<BLANK)) break;
 }
 OE_PORT|=1<<OE;//��������� ����� ������
 asm volatile ("nop"::);
 asm volatile ("nop"::);
 SELECT_PORT|=1<<SELECT;//����������� ���������� �� ����
 //������������� �����
 DATA_DDR=0xff;
 ROW_PORT=y;
 COL_PORT=x;
 DATA_PORT=color;
 //asm volatile ("nop"::);
 //asm volatile ("nop"::);
 WE_PORT&=0xff^(1<<WE);//��� ������ ������
 //asm volatile ("nop"::);
 //asm volatile ("nop"::);
 //asm volatile ("nop"::);
 //asm volatile ("nop"::);
 WE_PORT|=1<<WE;//������� ������ ������
 //asm volatile ("nop"::);
 //asm volatile ("nop"::);
 DATA_DDR=0;
 DATA_PORT=0;
 SELECT_PORT&=0xff^(1<<SELECT);//����������� ���������� �� ����������
 asm volatile ("nop"::);
 asm volatile ("nop"::);
 OE_PORT&=0xff^(1<<OE);//��������� ����� ������
}


//----------------------------------------------------------------------------------------------------
//����� ������ �� ����������� ������ � �������
//----------------------------------------------------------------------------------------------------
void PrintProgmem(unsigned char y,unsigned char x,unsigned char color,unsigned char bk_color,const char *string)
{
 unsigned char index=0;
 while(1)
 {
  if (y>=240-8) break;
  if (x>=250-6) break;
  unsigned char s=pgm_read_byte(&string[index]);
  if (s<0x20) break;
  if (s<0x90) s-=0x20;
         else s-=0x60;
  for(unsigned char tx=0;tx<5;tx++)
  {
   unsigned char byte=pgm_read_byte(&symboltable[s][tx]);
   unsigned char mask=1;
   for(unsigned char ty=0;ty<8;ty++,mask<<=1)
   {
    if (byte&mask) SetColor(y+ty,x+tx,color);
              else SetColor(y+ty,x+tx,bk_color);
   }
  }
  x+=6;
  index++;
 }
}


