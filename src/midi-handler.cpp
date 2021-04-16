#include <stdio.h>
#include <math.h>
#include "midi-handler.h"

void MidiHandler::init(void) {
    _inputBuffer.init(_buffer, MIDI_BUFFER_SIZE);
}

void MidiHandler::attach(DAC *dac) {
    _dac = dac;
}

void MidiHandler::saveByte(uint8_t byte) {
    _inputBuffer.writeByte(byte);
}

void MidiHandler::process() {
    while (!_inputBuffer.isEmpty()) {
        uint8_t byte = 0;
        _inputBuffer.readByte(byte);
        this->parseByte(byte);  // Parent class call
    }
}

void MidiHandler::noteOff(uint8_t channel, uint8_t note, uint8_t velocity) {
    printf("DAC: 0\r\n");
}

void MidiHandler::noteOn(uint8_t channel, uint8_t note, uint8_t velocity) {
    printf("DAC: %d\r\n", _cvForNote(note));
}

uint16_t MidiHandler::_cvForNote(uint8_t note) {
    int firstMidiNote = (LOWEST_OCTAVE + 1) * 12;
    float noteForCV = (float)note - firstMidiNote;
    // TODO: Pitch bend
    return (uint16_t)fmin(4000, fmax(0, round(noteForCV * 1000/12)));
}