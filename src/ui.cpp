#include "ui.h"
#include <cstring>

void UI::init(void) {
    leds.init();
    buttons.init();

    for (int i = 0; i < VOICES; i++) {
        _blinkLeds[i] = 0;
    }
}

void UI::process() {
    _processBlink();
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

void UI::blink(void) {
    _blinkOn = false;
    _blink = 5;
}

void UI::blink(int *voices, uint8_t times) {
    _blink = times;
    for (int i = 0; i < VOICES; i++) {
        _blinkLeds[i] = voices[i];
    }
}

void UI::_processBlink(void) {
    if (_blink > 0) {
        if (!_blinkStarted) {
            _startBlink = HAL_GetTick();
            _blinkStarted = true;
        }
        if (HAL_GetTick() > _startBlink + BLINK_DURATION) {
            if (!_blinkOn) {
                for (int i = 0; i < VOICES; i++) {
                    if (_blinkLeds[i]) {
                        ledOn(i);
                    }
                }
                _blinkOn = true;
            } else {
                for (int i = 0; i < VOICES; i++) {
                    if (_blinkLeds[i]) {
                        ledOff(i);
                    }
                }
                _blinkOn = false;
                _blink--;
                if (_blink == 0) {
                    _blinkStarted = false;
                }
            }
            _startBlink = HAL_GetTick();
        }
    }
}