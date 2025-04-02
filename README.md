# SysfsLedController
Control LEDs with sysfs linux kernel module

This Linux loadable module provides general-purpose processing invented by ChatGPT to control the LED driver through kernel code. The processing is divided into a library for controlling the LED via sysfs and a user interface using the loadable module.

Notes:Background to the creation of this module.

This module was originally devised as an additional assignment for participants in CQ Publishing's i.MX8ULP board hands-on seminar, with the purpose of lighting up an LED when data transfer via GeneralIoTModules is complete.
However, it can also be used to control the LED on the keyboard in PC-Linux.
