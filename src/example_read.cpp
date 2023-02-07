// Copyright (c) 2023. Leonhard Baschang

#include "LM75B.h"
#include "mbed.h"

LM75B lm75(PB_9, PB_8, 0b00000000);


float temperature;


int main() {

    lm75.init();
    temperature = lm75.getTemperature();

}
