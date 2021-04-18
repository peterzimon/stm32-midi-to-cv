#include <stdio.h>
#include <math.h>
#include "midi-handler.h"

void MidiHandler::init(void) {
    _inputBuffer.init(_buffer, MIDI_BUFFER_SIZE);

    for (int i = 0; i < VOICES; i++) {
        _cvs[i] = 0;
        _gates[i] = 0;
    }

    // The _cvGate pointer will be set to the appropriate implementation of 
    // the CVGate interface. With this, it's quite easy to add implementations
    // for new modes.
    switch (settings.mode) {
        case MONO:
            _cvGate = &_mono;
            break;
        case POLY:
            _cvGate = &_poly;
            break;
    }
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
    _cvGate->noteOff(channel, note, velocity);
    _cvGate->getCVGate(_cvs, _gates);
    _updateOutput();
}

void MidiHandler::noteOn(uint8_t channel, uint8_t note, uint8_t velocity) {
    _cvGate->noteOn(channel, note, velocity);
    _cvGate->getCVGate(_cvs, _gates);
    _updateOutput();
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

    // poly.debug();
}