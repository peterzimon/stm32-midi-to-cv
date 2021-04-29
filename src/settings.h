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

// Buttons
#define MODE_BUTTON_PORT        GPIOB
#define MODE_BUTTON_PIN         GPIO_PIN_0
#define DEBUG_BUTTON_PORT       GPIOB
#define DEBUG_BUTTON_PIN        GPIO_PIN_3

// LEDs
#define VOICE_LED_PORT          GPIOA
#define VOICE_A_LED_PIN         GPIO_PIN_4
#define VOICE_B_LED_PIN         GPIO_PIN_5
#define VOICE_C_LED_PIN         GPIO_PIN_6
#define VOICE_D_LED_PIN         GPIO_PIN_7
#define BUILT_IN_LED_PORT       GPIOC
#define BUILT_IN_LED_PIN        GPIO_PIN_13

// Gate GPIO pins
#define GATE_PORT               GPIOB
#define GATE_A                  GPIO_PIN_12
#define GATE_B                  GPIO_PIN_13
#define GATE_C                  GPIO_PIN_14
#define GATE_D                  GPIO_PIN_15

// UI constants
#define BLINK_DURATION          100
#define BUTTON_DEBOUNCE         250

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
    bool modeDirty; // Used for flagging if settings been updated
};

extern Settings settings;

#endif