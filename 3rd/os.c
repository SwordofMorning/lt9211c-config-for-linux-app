#include <string.h>
#include <stdio.h>
/* According to POSIX.1-2001, POSIX.1-2008 */
#include <sys/select.h>

/* According to earlier standards */
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "os.h"

long getFileSize(const char *fileName)
{
    long fileSize = -1;
    struct stat stFile;
    int fd = 0;

    fd = open(fileName, O_RDONLY);
    if(fd  < 0)
    {
        return -1;
    }

    /*
       说明：
       1. struct stat,这是一个保存文件状态信息的结构体
       2. stat中的 st_mode; //文件对应的模式，文件，目录等
       3. S_ISREG判断文件是否是一个常规文件
       4. 常用： S_ISDIR 判断是否是一个目录
    */
    if( (0 == fstat(fd, &stFile)) && S_ISREG(stFile.st_mode) )
    {
        fileSize = stFile.st_size;
    }

    close(fd);
    return fileSize ;
}


int runCmd(char * cmd ,char *buf, int buffsz)
{
    FILE *fp = NULL;

    if(!cmd) return -1;
    if(!buf) return -1;
    memset(buf, 0, buffsz);

    fp = popen(cmd, "r");
    if(!fp)
    {
        //perror("Get Frame Error\n");
        return -1;
    }
    fgets(buf, buffsz, fp);
    pclose(fp);
    return 0;
}


// sleep in millisecond(ms, 1 s = 1000 ms).
int msleep(unsigned int ms)
{
    int ret;
    struct timeval tval;
    tval.tv_sec  =  ms/1000;
    tval.tv_usec = (ms*1000)%1000000;
    ret = select(0,NULL,NULL,NULL,&tval);
    return ret;
}
