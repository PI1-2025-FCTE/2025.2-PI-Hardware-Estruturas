#include <Wire.h>
#include <Adafruit_INA219.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <ArduinoOTA.h>
#include "WiFiUdp.h"
// #include "../bateria/bateria.h"
#include "Atuadores.h"
#include "Encoder.h"
#include <ESP32Servo.h>

// === Bluetooth ===
// #include "BluetoothSerial.h"
// String device_name = "ESP32-BT-Logger";
// BluetoothSerial SerialBT;

// Check if Bluetooth is available
// #if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
// #error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
// #endif

// // Check Serial Port Profile
// #if !defined(CONFIG_BT_SPP_ENABLED)
// #error Serial Port Profile for Bluetooth is not available or not enabled. It is only available for the ESP32 chip.
// #endif

// === Definição dos pinos I2C ===
#define SDA_1 19
#define SCL_1 18
#define SDA_2 21
#define SCL_2 22

// === Armazenamento dos comandos ===
typedef struct Trajeto {
  String comandosEnviados;    // única string, texto longo
  String comandosExecutados;  // idem
  int status;
  int tempo;
  int ID;
} Trajeto;

Trajeto t;

bool umComandoFoiRecebido = false;
// === Wifi ===
// Configurações WiFi
const char* ssid = "ANDRE_5907";
const char* password = "99634M{m";

// === Comunicacao MQTT ===
// Configurações MQTT
const char* mqtt_server = "0.tcp.sa.ngrok.io";  // Exemplo: "broker.hivemq.com"
const int mqtt_port = 19127;
const char* mqtt_user = "user";      // seu usuário MQTT
const char* mqtt_pass = "password";  // sua senha MQTT
const char* data_topic = "esp32.data";
const char* device_id = "esp32";
const char* status_topic = "devices/esp32/status";
const char* trajeto_topic = "devices/esp32/trajeto";
const char* client_topic = "devices/esp32/commands";
// Instancia WiFi e MQTT
WiFiClient espClient;
PubSubClient client(espClient);

// === Comunicacao UDP ===
WiFiUDP udp;
const char* udp_host = "192.168.0.10";  // IP do PC/servidor que vai receber
const uint16_t udp_port = 5005;         // porta em que o PC escuta (ex.: 5005)

// === Giroscópio ===
// float yaw = 0.0;
// float gyroBiasZ = 0.0;
// unsigned long lastTime = 0;
// // Filtro low-pass para gz
// static float gz_filt = 0;
// const float alpha = 0.98;  // Filtro: 98% anterior + 2% novo

// Calibração do giroscópio
// void calibrarGyroZ();

// Atualiza o yaw
// float atualizarYaw();

// Gira até atingir o ângulo especificado
void girarAte(float anguloAlvo);

// === Criação das interfaces I2C ===
TwoWire I2C_1 = TwoWire(0);
TwoWire I2C_2 = TwoWire(1);

// === Criação dos sensores ===
Adafruit_INA219 ina219(0x40);
float porcentagem;
#define TENSAO_MAXIMA 7.24
#define TENSAO_MINIMA 5.5

Adafruit_MPU6050 mpu6050;
unsigned long lastStatusTime = 0;
// Instância dos motores e do AGV
// DCMotor Motor1, Motor2;
AGV AGV1;
Encoder encoderEsq(0);
Encoder encoderDir(1);

Servo servo_1;
uint8_t motor_angle;

void setup() {
  Serial.begin(115200);
  delay(3000);

  // SerialBT.begin(device_name);  //Bluetooth device name
  //SerialBT.deleteAllBondedDevices(); // Uncomment this to delete paired devices; Must be called after begin
  // Serial.printf("The device with name \"%s\" is started.\nNow you can pair it with Bluetooth!\n", device_name.c_str());
  delay(1000);

  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
  reconnect();
  ArduinoOTA.begin();

  servo_1.attach(15);
  AGV1.Create(25, 26, 33, 32, 220);  // Inicializando o AGV com os pinos dos motores
  encoderEsq.PinOut(27, 0);          // Exemplo
  encoderDir.PinOut(4, 1);           // Exemplo

  // Inicializando I2C_1 para o INA219
  I2C_1.begin(SDA_1, SCL_1, 100000);
  if (!ina219.begin(&I2C_1)) {  // Endereço padrão do INA219 é 0x40
    Serial.println("Não foi possível encontrar o INA219!");
  } else {
    Serial.println("INA219 detectado no barramento 1.");
  }

  // Inicializando I2C_2 para o MPU6050
  I2C_2.begin(SDA_2, SCL_2, 100000);
  if (!mpu6050.begin(0x68, &I2C_2)) {  // Endereço padrão do MPU6050 é 0x68
    Serial.println("Não foi possível encontrar o MPU6050!");
  } else {
    Serial.println("MPU6050 detectado no barramento 2.");
  }

  Serial.println("Pronto para leituras!\n");
  lastStatusTime = millis();

  // logQueue = xQueueCreate(20, sizeof(const char*));

  xTaskCreatePinnedToCore(taskColetaDados, "Data", 4096, NULL, 1, NULL, 1);
  // xTaskCreatePinnedToCore(taskExec, "Exec", 4096, NULL, 1, NULL, 1);
  // xTaskCreatePinnedToCore(taskBluetooth, "Logger", 4096, NULL, 1, NULL, 0);
  xTaskCreatePinnedToCore(taskWifi, "TCP e OTA", 4096, NULL, 1, NULL, 0);
  // xTaskCreatePinnedToCore(taskBlink, "Blink", 2048, NULL, 1, NULL, 1);

  servo_1.write(180);  // Posição inicial do servo
}

void loop() {

  // SerialBT.println("===== Monitor de Bateria =====");
  // Teste();
  if (umComandoFoiRecebido) {
    decodificaExecutaInstrucao(t.comandosEnviados);
    entregaCarga();
    publicarTrajeto(t);
    t.comandosEnviados = "";
    t.comandosExecutados = "";
    t.ID = 0;
    t.tempo = 0;
    umComandoFoiRecebido = false;
  }
}

// === Enfileira mensagens de log ===
// void logMessage(const char* msg) {
//   if (logQueue != NULL) {
//     xQueueSend(logQueue, &msg, 0);
//   }
// }

void taskColetaDados(void* pvParameters) {
  for (;;) {
    monitorarBateria();
    publicarStatus();
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}

// void taskBluetooth(void* pvParameters) {
//     for(;;){
//       SerialBT.println("===== Leitura INA219 =====");
//       SerialBT.printf("Corrente = %f mA | Tensao = %f | Potencia = %f mW\n", current, voltage, power);
//       // SerialBT.println("===== Leitura MPU6050 =====");
//       // SerialBT.printf("Acelerômetro: X= %f m/s^2 Y= %f m/s^2 Z= %f m/s^2\n", a.acceleration.x, a.acceleration.y, a.acceleration.z);
//       // SerialBT.printf("Giroscópio: X= %f rad/s Y= %f rad/s Z=%f rad/s\n", g.gyro.x, g.gyro.y, g.gyro.z);
//       // SerialBT.printf("Temperatura: %f C\n", temp.temperature);
//       // SerialBT.printf("Yaw Atual: %f graus\n", atualizarYaw());
//       vTaskDelay(pdMS_TO_TICKS(500));
//     }
// }

void taskWifi(void* pvParameters) {
  for (;;) {
    if (!client.connected()) {
      reconnect();
    }
    ArduinoOTA.handle();
    client.loop();
    vTaskDelay(pdMS_TO_TICKS(50));
  }
}

void entregaCarga(){
  servo_1.write(180);
  for(uint8_t i = 180; i>=140; i--){
    servo_1.write(i);
    delay(15);
  }
  delay(3000);
  servo_1.write(180);
}

int distanciaPorCm(float cm) {
  // float distancia = encoderEsq.calcularDistancia();
  // float distanciaEnc2 = encoderDir.calcularDistancia();
  // float distancia = (distanciaEnc1 + distanciaEnc2) / 2.0;
  float dist = 0;
  encoderDir.deletaDistancia();
  encoderEsq.deletaDistancia();
  AGV1.ForwardAGV();
  while (dist < cm) {
    dist = encoderDir.calcularDistancia();
    Serial.printf("Distancia = %f\n", dist);
    UdpLogf("Distancia = %f\n", dist);
    // SerialBT.printf("Distancia = %f\n", dist);
    delay(100);
  }
  AGV1.StopAGV();
  encoderDir.deletaDistancia();
  encoderEsq.deletaDistancia();
  return dist;
}

void Gira90GrausDireita() {
  float dist = 0;
  encoderDir.deletaDistancia();
  encoderEsq.deletaDistancia();
  AGV1.RightAGV();           // Diametro entre rodas = 19.45 cm
  while (dist < 15.26825) {  // 19.45 * 3.14 * (90.0 / 360.0) calcula o comprimento do arco para 90 graus
    dist = encoderDir.calcularDistancia();
    Serial.printf("Distancia = %f\n", dist);
    UdpLogf("Distancia = %f\n", dist);
    // SerialBT.printf("Distancia = %f\n", dist);
    delay(100);
  }
  AGV1.StopAGV();
  encoderDir.deletaDistancia();
  encoderEsq.deletaDistancia();
  // return dist;
}

void Gira90GrausEsquerda() {
  float dist = 0;
  encoderDir.deletaDistancia();
  encoderEsq.deletaDistancia();
  AGV1.LeftAGV();
  while (dist < 15.26825) {  // 19.45 * 3.14 * (90.0 / 360.0) calcula o comprimento do arco para 90 graus
    dist = encoderDir.calcularDistancia();
    Serial.printf("Distancia = %f\n", dist);
    UdpLogf("Distancia = %f\n", dist);
    // SerialBT.printf("Distancia = %f\n", dist);
    delay(100);
  }
  AGV1.StopAGV();
  AGV1.StopAGV();
  encoderDir.deletaDistancia();
  encoderEsq.deletaDistancia();
  // return dist;
}

void decodificaExecutaInstrucao(String buffer) {
  int i = 0;
  Serial.printf("Buffer: %s\n", buffer.c_str());  // .c_str() converte String para const char*
  while (i < buffer.length()) {                   // .length() ao invés de strlen()
    if (buffer[i] == 'a') {
      // char comprimento[5] = "";
      i++;  // Avança para o primeiro dígito após o 'a'

      // Pega EXATAMENTE 4 dígitos OU até fim
      int fim = min(i + 4, (int)buffer.length());
      String numeroStr = buffer.substring(i, fim);

      int valor = numeroStr.toInt();  // Converte String → int
      Serial.printf("Avancando %d cm\n", valor);
      int dist = distanciaPorCm((float)valor) * 100;
      String cmd = "a";

      // distância sempre com 4 dígitos (zero à esquerda)
      if (dist < 0) dist = 0;
      if (dist > 999999) dist = 999999;

      if (dist < 1000) cmd += "000";
      else if (dist < 10000) cmd += "00";
      else if (dist < 100000) cmd += "0";
      cmd += String(dist);  // agora vira "0100"
      t.comandosExecutados += cmd;
      // SerialBT.printf("Comando executado: %s", t.comandosExecutados);
      // logMessage("Comando executado: %s", t.comandosExecutados);
      Serial.printf("Comando executado: %s", t.comandosExecutados.c_str());
      UdpLogf("Comando executado: %s", t.comandosExecutados.c_str());
      i = fim;  // Pula os 4 dígitos
    } else if (buffer[i] == 'd') {
      Serial.printf("Virando à direita\n");
      Gira90GrausDireita();
      i++;
      t.comandosExecutados += 'd';
      // SerialBT.printf("Comando executado: %s", t.comandosExecutados);
      Serial.printf("Comando executado: %s\n", t.comandosExecutados.c_str());
      UdpLogf("Comando executado: %s", t.comandosExecutados.c_str());
    } else if (buffer[i] == 'e') {
      Serial.printf("Virando à esquerda\n");
      Gira90GrausEsquerda();
      i++;
      t.comandosExecutados += 'e';
      // SerialBT.printf("Comando executado: %s", t.comandosExecutados);
      Serial.printf("Comando executado: %s\n", t.comandosExecutados.c_str());
      UdpLogf("Comando executado: %s", t.comandosExecutados.c_str());
    } else if (buffer[i] == 'i') {
      i++;
      int fim = min(i + 2, (int)buffer.length());
      String numeroStr = buffer.substring(i, fim);
      t.ID = numeroStr.toInt();
      ;
      Serial.printf("ID do trajeto: %d\n", t.ID);
      UdpLogf("ID do trajeto: %d", t.ID);
    } else {
      i++;  // Pula caracteres desconhecidos
    }
  }
}


void Teste() {
  if (Serial.available()) {
    String leitura = Serial.readStringUntil('\n');
    leitura.trim();  // Remove \r\n e espaços
    Serial.printf("Comando recebido: [%s]", leitura);
    // SerialBT.printf("Comando recebido: [%s]", leitura);
    if (leitura == "andar1") {
      distanciaPorCm(50.0);  // Move o AGV para frente por 50 cm
      // AGV1.ForwardAGV();
      // Serial.println("AGV Andando pra frente");
      // delay(5000);
      // AGV1.StopAGV();
      // Serial.println("AGV Parado");
    } else if (leitura == "andar2") {
      distanciaPorCm(100.0);
    } else if (leitura == "del") {
      encoderDir.deletaDistancia();
      encoderEsq.deletaDistancia();
    } else if (leitura == "andar3") {
      // Serial.println("AGV andando pra frente");
      // AGV1.ForwardAGV();
      distanciaPorCm(200.0);
    } else if (leitura == "andar4") {
      // Serial.println("AGV andando pra tras");
      // AGV1.BackwardAGV();
      distanciaPorCm(500.0);
    } else if (leitura == "parar") {
      // Serial.println("AGV parado");
      AGV1.StopAGV();
    } else if (leitura == "right") {
      Serial.println("Girando 90 graus para a direita");
      Gira90GrausDireita();
    } else if (leitura == "left") {
      Serial.println("Girando 90 graus para a esquerda");
      Gira90GrausEsquerda();
    } else {
      decodificaExecutaInstrucao(leitura);
    }
  }
}

// === Funcoes do Wifi e MQTT
// Publica o status
void publicarStatus() {
  StaticJsonDocument<128> doc;
  doc["online"] = true;  //alterar isto
  doc["bateria"] = porcentagem;   //alterar isto
  char buffer[128];
  size_t n = serializeJson(doc, buffer);
  client.publish(status_topic, buffer, n);
  // Serial.println("Status publicado!");
}

// Publica o trajeto
void publicarTrajeto(Trajeto trajeto) {
  StaticJsonDocument<192> doc;
  doc["idTrajeto"] = trajeto.ID;
  doc["Executadas"] = trajeto.comandosExecutados;
  doc["device_id"] = "esp32";
  char buffer[192];
  size_t n = serializeJson(doc, buffer);
  client.publish(trajeto_topic, buffer, n);
  Serial.println("Trajeto publicado!");
}

// Função para conectar WiFi
void setup_wifi() {
  delay(10);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.printf("Nao foi possivel conectar-se ao wifi.\n");
    delay(500);
  }
  Serial.printf("Conectado ao wifi!\n");
  digitalWrite(2, HIGH);
  udp.begin(12345);  // porta local qualquer
}

// Função para conectar MQTT
void reconnect() {
  while (!client.connected()) {
    if (client.connect("ESP32Client", mqtt_user, mqtt_pass)) {
      Serial.printf("Conectado ao cliente!\n");
      client.subscribe(client_topic);
      digitalWrite(2, LOW);
      delay(100);
      digitalWrite(2, HIGH);
      delay(100);
      digitalWrite(2, LOW);
      delay(100);
      digitalWrite(2, HIGH);
    } else {
      // if(Serial.available()){
      //   String mqtt_server = Serial.readStringUntil('\n');
      // }
      delay(5000);
      Serial.println("Tentando conectar-se ao cliente.");
    }
  }
}

//Funcao que recebe mensagens
void callback(char* topic, byte* payload, unsigned int length) {
  // Crie um buffer para armazenar a string do payload
  char buffer[256];
  if (length >= sizeof(buffer)) length = sizeof(buffer) - 1;  // evita overflow
  memcpy(buffer, payload, length);
  buffer[length] = '\0';  // Termina a string corretamente
  Serial.println("Foram recebidos comandos!");
  UdpLogf("Comando recebido via MQTT: %s", buffer);
  // Agora, atribua uma cópia para comandosEnviados
  t.comandosEnviados = String(buffer);  // strdup faz cópia dinâmica
  Serial.printf("Comando recebido e armazenado: %s\n", t.comandosEnviados);
  umComandoFoiRecebido = true;
}

// Funções para envio de logs via UDP
void UdpLog(const char* msg) {
  udp.beginPacket(udp_host, udp_port);
  udp.write((const uint8_t*)msg, strlen(msg));
  udp.endPacket();
}

void UdpLogf(const char* fmt, ...) {
  char buf[256];
  va_list args;
  va_start(args, fmt);
  vsnprintf(buf, sizeof(buf), fmt, args);
  va_end(args);
  UdpLog(buf);
}

void monitorarBateria() {
    // Leitura direta do sensor INA219
  float voltagemBateria = ina219.getBusVoltage_V();
  float correnteAmperes = ina219.getCurrent_mA() / 1000.0; // Converte mA para A
  float potenciaWatts = ina219.getPower_mW() / 1000.0; // Converte mW para W
  porcentagem = calcularPorcentagem(voltagemBateria);

  Serial.print("Tensão: ");
  Serial.print(voltagemBateria, 2);
  Serial.print(" V | Corrente: ");
  Serial.print(correnteAmperes, 3);
  Serial.print(" A | Potência: ");
  Serial.print(potenciaWatts, 3);
  Serial.print(" W | Bateria: ");
  Serial.print(porcentagem);
  Serial.println(" %");
  
  // Aviso de bateria baixa
  if (porcentagem <= 20) {
    Serial.println("AVISO: Bateria baixa!");
  }
}

int calcularPorcentagem(float tensao) {
  if (tensao >= TENSAO_MAXIMA) return 100;
  if (tensao <= TENSAO_MINIMA) return 0;

  return (int)((tensao - TENSAO_MINIMA) / (TENSAO_MAXIMA - TENSAO_MINIMA) * 100.0);
}