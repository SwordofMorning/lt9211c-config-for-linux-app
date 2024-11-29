#include "include.h"

#ifndef _DRVMIPIRX_H
#define _DRVMIPIRX_H

// 移除 StructPcrPara 的定义，因为它已经在 type.h 中定义

// MIPI RX视频时序获取结构体
typedef struct {
    u16 uswc;
    u16 usHact;
    u16 usVact;
    u8 ucFmt;
    u8 ucPa_Lpn;
    u8 ucFrameRate;
    u8 ucLane0SetNum;
    u8 ucLane1SetNum;
    u8 ucLane2SetNum;
    u8 ucLane3SetNum;
    u8 ucLane0SotData;
    u8 ucLane1SotData;
    u8 ucLane2SotData;
    u8 ucLane3SotData;
} SrtuctMipiRx_VidTiming_Get;

// 视频时序结构体定义
typedef struct _VideoTimingList
{
    u16 usHfp;
    u16 usHs; 
    u16 usHbp;
    u16 usHact;
    u16 usHtotal;

    u16 usVfp;
    u16 usVs;
    u16 usVbp;
    u16 usVact;
    u16 usVtotal;

    u8 ucFrameRate;
} VideoTimingList;

// MIPI RX通道数枚举
typedef enum
{
    MIPIRX_4LANE = 0x00,
    MIPIRX_1LANE = 0x01, 
    MIPIRX_2LANE = 0x02,
    MIPIRX_3LANE = 0x03,
} Enum_MIPIRXPORTLANE_NUM;

// 全局变量声明
extern StructPcrPara g_stPcrPara;                           // 使用 type.h 中定义的 StructPcrPara
extern SrtuctMipiRx_VidTiming_Get g_stMipiRxVidTiming_Get;
extern StructChipRxVidTiming g_stRxVidTiming;

// 函数声明
extern void Drv_MipiRx_PhyPowerOn(void);
extern void Drv_MipiRxClk_Sel(void);
extern void Drv_MipiRx_InputSel(void);
extern void Drv_MipiRx_LaneSet(void);
extern void Drv_MipiRx_HsSettleSet(void);
extern void Drv_MipiRx_DesscPll_Set(void);
extern void Drv_MipiRx_VidTiming_Set(void);
extern u8 Drv_MipiRx_VidFmtUpdate(void);
extern u8 Drv_MipiRx_VidFmt_Get(u8 VidFmt);
extern u8 Drv_MipiRx_VidTiming_Get(void);
extern u8 Drv_MipiRx_VidTiming_Sel(void);
extern u8 Drv_MipiRx_PcrCali(void);
extern void DRV_DesscPll_SdmCal(void);

#endif