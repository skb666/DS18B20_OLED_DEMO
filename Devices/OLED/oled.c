#include "oled.h"

//IIC Start
void IIC_Start()
{
    OLED_SCLK_Set();
    OLED_SDIN_Set();
    OLED_SDIN_Clr();
    OLED_SCLK_Clr();
}

//IIC Stop
void IIC_Stop()
{
    OLED_SCLK_Set();
    OLED_SDIN_Clr();
    OLED_SDIN_Set();
}

void IIC_Wait_Ack()
{
    OLED_SCLK_Set();
    OLED_SCLK_Clr();
}

//IIC Write byte
void Write_IIC_Byte(uint8_t IIC_Byte)
{
    uint8_t i;
    uint8_t m, da;
    da = IIC_Byte;
    OLED_SCLK_Clr();

    for(i = 0; i < 8; i++)
    {
        m = da;
        //OLED_SCLK_Clr();
        m &= 0x80;

        if(m == 0x80)
        {
            OLED_SDIN_Set();
        }
        else
        {
            OLED_SDIN_Clr();
        }

        da <<= 1;
        OLED_SCLK_Set();
        OLED_SCLK_Clr();
    }
}

//IIC Write Command
void Write_IIC_Command(uint8_t IIC_Command)
{
    IIC_Start();
    Write_IIC_Byte(0x78);   //Slave address,SA0=0
    IIC_Wait_Ack();
    Write_IIC_Byte(0x00);   //write command
    IIC_Wait_Ack();
    Write_IIC_Byte(IIC_Command);
    IIC_Wait_Ack();
    IIC_Stop();
}

//IIC Write Data
void Write_IIC_Data(uint8_t IIC_Data)
{
    IIC_Start();
    Write_IIC_Byte(0x78);   //D/C#=0; R/W#=0
    IIC_Wait_Ack();
    Write_IIC_Byte(0x40);   //write data
    IIC_Wait_Ack();
    Write_IIC_Byte(IIC_Data);
    IIC_Wait_Ack();
    IIC_Stop();
}

void OLED_WR_Byte(uint8_t dat, OLED_MODE cmd)
{
    switch(cmd)
    {
        case OLED_CMD:
        {
            Write_IIC_Command(dat);
        }
        break;

        case OLED_DATA:
        {
            Write_IIC_Data(dat);
        }
        break;
    }
}

//��������
void OLED_Set_Pos(uint8_t x, uint8_t y)
{
    OLED_WR_Byte(0xb0 + y, OLED_CMD);
    OLED_WR_Byte(((x & 0xf0) >> 4) | 0x10, OLED_CMD);
    OLED_WR_Byte((x & 0x0f), OLED_CMD);
}

//��������,������,������Ļ�Ǻ�ɫ��!��û����һ��!!!
void OLED_Clear(void)
{
    uint8_t i, n;

    for(i = 0; i < 8; i++)
    {
        OLED_WR_Byte(0xb0 + i, OLED_CMD);  //����ҳ��ַ��0~7��
        OLED_WR_Byte(0x00, OLED_CMD);      //������ʾλ�á��е͵�ַ
        OLED_WR_Byte(0x10, OLED_CMD);      //������ʾλ�á��иߵ�ַ

        for(n = 0; n < 128; n++)
        {
            OLED_WR_Byte(0, OLED_DATA);
        }
    } //������ʾ
}

//��ָ��λ����ʾһ���ַ�,���������ַ�
//x:0~127 | y:0~63 | _size:ѡ������ 16/12
void OLED_ShowChar(uint8_t x, uint8_t y, uint8_t chr, CHAR_SIZE _size)
{
    uint8_t c = 0,
            i = 0;
    c = chr - ' ';  //�õ�ƫ�ƺ��ֵ

    if(_size == S6x8)
    {
        if(x > Max_Column - 6)
        {
            x = 0;
            y = (y + 1) % (Max_Row / 8);
        }

        OLED_Set_Pos(x, y);

        for(i = 0; i < 6; i++)
        {
            OLED_WR_Byte(F6x8[c][i], OLED_DATA);
        }
    }
    else
    {
        if(x > Max_Column - 8)
        {
            x = 0;
            y += 2;

            if(y > (Max_Row / 8 - 2))
            {
                y = 0;
            }
        }

        OLED_Set_Pos(x, y);

        for(i = 0; i < 8; i++)
        {
            OLED_WR_Byte(F8x16[c][i], OLED_DATA);
        }

        OLED_Set_Pos(x, y + 1);

        for(i = 0; i < 8; i++)
        {
            OLED_WR_Byte(F8x16[c][i + 8], OLED_DATA);
        }
    }
}

//��ʾһ���ַ��Ŵ�
void OLED_ShowString(uint8_t x, uint8_t y, uint8_t *chr, CHAR_SIZE _size)
{
    uint8_t j = 0;

    while(chr[j] != '\0')
    {
        OLED_ShowChar(x, y, chr[j], _size);

        if(_size == S6x8)
        {
            x += 6;

            if(x > Max_Column - 6)
            {
                x = 0;
                y += 1;
            }
        }
        else
        {
            x += 8;

            if(x > Max_Column - 8)
            {
                x = 0;
                y += 2;
            }
        }

        j++;
    }
}

//��ʾ����
void OLED_ShowChinese(uint8_t x, uint8_t y, uint8_t no)
{
    uint8_t t;
    OLED_Set_Pos(x, y);

    for(t = 0; t < 16; t++)
    {
        OLED_WR_Byte(Hzk[no][t], OLED_DATA);
    }

    OLED_Set_Pos(x, y + 1);

    for(t = 0; t < 16; t++)
    {
        OLED_WR_Byte(Hzk[no][t + 16], OLED_DATA);
    }
}

//��ʼ��SSD1306
void OLED_Init(void)
{
    OLED_WR_Byte(0xAE, OLED_CMD); //--display off
    OLED_WR_Byte(0x00, OLED_CMD); //---set low column address
    OLED_WR_Byte(0x10, OLED_CMD); //---set high column address
    OLED_WR_Byte(0x40, OLED_CMD); //--set start line address
    OLED_WR_Byte(0xB0, OLED_CMD); //--set page address
    OLED_WR_Byte(0x81, OLED_CMD); // contract control
    OLED_WR_Byte(0xFF, OLED_CMD); //--128
    OLED_WR_Byte(0xA1, OLED_CMD); //set segment remap
    OLED_WR_Byte(0xA6, OLED_CMD); //--normal / reverse
    OLED_WR_Byte(0xA8, OLED_CMD); //--set multiplex ratio(1 to 64)
    OLED_WR_Byte(0x3F, OLED_CMD); //--1/32 duty
    OLED_WR_Byte(0xC8, OLED_CMD); //Com scan direction
    OLED_WR_Byte(0xD3, OLED_CMD); //-set display offset
    OLED_WR_Byte(0x00, OLED_CMD); //

    OLED_WR_Byte(0xD5, OLED_CMD); //set osc division
    OLED_WR_Byte(0x80, OLED_CMD); //

    OLED_WR_Byte(0xD8, OLED_CMD); //set area color mode off
    OLED_WR_Byte(0x05, OLED_CMD); //

    OLED_WR_Byte(0xD9, OLED_CMD); //Set Pre-Charge Period
    OLED_WR_Byte(0xF1, OLED_CMD); //

    OLED_WR_Byte(0xDA, OLED_CMD); //set com pin configuartion
    OLED_WR_Byte(0x12, OLED_CMD); //

    OLED_WR_Byte(0xDB, OLED_CMD); //set Vcomh
    OLED_WR_Byte(0x30, OLED_CMD); //

    OLED_WR_Byte(0x8D, OLED_CMD); //set charge pump enable
    OLED_WR_Byte(0x14, OLED_CMD); //

    OLED_WR_Byte(0xAF, OLED_CMD); //--turn on oled panel
}
