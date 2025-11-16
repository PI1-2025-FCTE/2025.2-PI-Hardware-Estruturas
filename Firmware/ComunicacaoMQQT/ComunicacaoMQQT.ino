#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

// Configurações WiFi
const char* ssid = "ANDRE_5907";
const char* password = "99634M{m";

// Configurações MQTT
const char* mqtt_server = "0.tcp.sa.ngrok.io"; // Exemplo: "broker.hivemq.com"
const int mqtt_port = 14171;
const char* mqtt_user = "user";           // seu usuário MQTT
const char* mqtt_pass = "password";             // sua senha MQTT
const char* data_topic = "esp32.data";
// const char* device_id = "esp32";
const char* status_topic = "devices/esp32/status";
const char* trajeto_topic = "devices/esp32/trajeto";
const char* client_topic = "devices/esp32/commands";

// Controle de tempo
unsigned long lastStatusTime = 0;
unsigned long lastTrajetoTime = 0;

// Struct Trajeto
typedef struct Trajeto {
    char* comandosEnviados;     // única string, texto longo
    char* comandosExecutados;   // idem
    int status;
    int tempo;
    int ID;
} Trajeto;

Trajeto t;

// Instancia WiFi e MQTT
WiFiClient espClient;
PubSubClient client(espClient);

// Publica o status
void publicarStatus() {
    StaticJsonDocument<128> doc;
    doc["online"] = true; //alterar isto
    doc["bateria"] = 95;  //alterar isto
    char buffer[128];
    size_t n = serializeJson(doc, buffer);
    client.publish(status_topic, buffer, n);
    Serial.println("Status publicado!");
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
      delay(5000);
      Serial.println("Tentando conectar-se ao cliente.");
    }
  }
}

//Funcao que recebe mensagens
void callback(char* topic, byte* payload, unsigned int length) {
    // Crie um buffer para armazenar a string do payload
    char buffer[256];
    if (length >= sizeof(buffer)) length = sizeof(buffer) - 1; // evita overflow
    memcpy(buffer, payload, length);
    buffer[length] = '\0'; // Termina a string corretamente
    Serial.println("Foram recebidos comandos!");
    // Agora, atribua uma cópia para comandosEnviados
    t.comandosEnviados = strdup(buffer); // strdup faz cópia dinâmica
    Serial.printf("Comando recebido e armazenado: %s\n", t.comandosEnviados);
}

// // Serializa Trajeto para JSON e publica
// void sendTrajetoMQTT(Trajeto trajeto) {
//   StaticJsonDocument<512> doc;
//   doc["comandosEnviados"] = trajeto.comandosEnviados;
//   doc["comandosExecutados"] = trajeto.comandosExecutados;
//   doc["status"] = trajeto.status;
//   doc["tempo"] = trajeto.tempo;

//   char buffer[512];
//   size_t n = serializeJson(doc, buffer);
//   client.publish(data_topic, buffer, n);
// }

void setup() {
  Serial.begin(115200);
  delay(2000);
  pinMode(2, OUTPUT);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
  reconnect();

  t.comandosExecutados = strdup("a1219d89a0981e91a0100");
  t.status = 1;
  t.tempo = 120;
  t.ID = 10;

  lastStatusTime = millis();
  lastTrajetoTime = millis();
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Publica status a cada 30 segundos
  if (millis() - lastStatusTime > 30000) {
      publicarStatus();
      lastStatusTime = millis();
  }
  // Publica trajeto a cada 10 segundos
  if (millis() - lastTrajetoTime > 10000) {
      publicarTrajeto(t);
      lastTrajetoTime = millis();
  }
  // delay(3000); // envia a cada 5s
}

