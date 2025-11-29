#include "Atuadores.h"
#include "Encoder.h"
#include <Arduino.h>
#include "BluetoothSerial.h"

// Instanciação dos encoders no cpp para evitar dependência no header

// DCMotor implementação
DCMotor::DCMotor() : spd(255), pin1(0), pin2(0) {}

void DCMotor::Pinout(int in1, int in2) {
    pin1 = in1;
    pin2 = in2;
    pinMode(pin1, OUTPUT);
    pinMode(pin2, OUTPUT);
}

void DCMotor::Speed(int in1) {
    spd = in1;
}

void DCMotor::Forward() {
    Serial.printf("Ligando pino %d em %d e desligando %d\n", pin1, spd, pin2);    
    analogWrite(pin1, spd);
    analogWrite(pin2, 0);
    digitalWrite(pin2, LOW);
}

void DCMotor::Backward() {
    Serial.printf("Ligando pino %d em %d e desligando %d\n", pin2, spd, pin1);
    analogWrite(pin1, 0);
    digitalWrite(pin1, LOW);
    analogWrite(pin2, spd);
}

void DCMotor::Stop() {
    Serial.printf("Desligando pinos %d e %d\n", pin1, pin2);
    analogWrite(pin1, 0);
    analogWrite(pin2, 0);
    digitalWrite(pin1, LOW);
    digitalWrite(pin2, LOW);
}

// AGV implementação
AGV::AGV() /*: encoderEsq(0), encoderDir(1) */{}

void AGV::Create(int in1, int in2, int in3, int in4, int vel) {
    Motor1.Pinout(in1, in2);
    Motor1.Speed(vel);
    Motor2.Pinout(in3, in4);
    Motor2.Speed(vel);
}

// void AGV::Sensores(int in1, int in2, int inEncoder1, int inEncoder2) {
//     // Se aplicar sensor ultrassônico, deve instanciar e configurar aqui
//     encoderEsq.PinOut(inEncoder1, 0);
//     encoderDir.PinOut(inEncoder2, 1);
// }

void AGV::ForwardAGV() {
    Motor1.Forward();
    Motor2.Forward();
}
void AGV::BackwardAGV() {
    Motor1.Backward();
    Motor2.Backward();
}
void AGV::LeftAGV() {
    Motor1.Backward();
    Motor2.Forward();
}
void AGV::RightAGV() {
    Motor1.Forward();
    Motor2.Backward();
}
void AGV::StopAGV() {
    Motor1.Stop();
    Motor2.Stop();
}
