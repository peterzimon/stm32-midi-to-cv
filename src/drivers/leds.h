#ifndef LEDS_H
#define LEDS_H

#include "stm32f1xx_hal.h"

#define LED_PORT    GPIOC
#define LED_PIN     GPIO_PIN_13

class Leds {
    public:
        GPIO_InitTypeDef GPIOInitStruct;
        
        void init(void);
};

#endif