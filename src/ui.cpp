#include "ui.h"

void UI::init(void) {
    leds.init();
    buttons.init();
}

void UI::process() {
    
}

void UI::ledOn(uint16_t voice) {
    leds.on(voice);
}

void UI::ledOff(uint16_t voice) {
    leds.off(voice);
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