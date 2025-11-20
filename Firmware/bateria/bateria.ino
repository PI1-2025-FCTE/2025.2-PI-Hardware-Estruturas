
// --- Classe para Monitoramento de Bateria ---
class MonitorBateria {
private:
  const int PINO_TENSAO = 34; // Pino conectado ao divisor de tensão (Potenciômetro 1)
  const int PINO_CORRENTE = 35; // Pino simulando sensor de corrente (Potenciômetro 2)

  const float TENSAO_MAXIMA = 4.2; // Bateria 100% carregada
  const float TENSAO_MINIMA = 3.0; // Bateria 0%

  // Fatores de Conversão
  // O ESP32 tem ADC de 12 bits (0 a 4095) para 0 a 3.3V
  const float ADC_REF_VOLTAGE = 3.3;
  const int ADC_RESOLUTION = 4095;

  // Fator do Divisor de Tensão (R1 e R2).
  const float FATOR_DIVISOR = 1.27;

public:
  MonitorBateria() {}

  // Inicialização do monitor
  void inicializar() {
    analogReadResolution(12); // Garante resolução de 12 bits
    Serial.println("--- Monitor de Bateria Iniciado ---");
  }

  /**
    * monitorar
    *
    * Realiza a leitura dos sensores, calcula a porcentagem de carga e exibe os dados.
    */
  void monitorar() {
    // 1. Ler os valores brutos (RAW)
    int rawTensao = analogRead(PINO_TENSAO);
    int rawCorrente = analogRead(PINO_CORRENTE);

    // 2. Converter para Tensão Real (Volts)
    // Fórmula: (Leitura / Resolução) * VoltagemRef * FatorDivisor
    float voltagemBateria = (rawTensao / (float)ADC_RESOLUTION) * ADC_REF_VOLTAGE * FATOR_DIVISOR;

    // 3. Converter para Corrente (Simulado)
    // Assumindo que o sensor lê 1V por Ampere para facilitar
    float correnteAmperes = (rawCorrente / (float)ADC_RESOLUTION) * ADC_REF_VOLTAGE;

    int porcentagem = calcularPorcentagem(voltagemBateria);

    // 5. Mostrar Resultados
    Serial.print("Tensão: ");
    Serial.print(voltagemBateria, 2);
    Serial.print(" V | Corrente: ");
    Serial.print(correnteAmperes, 2);
    Serial.print(" A | Bateria: ");
    Serial.print(porcentagem);
    Serial.println(" %");
  }

private:
  /**
    * calcularPorcentagem
    *
    * Calcula a porcentagem de carga da bateria com base na tensão medida.
    *
    * @param { tensao } float - Tensão medida da bateria em Volts.
    * @return Porcentagem de carga da bateria (0 a 100).
    */
  int calcularPorcentagem(float tensao) {
    // Fórmula: (Valor - Min) / (Max - Min) * 100
    
    // Clamping: Impede que passe de 100% ou caia abaixo de 0%
    if (tensao >= TENSAO_MAXIMA) return 100;
    if (tensao <= TENSAO_MINIMA) return 0;

    float resultado = (tensao - TENSAO_MINIMA) / (TENSAO_MAXIMA - TENSAO_MINIMA) * 100.0;
    
    return (int)resultado;
  }
};

// Instância global do monitor de bateria
MonitorBateria monitorBateria;

/**
  * setup
  *
  * Configuração inicial do sistema
  */
void setup() {
  Serial.begin(115200);
  monitorBateria.inicializar();
}

/**
  * loop
  *
  * Loop principal do sistema
  */
void loop() {
  monitorBateria.monitorar();
  delay(1000);
}