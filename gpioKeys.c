// SPDX-License-Identifier: GPL-2.0
#include "gpioKeys.h"

static unsigned long flags = 0;

static unsigned int irqNumber;

static irqreturn_t irq_handler(int irq, void *data){
    local_irq_save(flags);
    pr_info("gkey irq triggered: %d\n", irq);
    local_irq_restore(flags);
    return IRQ_HANDLED;
}


int GPIOPin = 123;

int __init gkey_init(void)
{
    int ret;
    pr_info("Hello Key Driver Init\n");
    ret = gpio_request(GPIOPin, "key1");
    if (ret)
    {
        pr_err("Unable to request GPIOs for LEDs: %d\n", ret);
        return ret;
    }
    ret = gpio_direction_input(GPIOPin);
    if (ret)
    {
        pr_err("Failed to set GPIO %d direction\n", GPIOPin);
        goto fail_free_gpio;
    }
    ret = gpio_set_debounce(GPIOPin, 20);
    if (ret)
    {
        pr_err("Failed to set GPIO %d debounce\n", GPIOPin);
        goto fail_free_gpio;
    }
    irqNumber = gpio_to_irq(GPIOPin);
    if (irqNumber < 0)
    {
        pr_err("Failed to get IRQ number for GPIO %d\n", GPIOPin);
        ret = irqNumber;
        goto fail_free_gpio;
    }
    ret = request_irq(irqNumber,
                      irq_handler,
                      IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING,
                      "gkey_handler",
                      DEV_NAME);


    return 0;

fail_free_gpio:
    gpio_free(GPIOPin);
    return ret;
}

void __exit gkey_exit(void)
{
    pr_info("Goodbye Key Driver Exit\n");
    free_irq(irqNumber, DEV_NAME);
    gpio_free(GPIOPin);
}

module_init(gkey_init);
module_exit(gkey_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jeff");