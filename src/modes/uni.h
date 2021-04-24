/**
 * Implementation of CVGate interface for Unison mode. In this mode all the 
 * CV voices output the same voltage and the modwheel detunes them slightly
 * compared to each other.
*/

#ifndef UNI_H
#define UNI_H

#include <inttypes.h>
#include "cvgate.h"
#include "settings.h"

#define DETUNE_FACTOR 0.15

class Uni: public CVGate {
    public:
        Uni(void);

        void noteOff(uint8_t channel, uint8_t note, uint8_t velocity);
        void noteOn(uint8_t channel, uint8_t note, uint8_t velocity);
        void modWheel(uint8_t channel, uint8_t value);
        void getCVGate(uint16_t *cv, int *gate);
        void debug(void);

    private:
        uint8_t _note;
        uint8_t _detuneValue;
        void _reset(void);
};

#endif