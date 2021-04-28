#ifndef LEDS_H
#define LEDS_H

#include "stm32f1xx_hal.h"
#include "settings.h"

class LEDs {
    public:
        GPIO_InitTypeDef GPIOInitStruct;
        
        void init(void);
        void on(uint16_t led);
        void off(uint16_t led);

    private:
        uint16_t _leds[4] = {VOICE_A_LED_PIN, VOICE_B_LED_PIN, VOICE_C_LED_PIN, VOICE_D_LED_PIN};
};

#endif