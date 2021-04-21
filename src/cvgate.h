/**
 * Interface class for modes (mono, poly etc.)
*/

#ifndef CVGATE_H
#define CVGATE_H

#include <inttypes.h>
#include "settings.h"

class CVGate {
    public:
        virtual void noteOff(uint8_t channel, uint8_t note, uint8_t velocity) { }
        virtual void noteOn(uint8_t channel, uint8_t note, uint8_t velocity) { }
        virtual void modWheel(uint8_t channel, uint8_t value) { }
        virtual void getCVGate(uint16_t *cv, int *gate) { }
        uint16_t pitchBend(uint32_t bend);
        uint16_t cvForNote(uint8_t note, int naiveCalibration = 0);
        void debug(void) { }
};

#endif