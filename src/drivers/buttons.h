#ifndef BUTTONS_H
#define BUTTONS_H

#include "stm32f1xx_hal.h"
#include "settings.h"

class Buttons {
    public:
        GPIO_InitTypeDef GPIOInitStruct;
        
        void init(void);
};

#endif