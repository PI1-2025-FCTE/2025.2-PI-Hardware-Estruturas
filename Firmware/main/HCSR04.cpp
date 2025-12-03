#include "HCSR04.h"

void HCSR04::PinOut(int in1, int in2) {
    echo = in2;
    trig = in1;
    pinMode(trig, OUTPUT);
    digitalWrite(trig, LOW);
    pinMode(echo, INPUT);
}

void HCSR04::DisparaPulsoUltrassonico() {
    digitalWrite(trig, HIGH);
    delayMicroseconds(15);
    digitalWrite(trig, LOW);
}

float HCSR04::CalculaDistancia() {
    DisparaPulsoUltrassonico();

    tempEcho = pulseIn(echo, HIGH);
    distance = (tempEcho * 0.0343) / 2;
    return distance;
}
