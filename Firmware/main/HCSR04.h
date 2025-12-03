#ifndef HCSR04_H
#define HCSR04_H

#include <Arduino.h>

class HCSR04 {
private:
    long echo;
    int trig;
    float tempEcho = 0;
    float distance;

public:
    void PinOut(int in1, int in2);
    void DisparaPulsoUltrassonico();
    float CalculaDistancia();
};

#endif
