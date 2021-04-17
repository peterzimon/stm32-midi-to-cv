/**
 * Saves incoming MIDI data to a buffer and then processes. This class is inherited from
 * MidiParser and uses the parent class's parseByte() function to parse MIDI data and
 * call the appropriate handler function. Since it's a MIDI to CV application, only
 * note on/off and pitch bend MIDI messages are implemented.
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
#define CHANNELS 4

class MidiHandler: public MidiParser {
    public:
        void init(void);
        void attach(DAC *dac);
        void saveByte(uint8_t byte);
        void process();
        void noteOff(uint8_t channel, uint8_t note, uint8_t velocity);
        void noteOn(uint8_t channel, uint8_t note, uint8_t velocity);
        void debug(void);
    
    private:
        DAC *_dac;
        uint8_t _buffer[MIDI_BUFFER_SIZE];
        RingBuffer _inputBuffer;
        uint8_t _notes[CHANNELS];
        uint16_t _cvs[CHANNELS];
        uint8_t _noteHistory[CHANNELS];
        int _lruch = 0;  // Least recently used channel
        int _activeOutputs = 0;

        uint16_t _cvForNote(uint8_t note);
        int8_t _findOutputChannel(uint8_t note);
        int8_t _findIdleOutputChannel(void);
        void _updateOutput(void);
        void _reset(void);
        void _removeNoteFromHistory(uint8_t note);
};

#endif