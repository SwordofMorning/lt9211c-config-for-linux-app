/*
 * @brief :
 * @date :  2021-11-xx
 * @version : v1.0.0
 * @copyright(c) 2020 : OptoMedic company Co.,Ltd. All rights reserved
 * @Change Logs:
 * @date         author         notes:
 */
#ifndef __GPIO_H__
#define __GPIO_H__

#include "stdint.h"

enum {
    GPIO_DIR_OUTPUT = 0,
    GPIO_DIR_INPUT,
};
typedef uint8_t gpio_dir_t;

enum {
    ACTIVE_LOW = 0,
    ACTIVE_HIGH = 1,
};
typedef uint8_t active_level_t;

/* irq */
enum {
    EDGE_NONE = 0,
    EDGE_RISING,
    EDGE_FALLING,
    EDGE_BOTH,
};
typedef uint8_t edge_t;

int gpio_export(int gpio);
int gpio_unexport(int gpio);

int gpio_set_active(int gpio, active_level_t active);
int gpio_active_low(int gpio);
int gpio_active_high(int gpio);

int gpio_set_edge(int gpio, edge_t edge);
int gpio_edge_none(int gpio);
int gpio_edge_rising(int gpio);
int gpio_edge_falling(int gpio);
int gpio_edge_both(int gpio);

int gpio_set_direction(int gpio, gpio_dir_t dir);
int gpio_direction_input(int gpio);
int gpio_direction_output(int gpio);

void gpio_set_value(int gpio, int value);
int gpio_get_value(int gpio);

#endif /* __GPIO_H__ */

