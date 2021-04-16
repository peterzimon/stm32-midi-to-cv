/**
 * Saves incoming MIDI data to a buffer and then processes. This class is inherited from
 * MidiParser and uses the parent class's parseByte() function to parse MIDI data and
 * call the appropriate handler function. Since it's a MIDI to CV application, only
 * Note On and Note Off MIDI messages are implemented.
*/

#ifndef MIDI_HANDLER_H
#define MIDI_HANDLER_H

#include <inttypes.h>
#include <midi-parser.h>
#include <ringbuffer.h>
#include "drivers/dac.h"

#define MIDI_BUFFER_SIZE 32
#define OCTAVES 4
#define LOWEST_OCTAVE 2

class MidiHandler: public MidiParser {
    public:
        void init(void);
        void attach(DAC *dac);
        void saveByte(uint8_t byte);
        void process();
        void noteOff(uint8_t channel, uint8_t note, uint8_t velocity);
        void noteOn(uint8_t channel, uint8_t note, uint8_t velocity);
    
    private:
        DAC *_dac;
        uint8_t _buffer[MIDI_BUFFER_SIZE];
        RingBuffer _inputBuffer;
        uint16_t _cvForNote(uint8_t note);
};

#endif