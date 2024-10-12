#include    "include.h"

#ifndef     _LVDSRX_H
#define     _LVDSRX_H


extern void Drv_LvdsRx_PhySet(void);
extern void Drv_LvdsRx_ClkSel(void);
extern void Drv_LvdsRx_DataPathSel();
extern void Drv_LvdsRxPort_Set();
extern void Drv_LvdsRxLaneNum_Set();
extern void Drv_LvdsRxPort_Swap();
extern u8 Drv_LvdsRxVidFmt_Set();
extern void Drv_LvdsRx_SelfTimingSet();
extern void Drv_LvdsRxCsc_Set(void);
extern u8 Drv_LvdsRxPll_Cali();
extern void Drv_LvdsRx_VidChkDebug(void);
extern void Drv_LvdsRx_DesscPll_Set(void);
extern void Drv_LvdsRx_DesscDig_Set(void);

#endif
