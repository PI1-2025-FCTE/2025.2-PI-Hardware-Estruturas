#include "Encoder.h"

Encoder* Encoder::instanciaEncoder1 = nullptr;
Encoder* Encoder::instanciaEncoder2 = nullptr;

Encoder::Encoder(int interruptNumber) {
    if (interruptNumber == 0) {
        instanciaEncoder1 = this;
    } else if (interruptNumber == 1) {
        instanciaEncoder2 = this;
    }
}

void Encoder::PinOut(int pinEntrada, int encoder) {
    pin = pinEntrada;
    pinMode(pin, INPUT_PULLUP);
    if (encoder == 0) {
        attachInterrupt(digitalPinToInterrupt(pin), ISR_Encoder1, FALLING);
    } else if (encoder == 1) {
        attachInterrupt(digitalPinToInterrupt(pin), ISR_Encoder2, FALLING);
    }
}

void Encoder::handleInterrupt() {
    // Serial.printf("Interrompido %d interrupcoes", interrupcoes);
    interrupcoes++;
}

float Encoder::calcularRPM() {
    unsigned long tempoAtual = millis();
    float tempoDecorrido = (tempoAtual - lastTime) / 1000.0;
    if (tempoDecorrido > 0) {
        rpm = (interrupcoes / (float)furosPorRevolucao) / tempoDecorrido * 60.0;  
        interrupcoes = 0;
        lastTime = tempoAtual;
    }
    return rpm;
}

float Encoder::calcularDistancia() {
    float distancia = 0.0;
    float distanciaPorRevolucao = 21.36; // 3.14159 * 6.8;
    float giros = interrupcoes / 20.0; //furosPorRevolucao = 20;
    distancia = giros * distanciaPorRevolucao / 2.0;
    return distancia;
}

void Encoder::deletaDistancia(){
    interrupcoes = 0;
}

void Encoder::ISR_Encoder1() {
    if (instanciaEncoder1) instanciaEncoder1->handleInterrupt();
}

void Encoder::ISR_Encoder2() {
    if (instanciaEncoder2) instanciaEncoder2->handleInterrupt();
}
