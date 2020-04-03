//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//��������� SD-�����
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#define SD_CS_DDR   DDRB
#define SD_CS_PORT  PORTB
#define SD_CS       4

#define SD_DI_DDR   DDRB
#define SD_DI_PORT  PORTB
#define SD_DI       5

#define SD_DO_DDR   DDRB
#define SD_DO_PORT  PORTB
#define SD_DO       6

#define SD_SCK_DDR  DDRB
#define SD_SCK_PORT PORTB
#define SD_SCK      7

enum SD_RETURN_CODE
{
 SD_OK,
 SD_SPI_ERROR,
 SD_RESPONSE_ERROR,
 SD_SIZE_ERROR,
};

//----------------------------------------------------------------------------------------------------
//���������� ����������
//----------------------------------------------------------------------------------------------------
unsigned short BlockByteCounter=512;//��������� ���� �����
//----------------------------------------------------------------------------------------------------
//��������� �������
//----------------------------------------------------------------------------------------------------
enum SD_RETURN_CODE SD_Init(void);//������������� ����� ������
unsigned char SD_TransmitData(unsigned char data);//������� ������ SD-����� � ������� �����
unsigned char SD_SendCommandR1(unsigned char b0,unsigned char b1,unsigned char b2,unsigned char b3,unsigned char b4);//������� ������� � ������� R1 SD-�����
unsigned long SD_GetSize(void);//�������� ����� SD-����� � ������
unsigned short GetBits(unsigned char *data,unsigned char begin,unsigned char end);//�������� ���� � begin �� end ������������
bool SD_BeginReadBlock(unsigned long BlockAddr);//������ ������ �����
bool SD_ReadBlockByte(unsigned char *byte);//������� ���� �����
bool SD_ReadBlock(unsigned long BlockAddr,unsigned char *Addr);//������� ���� � 512 ���� � ������
bool SD_WriteBlock(unsigned long BlockAddr,unsigned char *Addr);//�������� ���� � 512 ���� �� SD-�����
//----------------------------------------------------------------------------------------------------
//������������� ����� ������
//----------------------------------------------------------------------------------------------------
enum SD_RETURN_CODE SD_Init(void)
{
 SD_CS_DDR|=(1<<SD_CS);
 SD_DI_DDR|=(1<<SD_DI);
 SD_SCK_DDR|=(1<<SD_SCK);
 SD_DO_DDR&=0xff^(1<<SD_DO);
 //����� SPI SS � ������ MASTER ��������������� ��� ����� � �� SPI �� ������
 _delay_ms(100);//�����, ���� ����� �� ���������
 unsigned char n;
 //��� �� ����� 74 ��������� ������������� ��� ������� ������ �� CS � DI
 SD_CS_PORT|=(1<<SD_CS);
 _delay_ms(500);
 SD_DI_PORT|=(1<<SD_DI);
 for(n=0;n<250;n++)
 {
  SD_SCK_PORT|=(1<<SD_SCK);
  _delay_ms(1);
  SD_SCK_PORT&=0xff^(1<<SD_SCK);
  _delay_ms(1);
 }
 SD_CS_PORT&=0xff^(1<<SD_CS);
 //����������� SPI
 SPCR=(0<<SPIE)|(1<<SPE)|(0<<DORD)|(1<<MSTR)|(0<<CPOL)|(0<<CPHA)|(0<<SPR1)|(0<<SPR0);
 SPSR=(1<<SPI2X);//��������� �������� SPI
 _delay_ms(100);
 unsigned char answer=SD_SendCommandR1(0x40,0x00,0x00,0x00,0x00);//CMD0
 if (answer!=1) return(SD_SPI_ERROR);//������
 SD_TransmitData(0xff);
 unsigned short m;
 for(m=0;m<65535;m++)
 {
  answer=SD_SendCommandR1(0x41,0x00,0x00,0x00,0x00);//CMD1
  SD_TransmitData(0xff);
  if (answer==0) break;//������������� �������
  _delay_us(1);
 }
 if (m==65535) return(SD_RESPONSE_ERROR);
 //������ ����� ����� ������
 unsigned long SD_Size=SD_GetSize();
 if (SD_Size==0xffff) return(SD_SIZE_ERROR);//������
 return(SD_OK);
}

//----------------------------------------------------------------------------------------------------
//������� ������ SD-����� � ������� �����
//----------------------------------------------------------------------------------------------------
inline unsigned char SD_TransmitData(unsigned char data)
{
 SPDR=data;//�������
 while(!(SPSR&(1<<SPIF)))
 {
  //��� ���������� �������� � ��������� ������
 }
 unsigned char res=SPDR;
 return(res);
}

//----------------------------------------------------------------------------------------------------
//������� ������� � ������� R1
//----------------------------------------------------------------------------------------------------
unsigned char SD_SendCommandR1(unsigned char b0,unsigned char b1,unsigned char b2,unsigned char b3,unsigned char b4)
{
 //���������� ������� � ������� � CRC7
 unsigned char crc7=0;
 unsigned char cmd[5]={b0,b1,b2,b3,b4};
 for(unsigned char n=0;n<5;n++)
 {
  SD_TransmitData(cmd[n]);
  /*
  unsigned char b=cmd[n];
  for (unsigned char i=0;i<8;i++)
  {
   crc7<<=1;
   if ((b&0x80)^(crc7&0x80)) crc7^=0x09;
   b<<=1;
  }*/
 }
 /*
 crc7=crc7<<1;
 crc7|=1;
 */
 crc7=0x95;
 SD_TransmitData(crc7);//CRC
 //����� ����� �������� �� �����
 //��������� ����� R1 (������� ��� ������ 0)
 for(unsigned short n=0;n<65535;n++)
 {
  unsigned char res=SD_TransmitData(0xff);
  if ((res&128)==0)
  {
   return(res);//��� ������������� �����
  }
 }
 return(0xff);//����� �� ������
}
//----------------------------------------------------------------------------------------------------
//�������� ����� SD-����� � ������
//----------------------------------------------------------------------------------------------------
unsigned long SD_GetSize(void)
{
 unsigned short n;
 if (SD_SendCommandR1(0x49,0x00,0x00,0x00,0x00)==0xff) return(0xffff);//����� �� ������
 //��������� 16 ���� ������
 for(n=0;n<65535;n++)
 {
  if (SD_TransmitData(0xff)==0xfe) break;//�������� ������ ������
  _delay_us(1);
 }
 if (n==65535) return(0xffff);//����� �� ������
 unsigned char b[16];
 for(n=0;n<16;n++) b[n]=SD_TransmitData(0xff);
 //������ �����
 for(n=0;n<255;n++) SD_TransmitData(0xff);
 //������� ������ ����� ������
 unsigned long c_size=GetBits(b,73,62);
 unsigned long c_size_mult=GetBits(b,49,47);
 unsigned long blocks=(c_size+1UL)*(1UL<<(c_size_mult+2UL));
 unsigned long read_bl_len=GetBits(b,83,80);
 blocks*=(1UL<<read_bl_len);
 return(blocks);
}
//----------------------------------------------------------------------------------------------------
//�������� ���� � begin �� end ������������
//----------------------------------------------------------------------------------------------------
unsigned short GetBits(unsigned char *data,unsigned char begin,unsigned char end)
{
 unsigned short bits=0;
 unsigned char size=1+begin-end;
 for(unsigned char i=0;i<size;i++)
 {
  unsigned char position=end+i;
  unsigned short byte=15-(position>>3);
  unsigned short bit=position&0x7;
  unsigned short value=(data[byte]>>bit)&1;
  bits|=value<<i;
 }
 return(bits);
}
//----------------------------------------------------------------------------------------------------
//������ ������ �����
//----------------------------------------------------------------------------------------------------
bool SD_BeginReadBlock(unsigned long BlockAddr)
{
 //��� ������� ������ �����
 unsigned char a1=(unsigned char)((BlockAddr>>24)&0xff);
 unsigned char a2=(unsigned char)((BlockAddr>>16)&0xff);
 unsigned char a3=(unsigned char)((BlockAddr>>8)&0xff);
 unsigned char a4=(unsigned char)(BlockAddr&0xff);
 unsigned char res=SD_SendCommandR1(0x51,a1,a2,a3,a4);//�������� CMD17
 if (res!=0) return(false);//������ �������
 SD_TransmitData(0xff);//�������� ����������
 //��� ������ ����������� ������
 unsigned short n;
 for(n=0;n<65535;n++)
 {
  res=SD_TransmitData(0xff);
  if (res==0xfe) break;//������ �������
 }
 if (n==65535) return(false);//������ ������ ������ �� �������
 BlockByteCounter=0;
 return(true);
}
//----------------------------------------------------------------------------------------------------
//������� ���� �����
//----------------------------------------------------------------------------------------------------
bool SD_ReadBlockByte(unsigned char *byte)
{
 if (BlockByteCounter>=512) return(false);
 *byte=SD_TransmitData(0xff);//������ ���� � SD-�����
 BlockByteCounter++;
 if (BlockByteCounter==512)
 {
  //��������� CRC
  SD_TransmitData(0xff);
  SD_TransmitData(0xff);
 }
 return(true);
}
//----------------------------------------------------------------------------------------------------
//������� ���� � 512 ���� � ������
//----------------------------------------------------------------------------------------------------
bool SD_ReadBlock(unsigned long BlockAddr,unsigned char *Addr)
{
 //��� ������� ������ �����
 unsigned char a1=(unsigned char)((BlockAddr>>24)&0xff);
 unsigned char a2=(unsigned char)((BlockAddr>>16)&0xff);
 unsigned char a3=(unsigned char)((BlockAddr>>8)&0xff);
 unsigned char a4=(unsigned char)(BlockAddr&0xff);
 unsigned char res=SD_SendCommandR1(0x51,a1,a2,a3,a4);//�������� CMD17
 if (res!=0) return(false);//������ �������
 SD_TransmitData(0xff);//�������� ����������
 //��� ������ ����������� ������
 unsigned short n;
 for(n=0;n<65535;n++)
 {
  res=SD_TransmitData(0xff);
  if (res==0xfe) break;//������ �������
 }
 if (n==65535) return(false);//������ ������ ������ �� �������
 for(n=0;n<512;n++,Addr++) *Addr=SD_TransmitData(0xff);//������ ���� � SD-�����
 //��������� CRC
 SD_TransmitData(0xff);
 SD_TransmitData(0xff);
 return(true);
}
//----------------------------------------------------------------------------------------------------
//�������� ���� � 512 ���� �� SD-�����
//----------------------------------------------------------------------------------------------------
bool SD_WriteBlock(unsigned long BlockAddr,unsigned char *Addr)
{
 unsigned short n;
 //��� ������� ������ �����
 unsigned char a1=(unsigned char)((BlockAddr>>24)&0xff);
 unsigned char a2=(unsigned char)((BlockAddr>>16)&0xff);
 unsigned char a3=(unsigned char)((BlockAddr>>8)&0xff);
 unsigned char a4=(unsigned char)(BlockAddr&0xff);
 unsigned char res=SD_SendCommandR1(0x58,a1,a2,a3,a4);//�������� CMD24
 if (res!=0) return(false);//������ �������
 SD_TransmitData(0xff);//�������� ����������
 SD_TransmitData(0xfe);//������ ������ ������ �����
 unsigned short CRC16=0;
 for(unsigned short n=0;n<512;n++,Addr++)
 {
  unsigned char byte=*Addr;//��������� ���� �� ������
  SD_TransmitData(byte);
  /*
  CRC16^=(byte<<8);
  for (unsigned char i=0;i<8;i++)
  {
   if (CRC16&0x8000) CRC16=(CRC16<<1)^0x1021;
                else CRC16<<=1;
  }
  */
 }
 //������� CRC
 SD_TransmitData((unsigned char)((CRC16>>8)&0xff));//CRC
 SD_TransmitData((unsigned char)(CRC16&0xff));//CRC
 res=SD_TransmitData(0xff);//�������� ����� �����
 if ((res&0x05)!=0x05) return(false);//���� ����� �� "������ �������"
 SD_TransmitData(0xff);//�������� ����������
 SD_TransmitData(0xff);//�������� ����������
 //��� ���������� ������ (����� DO ��������� � 0, ���� ����� ������
 for(n=0;n<65535;n++)
 {
  if (SD_TransmitData(0xff)==0xff) return(true);//����� DO ����� � 1; ����� ��������� ������
  _delay_us(1);
 }
 return(false);//�� ��������� ������������� �����
}


