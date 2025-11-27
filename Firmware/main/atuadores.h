#ifndef ATUADORES_H
#define ATUADORES_H
#include "Encoder.h"
class DCMotor {
    int spd, pin1, pin2;
public:
    DCMotor();
    void Pinout(int in1, int in2);
    void Speed(int in1);
    void Forward();
    void Backward();
    void Stop();
};

class AGV {
    DCMotor Motor1, Motor2;
    // Encoder encoderEsq;
    // Encoder encoderDir;
    // Declarar o Encoder como ponteiro se quiser evitar incluir o cabeçalho aqui
public:
    AGV();
    void Create(int in1, int in2, int in3, int in4, int vel);
    // void Sensores(int in1, int in2, int inEncoder1, int inEncoder2);
    void ForwardAGV();
    void BackwardAGV();
    void LeftAGV();
    void RightAGV();
    void StopAGV();
};

#endif // ATUADORES_H
