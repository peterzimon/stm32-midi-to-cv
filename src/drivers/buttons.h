#ifndef BUTTONS_H
#define BUTTONS_H

#include "stm32f1xx_hal.h"
#include "settings.h"
#include <inttypes.h>

class Buttons {
    public:
        GPIO_InitTypeDef GPIOInitStruct;
        
        void init(void);
        GPIO_PinState readButton(GPIO_TypeDef *port, uint16_t pin);
};

#endif