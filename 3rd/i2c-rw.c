
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
//#include "i2cbusses.h"
//#include "util.h"
//#include "../version.h"
#include "i2c-rw.h"
#define MISSING_FUNC_FMT   "Error: Adapter does not have %s capability\n"

// 是否打印调试信息
#define DEBUG 0
#define DEBUG_ERROR 0


#if DEBUG == 0
#define i2c_debug(format,...)
#else
#define i2c_debug printf
#endif

#if DEBUG_ERROR == 0
#define i2c_error(format,...)
#else
#define i2c_error(format,...) printf
#endif

int max_try = 1;

// 检查I2C是否具备功能
int i2c_check_funcs(int file)
{
    unsigned long funcs;

    /* check adapter functionality */
    if (ioctl(file, I2C_FUNCS, &funcs) < 0) {
        i2c_error("Error: Could not get the adapter "
            "functionality matrix: %s\n", strerror(errno));
        return -1;
    }

    if (!(funcs & I2C_FUNC_I2C)) {
        i2c_error(MISSING_FUNC_FMT, "I2C transfers");
        return -1;
    }

    return 0;
}

// sleep in millisecond(ms, 1 s = 1000 ms).
static inline int i2c_msleep(unsigned int ms)
{
#if 0
    int ret;
    struct timeval tval;
    tval.tv_sec  =  ms/1000;
    tval.tv_usec = (ms*1000)%1000000;
    ret = select(0,NULL,NULL,NULL,&tval);
    return ret;
#else
    return 0;
#endif
}

#define   getBits(x, end, start)   ( (x & ~(~(0U)<<((end)-(start) + 1))<<(start)) >> (start) )

// 写入8位寄存器中的8位值
int i2csend_addr8_value8(int file, char dev_addr, char reg_addr, char reg_value)
{
    struct i2c_msg msgs[I2C_RDRW_IOCTL_MAX_MSGS]; // 42 is I2C_RDRW_IOCTL_MAX_MSGS
    struct i2c_rdwr_ioctl_data rdwr;
    unsigned char buf[2];
    int nmsgs = 0, ret;

    //i2c_debug("i2ctransfer -f -y 11 w2@0x%x 0x%x 0x%x\n", dev_addr, reg_addr, reg_value);

    msgs[0].addr = dev_addr;
    msgs[0].flags = 0;
    msgs[0].len = 2;
    msgs[0].buf = buf;
    buf[0] = reg_addr;
    buf[1] = reg_value;

    rdwr.msgs = msgs;
    rdwr.nmsgs = 1;
    int retry = 0;
    for(retry = 0; retry < max_try;retry++)
    {
        ret = ioctl(file, I2C_RDWR, &rdwr);
        if (ret < 0) {
            i2c_error("Error(%d/%d): Sending messages failed: %s\n", retry + 1, max_try, strerror(errno));
        } else if (ret < nmsgs) {
            i2c_error("Warning: only %d/%d messages were sent\n", ret, nmsgs);
        }else {
            break;
        }
        i2c_msleep(5);
    }
    return 0;
}

// 读取8位寄存器中的8位值
int i2crecv_addr8_value8(int file, char dev_addr, char reg_addr)
{
    struct i2c_msg msgs[I2C_RDRW_IOCTL_MAX_MSGS]; // 42 is I2C_RDRW_IOCTL_MAX_MSGS
    struct i2c_rdwr_ioctl_data rdwr;
    unsigned char buf_0[1];
    unsigned char buf_1[1];
    int nmsgs = 0, ret;

    //i2c_debug("i2ctransfer -f -y 11 w1@0x%x 0x%x r1\n", dev_addr, reg_addr);

    msgs[0].addr = dev_addr;
    msgs[0].flags = 0;
    msgs[0].len = 1;
    msgs[0].buf = buf_0;
    buf_0[0] = reg_addr;

    msgs[1].addr = dev_addr;
    msgs[1].flags = I2C_M_RD;
    msgs[1].len = 1;
    msgs[1].buf = buf_1;
    buf_1[0] = 0x00;

    rdwr.msgs = msgs;
    rdwr.nmsgs = 2;
    int retry = 0;
    for(retry = 0; retry < max_try;retry++)
    {
        ret = ioctl(file, I2C_RDWR, &rdwr);
        if (ret < 0) {
            i2c_error("Error(%d/%d): Sending messages failed: %s\n", retry + 1, max_try, strerror(errno));
        } else if (ret < nmsgs) {
            i2c_error("Warning: only %d/%d messages were sent\n", ret, nmsgs);
        }else {
            break;
        }
        i2c_msleep(5);
    }
    //i2c_debug("ret %d\n", ret);
#if 0
    // for read
    unsigned i;
    __u16 j;
    //int flags = PRINT_READ_BUF | (0 ? PRINT_HEADER | PRINT_WRITE_BUF : 0);
    for (i = 0; i < ret; i++) {
        int read = msgs[i].flags & I2C_M_RD;
        int recv_len = msgs[i].flags & I2C_M_RECV_LEN;
        //int print_buf = (read && (flags & PRINT_READ_BUF)) ||
        //      (!read && (flags & PRINT_WRITE_BUF));
        __u16 len = recv_len ? msgs[i].buf[0] + 1 : msgs[i].len;

/*
        if (flags & PRINT_HEADER) {
            i2c_debug("msg %u: addr 0x%02x, %s, len ",
                i, msgs[i].addr, read ? "read" : "write");
            if (!recv_len || flags & PRINT_READ_BUF)
                i2c_debug("%u", len);
            else
                i2c_debug("TBD");
        }
*/

        if (len && read) {
            //if (flags & PRINT_HEADER) i2c_debug(", buf ");
            for (j = 0; j < len - 1; j++)
                i2c_debug("0x%02x ", msgs[i].buf[j]);
            /* Print final byte with newline */
            i2c_debug("0x%02x\n", msgs[i].buf[j]);
        }
    }
#else
    //i2c_debug("0x%02x\n", msgs[1].buf[0]);
#endif
    return msgs[1].buf[0];
    return 0;
}

// 写入16位寄存器中8位的值
int i2csend_addr16_value8(int file, unsigned char dev_addr, unsigned short reg_addr, unsigned char reg_value)
{
    struct i2c_msg msgs[I2C_RDRW_IOCTL_MAX_MSGS]; // 42 is I2C_RDRW_IOCTL_MAX_MSGS
    struct i2c_rdwr_ioctl_data rdwr;
    unsigned char buf[3];
    int nmsgs = 0, ret = 0;

    //i2c_debug("i2ctransfer -f -y 11 w3@0x%x 0x%x 0x%x  0x%x\n", dev_addr, getBits(reg_addr, 15, 8), getBits(reg_addr,  7, 0), reg_value);

    msgs[0].addr = dev_addr;
    msgs[0].flags = 0;
    msgs[0].len = 2 + 1;
    msgs[0].buf = buf;
    buf[0] = getBits(reg_addr, 15, 8);
    buf[1] = getBits(reg_addr,  7, 0);
    buf[2] = reg_value;

    rdwr.msgs = msgs;
    rdwr.nmsgs = 1;

    int retry = 0;
    for(retry = 0; retry < max_try;retry++)
    {
        ret = ioctl(file, I2C_RDWR, &rdwr);
        if (ret < 0) {
            //i2c_error("Error(%d/%d): Sending messages failed: %s\n", retry + 1, max_try, strerror(errno));
        } else if (ret < nmsgs) {
            i2c_error("Warning: only %d/%d messages were sent\n", ret, nmsgs);
        }else {
            break;
        }
        i2c_msleep(5);
    }
    return ret;
}

// 读取16位寄存器中8位的值
unsigned char i2crecv_addr16_value8(int file, unsigned char dev_addr, unsigned short reg_addr)
{
    struct i2c_msg msgs[I2C_RDRW_IOCTL_MAX_MSGS]; // 42 is I2C_RDRW_IOCTL_MAX_MSGS
    struct i2c_rdwr_ioctl_data rdwr;
    unsigned char buf_0[2];
    unsigned char buf_1[1];
    int nmsgs = 0, ret = 0;

    /*
    i2c_debug("i2ctransfer -f -y 11 w3@0x%x 0x%x 0x%x  0x%x\n", dev_addr,
           getBits(reg_addr, 15, 8), getBits(reg_addr,  7, 0),
           reg_addr);
    */

    msgs[0].addr = dev_addr;
    msgs[0].flags = 0;
    msgs[0].len = 2;
    msgs[0].buf = buf_0;
    buf_0[0] = getBits(reg_addr, 15, 8);
    buf_0[1] = getBits(reg_addr,  7, 0);

    msgs[1].addr = dev_addr;
    msgs[1].flags = I2C_M_RD;
    msgs[1].len = 1;
    msgs[1].buf = buf_1;
    buf_1[0] = 0x00;

    rdwr.msgs = msgs;
    rdwr.nmsgs = 2;
    int retry = 0;
    for(retry = 0; retry < max_try;retry++)
    {
        ret = ioctl(file, I2C_RDWR, &rdwr);
        if (ret < 0) {
            i2c_error("Error(%d/%d): Sending messages failed: %s\n", retry + 1, max_try, strerror(errno));
        } else if (ret < nmsgs) {
            i2c_error("Warning: only %d/%d messages were sent\n", ret, nmsgs);
        }else {
            break;
        }
        i2c_msleep(5);
    }
#if 1
    // for read
    unsigned i;
    __u16 j;
    //int flags = PRINT_READ_BUF | (0 ? PRINT_HEADER | PRINT_WRITE_BUF : 0);
    //i2c_debug("i2c_debug flags is 0x%x, %x, %x\n", flags, PRINT_HEADER, PRINT_WRITE_BUF, PRINT_READ_BUF);
    for (i = 0; i < ret; i++) {
        int read = msgs[i].flags & I2C_M_RD;
        int recv_len = msgs[i].flags & I2C_M_RECV_LEN;
        //int print_buf = (read && (flags & PRINT_READ_BUF)) ||
        //        (!read && (flags & PRINT_WRITE_BUF));
        __u16 len = recv_len ? msgs[i].buf[0] + 1 : msgs[i].len;

        if (len && read) {
            //if (flags & PRINT_HEADER) i2c_debug(", buf ");
            for (j = 0; j < len - 1; j++)
                i2c_debug("0x%02x ", msgs[i].buf[j]);
            /* Print final byte with newline */
            i2c_debug("0x%02x\n", msgs[i].buf[j]);
        }
    }
#else
    i2c_debug("0x%02x\n", msgs[1].buf[0]);
#endif
    return msgs[1].buf[0];
}

// 写入16位寄存器16位值
int i2csend_addr16_value16(int file, unsigned char dev_addr, unsigned short reg_addr, unsigned short reg_value)
{
    struct i2c_msg msgs[I2C_RDRW_IOCTL_MAX_MSGS]; // 42 is I2C_RDRW_IOCTL_MAX_MSGS
    struct i2c_rdwr_ioctl_data rdwr;
    unsigned char buf[4];
    int nmsgs = 0, ret = 0;

    /*
    i2c_debug("i2ctransfer -f -y 11 w3@0x%x 0x%x 0x%x  0x%x 0x%x\n", dev_addr,
           getBits(reg_addr, 15, 8), getBits(reg_addr,  7, 0),
           getBits(reg_value, 15, 8), getBits(reg_value,  7, 0));
    */

    msgs[0].addr = dev_addr;
    msgs[0].flags = 0;
    msgs[0].len = 2 + 2;
    msgs[0].buf = buf;
    buf[0] = getBits(reg_addr, 15, 8);
    buf[1] = getBits(reg_addr,  7, 0);
    buf[2] = getBits(reg_value, 15, 8);
    buf[3] = getBits(reg_value,  7, 0);

    rdwr.msgs = msgs;
    rdwr.nmsgs = 1;
    int retry = 0;
    for(retry = 0; retry < max_try;retry++)
    {
        ret = ioctl(file, I2C_RDWR, &rdwr);
        if (ret < 0) {
            i2c_error("Error(%d/%d): Sending messages failed: %s\n", retry + 1, max_try, strerror(errno));
        } else if (ret < nmsgs) {
            i2c_error("Warning: only %d/%d messages were sent\n", ret, nmsgs);
        }else {
            break;
        }
        i2c_msleep(5);
    }
    return ret;
}

// 读取16位寄存器中的16位值（端序未确定）
unsigned short i2crecv_addr16_value16(int file, unsigned char dev_addr, unsigned short reg_addr)
{
    struct i2c_msg msgs[I2C_RDRW_IOCTL_MAX_MSGS]; // 42 is I2C_RDRW_IOCTL_MAX_MSGS
    struct i2c_rdwr_ioctl_data rdwr;
    unsigned char buf_0[2];
    unsigned char buf_1[2];
    int nmsgs = 0, ret = 0;

    /*
    i2c_debug("i2ctransfer -f -y 11 w3@0x%x 0x%x 0x%x  0x%x\n", dev_addr,
           getBits(reg_addr, 15, 8), getBits(reg_addr,  7, 0),
           reg_addr);
    */

    msgs[0].addr = dev_addr;
    msgs[0].flags = 0;
    msgs[0].len = 2;
    msgs[0].buf = buf_0;
    buf_0[0] = getBits(reg_addr, 15, 8);
    buf_0[1] = getBits(reg_addr,  7, 0);

    msgs[1].addr = dev_addr;
    msgs[1].flags = I2C_M_RD;
    msgs[1].len = 2;
    msgs[1].buf = buf_1;
    buf_1[0] = 0x00;

    rdwr.msgs = msgs;
    rdwr.nmsgs = 2;
    int retry = 0;
    for(retry = 0; retry < max_try;retry++)
    {
        ret = ioctl(file, I2C_RDWR, &rdwr);
        if (ret < 0) {
            i2c_error("Error(%d/%d): Sending messages failed: %s\n", retry + 1, max_try, strerror(errno));
        } else if (ret < nmsgs) {
            i2c_error("Warning: only %d/%d messages were sent\n", ret, nmsgs);
        }else {
            break;
        }
        i2c_msleep(5);
    }
    //i2c_debug("ret %d\n", ret);
#if 1
    // for read
    unsigned i;
    __u16 j;
    //int flags = PRINT_READ_BUF | (0 ? PRINT_HEADER | PRINT_WRITE_BUF : 0);
    //i2c_debug("i2c_debug flags is 0x%x, %x, %x\n", flags, PRINT_HEADER, PRINT_WRITE_BUF, PRINT_READ_BUF);
    for (i = 0; i < ret; i++) {
        int read = msgs[i].flags & I2C_M_RD;
        int recv_len = msgs[i].flags & I2C_M_RECV_LEN;
        //int print_buf = (read && (flags & PRINT_READ_BUF)) ||
        //        (!read && (flags & PRINT_WRITE_BUF));
        __u16 len = recv_len ? msgs[i].buf[0] + 1 : msgs[i].len;

        if (len && read) {
            //if (flags & PRINT_HEADER) i2c_debug(", buf ");
            for (j = 0; j < len - 1; j++)
                i2c_debug("0x%02x ", msgs[i].buf[j]);
            /* Print final byte with newline */
            i2c_debug("0x%02x\n", msgs[i].buf[j]);
        }
    }
#else
    i2c_debug("0x%02x%02x\n", msgs[1].buf[0], msgs[1].buf[1]);
#endif
    return msgs[1].buf[0] | (msgs[1].buf[1] << 8);
}

// I2C总线连续收
int i2crecv_addr8_valuex(int file, unsigned char dev_addr, unsigned char reg_addr, unsigned char *values, unsigned int read_cnt)
{
    struct i2c_msg msgs[I2C_RDRW_IOCTL_MAX_MSGS]; // 42 is I2C_RDRW_IOCTL_MAX_MSGS
    struct i2c_rdwr_ioctl_data rdwr;
    unsigned char buf_0[1];
    //unsigned char buf_1[1];
    int nmsgs = 0, ret = 0;
    // for read
    unsigned i;
    __u16 j;
    //int flags = PRINT_READ_BUF | (0 ? PRINT_HEADER | PRINT_WRITE_BUF : 0);

    /*
    i2c_debug("i2ctransfer -f -y 11 w3@0x%x 0x%x 0x%x  0x%x\n", dev_addr,
           getBits(reg_addr, 15, 8), getBits(reg_addr,  7, 0),
           reg_addr);
    */

    msgs[0].addr = dev_addr;
    msgs[0].flags = 0;
    msgs[0].len = 1;
    msgs[0].buf = buf_0;
    buf_0[0] = reg_addr;

    msgs[1].addr = dev_addr;
    msgs[1].flags = I2C_M_RD;
    msgs[1].len = read_cnt;
    msgs[1].buf = values;

    rdwr.msgs = msgs;
    rdwr.nmsgs = 2;
    int retry = 0;
    for(retry = 0; retry < max_try;retry++)
    {
        ret = ioctl(file, I2C_RDWR, &rdwr);
        if (ret < 0) {
            i2c_error("Error(%d/%d): Sending messages failed: %s\n", retry + 1, max_try, strerror(errno));
        } else if (ret < nmsgs) {
            i2c_error("Warning: only %d/%d messages were sent\n", ret, nmsgs);
        }else {
            break;
        }
        i2c_msleep(5);
    }
    //i2c_debug("ret %d\n", ret);
    for (i = 0; i < ret; i++) {
        int recv_len = msgs[i].flags & I2C_M_RECV_LEN;
        __u16 len = recv_len ? msgs[i].buf[0] + 1 : msgs[i].len;

        if (len) {
            for (j = 0; j < len - 1; j++)
                i2c_debug("0x%02x ", msgs[i].buf[j]);
            /* Print final byte with newline */
            i2c_debug("0x%02x\n", msgs[i].buf[j]);
        }
    }
    //return msgs[1].buf[0];
    return ret;
}

// for i2c-switcher
int i2csend_addr8_value0(int file, unsigned char dev_addr, unsigned char reg_addr)
{
    struct i2c_msg msgs[I2C_RDRW_IOCTL_MAX_MSGS]; // 42 is I2C_RDRW_IOCTL_MAX_MSGS
    struct i2c_rdwr_ioctl_data rdwr;
    unsigned char buf[1];
    int nmsgs = 0, ret = 0;

    //i2c_debug("i2ctransfer -f -y 11 w1@0x%x 0x%x\n", dev_addr, reg_addr);

    msgs[0].addr = dev_addr;
    msgs[0].flags = 0;
    msgs[0].len = 1;
    msgs[0].buf = buf;
    buf[0] = reg_addr;

    rdwr.msgs = msgs;
    rdwr.nmsgs = 1;
    int retry = 0;
    for(retry = 0; retry < max_try;retry++)
    {
        ret = ioctl(file, I2C_RDWR, &rdwr);
        if (ret < 0) {
            i2c_error("Error(%d/%d): Sending messages failed: %s\n", retry + 1, max_try, strerror(errno));
        } else if (ret < nmsgs) {
            i2c_error("Warning: only %d/%d messages were sent\n", ret, nmsgs);
        }else {
            break;
        }
        i2c_msleep(5);
    }

    return ret;
}

// 针对i2c总线 连续写
int i2csend_addr8_valuex(int file, unsigned char dev_addr, unsigned char reg_addr, unsigned char *values, unsigned int value_cnt)
{
    struct i2c_msg msgs[1]; // 42 is I2C_RDRW_IOCTL_MAX_MSGS
    struct i2c_rdwr_ioctl_data rdwr;
    int i;
    unsigned char *buf = NULL;
    int nmsgs = 0, ret = 0;
    unsigned long len;
    if(values == NULL)
    {
        return -1;
    }

    len = 1 + value_cnt; // 第一个是寄存器地址
    if(len > 0xffff)
    {
        return -1;
    }
    if(len)
    {
        buf = malloc(len);
    }
    if(buf == NULL)
    {
        return -1;
    }

    msgs[0].addr = dev_addr;
    msgs[0].flags = 0;
    msgs[0].len = len;
    msgs[0].buf = buf;
    buf[0] = reg_addr;
    for(i = 0; i < len; i++)
    {
        buf[i + 1] = values[i];
    }

    rdwr.msgs = msgs;
    rdwr.nmsgs = 1;
    int retry = 0;
    for(retry = 0; retry < max_try;retry++)
    {
        ret = ioctl(file, I2C_RDWR, &rdwr);
        if (ret < 0) {
            i2c_error("Error(%d/%d): Sending messages failed: %s\n", retry + 1, max_try, strerror(errno));
        } else if (ret < nmsgs) {
            i2c_error("Warning: only %d/%d messages were sent\n", ret, nmsgs);
        }else {
            break;
        }
        i2c_msleep(5);
    }
#if 0
    i2c_debug("i2ctransfer -f -y 11 w%x@0x%x 0x%x", len, dev_addr, reg_addr);
    for(i = 0; i < value_cnt; i++)
    {
        i2c_debug(" 0x%x", values[i]);
    }
    i2c_debug("\n");
#endif

    return ret;
}

// 打开I2C设备
int open_i2c_dev(int i2cbus, char *filename, size_t size, int quiet)
{
    int file, len;

    len = snprintf(filename, size, "/dev/i2c/%d", i2cbus);
    if (len >= (int)size) {
        i2c_error("%s: path truncated\n", filename);
        return -EOVERFLOW;
    }
    file = open(filename, O_RDWR);

    if (file < 0 && (errno == ENOENT || errno == ENOTDIR)) {
        len = snprintf(filename, size, "/dev/i2c-%d", i2cbus);
        if (len >= (int)size) {
            i2c_error("%s: path truncated\n", filename);
            return -EOVERFLOW;
        }
        file = open(filename, O_RDWR);
    }

    if (file < 0 && !quiet) {
        if (errno == ENOENT) {
            i2c_error("Error: Could not open file "
                "`/dev/i2c-%d' or `/dev/i2c/%d': %s\n",
                i2cbus, i2cbus, strerror(ENOENT));
        } else {
            i2c_error("Error: Could not open file "
                "`%s': %s\n", filename, strerror(errno));
            if (errno == EACCES)
                i2c_error("Run as root?\n");
        }
    }

    return file;
}

#if 0
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
//#include "i2cbusses.h"
//#include "util.h"
//#include "../version.h"
#include "i2c-rw.h"

int main(int argc, char *argv[])
{
    char filename[20];
    int i2cbus,  file, i;

    i2cbus = 11;

    printf("i2c-bus is %d\n", i2cbus);

    file = open_i2c_dev(i2cbus, filename, sizeof(filename), 0);
    if (file < 0 || i2c_check_funcs(file))
        exit(1);

    unsigned char debug_buf[8] = {0x11, 0x22,0x33,0x44,0x55, 0x66,0x77,0x88};

    // test 1
    i2csend_addr8_value0(file, 0x70, 0x02);

    // test 2
    i2csend_addr8_value8(file, 0x20, 0x00, 0xab);

    // test 3 ； 16位寄存器地址，8位值
    i2csend_addr16_value8(file, 0x29, 0x2d, 0xff);
    i2crecv_addr16_value8(file, 0x29, 0x2d);


    // test 4 连续读
    i2crecv_addr8_valuex(file, 0x20, 0x00, debug_buf, 8);
    for (i = 0; i < 8; i++)
        printf("read--[%d]/[%x]\n",i,debug_buf[i]);

    close(file);

    exit(0);

}
#endif
