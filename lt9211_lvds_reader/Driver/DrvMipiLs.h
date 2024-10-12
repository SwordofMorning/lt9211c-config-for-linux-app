#include    "include.h"

#ifndef     _DRVMIPILS_H
#define     _DRVMIPILS_H

#if (LT9211C_MODE_SEL == MIPI_LEVEL_SHIFT)

typedef enum
{
    MIPILS_4LANE = 0x00,
    MIPILS_1LANE = 0x04,
    MIPILS_2LANE = 0x08,
    MIPILS_3LANE = 0x0c,
}Enum_MIPILSRX_PORTLANE_NUM;


extern u8 Drv_MipiLsClk_Change();
extern void Drv_MipiLs_PortSel();
extern void Drv_MipiLs_ClkSel();
extern void Drv_MipiLsRxPhy_Set();
extern void Drv_MipiLsRxDig_Set(void);
extern void Drv_MipiLsTxPhy_Set();
extern void Drv_MipiLsTx_PllSet(void);
extern u8 Drv_MipiLsTx_PllCali(void);
extern void Drv_MipiLsBta_Set(void);
extern void Drv_MipiLsClk_Check(void);

#endif

#endif

