#include "quad.h"

Quad::Quad(void) { }

void Quad::noteOn(uint8_t channel, uint8_t note, uint8_t velocity) {
    if (channel > (VOICES - 1) || _notes[channel] == note) { // Only handle first 4 MIDI channels 
                                                             // (actually channel value 0-3 so it maps 
                                                             // perfectly to voices )
        return;
    }
    _notes[channel] = note;
}

void Quad::noteOff(uint8_t channel, uint8_t note, uint8_t velocity) {
    if (channel > (VOICES - 1) || _notes[channel] != note) {
        return;
    }
    _notes[channel] = 0;
}

void Quad::getCVGate(uint16_t *cv, int *gate) {
    for (int voice = 0; voice < VOICES; voice++) {
        cv[voice] = cvForNote(_notes[voice]);
        gate[voice] = _notes[voice] ? 1 : 0;
    }
}

void Quad::_reset(void) {
    for (int i = 0; i < VOICES; i++) {
        _notes[i] = 0;
    }
}

void Quad::debug(void) { }