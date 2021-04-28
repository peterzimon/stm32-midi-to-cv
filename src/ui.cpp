#include "ui.h"

void UI::init(void) {
    leds.init();
    buttons.init();
}

void UI::process() {

}

void UI::ledOn(uint16_t voice) {
    HAL_GPIO_WritePin(VOICE_LED_PORT, _leds[voice], GPIO_PIN_SET);
}

void UI::ledOff(uint16_t voice) {
    HAL_GPIO_WritePin(VOICE_LED_PORT, _leds[voice], GPIO_PIN_RESET);
}

void UI::updateLeds(int *gates) {
    for (int i = 0; i < VOICES; i++) {
        if (gates[i]) {
            ledOn(i);
        } else {
            ledOff(i);
        }
    }
}