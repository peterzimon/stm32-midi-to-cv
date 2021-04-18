#ifndef CVGATE_H
#define CVGATE_H

#include <inttypes.h>
#include "settings.h"

class CVGate {
    public:
        virtual void getCVGate(uint8_t *cv, uint8_t *gate) { }
        uint16_t cvForNote(uint8_t note);
};

#endif