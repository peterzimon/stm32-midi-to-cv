#ifndef CVGATE_H
#define CVGATE_H

#include <inttypes.h>
#include "settings.h"

class CVGate {
    public:
        virtual void noteOff(uint8_t channel, uint8_t note, uint8_t velocity) { }
        virtual void noteOn(uint8_t channel, uint8_t note, uint8_t velocity) { }
        virtual void getCVGate(uint16_t *cv, int *gate) { }
        uint16_t cvForNote(uint8_t note);
};

#endif