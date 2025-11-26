// SPDX-License-Identifier: GPL-2.0
#include <linux/init.h>
#include <linux/gpio.h>
#include <linux/gpio/consumer.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/interrupt.h>
#define DEV_NAME "Front Key Driver"

#ifdef pr_fmt
#undef pr_fmt
#endif
#define pr_fmt(fmt) "%s[%s]: " fmt, KBUILD_MODNAME, __func__

struct gpio_keys_info {
    unsigned int gpio;
    unsigned int irq;
    const char *name;
};

const unsigned int GPIO0_BASE = 348;
const unsigned int GPIO1_BASE = 348;
const struct gpio_keys_info gpio_keys[] = {
    {GPIO0_BASE + 134, 0, "PIN21_SPI1_MISO"},
    {GPIO0_BASE + 96, 0, "PIN22_GPIO17"},
    {GPIO0_BASE + 133, 0, "PIN23_SPI1_CLK"},
    {GPIO0_BASE + 136, 0, "PIN24_SPI1_CS0_N"},
};
