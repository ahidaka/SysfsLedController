# Sysfs based general led control library and test module

## Initial load

```sh
insmod sysfs_led_lib.ko
insmod sysfs_led_test.ko
```

## Test

```sh
# echo 1 > /sys/modules/sysfs_led_test/parameters/pattern
```

The pattern value is bit array of all LEDs.

Ex.
bit 0 -- blue led
bit 1 -- red led
bit 2 -- green led

0: no leds
1: blue
2: red
3: blue, red
4: green
5: blue, green
6: red, green
7: all leds


The exported function **led_control_write(unsigned long value)** can execute the same thing.
