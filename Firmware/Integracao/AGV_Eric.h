// Classe para controle de um motor DC
class DCMotor {
    int pin1, pin2; // Definindo os pinos para controle do motor
  
    public:  
      // Método para inicializar os pinos que controlam o motor
      void Pinout(int in1, int in2) {
        pin1 = in1;
        pin2 = in2;
        pinMode(pin1, OUTPUT);  // Definindo o pino como saída
        pinMode(pin2, OUTPUT);  // Definindo o pino como saída
        digitalWrite(pin1, LOW);  // Iniciando os pinos em LOW
        digitalWrite(pin2, LOW);  // Iniciando os pinos em LOW
      }
  
      // Método para fazer o motor girar para frente
      void Forward() {
        digitalWrite(pin1, HIGH);  // Gira para frente, pino1 em LOW e pino2 em HIGH
        digitalWrite(pin2, LOW);
      }
  
      // Método para fazer o motor girar para trás
      void Backward() {
        digitalWrite(pin1, LOW);  // Gira para trás, pino1 em HIGH e pino2 em LOW
        digitalWrite(pin2, HIGH);
      }
  
      // Método para parar o motor
      void Stop() {
        digitalWrite(pin1, LOW);  // Parando os dois pinos
        digitalWrite(pin2, LOW);
      }
  };
  
  // Classe para controle de um AGV (veículo autônomo)
  class AGV {
    DCMotor Motor1, Motor2;  // Objetos de dois motores DC
  
    public:
      // Método para inicializar os motores com os pinos passados
      void Create(int in1, int in2, int in3, int in4) {
        Motor1.Pinout(in1, in2);  // Inicializa o primeiro motor
        Motor2.Pinout(in3, in4);  // Inicializa o segundo motor
      }
  
      // Método para o AGV ir para frente
      void ForwardAGV() {
        Motor1.Forward();
        Motor2.Forward();
      }
  
      // Método para o AGV ir para trás
      void BackwardAGV() {
        Motor1.Backward();
        Motor2.Backward();
      }
  
      // Método para o AGV virar para a esquerda
      void LeftAGV() {
        Motor1.Backward();
        Motor2.Forward();
      }
  
      // Método para o AGV virar para a direita
      void RightAGV() {
        Motor1.Forward();
        Motor2.Backward();
      }
  
      // Método para parar o AGV
      void StopAGV() {
        Motor1.Stop();
        Motor2.Stop();
      }
  };