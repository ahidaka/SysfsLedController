//
// sysfs_led_test.c
//
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/uaccess.h>
#include <linux/slab.h>

#include "sysfs_led.h"

static int pattern = 0;

/*
 * called when user writes the parameter
 */
static int sysfs_led_set_pattern(const char *val, const struct kernel_param *kp)
{
	pattern = simple_strtol(val, NULL, 0);
	printk(KERN_INFO "%s:set: val=%d, pattern=%02X\n", kp->name, *((int *)kp->arg), pattern);
	led_control_write(pattern);
	return 0;
}

/*
 * called when user reads the parameter
 */
static int sysfs_led_get_pattern(char *buffer, const struct kernel_param *kp) {
        printk(KERN_INFO "%s:get: val = <%d>\n", kp->name, *((int *)kp->arg));
        return(param_get_int(buffer, kp));
}

static const struct kernel_param_ops sysfs_led_ops = {
        .set    = sysfs_led_set_pattern,
        .get    = sysfs_led_get_pattern,
};

/*
 * initialize and cleanup
 */
static int sysfs_led_test_init(void) {
	printk(KERN_INFO "sysfs_led_init\n");
	return 0;
}

static void sysfs_led_test_exit(void) {
	printk(KERN_INFO "sysfs_led_exit\n");
}

module_param_cb(pattern, &sysfs_led_ops, &pattern, 0644);
MODULE_INFO(parmtype, "pattern:int");
MODULE_PARM_DESC(pattern, "Multiple LED lighting patterns: 0..0xFF (default=0)");

MODULE_DESCRIPTION("Sample Sysfs_Led Testing Module");
MODULE_ALIAS("sysfs_led_test");
MODULE_LICENSE("GPL v2");

module_init(sysfs_led_test_init);
module_exit(sysfs_led_test_exit);
