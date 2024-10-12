/******************************************************************************
  * @project: LT9211C
  * @file: ModLvdsRx.h
  * @author: xding
  * @company: LONTIUM COPYRIGHT and CONFIDENTIAL
  * @date: 2023.01.29
/******************************************************************************/

#ifndef _MODLVDSRX_H
#define _MODLVDSRX_H

#define     LVDSRX_DESSC_SEL        DISABLED         //ENABLED/DISABLED
#define     LVDSRX_PORT_SEL         PORTA        //PORTA/PORTB/DOU_PORT
#define     LVDSRX_PORT_SWAP        DISABLED         //ENABLED/DISABLED
#define     LVDSRX_LANENUM          FOUR_LANE       //FOUR_LANE/FIVE_LANE
#define     LVDSRX_MODE             SYNC_MODE       //SYNC_MODE/DE_MODE
#define     LVDSRX_DATAFORMAT       VESA            //VESA/JEIDA
#define     LVDSRX_COLORSPACE       RGB             //RGB/YUV422
#define     LVDSRX_COLORDEPTH       DEPTH_8BIT      //DEPTH_6BIT/DEPTH_8BIT/DEPTH_10BIT
#define     LVDSRX_SYNC_INTER_MODE  DISABLED         //ENABLED/DISABLED
#define     LVDSRX_VIDEO_FORMAT     P_FORMAT        //P_FORMAT/I_FORMAT
#define     LVDSRX_SYNC_CODE_SEND   NON_REPECTIVE       ///NON_REPECTIVE/REPECTIVE
#define     LVDSRX_VID_SEL          VID_1920x1080_30Hz      //VID_720x480_60Hz/VID_1280x720_60Hz/VID_1920x720_60Hz/VID_1920x1080_60Hz/VID_3840x2160_30Hz    set when YUV4224
#define     LVDSRX_PLL_6M_MODE      DISABLED        //ENABLED/DISABLED















int Mod_LvdsRx_StateHandler(void);
int Mod_LvdsRx_Handler(void);

#endif
