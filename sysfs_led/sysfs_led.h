//
// sysfd_led.h
//
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

#else

#define LED_BLUE_BRIGHTNESS_PATH  "/sys/class/leds/ledblue/brightness"          /* bit 0 */
#define LED_RED_BRIGHTNESS_PATH   "/sys/class/leds/ledred/brightness"           /* bit 1 */
#define LED_GREEN_BRIGHTNESS_PATH "/sys/class/leds/ledgreen/brightness"         /* bit 2 */

#endif

#define LED_INDEX_MAX (3)

extern int led_control_write(unsigned long value);
