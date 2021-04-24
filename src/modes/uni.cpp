#include "uni.h"
#include <utils.h>

Uni::Uni(void) {
    _reset();
}

void Uni::noteOff(uint8_t channel, uint8_t note, uint8_t velocity) {
    if (_note == note) {
        _note = 0;
    }
}

void Uni::noteOn(uint8_t channel, uint8_t note, uint8_t velocity) {
    if (_note == note) {
        return;
    }
    _note = note;
}

void Uni::modWheel(uint8_t channel, uint8_t value) {
    _detuneValue = value;
}

void Uni::getCVGate(uint16_t *cv, int *gate) {
    uint16_t cvOut = cvForNote(_note);
    for (int i = 0; i < VOICES; i++) {
        cv[i] = cvOut + (i * DETUNE_FACTOR * _detuneValue);
    }
    gate[0] = gate[1] = gate[2] = gate[3] = _note ? 1 : 0;  // TODO: adjust this to any number of VOICES
}

void Uni::_reset(void) {
    _note = 0;
    _detuneValue = 0;
}

void Uni::debug(void) {

}