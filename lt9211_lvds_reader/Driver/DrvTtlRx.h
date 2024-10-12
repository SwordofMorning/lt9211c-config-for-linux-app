


#include    "include.h"

#ifndef     _DRVTTLRX_H
#define     _DRVTTLRX_H

typedef struct VideoTimingList_ForTtl
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

    u8  ucFrameRate;

}VideoTimingList_ForTtl;

void Drv_TtlRx_PhyPowerOn();
void Drv_TtlRxHalfBit_Set();
void Drv_TtlRxDdrMode_Set();
void Drv_TtlRxDig_Set();
void Drv_TtlRxMapping_Set();
void Drv_TtlRxCsc_Set();
void Drv_TtlRxClk_Sel();
u8 Drv_TtlRx_Pll_Set();
void Drv_TtlRx_SelfTimingSet();
void Drv_TtlRx_DeSyncModeSet();
void Drv_TtlRx_VidChkDebug();



#endif
