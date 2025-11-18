## Dados de Sensores
As tasks são chamadas a cada *x* ms (esse valor é definido no próprio código).

- Arquivo: `Firmware/BasicOTAplusMultitaskplusODP_PI1/BasicOTAplusMultitaskplusODP_PI1.ino`

### Encoder
- Usa interrupção de pino.
- Toda vez que a interrupção é chamada, incrementa-se a distância percorrida.

---

## Decodificação
A cada novo conjunto de instruções recebido (quando a função callback é chamada), as instruções são decodificadas enquanto são executadas.

- Arquivo: `decodificaInstrucoes.ino`

---

## Controle dos Motores
É necessário escrever um código que utilize PWM nos motores para garantir transições suaves ao executar as instruções.  
Toda essa parte será implementada por meio de métodos da biblioteca `AGV_Eric.h`.

---

# Formato Básico do Código Completo

```cpp
// Importação de bibliotecas
// Definição de funções (puxar dados dos sensores, comunicação Wi-Fi…)

void setup() {
    // Inicialização de comunicações
    // Inicialização do Wi-Fi
    // Inicialização das interrupções do encoder
    // (attachInterrupt(digitalPinToInterrupt(ENCODER_PIN), encoderISR, RISING);)
    
    // Definição das tasks
}

void loop() {
    // Nada aqui
}
