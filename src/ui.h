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
        void blink(void);
        void blink(int *voices, uint8_t times);
    
    private:
        uint32_t _startBlink = 0;
        bool _blinkOn = false;
        bool _blinkStarted = false;
        uint8_t _blink = 0;
        int _blinkLeds[VOICES];

        void _processBlink(void);
};

#endif