#ifndef __OS_H__
#define __OS_H__

long getFileSize(const char *fileName);
int runCmd(char * cmd ,char *buf, int buffsz);
int msleep(unsigned int ms);

#endif
