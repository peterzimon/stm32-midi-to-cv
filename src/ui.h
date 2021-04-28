#ifndef UI_H
#define UI_H

#include <inttypes.h>
#include "stm32f1xx_hal.h"
#include "settings.h"
#include "drivers/leds.h"
#include "drivers/buttons.h"

class UI {
    public:
        LEDs leds;
        Buttons buttons;

        void init(void);
        void process(void);
        void ledOn(uint16_t voice);
        void ledOff(uint16_t voice);
        void updateLeds(int *gates);
};

#endif