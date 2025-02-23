#include "libCGPIODriver.h"

int fd;

void GPIOInit()
{
    // Open the device file
    fd = open("/dev/cgpio_driver", O_RDWR);
    if (fd < 0) {
        perror("Failed to open the device");
        return;
    }
}

void pinMode(int pinNo, int mode)
{
    struct gpio_control IO;
    IO.pin = pinNo;
    IO.value = mode;
    if (ioctl(fd, IOCTL_SET_DIRECTION, &IO) < 0) {
        perror("Failed to set direction");
        close(fd);
        return;
    }
}

void digitalWrite(int pinNo, int status)
{
    struct gpio_control IO;
    IO.pin = pinNo;
    IO.value = status;
    if (ioctl(fd, IOCTL_SET_VALUE, &IO) < 0) {
        perror("Failed to set value");
        close(fd);
        return;
    }
}

int digitalRead(int pinNo)
{
    struct gpio_control IO;
    IO.pin = pinNo;
    if (ioctl(fd, IOCTL_GET_VALUE, &IO) < 0) {
        perror("Failed to set value");
        close(fd);
        return -1;
    }
    return (IO.value);
}