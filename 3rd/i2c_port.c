#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
//#include "i2cbusses.h"
//#include "util.h"
//#include "../version.h"
#include "i2c-rw.h"

static int g_i2c;

#define u8  unsigned char
#define u16  unsigned short
extern u8 I2CADR;// IIC address // bit0 是读写标志位；
#define bool unsigned char


// 是否打印调试信息
#define DEBUG 0
#define DEBUG_ERROR 0

#if DEBUG == 0
#define i2c_debug(format,...)
#else
#define i2c_debug printf
#endif

#if DEBUG_ERROR == 0
#define i2c_error(format,...)
#else
#define i2c_error(format,...) printf
#endif

void I2C_Write_Byte(unsigned char RegAddr,unsigned char  data)
{
    int addr;
    addr = I2CADR >> 1;
    i2csend_addr8_value8(g_i2c, addr, RegAddr, data);
    i2c_debug("Write : %d : Address [%02x], Reg [%02x], Val [%02x]\n", ret, addr, RegAddr, data);
}

unsigned char I2C_Read_Byte(unsigned char RegAddr)
{
    int ret;
    int addr;
    addr = I2CADR >> 1;
    ret = i2crecv_addr8_value8(g_i2c, addr, RegAddr);

    //i2c_debug("READ : Address [%x], Reg [%x], Val [%x]\n", addr, RegAddr, ret);
    return ret;
}

bool I2C_Write_ByteN( u8 RegAddr, u8 *data, u16 Num )   // 往寄存器写Num 个 byte的数值
{
#if 0
    static u8 buff[512];
    int i;
    int addr;
    int ret;
    addr = I2CADR >> 2;
    for(i = 0; i < Num; i++)
    {
        ret = i2csend_addr8_value8(g_i2c, addr, RegAddr, data[i]);
    }
#else
    return 0;
#endif
}


int init_i2c(int i2cbus)
{
    char filename[20];
    int  file;


    i2c_debug("i2c-bus is %d\n", i2cbus);

    file = open_i2c_dev(i2cbus, filename, sizeof(filename), 0);
    if (file < 0 || i2c_check_funcs(file))
        exit(1);
    g_i2c = file;

#if 0
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
#endif
    return 0;

}
