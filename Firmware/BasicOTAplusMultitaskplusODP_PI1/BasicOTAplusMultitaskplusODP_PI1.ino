#include <WiFi.h>
#include <ArduinoOTA.h>

// === Config Wi-Fi ===
const char* ssid = "VIVOFIBRA-5681";
const char* password = "TFrRnicdP8";

// === Config TCP (logs e comandos via rede) ===
WiFiServer server(8888);
WiFiClient client;

// === FreeRTOS Queues ===
QueueHandle_t logQueue;
QueueHandle_t cmdQueue;

// === Enfileira mensagens de log ===
void logMessage(const char* msg) {
  if (logQueue != NULL) {
    xQueueSend(logQueue, &msg, 0);
  }
}

// === Task OTA (Core 0) ===
void taskOTA(void* pvParameters) {
  for (;;) {
    ArduinoOTA.handle();
    vTaskDelay(pdMS_TO_TICKS(50));
  }
}

// === Task TCP (Core 0): aceita cliente e lê comandos ===
void taskTCP(void* pvParameters) {
  for (;;) {
    if (!client || !client.connected()) {
      client = server.available();
      vTaskDelay(pdMS_TO_TICKS(100));
      continue;
    }

    // Lê comandos de 3 bytes
    if (client.available() >= 3) {
      uint8_t cmd[3];
      client.read(cmd, 3);
      xQueueSend(cmdQueue, &cmd, 0);

      logMessage("Comando recebido");
    }

    vTaskDelay(pdMS_TO_TICKS(10));
  }
}

// // === Task Blink (Core 1) ===
// void taskBlink(void* pvParameters) {
//   pinMode(2, OUTPUT);
//   for (;;) {
//     digitalWrite(2, HIGH);
//     logMessage("LED ON");
//     vTaskDelay(pdMS_TO_TICKS(1000));
//     digitalWrite(2, LOW);
//     logMessage("LED OFF");
//     vTaskDelay(pdMS_TO_TICKS(1000));
//   }
// }

// === Task Logger (Core 1) ===
void taskLogger(void* pvParameters) {
  const char* msg;
  for (;;) {
    if (xQueueReceive(logQueue, &msg, portMAX_DELAY)) {
      Serial.println(msg);
      if (client && client.connected()) {
        client.println(msg);
      }
    }
  }
}

// === Task Exec (Core 1) — executa comandos de 3 bytes ===
void taskExec(void* pvParameters) {
  uint8_t cmd[3];
  for (;;) {
    if (xQueueReceive(cmdQueue, &cmd, portMAX_DELAY)) {
      // Exemplo: usa o primeiro byte pra acender LED
      if (cmd[0] == 1) digitalWrite(2, HIGH);
      if (cmd[0] == 0) digitalWrite(2, LOW);

      logMessage("Comando executado");
    }
  }
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("WiFi failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }

  ArduinoOTA.begin();

  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
  server.begin();

  logQueue = xQueueCreate(20, sizeof(const char*));
  cmdQueue = xQueueCreate(10, sizeof(uint8_t[3]));

  xTaskCreatePinnedToCore(taskOTA, "OTA", 4096, NULL, 1, NULL, 0);
  xTaskCreatePinnedToCore(taskTCP, "TCP", 4096, NULL, 1, NULL, 0);
  // xTaskCreatePinnedToCore(taskBlink, "Blink", 2048, NULL, 1, NULL, 1);
  xTaskCreatePinnedToCore(taskLogger, "Logger", 4096, NULL, 1, NULL, 1);
  xTaskCreatePinnedToCore(taskExec, "Exec", 4096, NULL, 1, NULL, 1);
}

void loop() {}
