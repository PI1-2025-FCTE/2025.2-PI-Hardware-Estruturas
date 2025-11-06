#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define VIRAR_DIREITA 'd'
#define VIRAR_ESQUERDA 'e'
#define AVANCAR 'a'

void setup() {
  Serial.begin(115200);
  delay(5000);
  char buffer[] = "a0245ea0020da0001";
    int i = 0;
    while(i<strlen(buffer)){
        if(buffer[i] == AVANCAR){
            char comprimento[5] = ""; // Aumentamos para 5 para incluir o \0 no final
            i++; // Avança para o primeiro dígito após o 'a'
            strncpy(comprimento, &buffer[i], 4); // Copia os próximos 4 caracteres
            comprimento[4] = '\0'; // Garante que a string termine com \0
            int valor = atoi(comprimento); // Converte para inteiro
            Serial.printf("Avancando %d cm\n", valor);
            i += 3; // Ajusta o índice (já incrementamos 1 antes, agora mais 3 para pular os 4 dígitos)
        }else if(buffer[i] == 'd'){
            Serial.printf("Virando à direita\n");
        }else if(buffer[i] == 'e'){
            Serial.printf("Virando à esquerda\n");
        }
    i++;
    Serial.printf("%d\n", i);
    }

}

void loop() {

}
