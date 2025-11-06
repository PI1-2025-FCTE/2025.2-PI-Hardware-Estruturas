#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

// Configurações WiFi
const char* ssid = "ANDRE_5907";
const char* password = "99634M{m";

// Configurações MQTT
const char* mqtt_server = "0.tcp.sa.ngrok.io"; // Exemplo: "broker.hivemq.com"
const int mqtt_port = 19107;
const char* mqtt_user = "user";           // seu usuário MQTT
const char* mqtt_pass = "password";             // sua senha MQTT
const char* topic = "esp32.data";

// Struct Trajeto
typedef struct Trajeto {
    char* comandosEnviados;     // única string, texto longo
    char* comandosExecutados;   // idem
    int status;
    int tempo;
} Trajeto;

Trajeto t;

// Instancia WiFi e MQTT
WiFiClient espClient;
PubSubClient client(espClient);

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
      client.subscribe(topic);
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

    // Agora, atribua uma cópia para comandosEnviados
    t.comandosEnviados = strdup(buffer); // strdup faz cópia dinâmica
}

// Serializa Trajeto para JSON e publica
void sendTrajetoMQTT(Trajeto trajeto) {
  StaticJsonDocument<512> doc;
  JsonArray enviados = doc.createNestedArray("comandosEnviados");
  JsonArray executados = doc.createNestedArray("comandosExecutados");
  for (int i = 0; i < 3; i++) {
    enviados.add(trajeto.comandosEnviados[i]);
    executados.add(trajeto.comandosExecutados[i]);
  }
  doc["status"] = trajeto.status;
  doc["tempo"] = trajeto.tempo;

  char buffer[512];
  size_t n = serializeJson(doc, buffer);
  client.publish(topic, buffer, n);
}

void setup() {
  Serial.begin(115200);
  delay(2000);
  pinMode(2, OUTPUT);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
  reconnect();
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Exemplo de uso:
  // Trajeto t;
  // t.comandosEnviados = strdup("a1220da0980ea0099");            // Exemplo de string separada por vírgula
  t.comandosExecutados = strdup("a1219d89a0981e91a0100");
  t.status = 1;
  t.tempo = 120;
  sendTrajetoMQTT(t);
  delay(5000); // envia a cada 5s
}
