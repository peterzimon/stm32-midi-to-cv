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
}

/**
 * Enable MCU specific I2C pin and clock
*/
void HAL_I2C_MspInit(I2C_HandleTypeDef *i2cConfig) {
    GPIO_InitTypeDef pinConfig;
    if (i2cConfig->Instance == I2C1) {
        __HAL_RCC_GPIOB_CLK_ENABLE();

        /** 
         * I2C1 GPIO Configuration
         * PB8 --> I2C1_SCL
         * PB9 --> I2C1_SDA
        */
        pinConfig.Pin = GPIO_PIN_8|GPIO_PIN_9;
        pinConfig.Mode = GPIO_MODE_AF_OD;
        pinConfig.Speed = GPIO_SPEED_FREQ_HIGH;
        HAL_GPIO_Init(GPIOB, &pinConfig);

        __HAL_AFIO_REMAP_I2C1_ENABLE();
        __HAL_RCC_I2C1_CLK_ENABLE();
    }
}

/* ------------------------------------------------------------------------- */

/** 
 * MCP4728 driver
*/
void MCP4728::init(I2C_HandleTypeDef *I2CConfig, uint8_t deviceID) {
    _I2CConfig = I2CConfig;
    _deviceID = deviceID;
    _deviceAddress = (MCP4728_BASEADDR | _deviceID);
    _vdd = MCP4728_DEFAULT_VDD;
}

/**
 * Set VDD
*/
void MCP4728::setVDD(uint16_t vdd) {
    _vdd = vdd;
}

/**
 * General Call Reset of mcp4728 - EEPROM value will loaded to input register. 
 * Ref: DATASHEET 5.4.1
*/
void MCP4728::reset(void) {
    _generalCall(MCP4728_GENERAL_RESET);
}

/**
 * General Call Wake-Up of mcp4728 - Reset Power-Down bits (PD0,PD1 = 0,0).
 * Ref: DATASHEET 5.4.2
*/
void MCP4728::wake(void) {
    _generalCall(MCP4728_GENERAL_WAKEUP);
}

/**
 * General Call Software update of mcp4728 - All DAC ouput update.
 * Ref: DATASHEET 5.4.3
*/
void MCP4728::update(void) {
    _generalCall(MCP4728_GENERAL_UPDATE);
}

/** 
 * Write Voltage reference settings to input regiters
 *      Vref setting = 1 (internal), Gain = 0 (x1)   ==> Vref = 2.048V
 *      Vref setting = 1 (internal), Gain = 1 (x2)   ==> Vref = 4.096V
 *      Vref setting = 0 (external), Gain = ignored  ==> Vref = VDD
*/
void MCP4728::setVref(uint8_t value1, uint8_t value2, uint8_t value3, uint8_t value4) {
    _intVrefs[0] = value1;
    _intVrefs[1] = value2;
    _intVrefs[2] = value3;
    _intVrefs[3] = value4;
    _writeVref();
}

/**
 * Set a single channel's vref
*/
void MCP4728::setVref(uint8_t channel, uint8_t value) {
    _intVrefs[channel] = value;
    _writeVref();
}

/** 
 * Write gain settings to input regiters
 *      Vref setting = 1 (internal), Gain = 0 (x1)   ==> Vref = 2.048V
 *      Vref setting = 1 (internal), Gain = 1 (x2)   ==> Vref = 4.096V
 *      Vref setting = 0 (external), Gain = ignored  ==> Vref = VDD
*/
void MCP4728::setGain(uint8_t value1, uint8_t value2, uint8_t value3, uint8_t value4) {
    _gains[0] = value1;
    _gains[1] = value2;
    _gains[2] = value3;
    _gains[3] = value4;
    _writeGain();
}

/**
 * Set a single channel's gain
*/
void MCP4728::setGain(uint8_t channel, uint8_t value) {
    _gains[channel] = value;
    _writeGain();
}

/**
 * Write input register values to each channel using fastwrite method.
 * Values : 0-4095
*/
void MCP4728::write(uint16_t value1, uint16_t value2, uint16_t value3, uint16_t value4) {
    _values[0] = value1;
    _values[1] = value2;
    _values[2] = value3;
    _values[3] = value4;
    _fastWrite();
}

/**
 * Write to a specific channel using fastwrite method
 * Channel: 0-3
 * Value: 0-4095
*/
void MCP4728::write(uint8_t channel, uint16_t value) {
    _values[channel] = value;
    _fastWrite();
}

/**
 * Write voltage reference setting to input registers. 
 * Ref: DATASHEET 5.6.5
 * No EEPROM update
*/
void MCP4728::_writeVref(void) {
    uint8_t data[1];
    data[0] = (MCP4728_CMD_VREFWRITE | _intVrefs[0] << 3 | _intVrefs[1] << 2 | _intVrefs[2] << 1 | _intVrefs[3]);
    _transmit(data);
}

/**
 * Write gain setting to input registers. 
 * Ref: DATASHEET 5.6.7
 * No EEPROM update
*/
void MCP4728::_writeGain(void) {
    uint8_t data[1];
    data[0] = (MCP4728_CMD_GAINWRITE | _gains[0] << 3 | _gains[1] << 2 | _gains[2] << 1 | _gains[3]);
    _transmit(data);
}

/**
 * FastWrite input register values - All DAC ouput update. 
 * Ref: DATASHEET 5.6.1
 * DAC Input and PowerDown bits update.
 * No EEPROM update
*/
void MCP4728::_fastWrite(void) {
    uint8_t data[8];
    for (uint8_t channel = 0; channel < 4; channel++) {
        uint8_t hi = _hiByte(_values[channel]);
        uint8_t lo = _loByte(_values[channel]);
        data[channel * 2] = hi;
        data[(channel * 2) + 1] = lo;
    }
    _transmit(data);
}

/**
 * General calls
*/ 
void MCP4728::_generalCall(uint8_t command) {
    uint8_t data[1];
    data[0] = command;
    _transmit(data);
}

void MCP4728::_transmit(uint8_t *data) {
    HAL_I2C_Master_Transmit(_I2CConfig, MCP4728_GENERAL_CALL, data, 1, HAL_MAX_DELAY);
}

uint8_t MCP4728::_hiByte(uint16_t hw) {
    return (uint8_t) ((hw) & 0xFF);
}

uint8_t MCP4728::_loByte(uint16_t hw) {
    return (uint8_t) ((hw) >> 8);
}