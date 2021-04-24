/**
 * Implementation of CVGate interface for mono playback. The following device
 * modes are implemented here:
 * 
 * MONO
 * ----
 * This is a simple, last note priority mono mode. Output mapping:
 * 
 * CV 1     -> pitch
 * GATE 1   -> gate
 * CV 2     -> velocity
 * CV 3     -> modwheel
 * 
 * TODO: modwheel implementation
 * 
 * CIRCULAR
 * --------
 * In circular mode, the voice (CV+Gate) output is looping like: 1>2>3>4>1...
*/

#ifndef MONO_H
#define MONO_H

#include <inttypes.h>
#include "cvgate.h"
#include "settings.h"

enum mono_mode {
    MONOMODE_DEFAULT,
    MONOMODE_CIRCULAR
};

class Mono: public CVGate {
    public:
        Mono(void);

        void setMode(mono_mode monoMode);
        void noteOff(uint8_t channel, uint8_t note, uint8_t velocity);
        void noteOn(uint8_t channel, uint8_t note, uint8_t velocity);
        void getCVGate(uint16_t *cv, int *gate);
        void debug(void);

    private:
        mono_mode _monoMode;
        uint8_t _lastcv;
        uint8_t _note;
        uint16_t _velocity;
        void _zeroAllVoices(uint16_t *cv, int *gate);
        void _reset(void);
};

#endif