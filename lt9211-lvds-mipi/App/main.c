#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

#include <strings.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/shm.h>

#include "config.h"
#include "include.h"

extern u8 I2CADR;// IIC address // bit0 是读写标志位；
int init_i2c(int i2cbus);

// sleep in millisecond(ms, 1 s = 1000 ms).
int msleep(unsigned int ms);

#define CONFIG_RUN_ONCE_FORMER 0
#define CONFIG_RUN_ONCE_LATTER 1

// 保持前一个程序启动
// 通过多进程默认无法bind到同一个port的机制来显示进程单例模式
int run_once_keep_former(char id)
{
    int ret;
    int udpfd = -1;
    struct sockaddr_in address;
    const char* ip = "127.0.0.1";
    int ports[4] = {6703, 6704, 6705, 6706};
    if(id > 4 || id < 0)
    {
        return 0;
    }
    //创建udp socket ， 并将其绑定到端口port上
    bzero(&address, sizeof(address));
    address.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &address.sin_addr);
    address.sin_port = htons(ports[id]);

    udpfd = socket(PF_INET, SOCK_DGRAM, 0);
    if(udpfd < 0)
    {
        return -1;
    }
    ret = bind(udpfd, (struct sockaddr*)&address, sizeof(address));
    if(ret == -1)
    {
        //perror("Bind"); //assert(ret != -1);
        return -1;
    }
    return 0;
}

#define PIP_FILE_FORM "/tmp/.9211.%d"
// 保持最新的程序启动（如果新启动程序，自动杀死前程序）
int run_once_keep_latter(char id)
{
    FILE *fp;
    int ret;
    int pid;
    char pip_file[64] = {0};
    char tmp_str[128];

    if(id > 4 || id < 0)
    {
        return 0;
    }

    memset(tmp_str, 0, sizeof(tmp_str));
    sprintf(tmp_str, "%d", getpid());

    memset(pip_file, 0, sizeof(pip_file));
    sprintf(pip_file, PIP_FILE_FORM, id);

    printf("Checking pid file\n");

    fp = fopen(pip_file,"r");// read/write/append
    if(fp == NULL)
    {
        printf("We are the former!\n");
        fp = fopen(pip_file,"w");

        fwrite(tmp_str, strlen(tmp_str), 1, fp);

        fclose(fp);
        return 0;
    }

    printf("We are the latter! Killing formet.\n");
    while(!feof(fp))
    {
        if(fscanf(fp, "%d", &pid)!= 1) {
            printf("unenble to get pid !\n");
        }
        /* send signal SIGTERM to kill */
        if(pid > 0) {
            printf("Killing %d\n", pid);
            kill(pid, SIGTERM);
        }
    }
    fclose(fp);

    fp = fopen(pip_file,"w");// read/write/append
    fwrite(tmp_str, strlen(tmp_str), 1, fp);
    fclose(fp);

    //// touch /tmp/.9211.x
    //sprintf(tmp_cmd, "touch ", PIP_FILE_FORM, id);
    //memset(tmp_str, 0, sizeof(tmp_str));
    //system(tmp_cmd);



    return 0;
}

int reset_lt9211_by_gpiosys(char index);
void probe_reset(int i2c_bus, int dev_addr)
{
/*
    char id = -1;
    // 2
    if(i2c_bus == 8 && dev_addr == 0x2d)
    {
        reset_lt9211_by_gpiosys('b');
        id = 2;
    }


#if CONFIG_RUN_ONCE_FORMER
    if(run_once_keep_former(id) != 0)
    {
        exit(0);
    }
#elif CONFIG_RUN_ONCE_LATTER
    if(run_once_keep_latter(id) != 0)
    {
        exit(0);
    }
#endif

    //system(reset_cmd);
    //system(reset_log);
*/
    reset_lt9211_by_gpiosys('b');

    msleep(800);
}


int main(int argc, char * argv[])
{
    int i2c_bus = 8;
    int dev_addr = 0x2d;
    int enable_break = 1;// 默认允许跳出程序
    if(argc >= 2)
    {
        sscanf(argv[1], "%x", &i2c_bus);
    }
    if(argc >= 3)
    {
        sscanf(argv[2], "%x", &dev_addr);
        //dev_addr = atoi(argv[2]);
        I2CADR =  dev_addr << 1;
    }
    if(argc >= 4)
    {
        sscanf(argv[3], "%x", &enable_break);
    }
    printf("Schips v20241012\n");
    printf("Arg 1 for i2c-bus(%d)\n", i2c_bus);
    printf("Arg 2 for dev-adr(%x)\n", dev_addr);
    printf("Arg 3 for enable-break(%d)\n", enable_break);

    printf("I2C-[%d] , config [0x%02x]\n", i2c_bus, dev_addr);

    probe_reset(i2c_bus, dev_addr);
    sleep(1);
    init_i2c(i2c_bus);

    Mod_System_Init();
    Mod_LT9211C_Reset();
    LTLog(LOG_INFO, "LT9211C %s %s",__DATE__,__TIME__);
    Mod_ChipID_Read();
#if (LT9211C_MODE_SEL != PATTERN_OUT)
    Mod_SystemRx_PowerOnInit();
    Mod_SystemTx_PowerOnInit();
#endif

    int ret;
    int cnt = 0;
#if 1
    while(1)
    {
        cnt ++;
        #if (LT9211C_MODE_SEL == PATTERN_OUT)
        ret = Mod_ChipTx_PtnOut();
        if(ret == 1)
        {
            if(enable_break)
            {
                break;
            }
        }
        #endif
        #if (LT9211C_MODE_SEL == MIPI_REPEATER)
        Mod_MipiRpt_Handler();
        #endif
        #if (LT9211C_MODE_SEL == MIPI_LEVEL_SHIFT)
        Mod_MipiLs_Handler();
        #endif
        #if (LT9211C_MODE_SEL == LVDS_IN_LVDS_OUT)
        Mod_LvdsRx_Handler();
        Mod_LvdsTx_Handler();
        #endif
        #if (LT9211C_MODE_SEL == LVDS_IN_MIPI_OUT)
        ret = Mod_LvdsRx_Handler();
        ret += Mod_MipiTx_Handler();
        if(ret == 2)
        {
            if(enable_break)
            {
                return 0;
            }
        }
        #endif
        #if (LT9211C_MODE_SEL == LVDS_IN_TTL_OUT)
        Mod_LvdsRx_Handler();
        Mod_TtlTx_Handler();
        #endif
        #if (LT9211C_MODE_SEL == MIPI_IN_LVDS_OUT)
        Mod_MipiRx_Handler();
        Mod_LvdsTx_Handler();
        #endif
        #if (LT9211C_MODE_SEL == MIPI_IN_MIPI_OUT)
        Mod_MipiRx_Handler();
        Mod_MipiTx_Handler();
        #endif
        #if (LT9211C_MODE_SEL == MIPI_IN_TTL_OUT)
        Mod_MipiRx_Handler();
        Mod_TtlTx_Handler();
        #endif
        #if (LT9211C_MODE_SEL == TTL_IN_LVDS_OUT)
        Mod_TtlRx_Handler();
        Mod_LvdsTx_Handler();
        #endif
        #if (LT9211C_MODE_SEL == TTL_IN_MIPI_OUT)
        Mod_TtlRx_Handler();
        Mod_MipiTx_Handler();
        #endif
        #if (LT9211C_MODE_SEL == TTL_IN_TTL_OUT)
        Mod_TtlRx_Handler();
        Mod_TtlTx_Handler();
        #endif
        if(cnt >= 50)
        {
            return -1;
        }
    }
#endif
    Mod_ChipID_Read();
    return -1;
}
