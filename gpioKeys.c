// SPDX-License-Identifier: GPL-2.0
#include "gpioKeys.h"

struct gpio_keys_info gpio_keys[] = {
    {GPIO_BASE + 134, 0, "PIN21_SPI1_MISO"},
    {GPIO_BASE + 96, 0, "PIN22_GPIO17"},
    {GPIO_BASE + 133, 0, "PIN23_SPI1_CLK"},
    {GPIO_BASE + 136, 0, "PIN24_SPI1_CS0_N"}
};

const int key_counts = ARRAY_SIZE(gpio_keys);

static irqreturn_t irq_handler(int irq, void *data)
{
    unsigned long flags = 0;
    struct gpio_keys_info *info = (struct gpio_keys_info *)data;

    local_irq_save(flags);
    pr_info("gkey irq triggered: %d (%s)\n", irq, info->name);
    local_irq_restore(flags);

    return IRQ_HANDLED;
}

int register_gpio_irq(struct gpio_keys_info *info)
{
    int ret;
    ret = gpio_request(info->gpio_pin, info->name);
    if (ret)
    {
        pr_err("Unable to request GPIOs for LEDs: %d\n", ret);
        return ret;
    }
    ret = gpio_direction_input(info->gpio_pin);
    if (ret)
    {
        pr_err("Failed to set GPIO %d direction\n", info->gpio_pin);
        goto FAIL_REQUEST;
    }
    ret = gpio_set_debounce(info->gpio_pin, 20);
    if (ret)
    {
        pr_err("Failed to set GPIO %d debounce\n", info->gpio_pin);
        goto FAIL_REQUEST;
    }
    info->irq = gpio_to_irq(info->gpio_pin);
    if (info->irq < 0)
    {
        pr_err("Failed to get IRQ number for GPIO %d\n", info->gpio_pin);
        ret = info->irq;
        goto FAIL_REQUEST;
    }
    ret = request_irq(info->irq,
                      irq_handler,
                      IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING,
                      "gkey_handler",
                      info);
    return ret;
FAIL_REQUEST:
    gpio_free(info->gpio_pin);
    return ret;
}

int unregister_gpio_irq(struct gpio_keys_info *info)
{
    free_irq(info->irq, info);
    gpio_free(info->gpio_pin);
    return 0;
}

int __init gkey_init(void)
{
    int ret;
    int i;
    pr_info("Hello Key Driver Init\n");
    pr_info("Loading %d GPIO keys\n", key_counts);
    for (i = 0; i < key_counts; i++)
    {
        ret = register_gpio_irq(&gpio_keys[i]);
        if (ret)
        {
            pr_err("Failed to register GPIO key %s\n", gpio_keys[i].name);
            goto REGISTER_FAIL;
        }
        pr_info("Registered GPIO key %s on GPIO pin %d with IRQ %d\n",
                gpio_keys[i].name,
                gpio_keys[i].gpio_pin,
                gpio_keys[i].irq);
    }
    ret = 0;
    return ret;
REGISTER_FAIL:
    while (i > 0){
        i--;
        unregister_gpio_irq(&gpio_keys[i]);
    }
    return ret;
}

void __exit gkey_exit(void)
{
    pr_info("Goodbye Key Driver Exit\n");
    for (int i = 0; i < key_counts; i++)
        unregister_gpio_irq(&gpio_keys[i]);
}

module_init(gkey_init);
module_exit(gkey_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jeff");