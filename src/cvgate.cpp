#include "cvgate.h"
#include <math.h>

// TODO: pitch bend
uint16_t CVGate::cvForNote(uint8_t note) {
    int firstMidiNote = (LOWEST_OCTAVE + 1) * 12;
    float noteForCV = (float)note - firstMidiNote;
    return (uint16_t)fmin(MAX_NOTE_VOLTAGE, fmax(0, round(noteForCV * 1000/12)));
}