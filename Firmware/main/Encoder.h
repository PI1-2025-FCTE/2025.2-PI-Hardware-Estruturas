#ifndef ENCODER_H
#define ENCODER_H

#include <Arduino.h>

class Encoder {
public:
    Encoder(int interruptNumber);

    void PinOut(int pinEntrada, int encoder);
    void handleInterrupt();
    float calcularRPM();
    float calcularDistancia();
    void deletaDistancia();

    static void ISR_Encoder1();
    static void ISR_Encoder2();

private:
    int pin;
    volatile int interrupcoes = 0;
    float rpm = 0.0;
    unsigned long lastTime = 0;
    float diametroRoda = 6.8; //Em cm
    int furosPorRevolucao = 20;

    static Encoder* instanciaEncoder1;
    static Encoder* instanciaEncoder2;
};

#endif
