// Copyright (c) 2023. Leonhard Baschang

#ifndef UNTITLED7_LM75B_H
#define UNTITLED7_LM75B_H

#include "mbed.h"

#define LM75_TEMPERATURE_REGISTER 0b00000000
#define LM75_CONFIGURATION_REGISTER 0b00000001
#define LM75_THYST_REGISTER 0b00000010
#define LM75_TOS_REGISTER 0b00000011

class LM75B {
    I2C *i2c;
    uint8_t address;

    InterruptIn *os;

private:
    static float calc2sComplement(::uint16_t data);

    static uint16_t inv2sComplement(float data);

    char readRegisterByte(uint8_t readRegister);


    void writeRegisterByte(uint8_t writeRegister, uint8_t data);

public:
    LM75B(PinName I2C_SDA, PinName I2C_SCL, uint8_t address);

    LM75B(PinName I2C_SDA, PinName I2C_SCL, uint8_t address, PinName osPin, Callback<void()> osISR);


    void init();

    void setShutdownMode(bool shutdown);

    void setCmpIntMode(bool cmpMode);

    void setOSPolarity(bool activeLow);

    int getFaultQueue();

    float getTemperatureHYST();

    float getTemperatureOS();

    void setTemperatureHYST(float temp);

    void setTemperatureOS(float temp);

    float getTemperature();

    void enableIRQ();

    void disableIRQ();
};


#endif//UNTITLED7_LM75B_H
