#include "cvgate.h"
#include <utils.h>
#include <math.h>

/**
 * The output of the DAC is amplified by A = 1 + (69/47). This results in a pretty 
 * accurate amplification of 4095mV (max output voltage of DAC) to 10V. Since the
 * device covers 10 octaves we have to map 120 notes (10 octaves) to 0-4095.
 * TODO: pitch bend
*/
uint16_t CVGate::cvForNote(uint8_t note) {
    uint8_t notes = OCTAVES * 12;
    return (uint16_t)Utils::map(note, LOWEST_MIDI_NOTE, (LOWEST_MIDI_NOTE + notes), 0, MAX_NOTE_VOLTAGE);
}