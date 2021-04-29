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
    _listen();
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

void UI::blink(int *voices, uint8_t times, uint16_t *delay) {
    for (int i = 0; i < VOICES; i++) {
        if (voices[i]) {
            _blink[i] = times;
        } else {
            _blink[i] = 0;
        }
        _blinkLeds[i] = voices[i];
        _blinkDelay[i] = delay[i];
    }
}

void UI::_listen(void) {
    // Handle mode change
    GPIO_PinState buttonState = buttons.readButton(MODE_BUTTON_PORT, MODE_BUTTON_PIN);
    if (buttonState != _lastModeButtonState) {
        if (!buttonState) {

            /**
             * Play LED lightshow when changing modes
            */
            if (settings.mode == MONO) {
                int v[4] = {1, 1, 1, 1};
                uint16_t d[4] = {0, 200, 400, 600};
                blink(v, 1, d);
                settings.mode = CIRCULAR;
            } else if (settings.mode == CIRCULAR) {
                int v[4] = {1, 1, 1, 1};
                uint16_t d[4] = {0, 0, 0, 0};
                blink(v, 1, d);
                settings.mode = UNISON;
            } else if (settings.mode == UNISON) {
                int v[4] = {1, 1, 1, 1};
                uint16_t d[4] = {0, 0, 0, 0};
                blink(v, 5, d);
                settings.mode = POLY;
            } else if (settings.mode == POLY) {
                int v[4] = {1, 1, 1, 1};
                uint16_t d[4] = {0, 600, 600, 600};
                blink(v, 3, d);
                settings.mode = BCH;
            } else if (settings.mode == BCH) {
                int v[4] = {1, 1, 1, 1};
                uint16_t d[4] = {0, 0, 0, 600};
                blink(v, 3, d);
                settings.mode = CHL;
            } else if (settings.mode == CHL) {
                int v[4] = {1, 1, 1, 1};
                uint16_t d[4] = {200, 0, 350, 600};
                blink(v, 3, d);
                settings.mode = QUAD;
            } else if (settings.mode == QUAD) {
                int v[4] = {1, 0, 0, 0};
                uint16_t d[4] = {0, 0, 0, 0};
                blink(v, 3, d);
                settings.mode = MONO;
            }

            settings.modeDirty = true;

            _debounce = HAL_GetTick();
            _lastModeButtonState = buttonState;
        } else {
            if (HAL_GetTick() > _debounce + BUTTON_DEBOUNCE) {
                _lastModeButtonState = buttonState;
            }
        }
    }
}

void UI::_processBlink(void) {
    if (_blink[0] > 0 || _blink[1] > 0 || _blink[2] > 0 || _blink[3] > 0) {
        for (int i = 0; i < VOICES; i++) {
            if (_blinkLeds[i] && _blink[i] > 0) {
                if (!_blinkStarted[i]) {
                    _blinkStartTime[i] = HAL_GetTick() + _blinkDelay[i];
                    _blinkStarted[i] = true;
                }
                if (HAL_GetTick() > _blinkStartTime[i] + BLINK_DURATION) {
                    if (!_blinkOn[i]) {
                        ledOn(i);
                        _blinkOn[i] = true;
                    } else {
                        ledOff(i);
                        _blinkOn[i] = false;
                        _blink[i]--;
                        if (_blink[i] == 0) {
                            _blinkStarted[i] = false;
                        }
                    }
                    _blinkStartTime[i] = HAL_GetTick();
                }
            }
        }


        // if (HAL_GetTick() > _startBlink + BLINK_DURATION) {
        //     if (!_blinkOn) {
        //         for (int i = 0; i < VOICES; i++) {
        //             if (_blinkLeds[i]) {
        //                 ledOn(i);
        //             }
        //         }
        //         _blinkOn = true;
        //     } else {
        //         for (int i = 0; i < VOICES; i++) {
        //             if (_blinkLeds[i]) {
        //                 ledOff(i);
        //             }
        //         }
        //         _blinkOn = false;
        //         _blink--;
        //         if (_blink == 0) {
        //             _blinkStarted = false;
        //         }
        //     }
        //     _startBlink = HAL_GetTick();
        // }
    }
}