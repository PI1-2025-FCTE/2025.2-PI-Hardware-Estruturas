// class HCSR04{
//     int echo, trig;
//     float tempEcho = 0;
//     float  distance;

//     public:

//         void PinOut(int in1, int in2){
//         echo = in2;
//         trig = in1;
//         pinMode(trig, OUTPUT);
//         digitalWrite(trig, LOW);
//         pinMode(echo, INPUT);
//         }

//         void DisparaPulsoUltrassonico(){
//         digitalWrite(trig, HIGH);
//         delayMicroseconds(10);
//         digitalWrite(trig, LOW);
//         }


//         // Função para calcular a distancia em metros
//         float CalculaDistancia(){
//         DisparaPulsoUltrassonico();

//         tempEcho = pulseIn(echo, HIGH);
//         distance = (tempEcho*.0343)/2;
//         return distance;
//         }
// };

// class Encoder {
//     int pin;                  // Pino digital de entrada
//     volatile int interrupcoes = 0;     // Contador de interrupções (furos no disco)
//     float rpm = 0.0;          // Rotações por minuto (RPM)
//     unsigned long lastTime = 0; // Para controlar o tempo de contagem das interrupções
//     float diametroRoda;       // Diâmetro da roda em centímetros
//     int furosPorRevolucao = 20; // Quantidade de furos no disco do encoder

//     // Ponteiro estático para a instância atual vinculada à ISR
//     static Encoder* instanciaEncoder1;
//     static Encoder* instanciaEncoder2;

//   public:
//       Encoder(int interruptNumber) {
//       // Registre o ponteiro estático para cada ISR conforme o número de interrupção
//       if (interruptNumber == 0) {
//         instanciaEncoder1 = this;
//       } else if (interruptNumber == 1) {
//         instanciaEncoder2 = this;
//       }
//     }
//     // Construtor para inicializar o pino e o diâmetro da roda
//     void PinOut(int pinEntrada, float diametro) {
//       pin = pinEntrada;
//       diametroRoda = diametro;
//       pinMode(pin, INPUT_PULLUP); // Configura o pino como entrada com pull-up
//       attachInterrupt(digitalPinToInterrupt(pin), onInterrupt, FALLING); // Configura a interrupção para contar os furos
//     }
  
//     // Função chamada em cada interrupção (quando o fototransistor detecta uma mudança)
//     static void onInterrupt() {
//       interrupcoes++; // Incrementa o contador de interrupções
//     }
  
//     // Função para calcular o RPM (rotações por minuto)
//     float calcularRPM() {
//       unsigned long tempoAtual = millis();
//       // Calcula o tempo decorrido em segundos
//       float tempoDecorrido = (tempoAtual - lastTime) / 1000.0;
      
//       // Se o tempo decorrido for maior que 0 (evita divisão por zero)
//       if (tempoDecorrido > 0) {
//         rpm = (interrupcoes / furosPorRevolucao) / tempoDecorrido * 60.0;  // Calculo do RPM
//         interrupcoes = 0; // Reseta o contador de interrupções para o próximo cálculo
//         lastTime = tempoAtual; // Atualiza o último tempo
//       }
  
//       return rpm;
//     }
  
//     // Função para calcular a distância percorrida em centímetros
//     float calcularDistancia() {
//       // Distância percorrida por rotação da roda (em centímetros)
//       float distanciaPorRevolucao = 3.14159 * diametroRoda;
  
//       // Número de giros feitos (baseado nas interrupções)
//       float giros = interrupcoes / furosPorRevolucao;
  
//       // Calcula a distância total percorrida
//       float distancia = giros * distanciaPorRevolucao;
  
//       return distancia;
//     }

//     // ISRs globais que chamam o método da instância adequada
//     static void ISR_Encoder1() {
//       if (instanciaEncoder1) instanciaEncoder1->OnInterrupt();
//     }

//     static void ISR_Encoder2() {
//       if (instanciaEncoder2) instanciaEncoder2->OnInterrupt();
//     }
//   };
  
  
