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
        void blink(int *voices, uint8_t times, uint16_t *delay = 0);
    
    private:
        uint32_t _debounce = 0;
        GPIO_PinState _lastModeButtonState = GPIO_PIN_SET;
        uint32_t _blinkStartTime[VOICES] = {0, 0, 0, 0};
        uint16_t _blinkDelay[VOICES] = {0, 0, 0, 0};
        uint8_t _blink[VOICES] = {0, 0, 0, 0};
        bool _blinkStarted[VOICES] = {false, false, false, false};
        bool _blinkOn[VOICES] = {false, false, false, false};
        int _blinkLeds[VOICES];


        void _listen(void);
        void _processBlink(void);
};

#endif