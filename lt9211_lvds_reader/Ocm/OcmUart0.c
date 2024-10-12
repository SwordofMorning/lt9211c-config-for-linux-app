
#include "include.h"

#define MAX_NUMBER_BYTES  128
u8 g_ucLogLevel = LOG_INFO;

RDATA char *messageTypeStr[] = {"DEBUG","INFO","WARN", "ERROR","CRIT"};


//    example
//    codea = 0x01;
//    codeb = 0x0002;
//    codec = 0x00000003;
//    printLT(PRINT_PRIO_HIG, "\r\nh'u8  is 0x%02x", codea);
//    printLT(PRINT_PRIO_HIG, "\r\nh'u16 is 0x%04x", codeb);
//    printLT(PRINT_PRIO_HIG, "\r\nh'u32 is 0x%08lx",codec);
//    printLT(PRINT_PRIO_HIG, "\r\nd'u8  is %d", codea);
//    printLT(PRINT_PRIO_HIG, "\r\nd'u16 is %d", codeb);
//    printLT(PRINT_PRIO_HIG, "\r\nd'u32 is %ld", codec);


//===========================================
//func  :UART0 Timer1 baud rate initial setting
//input :
//output:N/A
//return:void
//Info  :N/A
//===========================================

void Ocm_UART0_Timer1Init(u32 ulBaudrate)    //T1M = 1, SMOD = 1
{
#if 0
    P06_Quasi_Mode;        //Setting UART pin as Quasi mode for transmit
    P07_Quasi_Mode;        //Setting UART pin as Quasi mode for transmit

    SCON =  0x50;         //UART0 Mode1,REN=1,TI=1
    TMOD |= 0x20;         //Timer1 Mode1

    set_SMOD;            //UART0 Double Rate Enable
    set_T1M;
    clr_BRCK;            //Serial port 0 baud rate clock source = Timer1

    #ifdef FOSC_160000
    TH1 = 256 - (1000000/ulBaudrate+1);               /*16 MHz */
    #endif
    #ifdef FOSC_166000
    TH1 = 256 - (1037500/ulBaudrate);                 /*16.6 MHz */
    #endif
    set_TR1;
    set_TI;                               //For printf function must setting TI = 1
#endif
}


void Ocm_PrintLevel_Set(u8 ucLvl)
{
    g_ucLogLevel = ucLvl;
}

void LTLog(u8 ucLvl, const char* fmt, ...)
{
    char buf[MAX_NUMBER_BYTES] = {0};
    va_list args;
    if (ucLvl > LOG_NOTRACE)
    {
        return;
    }

    //打印大于等于该级别的字符串
    if(ucLvl >= g_ucLogLevel)
    {
        va_start(args, fmt);
        vsprintf(buf, fmt, args);
        va_end(args);
        //左对齐5个字符
        printf("[%-5s] %s\n", messageTypeStr[ucLvl], buf);
    }
}
