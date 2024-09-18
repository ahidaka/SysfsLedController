//
// sysfs_led_lib.c
//
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/uaccess.h>
#include <linux/slab.h>

#include "sysfs_led.h"

#ifdef __x86_64__
static const char *class_leds_table[] = {
	LED_CAPS_BRIGHTNESS_PATH,
	LED_SCRL_BRIGHTNESS_PATH,
	LED_NUM_BRIGHTNESS_PATH,
	NULL
};

#else
static const char *class_leds_table[] = {
	LED_BLUE_BRIGHTNESS_PATH,
	LED_RED_BRIGHTNESS_PATH,
	LED_GREEN_BRIGHTNESS_PATH,
	NULL
};
#endif

static const char *digit_strings[] = {
	"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", NULL
};

static int write_led_brightness(int led_index, unsigned long value);

/*
 * LED control for sysgs
 */
static int write_led_brightness(int led_index, unsigned long value) {
	struct file *filp;
	loff_t pos = 0;
	int ret;
	char *digit;

	printk(KERN_INFO "write_led:%s index=%d value=%lu",
	       class_leds_table[led_index], led_index, value);

	// debug
	filp = filp_open(class_leds_table[led_index], O_WRONLY, 0);
	if (IS_ERR(filp)) {
		printk(KERN_ERR "Failed to open file: <%s>=%ld\n",
		       class_leds_table[led_index], PTR_ERR(filp));
		return PTR_ERR(filp);
	}

	digit = (char *) digit_strings[value];
	ret = kernel_write(filp, digit, strlen(digit), &pos);
	if (ret < 0) {
		printk(KERN_ERR "Failed to write file value: %s\n", digit);
	}
	filp_close(filp, NULL);

	return ret;
}

/*
 *
 */
int led_control_write(unsigned long value) {
	int i;
	int bit;
	const unsigned int mask = 1;

	printk(KERN_INFO "Changing LED brightness via sysfs:%lu\n", value);
	for(i = 0; i < LED_INDEX_MAX; i++) {
		bit = value & mask;
		// set the value
		write_led_brightness(i, bit);
		value >>= 1;
	}
	return 0;
}

/*
 * initialize and cleanup
 */
static int sysfs_led_init(void) {
	printk(KERN_INFO "sysfs_led_init\n");
	return 0;
}

static void sysfs_led_exit(void) {
	printk(KERN_INFO "sysfs_led_exit\n");
}

EXPORT_SYMBOL_GPL(led_control_write);

MODULE_DESCRIPTION("Sample Sysfs_Led Module");
MODULE_ALIAS("sysfs_led");
MODULE_LICENSE("GPL v2");

module_init(sysfs_led_init);
module_exit(sysfs_led_exit);
