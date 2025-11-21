/**
 * @file battery_monitor.ino
 * @brief Sistema de monitoramento de bateria Li-Ion para ESP32
 * 
 * @note ADC ESP32 tem não-linearidade nas extremidades (0-150mV e 3.1-3.3V)
 * @note Recomenda-se calibração com esp_adc_cal para maior precisão
 */

class MonitorBateria {
private:
  static constexpr int PINO_TENSAO = 34;
  static constexpr int PINO_CORRENTE = 35;
  
  static constexpr float TENSAO_MAXIMA = 4.2;
  static constexpr float TENSAO_MINIMA = 3.0;
  static constexpr float ADC_REF_VOLTAGE = 3.3;
  static constexpr int ADC_RESOLUTION = 4095;
  
  // Fator do divisor de tensão (ajustar conforme R1 e R2 utilizados)
  static constexpr float FATOR_DIVISOR = 1.27;

public:
  void inicializar() {
    analogReadResolution(12);
    Serial.println("--- Monitor de Bateria Iniciado ---");
  }

  void monitorar() {
    int rawTensao = analogRead(PINO_TENSAO);
    int rawCorrente = analogRead(PINO_CORRENTE);

    float voltagemBateria = (rawTensao / (float)ADC_RESOLUTION) * ADC_REF_VOLTAGE * FATOR_DIVISOR;
    float correnteAmperes = (rawCorrente / (float)ADC_RESOLUTION) * ADC_REF_VOLTAGE;
    int porcentagem = calcularPorcentagem(voltagemBateria);

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
   * @brief Calcula porcentagem de carga baseada em tensão linear
   * @param tensao Tensão medida da bateria (V)
   * @return Porcentagem de carga (0-100)
   * @note Curva real de descarga Li-Ion não é linear - considere lookup table para maior precisão
   */
  int calcularPorcentagem(float tensao) {
    if (tensao >= TENSAO_MAXIMA) return 100;
    if (tensao <= TENSAO_MINIMA) return 0;
    
    return (int)((tensao - TENSAO_MINIMA) / (TENSAO_MAXIMA - TENSAO_MINIMA) * 100.0);
  }
};

MonitorBateria monitorBateria;

void setup() {
  Serial.begin(115200);
  monitorBateria.inicializar();
}

void loop() {
  monitorBateria.monitorar();
  delay(1000);
}