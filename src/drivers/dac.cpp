#include "dac.h"

/**
 * DAC driver
 * Fast mode: 400kHz -> t(period) = 2.5us
 * Duty cycle | t(low) | t(high)
 * -----------------------------
 * 2:1        | 1.6us  | 0.83us
 * 
 * MCP4728 minimum clock times (Ref: DATASHEET 1.0 - I2C SERIAL TIMING SPECIFICATIONS)
 * t(low)  = 1.3us
 * t(high) = 0.6us
 * 
 * 2:1 duty cycle will work
*/

/**
 * Init I2C via HAL
*/
void DAC::init(void) {
    _I2CConfig.Instance = I2C1;
    _I2CConfig.Init.ClockSpeed = 400000;
    _I2CConfig.Init.DutyCycle = I2C_DUTYCYCLE_2;
    _I2CConfig.Init.OwnAddress1 = 0;
    _I2CConfig.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    _I2CConfig.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    _I2CConfig.Init.OwnAddress2 = 0;
    _I2CConfig.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    _I2CConfig.Init.NoStretchMode = I2C_GENERALCALL_DISABLE;
    HAL_I2C_Init(&_I2CConfig);

    _mcp4728.init(&_I2CConfig, 0x00);

    _mcp4728.reset();
    _mcp4728.wake();

    // Use internal Vref with 2x gain. Obviously this requires at 5V VDD on the MCP4728
    _mcp4728.setVref(MCP4728_VREF_INTERNAL, MCP4728_VREF_INTERNAL, MCP4728_VREF_INTERNAL, MCP4728_VREF_INTERNAL);
    _mcp4728.setGain(MCP4728_GAIN_2, MCP4728_GAIN_2, MCP4728_GAIN_2, MCP4728_GAIN_2);

    uint16_t values[4] = {0, 0, 0, 0};
    write(values);
}

/**
 * Writes values (0-4095) to MCP4728. Must be an array with 4 values.
*/
void DAC::write(uint16_t *values) {
    _mcp4728.write(values[0], values[1], values[2], values[3]);
}

void DAC::write(uint8_t channel, uint16_t value) {
    _mcp4728.write(channel, value);
}

// Related HAL Msp callback functions in system.cpp