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

const unsigned int GPIO_BASE = 348;

struct gpio_keys_info {
    unsigned int gpio_pin;
    int irq;
    const char *name;
};

