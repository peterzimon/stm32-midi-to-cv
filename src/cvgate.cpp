#include "cvgate.h"
#include <utils.h>
#include <math.h>

/**
 * The output of the DAC is amplified by A = 1 + (69/47). This results in a pretty 
 * accurate amplification of 4095mV (max output voltage of DAC) to 10V. Since the
 * device covers 10 octaves we have to map 120 notes (10 octaves) to 0-4095.
 * TODO: proper calibration
*/
uint16_t CVGate::cvForNote(uint8_t note, int naiveCalibration) {
    uint8_t notes = (OCTAVES * 12) + naiveCalibration;
    return (uint16_t)Utils::map(note, LOWEST_MIDI_NOTE, (LOWEST_MIDI_NOTE + notes), 0, MAX_NOTE_VOLTAGE);
}

/**
 * TODO: pitch bend
 * 1. set a global bend variable 
 * 2. adjust the CV output in `cvForNote` function based on bend value. Adjusting
 *    can be done by multiplying the actual note, the min and the max note with
 *    1000 and the add the bend value(?)
*/
uint16_t CVGate::pitchBend(uint32_t bend) {

}