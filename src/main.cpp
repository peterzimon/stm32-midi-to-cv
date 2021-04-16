/**
 * MIDI to 4 polyphonic CV out on STM32F103C8 using HAL libraries (mostly).
 * Uses Shmoergh stm32lib: https://github.com/peterzimon/stm32lib
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
    
    HAL_Init();

    system.init();
    leds.init();
    serial.init();
    poly.init();
    dac.init();

    // Attach handlers
    // TODO: think about this. The whole thing could be done with one static class? Not sure...
    poly.attach(&dac); // Attach dac driver to MIDI to CV handler

    printf("Let's see y'all naked!\r\n");
    
    // Main loop
    while (1) {

        // Read MIDI from serial
        if (serial.midiReadable()) {
            poly.saveByte(serial.midiRead());
        }

        // Process MIDI and convert to CV
        poly.process();
    }
}