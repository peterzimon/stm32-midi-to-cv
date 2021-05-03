/**
 * Quad mode sends MIDI channel notes 1-4 on voices 1-4.
*/

#ifndef QUAD_H
#define QUAD_H

#include <inttypes.h>
#include "cvgate.h"
#include "settings.h"

class Quad: public CVGate {
    public:
        Quad(void);

        void reset(void);
        void noteOn(uint8_t channel, uint8_t note, uint8_t velocity);
        void noteOff(uint8_t channel, uint8_t note, uint8_t velocity);
        void getCVGate(uint16_t *cv, int *gate);
        void debug(void);

    private:
        uint8_t _latchNotes[VOICES];
        uint8_t _notes[VOICES];
};

#endif