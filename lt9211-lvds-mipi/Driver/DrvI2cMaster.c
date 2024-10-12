/******************************************************************************
  * @project: LT9211C
  * @file: DrvI2cMaster.c
  * @author: sxue
  * @company: LONTIUM COPYRIGHT and CONFIDENTIAL
  * @date: 2023.01.29
/******************************************************************************/

#include "include.h"
#include "i2c-rw.h"

#if 0
#define  SCL_Set_High     P13=1
#define  SCL_Set_Low      P13=0

#define  SDA_Set_High     P14=1
#define  SDA_Set_Low      P14=0

#define I2C_SDA_READ      P14
#endif

u8  I2CADR =  LT9211_I2CADR;

unsigned char I2C_Read_Byte(unsigned char RegAddr);
void I2C_Write_Byte(unsigned char RegAddr,unsigned char  data);
u8 HDMI_ReadI2C_Byte(u8 RegAddr)
{
    return I2C_Read_Byte(RegAddr);
}

bool HDMI_ReadI2C_ByteN(u8 RegAddr,u8 *p_data,u8 N)
{
#if 0
    bool flag;
    flag = i2c_read_byte(ucI2cAddr, RegAddr, p_data,N);
    return flag;
#endif
    return 0;
}

bool HDMI_WriteI2C_Byte(u8 RegAddr, u8 d)
{
     I2C_Write_Byte(RegAddr, d);
     return 0;
}

bool HDMI_WriteI2C_ByteN(u8 RegAddr, u8 *d,u8 N)
{
#if 0
    bool flag;
    flag=i2c_write_byte(ucI2cAddr, RegAddr,d,N) ;
    return flag;
#endif
    return 0;
}

#if 0
int main(int argc, char *argv[])
{
    char filename[20];
    int i2cbus,  file, i;

    i2cbus = 11;

    printf("i2c-bus is %d\n", i2cbus);

    file = open_i2c_dev(i2cbus, filename, sizeof(filename), 0);
    if (file < 0 || i2c_check_funcs(file))
        exit(1);
    g_i2c = file;

    unsigned char debug_buf[8] = {0x11, 0x22,0x33,0x44,0x55, 0x66,0x77,0x88};

    // test 1
    i2csend_addr8_value0(file, 0x70, 0x02);

    // test 2
    i2csend_addr8_value8(file, 0x20, 0x00, 0xab);

    // test 3 ； 16位寄存器地址，8位值
    i2csend_addr16_value8(file, 0x29, 0x2d, 0xff);
    i2crecv_addr16_value8(file, 0x29, 0x2d);


    // test 4 连续读
    i2crecv_addr8_valuex(file, 0x20, 0x00, debug_buf, 8);
    for (i = 0; i < 8; i++)
        printf("read--[%d]/[%x]\n",i,debug_buf[i]);

    close(file);

    exit(0);

}
#endif
