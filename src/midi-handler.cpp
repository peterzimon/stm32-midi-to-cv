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
    switch (settings.mode) {
        case MONO:
            // TODO: implement mono
            break;
        case POLY:
            poly.noteOff(channel, note, velocity);
            poly.getCVGate(_cvs, _gates);
            break;
    }
    _updateOutput();
}


void MidiHandler::noteOn(uint8_t channel, uint8_t note, uint8_t velocity) {
    switch (settings.mode) {
        case MONO:
            // _notes[0] = note; TODO: implement mono
            break;

        case POLY:
            poly.noteOn(channel, note, velocity);
            poly.getCVGate(_cvs, _gates);
            break;
    }
    _updateOutput();
}

void MidiHandler::_reset(void) {
    for (int i = 0; i < VOICES; i++) {
        _cvs[i] = 0;
        _gates[i] = 0;
    }
}

void MidiHandler::_updateOutput(void) {
    // Update DAC
}

void MidiHandler::debug(void) {
    printf("Voice - CV\r\n");
    printf("-------------------\r\n");
    for (int i = 0; i < VOICES; i++) {
        printf("%d - %d\r\n", i, _cvs[i]);
    }

    poly.debug();
}