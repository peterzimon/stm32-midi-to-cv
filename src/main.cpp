/**
 * MIDI to 4 polyphonic CV out 
 * ---------------------------
 * Platform: STM32F103 + PlatformIO using ST LINK V2 for programming
 * Framework: STM32CUBE
 * Hardware: MCP4728 DAC + standard MIDI Input using 6N138 optoisolator
 * Optional: UART/TTL converter for serial debug
 * 
 * Uses Shmoergh stm32lib: https://github.com/peterzimon/stm32lib. These
 * libraries must be added to .vscode/c_cpp_properties.json includePath 
 * and browse/path.
*/
#include <string.h>
#include "stm32f1xx_hal.h"

#include "drivers/serial.h"
#include "drivers/system.h"
#include "drivers/leds.h"
#include "drivers/dac.h"        // TODO: not implemented
#include "midi-handler.h"

Serial serial;
System system;
Leds leds;
MidiHandler poly;
DAC dac;

int main(void) {

    system.init();
    leds.init();
    serial.init();
    poly.init();
    dac.init();

    poly.attach(&dac);

    printf("Let's see y'all naked!\r\n");
    
    while (1) {
        // Read MIDI from serial
        if (serial.midiReadable()) {
            poly.saveByte(serial.midiRead());
        }

        // Process MIDI and convert to CV
        poly.process();
    }
}