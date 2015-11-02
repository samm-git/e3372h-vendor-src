#ifndef _GPIO_EXPANDER_H_
#define _GPIO_EXPANDER_H_

#ifdef __cplusplus
extern "C" { /* allow C++ to use these headers */
#endif /* __cplusplus */

#include "hi_gpio.h"

#define EXPANDER_GPIO_START GPIO_EXPANDER_BASE
#define EXPANDER_GPIO_END   GPIO_TOTAL_PINS_NUM

#ifdef CONFIG_GPIO_EXPANDER
int gpio_ex_init(void);
int gpio_ex_direction_input(unsigned int gpio);
int gpio_ex_direction_output(unsigned int gpio, unsigned int val);
int gpio_ex_get_value(unsigned int gpio);
void gpio_ex_set_value(unsigned int gpio, unsigned int val);
void gpio_ex_show(void);

#else
static inline int gpio_ex_init(void)
{
    return -1;
}
static inline int gpio_ex_direction_input(unsigned int gpio)
{
    return -1;
}
static inline int gpio_ex_direction_output(unsigned int gpio, unsigned int val)
{
    return -1;
}
static inline int gpio_ex_get_value(unsigned int gpio)
{
    return -1;
}
static inline void gpio_ex_set_value(unsigned int gpio, unsigned int val)
{
    return -1;
}
static inline void gpio_ex_show(void)
{
    return -1;
}

#endif

#ifdef __cplusplus
} /* allow C++ to use these headers */
#endif /* __cplusplus */

#endif    /* End #define _GPIO_EXPANDER_H_ */

