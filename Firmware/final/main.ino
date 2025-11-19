#include <Wire.h>
#include <Adafruit_INA219.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
// #include <WiFi.h>
#include <atuadores.h>
#include <ESP32Servo.h>

// === Bluetooth ===
#include "BluetoothSerial.h"

String device_name = "ESP32-BT-Logger";
BluetoothSerial SerialBT;

// Check if Bluetooth is available
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

// Check Serial Port Profile
#if !defined(CONFIG_BT_SPP_ENABLED)
#error Serial Port Profile for Bluetooth is not available or not enabled. It is only available for the ESP32 chip.
#endif

// === Definição dos pinos ===
#define SDA_1 19
#define SCL_1 18
#define SDA_2 21
#define SCL_2 22

// === Wifi ===
// // Configurações WiFi
// const char* ssid = "ANDRE_5907";
// const char* password = "99634M{m";

// === Criação das interfaces I2C ===
TwoWire I2C_1 = TwoWire(0);
TwoWire I2C_2 = TwoWire(1);

// === Criação dos sensores ===
Adafruit_INA219 ina219(0x40);
Adafruit_MPU6050 mpu6050;
  
// Instância dos motores e do AGV
DCMotor Motor1, Motor2;
AGV AGV1;
Servo servo_1;
uint8_t motor_angle;

void setup() {
  SerialBT.begin(device_name);  //Bluetooth device name
  //SerialBT.deleteAllBondedDevices(); // Uncomment this to delete paired devices; Must be called after begin
  Serial.printf("The device with name \"%s\" is started.\nNow you can pair it with Bluetooth!\n", device_name.c_str());
  
  // WiFi.begin(ssid, password);
  // while (WiFi.status() != WL_CONNECTED) {
  //   Serial.printf("Nao foi possivel conectar-se ao wifi.\n");
  //   delay(500);
  // }
  Serial.begin(115200);
  delay(1000);
  
    servo_1.attach(33);
    AGV1.Create(12, 13, 14, 27, 220);  // Inicializando o AGV com os pinos dos motores
    AGV1.Sensores(25, 26, 34, 35, 6.5); // Inicializando os sensores do AGV: Ultrassônico (trig, echo), Encoders (pino1, pino2), diâmetro da roda em cm

  // Inicializando I2C_1 para o INA219
  I2C_1.begin(SDA_1, SCL_1, 100000);
  if (!ina219.begin(&I2C_1)) { // Endereço padrão do INA219 é 0x40
    SerialBT.println("Não foi possível encontrar o INA219!");
  } else {
    SerialBT.println("INA219 detectado no barramento 1.");
  }

  // Inicializando I2C_2 para o MPU6050
  I2C_2.begin(SDA_2, SCL_2, 100000);
  if (!mpu6050.begin(0x68, &I2C_2)) { // Endereço padrão do MPU6050 é 0x68
    SerialBT.println("Não foi possível encontrar o MPU6050!");
  } else {
    SerialBT.println("MPU6050 detectado no barramento 2.");
  }

  SerialBT.println("Pronto para leituras!\n");

  //Move axis to specific positions, both limits 0, 180 and mid 90
  servo_1.write(0);
  Serial.println("MOVE TO POS_0");              
  delay(3000);           
  
  servo_1.write(90);
  Serial.println("MOVE TO POS_90");              
  delay(3000);   
  
  servo_1.write(180);
  Serial.println("MOVE TO POS_180");              
  delay(3000);   

}

void loop() {

  AGV1.ForwardAGV();

  // === Leitura do INA219 ===
  float current = ina219.getCurrent_mA();
  float voltage = ina219.getBusVoltage_V();
  float power = ina219.getPower_mW();

  // === Leitura do MPU6050 ===
  sensors_event_t a, g, temp;
  mpu6050.getEvent(&a, &g, &temp);

  // === Exibe os valores ===
  SerialBT.println("===== Leitura INA219 =====");
  SerialBT.printf("Corrente = %f mA | Tensao = %f | Potencia = %f mW\n", current, voltage, power); 

  SerialBT.println("===== Leitura MPU6050 =====");
  SerialBT.printf("Acelerômetro: X= %f m/s^2 Y= %f m/s^2 Z= %f m/s^2\n", a.acceleration.x, a.acceleration.y, a.acceleration.z); 

  SerialBT.printf("Giroscópio: X= %f rad/s Y= %f rad/s Z=%f rad/s\n", g.gyro.x, g.gyro.y, g.gyro.z); 

  SerialBT.printf("Temperatura: %f C\n", temp.temperature);

  servo_1.write(0);
  delay(3000);
  servo_1.write(180);
  delay(3000);
  // SerialBT.println("FROM 0 to 180");  
  // for (motor_angle = 0; motor_angle <= 180; motor_angle ++) { 
  //   servo_1.write(motor_angle);
  //   SerialBT.println(motor_angle);           
  //   delay(15);                       
  // }
  // delay(1000);

  // SerialBT.println("FROM 180 to 0"); 
  // for (motor_angle = 180; motor_angle >= 0; motor_angle --) { 
  //   servo_1.write(motor_angle); 
  //   SerialBT.println(motor_angle);                
  //   delay(15);                       
  // }

  // delay(500);
}
  