// Copyright (c) 2023. Leonhard Baschang
#include "LM75B.h"
#include "mbed.h"


static BufferedSerial serial_port(USBTX, USBRX);

void printTemp(float temp) {
    char buffer[32];
    sprintf(buffer, "%d\n", int(temp * 10));


    serial_port.write(buffer, 32);
}

void osIsr();

LM75B lm75(PB_9, PB_8, 0b00000000, PC_8, &osIsr);
DigitalOut led(LED1);


void osIsr() {
    led = !led;
}


int main() {
    led = 0;
    serial_port.set_baud(9600);
    serial_port.set_format(
            /* bits */ 8,
            /* parity */ BufferedSerial::None,
            /* stop bit */ 1);

    lm75.setTemperatureOS(26);
    lm75.setTemperatureHYST(23);


    while (true) {
        //printTemp(lm75.getTemperature());
        //thread_sleep_for(100);
    }
}
