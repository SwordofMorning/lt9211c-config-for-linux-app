#include <stdio.h>
#include <string.h>
#include <math.h>
#include "rk_gpio.h"

static int platform = RK3588_GPIO_PLATFORM;

#define DEBUG 1
#define DEBUG_ERROR 1
#if DEBUG == 0
#define rkgpio_debug(format,...)
#else
#define rkgpio_debug printf
#endif

#if DEBUG_ERROR == 0
#define rkgpio_error(format,...)
#else
#define rkgpio_error(format,...) printf
#endif

static int for_rk_str_to_lower(char * str, int len)
{
    int i;
    for(i=0;i<len;i++)//遍历
    {
        if(0)//str[i]>='a'&&str[i]<='z')
        {
            str[i]-=32;
        }
        else if(str[i]>='A'&&str[i]<='Z')
        {
            str[i]+=32;
        }
    }
    return 0;
}


/***********************************************
 *  * _for_rk_str_replace
 *   * 字符串替换
 *    *
 *     * pszInput       要转变的字符串
 *      * pszOld                要替换的子串
 *       * pszNew               被替换成的子串
 *        * pszOutput   输出的缓冲区
 *         * nOutputlen 输出缓冲区的长度
 *          *
 *           **********************************************/
static void _for_rk_str_replace(char * pszInput, char * pszOld, char * pszNew,char * pszOutput,int nOutputlen)
{
    char temp[128];
    int nLen=0;
    char *s, *p;
    s = pszInput;
    while (s != NULL)
    {
        p = strstr(s, pszOld);

        if (p == NULL )
        {
            memcpy(pszOutput+nLen,s,strlen(s)+nLen>nOutputlen?nOutputlen-nLen:strlen(s));
            return ;
        }
        memcpy(pszOutput+nLen,s,p-s+nLen>nOutputlen?nOutputlen-nLen:p-s);
        nLen+=p-s+nLen>nOutputlen?nOutputlen-nLen:p-s;
        if(nLen>=nOutputlen)
        {
            return;
        }
        memcpy(pszOutput+nLen,pszNew,strlen(pszNew)+nLen>nOutputlen?nOutputlen-nLen:strlen(pszNew));
        nLen+=strlen(pszNew)+nLen>nOutputlen?nOutputlen-nLen:strlen(pszNew);
        if(nLen>=nOutputlen)
        {
            return;
        }
        s+=strlen(pszOld)+p-s;
    }
    return ;
}

int set_rk_gpio_platform(enum RK_GPIO_PLATFORM pl)
{
    if(pl >= RKUNKNOW_GPIO_PLATFORM)
    {
        rkgpio_error("[%s] RKUNKNOW_GPIO_PLATFORM %d/%d\n", __func__, pl, RKUNKNOW_GPIO_PLATFORM);
        return -1;
    }
    platform = pl;
    return 0;
}

int str_to_rk_gpio(const char * str)
{
    char str_buff[64]= {0};
    char tmp_str[64]= {0};
    int ret;
    int bank = 0, pos = 0;
    char group = 0;
    int pin;
    if(!str)
    {
        rkgpio_error("[%s] NULL\n", __func__);
        return -1;
    }
    if(strlen(str) < 3)
    {
        rkgpio_error("[%s] string [%s] to short.\n", __func__, str);
        return -1;
    }

    memset(tmp_str, 0, sizeof(tmp_str));
    strncpy(str_buff, str, strlen(str));
    rkgpio_debug("Input : %s\n", str_buff);

    for_rk_str_to_lower(str_buff, strlen(str_buff));

    memset(tmp_str, 0, sizeof(tmp_str));
    _for_rk_str_replace(str_buff, "gpio", "", tmp_str, sizeof(tmp_str));
    sprintf(str_buff, "%s", tmp_str);

    memset(tmp_str, 0, sizeof(tmp_str));
    _for_rk_str_replace(str_buff, "_", "", tmp_str, sizeof(tmp_str));
    sprintf(str_buff, "%s", tmp_str);

    memset(tmp_str, 0, sizeof(tmp_str));
    _for_rk_str_replace(str_buff, "rk", "", tmp_str, sizeof(tmp_str));
    sprintf(str_buff, "%s", tmp_str);

    memset(tmp_str, 0, sizeof(tmp_str));
    _for_rk_str_replace(str_buff, "rockchip", "", tmp_str, sizeof(tmp_str));
    sprintf(str_buff, "%s", tmp_str);

#if 0
    int v;
    sscanf("2d", "%d", &v);
    printf("%d\n", v);
    // v =  2;
#endif

    //ret = sscanf(str_buff, "gpio%d_%c%d", &bank, &group, &pos);
    ret = sscanf(str_buff, "%d%c%d", &bank, &group, &pos);

    if(platform == RK3588_GPIO_PLATFORM || platform == RK3568_GPIO_PLATFORM)
    {
        pin = bank * 32 + (((group - 'a') * 8) + pos);
        rkgpio_debug("pin = %d \t= [ bank(%d) * 32 ] + [ group(%c->%d) * 8 ] + pos(%d)\n", pin, bank, group, group - 'a', pos);
        rkgpio_debug("\t\t= %d + %d + %d\n", bank * 32, (group - 'a') * 8, pos);
    }
    return pin;
}

// int test(void)
// {
//     set_rk_gpio_platform(RK3588_GPIO_PLATFORM);

//     str_to_rk_gpio("GPIO1_A0");
//     str_to_rk_gpio("1a0");
//     str_to_rk_gpio("gpio1_a6");

//     str_to_rk_gpio("GPIO4_C6");
//     str_to_rk_gpio("gpio4_c6");
//     str_to_rk_gpio("4_c6");
//     str_to_rk_gpio("4c6");

//     return 0;
// }

// int main(void)
// {
//     test();
// }
