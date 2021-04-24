#include "mono.h"
#include <utils.h>

Mono::Mono(void) {
    _reset();
}

void Mono::setMode(mono_mode monoMode) {
    _monoMode = monoMode;
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
    _lastcv++; // Circulate cv out index
    if (_lastcv > (VOICES - 1)) {
        _lastcv = 0;
    }
}

void Mono::getCVGate(uint16_t *cv, int *gate) {
    switch (_monoMode)
    {
    case MONOMODE_CIRCULAR:
        _zeroAllVoices(cv, gate);
        cv[_lastcv] = cvForNote(_note);
        gate[_lastcv] = _note ? 1 : 0;
        break;
    
    default: // Simple mono mode
        cv[0] = cvForNote(_note);
        cv[1] = Utils::map(_velocity, 0, 127, 0, MAX_CV_VOLTAGE);
        gate[0] = _note ? 1 : 0;
        break;
    }
}

void Mono::_zeroAllVoices(uint16_t *cv, int *gate) {
    for (int i = 0; i < VOICES; i++) {
        cv[i] = 0;
        gate[i] = 0;
    }
}

void Mono::_reset(void) {
    _note = 0;
    _monoMode = MONOMODE_DEFAULT;
    _lastcv = 0;
}

void Mono::debug(void) {

}