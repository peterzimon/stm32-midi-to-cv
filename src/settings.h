#ifndef SETTINGS_H
#define SETTINGS_H

#define SERIAL_DEBUG // Comment to turn off serial debugging
                     // TODO: final test without serial debug

#define OCTAVES 4
#define LOWEST_OCTAVE 2
#define VOICES 4
#define MAX_NOTE_VOLTAGE 4000
#define MAX_CV_VOLTAGE 4000

enum Mode {
    MONO,
    POLY
};

struct Settings
{
    Mode mode;
};

extern Settings settings;

#endif