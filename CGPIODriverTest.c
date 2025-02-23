
#include "libCGPIODriver.h"
#define LED 25
#define BTN 17

int main()
{
    GPIOInit();
    pinMode(LED,1);
    pinMode(BTN,0);

    while(1)
    {
        if(digitalRead(BTN) == 1)
            digitalWrite(LED,1);
        else
            digitalWrite(LED,0);
    
    }
}

#if 0
int main() {
    int fd;
    struct gpio_control led;
    struct gpio_control btn;

    // Open the device file
    fd = open("/dev/cgpio_driver", O_RDWR);
    if (fd < 0) {
        perror("Failed to open the device");
        return -1;
    }


    // Set GPIO pin 25 as output
    led.pin = 25;
    led.value = 1; // 1 = output

    // Set GPIO pin 17 as output
    btn.pin = 17;
    btn.value = 0; // 0 = input
    if (ioctl(fd, IOCTL_SET_DIRECTION, &led) < 0) {
        perror("Failed to set direction");
        close(fd);
        return -1;
    }

    if (ioctl(fd, IOCTL_SET_DIRECTION, &btn) < 0) {
        perror("Failed to set btn direction");
        close(fd);
        return -1;
    }

    // Set GPIO pin 4 to HIGH
    led.value = 0; // 1 = HIGH
    if (ioctl(fd, IOCTL_SET_VALUE, &led) < 0) {
        perror("Failed to set value");
        close(fd);
        return -1;
    }

    while(1)
    {
        if(ioctl(fd, IOCTL_GET_VALUE, &btn) < 0)
        {
            perror("Failed to set value");
            close(fd);
            return -1;
        }

        if(btn.value == 1)
            led.value = 1;
        else
            led.value = 0;
        ioctl(fd, IOCTL_SET_VALUE, &led);
    }

    // Clean up
    close(fd);
    return 0;
}

#endif