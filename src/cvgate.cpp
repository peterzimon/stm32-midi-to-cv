#include "cvgate.h"
#include <math.h>

uint16_t CVGate::cvForNote(uint8_t note) {
    int firstMidiNote = (LOWEST_OCTAVE + 1) * 12;
    float noteForCV = (float)note - firstMidiNote;
    return (uint16_t)fmin(4000, fmax(0, round(noteForCV * 1000/12)));
}