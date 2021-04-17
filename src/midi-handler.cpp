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
    int8_t outputChannel = _findOutputChannel(note);
    if (outputChannel != -1) {
        _notes[outputChannel] = 0;
        _removeNoteFromHistory(note);
        _activeOutputs--;
        if (_activeOutputs >= CHANNELS || _activeOutputs < 0) {
            _activeOutputs = 0;
        }
        _updateOutput();
    }
}

void MidiHandler::noteOn(uint8_t channel, uint8_t note, uint8_t velocity) {
    switch (settings.mode) {
        case MONO:
            _notes[0] = note;
            break;

        case POLY:
            // Find next idle channel. If all channels are on, use least recently used channel
            int8_t outputChannel = _findIdleOutputChannel();
            if (outputChannel == -1) {
                uint8_t lruch = _findOutputChannel(_noteHistory[0]);    // Get least recently used output channel
                noteOff(channel, _noteHistory[0], velocity);
                _notes[lruch] = note;
            } else {
                _notes[outputChannel] = note;
            }
            break;
    }

    
    _activeOutputs++;
    if (_activeOutputs > CHANNELS || _activeOutputs < 0) {
        _activeOutputs = CHANNELS;
    }

    _noteHistory[_activeOutputs - 1] = note;

    _updateOutput();
}

void MidiHandler::debug(void) {
    printf("Channel - Note - CV\r\n");
    printf("-------------------\r\n");
    for (int i = 0; i < CHANNELS; i++) {
        printf("%d - %d - %d\r\n", i, _notes[i], _cvs[i]);
    }

    printf("History\r\n");
    for (int i = 0; i < CHANNELS; i++) {
        printf("%d\r\n", _noteHistory[i]);
    }

    printf("Active outputs: %d\r\n", _activeOutputs);
}

int8_t MidiHandler::_findIdleOutputChannel(void) {
    for (int i = 0; i < CHANNELS; i++) {
        if (!_notes[i]) {
            return i;
        }
    }
    return -1;
}

void MidiHandler::_updateOutput(void) {
    for (int i = 0; i < CHANNELS; i++) {
        _cvs[i] = _cvForNote(_notes[i]);
    }
}

int8_t MidiHandler::_findOutputChannel(uint8_t note) {
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
        _noteHistory[i] = 0;
    }
}

void MidiHandler::_removeNoteFromHistory(uint8_t note) {
    bool noteFound = false;
    for (int i = 0; i < CHANNELS; i++) {
        if (noteFound) {
            _noteHistory[i - 1] = _noteHistory[i];
            _noteHistory[i] = 0;
        }
        if (_noteHistory[i] == note) {
            noteFound = true;
        }
    }
}