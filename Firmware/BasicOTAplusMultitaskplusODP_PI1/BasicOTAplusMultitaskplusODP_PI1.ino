#include <WiFi.h>
#include <ArduinoOTA.h>
#include <WebServer.h>
#include <WebSocketsServer.h>
// #include <FreeRTOS.h>

// ======= CONFIGURAÇÃO =======
const char* ssid = "VIVOFIBRA-5681";
const char* password = "TFrRnicdP8";

WebServer server(80);            // servidor HTTP
WebSocketsServer webSocket(81);  // servidor WebSocket

// ======= FILAS =======
QueueHandle_t logQueue;         // fila de char* (strings alocadas)
QueueHandle_t cmdQueue;         // fila de comandos (3 bytes)

// ======= UTIL: enfileira log (duplica string) =======
void logMessage(const char* txt) {
  if (!logQueue) return;
  size_t n = strlen(txt) + 1;
  char* copy = (char*)malloc(n);
  if (!copy) return; // sem memória, dropa a mensagem (ou trate)
  memcpy(copy, txt, n);
  xQueueSend(logQueue, &copy, 0);
}

// ======= TASK: Logger (Core 1) =======
// Retira ponteiros da fila, printa Serial e envia via WebSocket para todos os clientes.
// Depois dá free no ponteiro.
void taskLogger(void* pv) {
  char* msg = nullptr;
  for (;;) {
    if (xQueueReceive(logQueue, &msg, portMAX_DELAY) == pdTRUE) {
      // Serial
      Serial.println(msg);

      // WebSocket: broadcast para todos os clientes
      webSocket.broadcastTXT(msg);

      // libera memória
      free(msg);
      msg = nullptr;
    }
    vTaskDelay(pdMS_TO_TICKS(1));
  }
}

// ======= TASK: Exec (Core 1) =======
// Consome comandos de 3 bytes da fila e executa (exemplo: controlar LED)
void taskExec(void* pv) {
  uint8_t cmd[3];
  for (;;) {
    if (xQueueReceive(cmdQueue, &cmd, portMAX_DELAY) == pdTRUE) {
      // Exemplo simples: se cmd[0] == 1 => LED ON, 0 => LED OFF
      if (cmd[0] == 1) {
        digitalWrite(2, HIGH);
        logMessage("EXEC: LED ON (via comando)");
      } else if (cmd[0] == 0) {
        digitalWrite(2, LOW);
        logMessage("EXEC: LED OFF (via comando)");
      } else {
        // registra comando recebido
        char buf[64];
        snprintf(buf, sizeof(buf), "EXEC: comando recebido %02X %02X %02X", cmd[0], cmd[1], cmd[2]);
        logMessage(buf);
      }
    }
  }
}

// ======= TASK: Blink (Core 1) =======
void taskBlink(void* pv) {
  pinMode(2, OUTPUT);
  for (;;) {
    // digitalWrite(2, HIGH);
    logMessage("BLINK: LED ON");
    vTaskDelay(pdMS_TO_TICKS(1200));
    // digitalWrite(2, LOW);
    logMessage("BLINK: LED OFF");
    vTaskDelay(pdMS_TO_TICKS(1200));
  }
}

// ======= WebSocket event handler =======
void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
  switch (type) {
    case WStype_CONNECTED: {
      IPAddress ip = webSocket.remoteIP(num);
      char buf[80];
      snprintf(buf, sizeof(buf), "WS: cliente %u conectado: %u.%u.%u.%u", num, ip[0], ip[1], ip[2], ip[3]);
      logMessage(buf);
      break;
    }
    case WStype_DISCONNECTED: {
      char buf[48];
      snprintf(buf, sizeof(buf), "WS: cliente %u desconectado", num);
      logMessage(buf);
      break;
    }
    case WStype_TEXT: {
      // Recebe texto do cliente — enfileira como log e, se quiser, interpretar comandos em texto
      std::string s((char*)payload, length);
      char buf[128];
      snprintf(buf, sizeof(buf), "WS TXT de %u: %s", num, s.c_str());
      logMessage(buf);

      // Se o texto for algo tipo "cmd:1,0,0" poderíamos parsear e enfileirar um comando.
      // Exemplo simples: se texto "1 0 0" => envia como 3 bytes:
      int a,b,c;
      if (sscanf(s.c_str(), "%d %d %d", &a, &b, &c) == 3) {
        uint8_t cmd[3] = { (uint8_t)a, (uint8_t)b, (uint8_t)c };
        xQueueSend(cmdQueue, &cmd, 0);
      }
      break;
    }
    case WStype_BIN: {
      // Payload binário — se tiver exatamente 3 bytes, trate como comando
      if (length == 3) {
        uint8_t cmd[3];
        memcpy(cmd, payload, 3);
        xQueueSend(cmdQueue, &cmd, 0);
        char buf[64];
        snprintf(buf, sizeof(buf), "WS BIN comando recebido de %u: %02X %02X %02X", num, cmd[0], cmd[1], cmd[2]);
        logMessage(buf);
      } else {
        logMessage("WS BIN recebido com tamanho != 3, ignorado");
      }
      break;
    }
    default:
      break;
  }
}

// ======= HTTP: página simples que abre WS =======
const char index_html[] PROGMEM = R"rawliteral(
<!doctype html>
<html>
<head>
  <meta charset="utf-8">
  <title>ESP32 WebSocket Logs</title>
  <style>
    body { font-family: Arial, sans-serif; margin: 10px; }
    #log { width: 100%; height: 60vh; }
    input { width: 3em; }
  </style>
</head>
<body>
  <h3>ESP32 WebSocket Logs</h3>
  <textarea id="log" readonly></textarea><br>
  <input id="b0" type="number" min="0" max="255" value="1"> 
  <input id="b1" type="number" min="0" max="255" value="0"> 
  <input id="b2" type="number" min="0" max="255" value="0"> 
  <button onclick="sendCmd()">Enviar comando (3 bytes)</button>
  <script>
    let ws;
    function connect() {
      const host = location.hostname;
      ws = new WebSocket('ws://' + host + ':81/');
      ws.onopen = () => log('WS conectado');
      ws.onclose = () => { log('WS desconectado, tentando reconectar em 2s'); setTimeout(connect,2000); };
      ws.onmessage = (evt) => log(evt.data);
    }
    function log(msg) {
      const ta = document.getElementById('log');
      ta.value += msg + '\n';
      ta.scrollTop = ta.scrollHeight;
    }
    function sendCmd() {
      const b0 = parseInt(document.getElementById('b0').value) & 0xFF;
      const b1 = parseInt(document.getElementById('b1').value) & 0xFF;
      const b2 = parseInt(document.getElementById('b2').value) & 0xFF;
      // envia binário de 3 bytes
      const arr = new Uint8Array([b0,b1,b2]);
      ws.send(arr);
      log('Enviado: ' + arr[0].toString(16).padStart(2,'0') + ' ' + arr[1].toString(16).padStart(2,'0') + ' ' + arr[2].toString(16).padStart(2,'0'));
    }
    connect();
  </script>
</body>
</html>
)rawliteral";

void handleRoot() {
  server.send_P(200, "text/html", index_html);
}

// ======= TASK: Network (Core 0) =======
// Roda ArduinoOTA.handle(), WebServer.handleClient() e webSocket.loop()
void taskNetwork(void* pv) {
  for (;;) {
    ArduinoOTA.handle();
    server.handleClient();
    webSocket.loop();
    vTaskDelay(pdMS_TO_TICKS(10));
  }
}

// ======= SETUP =======
void setup() {
  Serial.begin(115200);
  delay(100);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Conectando WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("WiFi conectado. IP: ");
  Serial.println(WiFi.localIP());

  // OTA
  ArduinoOTA.setHostname("ESP32-WS");
  ArduinoOTA.begin();

  // HTTP + WebSocket
  server.on("/", handleRoot);
  server.begin();
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);

  // Cria filas
  logQueue = xQueueCreate(40, sizeof(char*));       // até 40 mensagens pendentes
  cmdQueue = xQueueCreate(20, sizeof(uint8_t[3]));  // até 20 comandos

  // Cria tasks
  xTaskCreatePinnedToCore(taskNetwork, "net", 4096, NULL, 1, NULL, 0); // core 0
  xTaskCreatePinnedToCore(taskLogger,  "logger", 4096, NULL, 1, NULL, 1); // core 1
  xTaskCreatePinnedToCore(taskExec,    "exec",  2048, NULL, 1, NULL, 1); // core 1
  xTaskCreatePinnedToCore(taskBlink,   "blink", 2048, NULL, 1, NULL, 1); // core 1

  logMessage("Sistema inicializado");
}

// ======= LOOP =======
void loop() {
  // Nada aqui. Tudo é controlado por tasks.
}
