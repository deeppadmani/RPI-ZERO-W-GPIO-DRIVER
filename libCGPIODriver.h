#ifndef __LIB_GPIO_DRIVER_H__
#define __LIB_GPIO_DRIVER_H__

#include "CGPIODriver.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

void GPIOInit();
void pinMode(int pinNo, int mode);
void digitalWrite(int pinNo, int status);
int digitalRead(int pinNo);

#endif /*__LIB_GPIO_DRIVER_H__*/