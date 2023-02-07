// Copyright (c) 2023. Leonhard Baschang
#include "LM75b.h"

LM75B::LM75B(PinName I2C_SDA, PinName I2C_SCL, uint8_t address) {

    i2c = new I2C(I2C_SDA, I2C_SCL);
    if (address > 0b00000111) {
        address = 0b00000111;
    }
    this->address = (0b1001 << 4) | ((address & 0b00000111) << 1) | 0;
    os = new InterruptIn(NC);
}

LM75B::LM75B(PinName I2C_SDA, PinName I2C_SCL, uint8_t address, PinName osPin,
             Callback<void()> osISR) {

    i2c = new I2C(I2C_SDA, I2C_SCL);
    if (address > 0b00000111) {
        address = 0b00000111;
    }
    this->address = (0b1001 << 4) | ((address & 0b00000111) << 1) | 0;

    setCmpIntMode(true);
    setShutdownMode(false);
    setOSPolarity(true);

    os = new InterruptIn(osPin);
    os->mode(PullUp);
    os->rise(osISR);
    os->fall(osISR);
    os->enable_irq();
}

float LM75B::calc2sComplement(uint16_t data) {
    auto temp = float(static_cast<int8_t>((data >> 8) & 0xFF));
    if (data & 0x80) {
        temp = float(temp + 0.5);
    }
    return temp;
}

uint16_t LM75B::inv2sComplement(float data) {
    float castData = data;
    if (data < -0.1) {
        castData = float(data - 0.5);
    }

    uint16_t returnData = static_cast<uint8_t>(int(castData));
    returnData = (returnData << 8);

    if ((int(data * 10) % 10) != 0) {
        returnData |= 0x80;
    }

    return returnData;
}

char LM75B::readRegisterByte(uint8_t readRegister) {
    char cmd[2];
    cmd[0] = readRegister;
    i2c->read(address, cmd, 1);
    return cmd[0];
}

void LM75B::writeRegisterByte(uint8_t writeRegister, uint8_t data) {
    char cmd[2];
    cmd[0] = writeRegister;
    cmd[1] = data;

    i2c->write(address, cmd, 2);
}

void LM75B::setShutdownMode(bool shutdown) {
    char data = readRegisterByte(LM75_CONFIGURATION_REGISTER);

    if (shutdown) {
        data = data | 0x01;
    } else {
        data = data & ~(0x01);
    }

    writeRegisterByte(LM75_CONFIGURATION_REGISTER, data);
}

void LM75B::setCmpIntMode(bool cmpMode) {
    char data = readRegisterByte(LM75_CONFIGURATION_REGISTER);

    if (cmpMode) {
        data = data & (~(0x02));
    } else {
        data = data | 0x02;
    }

    writeRegisterByte(LM75_CONFIGURATION_REGISTER, data);
}

void LM75B::setOSPolarity(bool activeLow) {
    char data = readRegisterByte(LM75_CONFIGURATION_REGISTER);

    if (activeLow) {
        data = data & ~(0x04);
    } else {
        data = data | 0x04;
    }

    writeRegisterByte(LM75_CONFIGURATION_REGISTER, data);
}

int LM75B::getFaultQueue() {
    char data = readRegisterByte(LM75_CONFIGURATION_REGISTER);
    return data;
}

float LM75B::getTemperatureHYST() {
    char cmd[2];
    cmd[0] = LM75_THYST_REGISTER;

    i2c->write(address, cmd, 1);
    i2c->read(address, cmd, 2);

    uint16_t data = (cmd[0] << 8) | cmd[1];

    return calc2sComplement(data);
}

float LM75B::getTemperatureOS() {
    char cmd[2];
    cmd[0] = LM75_TOS_REGISTER;

    i2c->write(address, cmd, 1);
    i2c->read(address, cmd, 2);

    uint16_t data = (cmd[0] << 8) | cmd[1];

    return calc2sComplement(data);
}

void LM75B::setTemperatureHYST(float temp) {
    uint16_t data = inv2sComplement(temp);
    char cmd[3];
    cmd[0] = LM75_THYST_REGISTER;
    cmd[1] = (data >> 8) & 0xFF;
    cmd[2] = data & 0xFF;

    i2c->write(address, cmd, 3);
}

void LM75B::setTemperatureOS(float temp) {
    uint16_t data = inv2sComplement(temp);
    char cmd[3];
    cmd[0] = LM75_TOS_REGISTER;
    cmd[1] = (data >> 8) & 0xFF;
    cmd[2] = data & 0xFF;

    i2c->write(address, cmd, 3);
}

float LM75B::getTemperature() {
    char cmd[2];
    cmd[0] = LM75_TEMPERATURE_REGISTER;

    i2c->write(address, cmd, 1);
    i2c->read(address, cmd, 2);

    uint16_t data = (cmd[0] << 8) | cmd[1];

    return calc2sComplement(data);
}

void LM75B::init() {
    setCmpIntMode(true);
    setOSPolarity(true);
    setShutdownMode(false);
}

void LM75B::disableIRQ() { os->disable_irq(); }

void LM75B::enableIRQ() { os->enable_irq(); }
