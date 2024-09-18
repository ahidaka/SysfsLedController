#include <linux/module.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/uaccess.h>
#include <linux/slab.h>

#define LED_BRIGHTNESS_PATH "/sys/class/leds/ledblue/brightness"

static int write_led_brightness(const char *path, const char *value) {
    struct file *filp;
    loff_t pos = 0;
    int ret;

    printk(KERN_INFO "Enter write_led_brightness\n");
    // ファイルを開く
    filp = filp_open(path, O_WRONLY, 0);
    if (IS_ERR(filp)) {
        printk(KERN_ERR "Failed to open file: %s\n", path);
        return PTR_ERR(filp);
    }

    // ファイルにデータを書き込む
    ret = kernel_write(filp, value, strlen(value), &pos);
    if (ret < 0) {
        printk(KERN_ERR "Failed to write to file: %s\n", path);
    }

    // ファイルを閉じる
    filp_close(filp, NULL);

    return ret;
}

static int __init my_led_control_init(void) {
    printk(KERN_INFO "Changing LED brightness via sysfs\n");
    // LED の明るさを 100 に設定
    write_led_brightness(LED_BRIGHTNESS_PATH, "1");
    return 0;
}

static void __exit my_led_control_exit(void) {
    printk(KERN_INFO "LED control module unloaded\n");
}

module_init(my_led_control_init);
module_exit(my_led_control_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("Kernel module to control LED brightness via sysfs");
