#include "mono.h"
#include <utils.h>

Mono::Mono(void) {
    _reset();
}

void Mono::setMode(mono_mode monoMode) {
    _monoMode = monoMode;
}

void Mono::noteOff(uint8_t channel, uint8_t note, uint8_t velocity) {
    _popNote(note);
    _note = _noteStack[0];
    if (_note == 0) {
        _velocity = 0;
    }
}

void Mono::noteOn(uint8_t channel, uint8_t note, uint8_t velocity) {
    if (_note == note) {
        return;
    }
    _pushNote(note);
    _note = _noteStack[0];
    _velocity = velocity;

    switch (_monoMode)
    {
    case MONOMODE_DEFAULT:
        break;

    case MONOMODE_CIRCULAR:
        _lastcv++; // Circulate cv out index
        if (_lastcv > (VOICES - 1)) {
            _lastcv = 0;
        }
        break;
    
    case MONOMODE_UNISON:
        break;
    }
}

void Mono::modWheel(uint8_t channel, uint8_t value) {
    switch (_monoMode)
    {
    case MONOMODE_DEFAULT:
    case MONOMODE_CIRCULAR:
        break;

    case MONOMODE_UNISON:
        _detuneValue = value;
        break;
    }
}

void Mono::getCVGate(uint16_t *cv, int *gate) {
    uint16_t cvOut;

    switch (_monoMode)
    {
    case MONOMODE_DEFAULT:
        cv[0] = cvForNote(_note);
        cv[1] = Utils::map(_velocity, 0, 127, 0, MAX_CV_VOLTAGE);
        gate[0] = _note ? 1 : 0;
        break;

    case MONOMODE_CIRCULAR:
        _zeroAllVoices(cv, gate);
        cv[_lastcv] = cvForNote(_note);
        gate[_lastcv] = _note ? 1 : 0;
        break;
    
    case MONOMODE_UNISON:
        cvOut = cvForNote(_note);
        for (int i = 0; i < VOICES; i++) {
            cv[i] = cvOut + (i * UNISON_DETUNE_FACTOR * _detuneValue);
            gate[i] = _note ? 1 : 0;
        }
        break;
    }
}

void Mono::_zeroAllVoices(uint16_t *cv, int *gate) {
    for (int i = 0; i < VOICES; i++) {
        cv[i] = 0;
        gate[i] = 0;
    }
}

void Mono::_pushNote(uint8_t note) {
    if (_findNote(note)) {
        return;
    }
    int i = 0;
    int temp = _noteStack[i], temp2;
    while (temp) // Quit loop as soon as note is 0
    {
         // Right shift stack
        temp2 = temp;
        temp = _noteStack[i + 1];
        if (i < (NOTE_STACK_SIZE - 1)) {
            _noteStack[i + 1] = temp2;
        }
        i++;
    }
    _noteStack[0] = note;
}

void Mono::_popNote(uint8_t note) {
    bool shift = false;
    for (int i = 0; i < NOTE_STACK_SIZE; i++) {
        if (_noteStack[i] == note) {
            shift = true;
        }
        if (shift) {
            _noteStack[i] = _noteStack[i + 1];
        }
    }
}

int Mono::_findNote(uint8_t note) {
    for (int i = 0; i < NOTE_STACK_SIZE; i++) {
        if (_noteStack[i] == note) return i;
    }
    return 0;
}

void Mono::_reset(void) {
    _note = 0;
    _monoMode = MONOMODE_DEFAULT;
    _lastcv = 0;
    for (int i = 0; i < NOTE_STACK_SIZE; i++) {
        _noteStack[i] = 0;
    }
}

void Mono::debug(void) { }