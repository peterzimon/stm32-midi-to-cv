/**
 * Implementation of CVGate interface for mono mode. Channel mapping 
 * in mono mode is the following:
 * 
 * CV 1     -> pitch
 * GATE 1   -> gate
 * CV 2     -> velocity
 * CV 3     -> modwheel
*/

#ifndef MONO_H
#define MONO_H

#include <inttypes.h>
#include "cvgate.h"
#include "settings.h"

class Mono: public CVGate {
    public:
        Mono(void);

        void noteOff(uint8_t channel, uint8_t note, uint8_t velocity);
        void noteOn(uint8_t channel, uint8_t note, uint8_t velocity);
        void getCVGate(uint16_t *cv, int *gate);

    private:
        uint8_t _note;
        void _reset(void);
};

#endif