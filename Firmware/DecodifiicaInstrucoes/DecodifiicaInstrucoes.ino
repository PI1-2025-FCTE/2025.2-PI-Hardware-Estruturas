#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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

// === Armazenamento dos comandos ===
typedef struct Trajeto {
    String comandosEnviados;     // única string, texto longo
    String comandosExecutados;   // idem
    int status;
    int tempo;
    int ID;
} Trajeto;

Trajeto t;

void setup() {
  Serial.begin(115200);
  delay(5000);
  String buffer = "a0245ea0020da0001";

}



void loop() {
    if(Serial.available()){
    String leitura = Serial.readStringUntil('\n');
    leitura.trim();  // Remove \r\n e espaços 
    Serial.printf("Comando recebido: [%s]", leitura);
    // SerialBT.printf("Comando recebido: [%s]", leitura);
    decodificaExecutaInstrucao(leitura);
    }
}

void decodificaExecutaInstrucao(String buffer){
    int i = 0;
    Serial.printf("Buffer: %s\n", buffer.c_str());  // .c_str() converte String para const char*
    while(i < buffer.length()){  // .length() ao invés de strlen()
        if(buffer[i] == 'a'){
            // char comprimento[5] = "";
            i++; // Avança para o primeiro dígito após o 'a'

            // Pega EXATAMENTE 4 dígitos OU até fim
            int fim = min(i+4, (int)buffer.length());
            String numeroStr = buffer.substring(i, fim);

            int valor = numeroStr.toInt();  // Converte String → int
            Serial.printf("Avancando %d cm\n", valor);
            int dist = distanciaPorCm((float)valor)*100;
            String cmd = "a";

            // distância sempre com 4 dígitos (zero à esquerda)
            if (dist < 0) dist = 0;
            if (dist > 999999) dist = 999999;

            if (dist < 1000)   cmd += "000";
            else if (dist < 10000)  cmd += "00";
            else if (dist < 100000) cmd += "0";
            cmd += String(dist);  // agora vira "0100"
            t.comandosExecutados += cmd;
            SerialBT.printf("Comando executado: %s", t.comandosExecutados);
            Serial.printf("Comando executado: %s", t.comandosExecutados.c_str());
            i = fim;  // Pula os 4 dígitos
        }
        else if(buffer[i] == 'd'){
            Serial.printf("Virando à direita\n");
            // Gira90GrausDireita();
            i++;
            t.comandosExecutados += 'd';
            SerialBT.printf("Comando executado: %s", t.comandosExecutados);
            Serial.printf("Comando executado: %s\n", t.comandosExecutados.c_str());
        }
        else if(buffer[i] == 'e'){
            Serial.printf("Virando à esquerda\n");
            // Gira90GrausEsquerda();
            i++;
            t.comandosExecutados += 'e';
            SerialBT.printf("Comando executado: %s", t.comandosExecutados);
            Serial.printf("Comando executado: %s\n", t.comandosExecutados.c_str());
        }
        else if(buffer[i] == 'i'){
          i++;
          int fim = min(i+2, (int)buffer.length());
          String numeroStr = buffer.substring(i, fim);
          t.ID = numeroStr.toInt();;
          Serial.printf("ID do trajeto: %d\n", t.ID);
        } else {
        i++;  // Pula caracteres desconhecidos
        }
    }
    // publicarTrajeto(t);
}

float distanciaPorCm(float cm){
    float dist = 0;
    // encoderDir.deletaDistancia();
    // encoderEsq.deletaDistancia();
    // AGV1.ForwardAGV();
    while(dist < cm) {
        dist = dist + 1.27;
        Serial.printf("Distancia = %f\n", dist);
        // SerialBT.printf("Distancia = %f\n", dist);
        delay(10);
    }
    return dist;
}