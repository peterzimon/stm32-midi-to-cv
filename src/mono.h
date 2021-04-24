/**
 * Implementation of CVGate interface for mono playback. The following device
 * modes are implemented here:
 * 
 * MONO
 * ----
 * This is a simple, last note priority mono mode. Output mapping:
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
 * 
 * TODO: modwheel implementation
 * 
 * UNISON
 * ------
 * Unison mode plays the same note on all voices. It's cool for tuning VCOs and
 * playing fat basses. Modwheel detunes oscillators #2, #3 and #4 slightly so it
 * can even make basses fatter. Just make sure that the modwheel is all the way
 * down when tuning.
*/

#ifndef MONO_H
#define MONO_H

#include <inttypes.h>
#include "cvgate.h"
#include "settings.h"

#define DETUNE_FACTOR 0.09
#define NOTE_STACK_SIZE 10

enum mono_mode {
    MONOMODE_DEFAULT,
    MONOMODE_CIRCULAR,
    MONOMODE_UNISON
};

class Mono: public CVGate {
    public:
        Mono(void);

        void setMode(mono_mode monoMode);
        void noteOff(uint8_t channel, uint8_t note, uint8_t velocity);
        void noteOn(uint8_t channel, uint8_t note, uint8_t velocity);
        void modWheel(uint8_t channel, uint8_t value);
        void getCVGate(uint16_t *cv, int *gate);
        void debug(void);

    private:
        mono_mode _monoMode;
        uint8_t _lastcv;
        uint8_t _note;
        uint16_t _velocity;
        uint8_t _detuneValue;
        uint8_t _noteStack[NOTE_STACK_SIZE];
        void _zeroAllVoices(uint16_t *cv, int *gate);
        void _pushNote(uint8_t note);
        void _popNote(uint8_t note);
        int _findNote(uint8_t note);
        void _reset(void);
};

#endif