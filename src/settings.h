#ifndef SETTINGS_H
#define SETTINGS_H

#define SERIAL_DEBUG // Comment to turn off serial debugging
                     // TODO: final test when no serial debug

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