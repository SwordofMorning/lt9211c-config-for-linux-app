#ifndef ___MISC_H__
#define ___MISC_H__
#include <stdint.h>

typedef unsigned char         UINT8;
typedef unsigned int          UINT16;
typedef unsigned long         UINT32;


typedef enum {FALSE = 0, TRUE = !FALSE} PinStatus;
typedef enum {RESET = 0, SET = !RESET} FlagStatus, ITStatus, BitStatus, BitAction;

#define BIT0_1  0x01
#define BIT1_1  0x02
#define BIT2_1  0x04
#define BIT3_1  0x08
#define BIT4_1  0x10
#define BIT5_1  0x20
#define BIT6_1  0x40
#define BIT7_1  0x80

#define BIT0_0  0xFE
#define BIT1_0  0xFD
#define BIT2_0  0xFB
#define BIT3_0  0xF7
#define BIT4_0  0xEF
#define BIT5_0  0xDF
#define BIT6_0  0xBF
#define BIT7_0  0x7F

#endif/*___MISC_H__*/
