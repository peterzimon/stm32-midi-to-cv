#include <stdio.h>
#include <math.h>
#include "midi-handler.h"
#include "settings.h"

void MidiHandler::init(void) {
    _inputBuffer.init(_buffer, MIDI_BUFFER_SIZE);
    _reset();
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
    switch (settings.mode)
    {
    case MONO:
        if (_findNote(note) != -1) {
            _notes[0] = 0;
        }
        break;

    case POLY:
        
        break;
    }
    _updateOutput();
}

void MidiHandler::noteOn(uint8_t channel, uint8_t note, uint8_t velocity) {
    switch (settings.mode)
    {
    case MONO:
        _notes[0] = note;
        break;

    case POLY:
        
        break;
    }
    _updateOutput();
}

void MidiHandler::_updateOutput(void) {
    for (int i = 0; i < CHANNELS; i++) {
        _cvs[i] = _cvForNote(_notes[i]);
    }
    _debug();
}

int8_t MidiHandler::_findNote(uint8_t note) {
    for (int i = 0; i < CHANNELS; i++) {
        if (_notes[i] == note) return i;
    }
    return -1;
}

// TODO: Pitch bend
uint16_t MidiHandler::_cvForNote(uint8_t note) {
    int firstMidiNote = (LOWEST_OCTAVE + 1) * 12;
    float noteForCV = (float)note - firstMidiNote;
    return (uint16_t)fmin(4000, fmax(0, round(noteForCV * 1000/12)));
}

void MidiHandler::_reset(void) {
    for (int i = 0; i < CHANNELS; i++) {
        _notes[i] = 0;
        _cvs[i] = 0;
    }
}

void MidiHandler::_debug(void) {
    printf("Channel - Note - CV\r\n");
    printf("-------------------\r\n");
    for (int i = 0; i < CHANNELS; i++) {
        printf("%d - %d - %d\r\n", i, _notes[i], _cvs[i]);
    }
}