// Copyright (c) 2023. Leonhard Baschang
#include "LM75b.h"
#include "mbed.h"


void osIsr();

LM75B lm75(PB_9, PB_8, 0b00000000, PC_8, &osIsr);
DigitalOut led(LED1);


void osIsr() {
    led = !led;
}


int main() {
    led = 0;
    lm75.setTemperatureOS(26);
    lm75.setTemperatureHYST(23);
}
