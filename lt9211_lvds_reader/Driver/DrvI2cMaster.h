#include    "include.h"

#ifndef     _DRVI2CMASTER_H
#define     _DRVI2CMASTER_H


extern u8 HDMI_ReadI2C_Byte(u8 RegAddr);
extern bool HDMI_ReadI2C_ByteN(u8 RegAddr,u8 *p_data,u8 N);
extern bool HDMI_WriteI2C_Byte(u8 RegAddr, u8 d);
extern bool HDMI_WriteI2C_ByteN(u8 RegAddr, u8 *d,u8 N);
extern void I2cMasterChoice(u8 i2cchannnel);


#endif
