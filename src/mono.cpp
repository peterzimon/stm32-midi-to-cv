#include "mono.h"
#include <utils.h>

Mono::Mono(void) {
    _reset();
}

void Mono::noteOff(uint8_t channel, uint8_t note, uint8_t velocity) {
    if (_note == note) {
        _note = 0;
        _velocity = 0;
    }
}

void Mono::noteOn(uint8_t channel, uint8_t note, uint8_t velocity) {
    if (_note == note) {
        return;
    }
    _note = note;
    _velocity = velocity;
}

void Mono::getCVGate(uint16_t *cv, int *gate) {
    cv[0] = cvForNote(_note);
    cv[1] = Utils::map(_velocity, 0, 127, 0, MAX_CV_VOLTAGE);
    gate[0] = _note ? 1 : 0;
}

void Mono::_reset(void) {
    _note = 0;
}

void Mono::debug(void) {

}