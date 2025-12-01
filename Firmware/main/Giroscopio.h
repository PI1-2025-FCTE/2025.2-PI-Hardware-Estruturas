#include <Adafruit_MPU6050.h>

// === Variáveis globais ===
float yaw;
float gyroBiasZ;
unsigned long lastTime;

// === Calibração do giroscópio ===
void calibrarGyroZ(int samples = 2000, Adafruit_MPU6050& mpu);

// === Atualiza o yaw ===
float atualizarYaw();

// === Gira até atingir o ângulo especificado ===
void girarAte(float anguloAlvo);