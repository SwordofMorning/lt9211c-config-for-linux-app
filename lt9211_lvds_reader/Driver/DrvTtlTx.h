


#include    "include.h"

#ifndef     _DRVTTLTX_H
#define     _DRVTTLTX_H

extern void Drv_TtlTxHalfBit_Set(void);
extern void Drv_TtlTxSyncInter_Set(void);
extern void Drv_TtlTxDdrMode_Set(void);
extern void Drv_TTlTxPhy_Poweron(void);
extern void Drv_TtlTxDig_Set(void);
extern void Drv_TtlTxMapping_Set(void);
extern void Drv_TtlTxPhase_Set(void);
extern void Drv_CscSet_ForTtlTx(void);
extern void Drv_DesscPll_ForTtlTx(void);

#endif
