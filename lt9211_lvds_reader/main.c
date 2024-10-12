#include <stdio.h>
#include <stdlib.h>

#include "config.h"
#include "include.h"

extern u8 I2CADR;// IIC address // bit0 是读写标志位；
int init_i2c(int i2cbus);

// sleep in millisecond(ms, 1 s = 1000 ms).
int msleep(unsigned int ms);

#if 0
void probe_reset(int i2c_bus, int dev_addr)
{
    // 1
    if(i2c_bus == 7 && dev_addr == 0x2d)
    {
        system("i2cset -y -f 2 0x20 0x06 0x01;");
        system("echo 1 >> /tmp/.9211.run.log");
    }
    // 2
    if(i2c_bus == 8 && dev_addr == 0x2d)
    {
        system("i2cset -y -f 2 0x20 0x07 0x01;");
        system("echo 2 >> /tmp/.9211.run.log");
    }
    // 3
    if(i2c_bus == 3 && dev_addr == 0x2d)
    {
        system("i2cset -y -f 2 0x20 0x08 0x01;");
        system("echo 3 >> /tmp/.9211.run.log");
    }
    // 4
    if(i2c_bus == 4 && dev_addr == 0x2d)
    {
        system("i2cset -y -f 2 0x20 0x09 0x01;");
        system("echo 4 >> /tmp/.9211.run.log");
    }
    msleep(800);
}
#endif

void Drv_LvdsRx_VidChkDebug_SC(void)
{
    u16 ushact, usvact;
    u16 ushs, usvs;
    u16 ushbp, usvbp;
    u16 ushtotal, usvtotal;
    u16 ushfp, usvfp;
    u8 ucsync_polarity;

    HDMI_WriteI2C_Byte(0xff,0xd8);
    #if LVDSRX_PORT_SEL == PORTB
    HDMI_WriteI2C_Byte(0xff,HDMI_ReadI2C_Byte(0x2a) | BIT6_1);
    #else
    HDMI_WriteI2C_Byte(0xff,HDMI_ReadI2C_Byte(0x2a) & BIT6_0);
    #endif

    Ocm_Timer0_Delay1ms(100);
    HDMI_WriteI2C_Byte(0xff,0x86);

    ucsync_polarity = HDMI_ReadI2C_Byte(0x4f);

    usvs = HDMI_ReadI2C_Byte(0x52); //[15:8]
    usvs = (usvs <<8 ) + HDMI_ReadI2C_Byte(0x53);

    ushs = HDMI_ReadI2C_Byte(0x50);
    ushs = (ushs << 8) + HDMI_ReadI2C_Byte(0x51);

    usvbp = HDMI_ReadI2C_Byte(0x57);
    usvfp = HDMI_ReadI2C_Byte(0x5b);

    ushbp = HDMI_ReadI2C_Byte(0x54);
    ushbp = (ushbp << 8) + HDMI_ReadI2C_Byte(0x55);

    ushfp = HDMI_ReadI2C_Byte(0x58);
    ushfp = (ushfp << 8) + HDMI_ReadI2C_Byte(0x59);

    usvtotal = HDMI_ReadI2C_Byte(0x62);
    usvtotal = (usvtotal << 8) + HDMI_ReadI2C_Byte(0x63);

    ushtotal = HDMI_ReadI2C_Byte(0x60);
    ushtotal = (ushtotal << 8) + HDMI_ReadI2C_Byte(0x61);

    usvact = HDMI_ReadI2C_Byte(0x5e);
    usvact = (usvact << 8)+ HDMI_ReadI2C_Byte(0x5f);

    ushact = HDMI_ReadI2C_Byte(0x5c);
    ushact = (ushact << 8) + HDMI_ReadI2C_Byte(0x5d);


#if 0
    HDMI_WriteI2C_Byte(0xff,0xd8);
    if(!(ucsync_polarity & 0x01)) //hsync
    {
        HDMI_WriteI2C_Byte(0x10,(HDMI_ReadI2C_Byte(0x10) | 0x10));
    }
    if(!(ucsync_polarity & 0x02)) //vsync
    {
        HDMI_WriteI2C_Byte(0x10, (HDMI_ReadI2C_Byte(0x10)| 0x20));
    }
#else
    //LTLog(LOG_INFO,"Skip H/V Sync");
#endif


    LTLog(LOG_INFO,"sync_polarity = 0x%02x", ucsync_polarity);
    LTLog(LOG_INFO,"hfp, hs, hbp, hact, htotal = %d %d %d %d %d",ushfp, ushs, ushbp, ushact, ushtotal);
    LTLog(LOG_INFO,"vfp, vs, vbp, vact, vtotal = %d %d %d %d %d",usvfp, usvs, usvbp, usvact, usvtotal);
}

int main(int argc, char * argv[])
{
    int i2c_bus = 8;
    int dev_addr = 0x2d;
    int ret;
    if(argc >= 2)
    {
        sscanf(argv[1], "%x", &i2c_bus);
    }
    if(argc >= 3)
    {
        sscanf(argv[2], "%x", &dev_addr);
        //dev_addr = atoi(argv[2]);
        I2CADR =  dev_addr << 1;
    }
    printf("Arg 1 for i2c-bus\n");
    printf("Arg 2 for dev-adr\n");
    printf("I2C-[%d] , config [0x%02x]\n\n", i2c_bus, dev_addr);
    init_i2c(i2c_bus);
    //probe_reset(i2c_bus, dev_addr);

    //Mod_System_Init();
    //Mod_LT9211C_Reset();
    LTLog(LOG_INFO, "LT9211C %s %s",__DATE__,__TIME__);
    Mod_ChipID_Read();
    Drv_LvdsRx_VidChkDebug_SC();
}
