/**
 * Saves incoming MIDI data to a buffer and then processes. This class is inherited from
 * MidiParser and uses the parent class's parseByte() function to parse MIDI data and
 * call the appropriate handler function.
*/

#ifndef MIDI_HANDLER_H
#define MIDI_HANDLER_H

#include <inttypes.h>
#include <midi-parser.h>
#include <ringbuffer.h>
#include "drivers/dac.h"
#include "drivers/gate.h"
#include "settings.h"
#include "cvgate.h"
#include "mono.h"
#include "poly.h"
#include "quad.h"
#include "ui.h"

#define MIDI_BUFFER_SIZE 32

class MidiHandler: public MidiParser {
    public:
        void init(void);
        void attach(DAC *dac);
        void attach(Gate *gate);
        void attach(UI *ui);
        void saveByte(uint8_t byte);
        void process();
        void noteOff(uint8_t channel, uint8_t note, uint8_t velocity);
        void noteOn(uint8_t channel, uint8_t note, uint8_t velocity);
        void controlChange(uint8_t channel, uint8_t data1, uint8_t data2);
        void debug(void);
    
    private:
        DAC *_dac;
        Gate *_gate;
        UI *_ui;
        uint8_t _buffer[MIDI_BUFFER_SIZE];
        RingBuffer _inputBuffer;
        Mono _mono;
        Poly _poly;
        Quad _quad;
        CVGate *_cvGate;
        int _gates[VOICES];
        uint16_t _cvs[VOICES];

        void _updateOutput(void);
};

#endif