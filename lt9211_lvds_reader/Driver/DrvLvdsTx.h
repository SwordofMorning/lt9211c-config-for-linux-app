#include    "include.h"

#ifndef     _DRVLVDSTX_H
#define     _DRVLVDSTX_H

#if ((LT9211C_MODE_SEL == LVDS_IN_LVDS_OUT)||(LT9211C_MODE_SEL == MIPI_IN_LVDS_OUT)||(LT9211C_MODE_SEL == TTL_IN_LVDS_OUT))

extern void Drv_LvdsTxSw_Rst();
extern void Drv_LVDSTxPhy_PowerOff();
extern void Drv_LvdsTxPhy_Poweron(void);
extern void Drv_LvdsTxPll_RefPixClk_Set();
extern void Drv_LvdsTxPll_Config(void);
extern u8 Drv_LvdsTxPll_Cali(void);
extern void Drv_LvdsTxPort_Set();
extern void Drv_LvdsTxVidFmt_Set();
extern void Drv_LvdsTxLaneNum_Set();
extern void Drv_LvdsTxPort_Swap();
extern void Drv_LvdsTxPort_Copy();
extern void Drv_LvdsTxCsc_Set();

#endif

#endif
