#include <sensores.h>

// Classe para controle de um motor DC
class DCMotor {  
    int spd = 255, pin1, pin2;
    
    public:  
    
      void Pinout(int in1, int in2){ // Pinout é o método para a declaração dos pinos que vão controlar o objeto motor
        pin1 = in1;
        pin2 = in2;
        pinMode(pin1, OUTPUT);
        pinMode(pin2, OUTPUT);
        }   
      void Speed(int in1){ // Speed é o método que irá ser responsável por salvar a velocidade de atuação do motor
        spd = in1;
        }     
      void Forward(){ // Forward é o método para fazer o motor girar para frente
        analogWrite(pin1, spd);
        digitalWrite(pin2, LOW);
        }   
      void Backward(){ // Backward é o método para fazer o motor girar para trás
        digitalWrite(pin1, LOW);
        analogWrite(pin2, spd);
        }
      void Stop(){ // Stop é o metodo para fazer o motor ficar parado.
        digitalWrite(pin1, LOW);
        digitalWrite(pin2, LOW);
        }
     };

  
  // Classe para controle de um AGV (veículo autônomo)
  class AGV {
    DCMotor Motor1, Motor2;  // Objetos de dois motores DC
    HCSR04 SensorUltra;
    Encoder Encoder1;
    Encoder Encoder2;
  
    public:
      // Método para inicializar os motores com os pinos passados
      void Create(int in1, int in2, int in3, int in4, int vel) {
        Motor1.Pinout(in1, in2);
        Motor1.Speed(vel);  // Inicializa o primeiro motor
        Motor2.Pinout(in3, in4);  // Inicializa o segundo motor
        Motor2.Speed(vel);
      }
  
      void Sensores(int in1, int in2, int inEncoder1, int inEncoder2, float diametro){
        SensorUltra.PinOut(in1, in2);
        Encoder1.PinOut(inEncoder1, diametro);
        Encoder2.PinOut(inEncoder2, diametro);
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

      void distanciaPorCm(float cm){
        float distanciaEnc1 = Encoder1.calcularDistancia();
        float distanciaEnc2 = Encoder2.calcularDistancia();
        float distancia = (distanciaEnc1 + distanciaEnc2) / 2.0;

        while(distancia < cm){
          ForwardAGV();
          distancia = Encoder1.calcularDistancia();
        }
      }
  };
  