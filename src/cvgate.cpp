#include "cvgate.h"
#include <utils.h>
#include <math.h>

CVGate::CVGate() {
    // Init calibration values
    for (int i = 0; i < VOICES; i++) {
        for (int j = 0; j < OCTAVES; j++) {
            _cal[i][j] = 0;
        }
    }
    
    /**
     * Set _cal calibration values here. How? Read the header comment in cvgate.h
     * 
     * _cal[VOICE][Cx] = RELATIVE ADJUSTMENT VALUE
     *        ^     ^              ^ 
     *        ^     ^              Use negative values to decrease the CV output
     *        ^     C note on given octave. Should output xV (C1 -> 1V, C2 -> 2V etc.) [0-10]
     *        CV output [0-3]
    */

    // Voice A
    _cal[0][1] = -18;
    _cal[0][2] = -22;
    _cal[0][3] = -23;
    _cal[0][4] = -27;
    _cal[0][5] = -28;
    _cal[0][6] = -29;
    _cal[0][7] = -32;
    _cal[0][8] = -33;
    _cal[0][8] = -33;
    _cal[0][9] = -35;
    _cal[0][10] = -35;

    // Voice B
    _cal[1][1] = -23;
    _cal[1][2] = -24;
    _cal[1][3] = -24;
    _cal[1][4] = -27;
    _cal[1][5] = -25;
    _cal[1][6] = -28;
    _cal[1][7] = -28;
    _cal[1][8] = -28;
    _cal[1][9] = -28;
    _cal[1][10] = -28;

    // Voice C
    _cal[2][1] = -9;
    _cal[2][2] = -8;
    _cal[2][3] = -5;
    _cal[2][4] = -3;
    _cal[2][5] = -1;
    _cal[2][6] = -2;
    _cal[2][7] = -3;
    _cal[2][8] = -3;
    _cal[2][9] = -3;
    _cal[2][10] = -3;

    // Voice D
    _cal[3][1] = -3;
    _cal[3][2] = -7;
    _cal[3][3] = -11;
    _cal[3][4] = -14;
    _cal[3][5] = -18;
    _cal[3][6] = -20;
    _cal[3][7] = -25;
    _cal[3][8] = -32;
    _cal[3][9] = -36;
    _cal[3][10] = -36;
}

/**
 * The output of the DAC is amplified by A = 1 + (69/47). This results in a pretty 
 * accurate amplification of 4095mV (max output voltage of DAC) to 10V. Since the
 * device covers 10 octaves we have to map 120 notes (10 octaves) to 0-4095.
*/
uint16_t CVGate::cvForNote(uint8_t note, int voice) {

    // Return calibrated CV value
    uint8_t tNote = note;
    uint8_t notes = OCTAVES * 12;
    if (tNote > LOWEST_MIDI_NOTE + notes) {
        tNote = LOWEST_MIDI_NOTE + notes;
    }
    uint8_t octave = floor(tNote / 12) + 1;
    uint16_t voltPerOctave = (uint16_t) (MAX_NOTE_VOLTAGE / OCTAVES);
    uint16_t rawCVlo = voltPerOctave * (octave - 1);
    uint16_t rawCVhi = voltPerOctave * octave;

    uint16_t rawCV = (uint16_t)Utils::map(tNote, LOWEST_MIDI_NOTE, (LOWEST_MIDI_NOTE + notes), 0, MAX_NOTE_VOLTAGE);
    uint16_t cv = (uint16_t)Utils::map(rawCV, rawCVlo, rawCVhi, (rawCVlo + _cal[voice][octave - 1]), (rawCVhi + _cal[voice][octave]));

    return cv;
    
    // uint8_t notes = (OCTAVES * 12;
    // return (uint16_t)Utils::map(note, LOWEST_MIDI_NOTE, (LOWEST_MIDI_NOTE + notes), 0, MAX_NOTE_VOLTAGE);
}

/**
 * TODO: pitch bend
 * 1. set a global bend variable 
 * 2. adjust the CV output in `cvForNote` function based on bend value. Adjusting
 *    can be done by multiplying the actual note, the min and the max note with
 *    1000 and the add the bend value(?)
*/
uint16_t CVGate::pitchBend(uint32_t bend) {
    return 0;
}