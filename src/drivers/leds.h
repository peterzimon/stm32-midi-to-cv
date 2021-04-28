#ifndef LEDS_H
#define LEDS_H

#include "stm32f1xx_hal.h"
#include "settings.h"

class LEDs {
    public:
        GPIO_InitTypeDef GPIOInitStruct;
        
        void init(void);
};

#endif