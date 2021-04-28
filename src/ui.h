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

    private:
        uint16_t _leds[4] = {VOICE_A_LED_PIN, VOICE_B_LED_PIN, VOICE_C_LED_PIN, VOICE_D_LED_PIN};
};

#endif