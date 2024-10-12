#include "gpio.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

/*
gpio 编号计算
    index = GPIOn_IOx = (n-1)*32 + x
    例如蜂鸣器使用的引脚编号为：index = GPIO1_19 = (1-1)*32 +19 = 19。
    又例如GPIO4_IO20的编号为：index = GPIO4_IO20 = (4-1)*32+20=116。
*/
int gpio_export(int gpio)
{
    int fd;
    char gpio_path[100] = {0};

   /* GPIO目录路径 */
    sprintf(gpio_path, "/sys/class/gpio/gpio%d", gpio);
    /* 判断GPIO目录是否存在 */
    if (access(gpio_path, F_OK) == 0) return 0;

    if (0 > (fd = open("/sys/class/gpio/export", O_WRONLY)))
    {
        perror("open error");
        return -1;
    }

    /* 导出 gpio */
    char gpio_str[10] = {0};
    sprintf(gpio_str, "%d", gpio);
    int len = strlen(gpio_str);
    if (len != write(fd, gpio_str, len))
    {
        perror("write error");
        return -2;
    }

    return 0;
}

int gpio_unexport(int gpio)
{
    int fd;
    fd = open("/sys/class/gpio/unexport", O_WRONLY);
    if(fd < 0)
        return -1;

    char gpio_str[10] = {0};
    sprintf(gpio_str, "%d", gpio);
    write(fd, gpio_str, strlen(gpio_str));
    close(fd);
    return 0;
}

static int gpio_cfg_attr(int gpio, char *attr,  char *val)
{
    int fd;
    char path[100] = {0};
    sprintf(path, "/sys/class/gpio/gpio%d/%s", gpio, attr);
    if (0 > (fd = open(path, O_WRONLY)))
    {
        perror("open error");
        return -1;
    }

    int len = strlen(val);
    if (len != write(fd, val, len))
    {
        perror("write error");
        close(fd);
        return -2;
    }

    close(fd);
    return 0;
}

int gpio_set_direction(int gpio, gpio_dir_t dir)
{
    char dirs[10] = {0};
    if (dir == GPIO_DIR_OUTPUT)
    {
        sprintf(dirs, "%s", "out");
    }
    else
    {
        sprintf(dirs, "%s", "in");
    }
    return gpio_cfg_attr(gpio, "direction", dirs);
}

int gpio_direction_output(int gpio)
{
    return gpio_set_direction(gpio, GPIO_DIR_OUTPUT);
}

int gpio_direction_input(int gpio)
{
    return gpio_set_direction(gpio, GPIO_DIR_INPUT);
}

int gpio_set_active(int gpio, active_level_t active)
{
    char active_str[10] = {0};

    if (active == ACTIVE_LOW)
    {
        gpio_cfg_attr(gpio, "active_low", "0");
    }
    else
    {
        gpio_cfg_attr(gpio, "active_low", "1");
    }
}

int gpio_active_low(int gpio)
{
    return gpio_cfg_attr(gpio, "active_low", "0");
}

int gpio_active_high(int gpio)
{
    return gpio_cfg_attr(gpio, "active_low", "1");
}

int gpio_set_edge(int gpio, edge_t edge)
{
    int ret;
    edge_t edge_index[4] = {EDGE_NONE, EDGE_RISING, EDGE_FALLING, EDGE_BOTH};
    char str_edge_index[4][10] = {{"none"}, {"rising"}, {"falling"}, {"both"}};
    for (int i = 0; i < 4; i++)
    {
        if (edge == edge_index[i])
        {
            ret = gpio_cfg_attr(gpio, "edge", str_edge_index[i]);
            break;
        }
    }
    return ret;
}

int gpio_edge_none(int gpio)
{
    return gpio_set_edge(gpio, EDGE_NONE);
}

int gpio_edge_rising(int gpio)
{
    return gpio_set_edge(gpio, EDGE_RISING);
}

int gpio_edge_falling(int gpio)
{
    return gpio_set_edge(gpio, EDGE_FALLING);
}

int gpio_edge_both(int gpio)
{
    return gpio_set_edge(gpio, EDGE_BOTH);
}


void gpio_set_value(int gpio, int value)
{
    char val[10] = {0};
    if (value == 0)
    {
        sprintf(val, "%d", 0);
    }
    else
    {
        sprintf(val, "%d", 1);
    }

    gpio_cfg_attr(gpio, "value", val);
}

int gpio_get_value(int gpio)
{
    char gpio_path[100] = {0};
    char vals[5] = {0};
    int fd;

    sprintf(gpio_path, "/sys/class/gpio/gpio%d/value", gpio);
    if (0 > (fd = open(gpio_path, O_WRONLY)))
    {
        perror("open error");
        return -1;
    }

    if (0 > read(fd, vals, sizeof(vals)))
    {
        perror("read error");
        close(fd);
        return -1;
    }

    int value = atoi(vals);

    return value;
}
