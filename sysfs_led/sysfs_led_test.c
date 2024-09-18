#include <linux/module.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/uaccess.h>
#include <linux/slab.h>

#ifdef __x86_64__

#define LED_CAPS_BRIGHTNESS_PATH "/sys/class/leds/input2::capslock/brightness"   /* bit 0 */
#define LED_SCRL_BRIGHTNESS_PATH "/sys/class/leds/input2::scrolllock/brightness" /* bit 1 */
#define LED_NUM_BRIGHTNESS_PATH  "/sys/class/leds/input2::numlock/brightness"    /* bit 2 */

static const char *class_leds_table[] = {
	LED_CAPS_BRIGHTNESS_PATH,
	LED_SCRL_BRIGHTNESS_PATH,
	LED_NUM_BRIGHTNESS_PATH,
	NULL
};

#else

#define LED_BLUE_BRIGHTNESS_PATH  "/sys/class/leds/ledblue/brightness"          /* bit 0 */
#define LED_RED_BRIGHTNESS_PATH   "/sys/class/leds/ledred/brightness"           /* bit 1 */
#define LED_GREEN_BRIGHTNESS_PATH "/sys/class/leds/ledgreen/brightness"         /* bit 2 */

static const char *class_leds_table[] = {
	LED_BLUE_BRIGHTNESS_PATH,
	LED_RED_BRIGHTNESS_PATH,
	LED_GREEN_BRIGHTNESS_PATH,
	NULL
};
#endif

#define LED_INDEX_MAX (3)

static const char *digit_strings[] = {
	"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", NULL
};

static int pattern = 0;

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


static int led_control_write(unsigned long value) {
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
 * called when user writes the parameter
 */
static int sysfs_led_set_pattern(const char *val, const struct kernel_param *kp)
{
	pattern = simple_strtol(val, NULL, 0);
	printk("%s:set: val=%d, pattern=%02X\n", kp->name, *((int *)kp->arg), pattern);
	led_control_write(pattern);
	return 0;
}

/*
 * called when user reads the parameter
 */
static int sysfs_led_get_pattern(char *buffer, const struct kernel_param *kp) {
	printk("%s:get: val = <%d>\n", kp->name, *((int *)kp->arg));
	return(param_get_int(buffer, kp));
}

static const struct kernel_param_ops sysfs_led_ops = {
        .set    = sysfs_led_set_pattern,
        .get    = sysfs_led_get_pattern,
};

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

module_param_cb(pattern, &sysfs_led_ops, &pattern, 0644);
MODULE_INFO(parmtype, "pattern:int");
MODULE_PARM_DESC(pattern, "Multiple LED lighting patterns: 0..0xFF (default=0)");

MODULE_DESCRIPTION("Sample Sysfs_Led Module");
MODULE_ALIAS("sysfs_led");
MODULE_LICENSE("GPL v2");

module_init(sysfs_led_init);
module_exit(sysfs_led_exit);
