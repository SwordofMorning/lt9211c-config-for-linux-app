
#ifndef _MAIN_H
#define _MAIN_H

#define  LT9211_I2CADR 0x5a

// 推荐用U2的配置，U2的配置可以兼容U1的
#define         LT9211C_VERSION              U2             //U1/U2
#define         LT9211C_MODE_SEL             LVDS_IN_MIPI_OUT
/*
LVDS_IN_LVDS_OUT  LVDS_IN_MIPI_OUT  LVDS_IN_TTL_OUT
MIPI_IN_LVDS_OUT  MIPI_IN_MIPI_OUT  MIPI_IN_TTL_OUT
TTL_IN_LVDS_OUT   TTL_IN_MIPI_OUT   TTL_IN_TTL_OUT     //TTL_IN_TTL_OUT时需注意ttltx 8264寄存器配置
MIPI_REPEATER     MIPI_LEVEL_SHIFT  PATTERN_OUT
*/


#endif
