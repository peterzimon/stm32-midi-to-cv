/**
 * MIDIPOLY - MIDI to CV converter 
 * -------------------------------
 * Platform: STM32F103 + PlatformIO using ST LINK V2 for programming
 * Framework: STM32CUBE
 * Hardware: MCP4728 DAC + standard MIDI Input using 6N138 optoisolator. CD4504 
 *           level shifter for gate outputs.
 * Optional: UART/TTL converter for serial debug
 * 
 * Uses Shmoergh stm32lib: https://github.com/peterzimon/stm32lib. These
 * libraries must be added to .vscode/c_cpp_properties.json includePath 
 * and browse/path.
 * 
 * Pins
 * ----
 * UART2 for serial console logging:
 *      TX --> PA2
 * 
 * UART3 for MIDI input:
 *      RX --> PB11
 * 
 * I2C1 for MCP4728:
 *      PB8 --> I2C1_SCL
 *      PB9 --> I2C1_SDA
 * 
 * GPIO for gates:
 *      PB12 --> GATE A
 *      PB13 --> GATE B
 *      PB14 --> GATE C
 *      PB15 --> GATE D
 * 
 * GPIO for debugging:
 *      PC13 --> LED
 *      PB3  --> Debug button
 * 
 * !!! HAL Msp callback functions are in drivers/system.cpp !!!
 * 
 * Playback modes
 * --------------
 * Short press the button to switch between playback modes.
 * 
 * MONO - Last note priority monophonic mode
 * CIRCULAR - CVs output notes, one at a time in a circular fashion
 * UNISON - Same CV on all 4 voices with modwheel sequential detune function
 * POLY - Last note priority 4 voice polyphonic CV out
 * BCH - 1 note bass + 3 note chords on top
 * CHL - 3 note chords on bottom + 1 note lead on top
 * 
 * Settings
 * --------
 * To enter settings, long press the button. All leds flash rapidly if in this
 * mode.
 * 
 * CALIBRATION - Calibrate output voltages. All DACs and discrete components 
 *               in the hardware are different. 
 * 
 * 
 * ~~~~
 * To be tested:
 * - all modes
 * - HAL include in MCP4728 driver should be conditioned to F1 vs F4
 * - mono velocity
 * - pitch bend is not implemented
 * - modwheel is not implemented
 * - ...global search for "TODO:" biatch!
*/
#include <string.h>
#include "stm32f1xx_hal.h"

#include "drivers/serial.h"
#include "drivers/system.h"
#include "drivers/ui.h"
#include "drivers/dac.h"
#include "drivers/gate.h"
#include "midi-handler.h"
#include "settings.h"

Serial serial;
System system;
UI ui;
MidiHandler midi;
DAC dac;
Gate gate;
Settings settings;

int main(void) {

    settings.mode = UNISON;

    system.init();
    ui.init();
    serial.init();
    midi.init();
    dac.init();
    gate.init();
    midi.attach(&dac);
    midi.attach(&gate);
    
    while (1) {
        // Read MIDI from serial
        if (serial.midiReadable()) {
            midi.saveByte(serial.midiRead());
        }

        // Process MIDI and convert to CV
        midi.process();

#ifdef SERIAL_DEBUG
        // Debug: print notes and CV values when button is pressed
        if (!HAL_GPIO_ReadPin(DEBUG_BUTTON_PORT, DEBUG_BUTTON_PIN)) {
            midi.debug();
            HAL_Delay(500); // Debounce
        }
#endif

    }
}