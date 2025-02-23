#ifndef __C_GPIO_DRIVER_H__
#define __C_GPIO_DRIVER_H__

#define DEVICE_NAME "cgpio_driver"
#define CLASS_NAME "cgpio"
#define MAX_PINS 32 // Maximum number of GPIO pins to manage

// Structure to pass GPIO control data
struct gpio_control {
    int pin;    // GPIO pin number
    int value;  // Direction or value (0/1)
};

// IOCTL commands
#define IOCTL_SET_DIRECTION _IOW('k', 1, struct gpio_control) // Set GPIO direction
#define IOCTL_SET_VALUE _IOW('k', 2, struct gpio_control)     // Set GPIO value
#define IOCTL_GET_VALUE _IOR('k', 3, struct gpio_control) // Get GPIO value



#endif /*__C_GPIO_DRIVER_H__*/