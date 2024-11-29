
#ifndef _MAIN_H
#define _MAIN_H

#define  LT9211_I2CADR 0x52

// �Ƽ���U2�����ã�U2�����ÿ��Լ���U1��
#define         LT9211C_VERSION              U2             //U1/U2
#define         LT9211C_MODE_SEL             MIPI_IN_TTL_OUT
/*
LVDS_IN_LVDS_OUT  LVDS_IN_MIPI_OUT  LVDS_IN_TTL_OUT
MIPI_IN_LVDS_OUT  MIPI_IN_MIPI_OUT  MIPI_IN_TTL_OUT
TTL_IN_LVDS_OUT   TTL_IN_MIPI_OUT   TTL_IN_TTL_OUT     //TTL_IN_TTL_OUTʱ��ע��ttltx 8264�Ĵ�������
MIPI_REPEATER     MIPI_LEVEL_SHIFT  PATTERN_OUT
*/


#endif
