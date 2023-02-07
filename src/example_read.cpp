// Copyright (c) 2023. Leonhard Baschang

#include "LM75B.h"
#include "mbed.h"

LM75B lm75_read(PB_9, PB_8, 0b00000000);


float temperature;


void main_read_example() {

    lm75_read.init();
    temperature = lm75_read.getTemperature();

}
