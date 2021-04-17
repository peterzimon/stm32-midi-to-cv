#ifndef LEDS_H
#define LEDS_H

#include "stm32f1xx_hal.h"

#define LED_PORT    GPIOC
#define LED_PIN     GPIO_PIN_13

#define DEBUG_BUTTON_PORT GPIOB
#define DEBUG_BUTTON_PIN GPIO_PIN_3

class UI {
    public:
        GPIO_InitTypeDef GPIOInitStruct;
        
        void init(void);
};

#endif