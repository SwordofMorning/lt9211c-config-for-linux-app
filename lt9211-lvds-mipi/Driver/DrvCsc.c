

#include "include.h"
#if ((LT9211C_MODE_SEL != PATTERN_OUT)&&(LT9211C_MODE_SEL != MIPI_LEVEL_SHIFT)&&(LT9211C_MODE_SEL != MIPI_REPEATER))

void Drv_Csc_Set(void)
{
    #if ((CSC_RX_MODE == RGB)&&(CSC_TX_MODE == YUV422))
          
         
            HDMI_WriteI2C_Byte(0xff,0x86);
            HDMI_WriteI2C_Byte(0x85,0x10);
            HDMI_WriteI2C_Byte(0x86,0x40);
            HDMI_WriteI2C_Byte(0x87,0x00);
            LTLog(LOG_INFO,"Csc Set:    RGB to YUV422");
    #elif ((CSC_RX_MODE == YUV422)&&(CSC_TX_MODE == RGB))
        
            HDMI_WriteI2C_Byte(0xff,0x86);
            HDMI_WriteI2C_Byte(0x85,0x00);
            HDMI_WriteI2C_Byte(0x86,0x04);
            HDMI_WriteI2C_Byte(0x87,0x10);
            LTLog(LOG_INFO,"Csc Set:    YUV422 to RGB");
    #endif
}
#endif