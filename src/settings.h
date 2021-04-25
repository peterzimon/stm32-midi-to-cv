#ifndef SETTINGS_H
#define SETTINGS_H

#define SERIAL_DEBUG // Comment to turn off serial debugging
                     // TODO: final test without serial debug
#define VOICES 4

// Midi note mapping to CV
#define LOWEST_MIDI_NOTE 0x00
#define OCTAVES 10
#define MAX_NOTE_VOLTAGE 4095   // Max voltage to map the midi notes to. 
                                // Read comment Note in cvgate.cpp::cvForNote()

// Non-note CV
#define MAX_CV_VOLTAGE 2047 // Since the hardware amplifies the output by A = 1 + (69/47), 
                            // a max of 2047 will result in cca. +5V

enum device_mode {
    MONO,
    CIRCULAR,
    UNISON,
    POLY,
    BCH,
    CHL,
    QUAD
};

struct Settings
{
    device_mode mode;
};

extern Settings settings;

#endif