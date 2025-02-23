#include <linux/module.h>
#include <linux/init.h>
#include <linux/gpio.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/cdev.h>
#include <linux/ioctl.h>
#include <linux/slab.h> // For kmalloc/kfree
#include "CGPIODriver.h"

static int majorNumber;
static struct class* gpioClass = NULL;
static struct device* gpioDevice = NULL;
static bool gpio_used[MAX_PINS] = {false}; // Track which GPIO pins are in use

static int dev_open(struct inode *inodep, struct file *filep) {
    printk(KERN_INFO "GPIO Driver: open()\n");
    return 0;
}

static int dev_release(struct inode *inodep, struct file *filep) {
    printk(KERN_INFO "GPIO Driver: close()\n");
    return 0;
}

static long dev_ioctl(struct file *filep, unsigned int cmd, unsigned long arg) {
    struct gpio_control ctrl;

    // Copy control data from user space
    if (copy_from_user(&ctrl, (struct gpio_control *)arg, sizeof(ctrl))) {
        return -EFAULT;
    }

    // Validate GPIO pin number
    if (ctrl.pin < 0 || ctrl.pin >= MAX_PINS) {
        printk(KERN_INFO "GPIO Driver: Invalid GPIO pin %d\n", ctrl.pin);
        return -EINVAL;
    }

    switch (cmd) {
        case IOCTL_SET_DIRECTION:
            if (ctrl.value == 0) {
                gpio_direction_input(ctrl.pin); // Set as input
                printk(KERN_INFO "GPIO Driver: Pin %d set as INPUT\n", ctrl.pin);
            } else if (ctrl.value == 1) {
                gpio_direction_output(ctrl.pin, 0); // Set as output, default LOW
                printk(KERN_INFO "GPIO Driver: Pin %d set as OUTPUT\n", ctrl.pin);
            } else {
                printk(KERN_INFO "GPIO Driver: Invalid direction value\n");
                return -EINVAL;
            }
            break;

        case IOCTL_SET_VALUE:
            if (ctrl.value == 0) {
                gpio_set_value(ctrl.pin, 0); // Set GPIO LOW
                printk(KERN_INFO "GPIO Driver: Pin %d set to LOW\n", ctrl.pin);
            } else if (ctrl.value == 1) {
                gpio_set_value(ctrl.pin, 1); // Set GPIO HIGH
                printk(KERN_INFO "GPIO Driver: Pin %d set to HIGH\n", ctrl.pin);
            } else {
                printk(KERN_INFO "GPIO Driver: Invalid value\n");
                return -EINVAL;
            }
            break;
        case IOCTL_GET_VALUE:
            // Read the value of the specified GPIO pin
            ctrl.value = gpio_get_value(ctrl.pin);
            printk(KERN_INFO "GPIO Driver: Pin %d read as %d\n", ctrl.pin, ctrl.value);

            // Copy the result back to user space
            if (copy_to_user((struct gpio_control *)arg, &ctrl, sizeof(ctrl))) {
                return -EFAULT;
            }
            break;

        default:
            printk(KERN_INFO "GPIO Driver: Invalid IOCTL command\n");
            return -EINVAL;
    }

    return 0;
}

static struct file_operations fops = {
    .open = dev_open,
    .release = dev_release,
    .unlocked_ioctl = dev_ioctl,
};

static int __init gpio_driver_init(void) {
    printk(KERN_INFO "GPIO Driver: Initializing the GPIO Driver\n");

    // Dynamically allocate a major number for the device
    majorNumber = register_chrdev(0, DEVICE_NAME, &fops);
    if (majorNumber < 0) {
        printk(KERN_ALERT "GPIO Driver failed to register a major number\n");
        return majorNumber;
    }
    printk(KERN_INFO "GPIO Driver: registered correctly with major number %d\n", majorNumber);

    // Register the device class
    gpioClass = class_create(THIS_MODULE, CLASS_NAME);
    if (IS_ERR(gpioClass)) {
        unregister_chrdev(majorNumber, DEVICE_NAME);
        printk(KERN_ALERT "Failed to register device class\n");
        return PTR_ERR(gpioClass);
    }
    printk(KERN_INFO "GPIO Driver: device class registered\n");

    // Register the device driver
    gpioDevice = device_create(gpioClass, NULL, MKDEV(majorNumber, 0), NULL, DEVICE_NAME);
    if (IS_ERR(gpioDevice)) {
        class_destroy(gpioClass);
        unregister_chrdev(majorNumber, DEVICE_NAME);
        printk(KERN_ALERT "Failed to create the device\n");
        return PTR_ERR(gpioDevice);
    }
    printk(KERN_INFO "GPIO Driver: device class created\n");

    printk(KERN_INFO "GPIO Driver: Initialized successfully\n");
    return 0;
}

static void __exit gpio_driver_exit(void) {
    // Free all GPIO pins that were used
    for (int i = 0; i < MAX_PINS; i++) {
        if (gpio_used[i]) {
            gpio_free(i);
            printk(KERN_INFO "GPIO Driver: Freed GPIO pin %d\n", i);
        }
    }

    device_destroy(gpioClass, MKDEV(majorNumber, 0)); // Remove the device
    class_unregister(gpioClass); // Unregister the device class
    class_destroy(gpioClass); // Remove the device class
    unregister_chrdev(majorNumber, DEVICE_NAME); // Unregister the major number
    printk(KERN_INFO "GPIO Driver: Exiting\n");
}

module_init(gpio_driver_init);
module_exit(gpio_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Deep Padmani");
MODULE_DESCRIPTION("A generic GPIO driver for Raspberry Pi Zero W");
MODULE_VERSION("1.0");