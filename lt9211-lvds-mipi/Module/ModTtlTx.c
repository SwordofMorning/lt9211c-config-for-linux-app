/******************************************************************************
  * @project: LT9211C
  * @file: ModTtlTx.c
  * @author: xding
  * @company: LONTIUM COPYRIGHT and CONFIDENTIAL
  * @date: 2023.01.29
/******************************************************************************/
#include    "include.h"
#if ((LT9211C_MODE_SEL == LVDS_IN_TTL_OUT)||(LT9211C_MODE_SEL == MIPI_IN_TTL_OUT)||(LT9211C_MODE_SEL == TTL_IN_TTL_OUT))

void Mod_TtlTx_StateJudge(void)
{
    //monitor upstream video stable.
    if(g_stChipTx.ucTxState > STATE_CHIPTX_UPSTREAM_VIDEO_READY)
    {
        if(g_stChipTx.b1UpstreamVideoReady == FALSE)
        {
            Mod_SystemTx_SetState(STATE_CHIPTX_UPSTREAM_VIDEO_READY);
        }
    }
}

void Mod_TtlTx_StateHandler(void)
{
    LTLog(LOG_INFO, "g_stChipTx.ucTxState = %d", g_stChipTx.ucTxState);
    switch (g_stChipTx.ucTxState)
    {
        case STATE_CHIPTX_POWER_ON:
            Mod_SystemTx_SetState(STATE_CHIPTX_UPSTREAM_VIDEO_READY);
        break;

        case STATE_CHIPTX_UPSTREAM_VIDEO_READY:
            if(g_stChipTx.b1TxStateChanged == TRUE)
            {
                g_stChipTx.b1TxStateChanged = FALSE;
            }

            if(g_stChipTx.b1UpstreamVideoReady == TRUE)
            {
                #if LT9211C_MODE_SEL == LVDS_IN_TTL_OUT
                    #if LVDSRX_DESSC_SEL == DISABLED
                        Drv_DesscPll_ForTtlTx();
                    #endif
                #endif
                #if LT9211C_MODE_SEL == TTL_IN_TTL_OUT
                    Drv_DesscPll_ForTtlTx();
                #endif
                Drv_TtlTxHalfBit_Set();
                Drv_TtlTxSyncInter_Set();
                Drv_TtlTxDdrMode_Set();
                Drv_TTlTxPhy_Poweron();
                Mod_SystemTx_SetState(STATE_CHIPTX_CONFIG_VIDEO);
            }
        break;

        case STATE_CHIPTX_CONFIG_VIDEO:
            Mod_SystemTx_SetState(STATE_CHIPTX_VIDEO_OUT);
        break;

        case STATE_CHIPTX_VIDEO_OUT:
            Drv_VidChkAll_Get(&g_stVidChk);
            Drv_TtlTxDig_Set();
            Drv_TtlTxMapping_Set();
            Drv_CscSet_ForTtlTx();
            Drv_TtlTxPhase_Set();
            Mod_SystemTx_SetState(STATE_CHIPTX_PLAY_BACK);
        break;

        case STATE_CHIPTX_PLAY_BACK:
        break;

    }
}

void Mod_TtlTx_Handler(void)
{
    Mod_TtlTx_StateJudge();
    Mod_TtlTx_StateHandler();
}

#endif
