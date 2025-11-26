/**
 * @file bateria.h
 * @brief Sistema de monitoramento de bateria Li-Ion para ESP32 usando sensor INA219
 * 
 * @note Utiliza sensor INA219 para medições precisas de tensão e corrente
 * @note Integra com sistema I2C já existente no projeto
 */

#include <Wire.h>
#include <Adafruit_INA219.h>

class MonitorBateria {
private:
  Adafruit_INA219* ina219_sensor;
  TwoWire* i2c_interface;
  
  static constexpr float TENSAO_MAXIMA = 4.2;
  static constexpr float TENSAO_MINIMA = 3.0;
  
  // Endereço I2C do sensor (mesmo usado no Integracao.ino)
  static constexpr uint8_t INA219_ADDRESS = 0x40;

public:
  // Construtor que recebe a instância do INA219 e interface I2C
  MonitorBateria(Adafruit_INA219* sensor, TwoWire* i2c) 
    : ina219_sensor(sensor), i2c_interface(i2c) {}
  
  bool inicializar() {
    if (!ina219_sensor->begin(i2c_interface)) {
      Serial.println("Erro: Não foi possível inicializar o sensor INA219 para monitoramento de bateria!");
      return false;
    }
    Serial.println("--- Monitor de Bateria INA219 Iniciado ---");
    return true;
  }

  void monitorar() {
    // Leitura direta do sensor INA219
    float voltagemBateria = ina219_sensor->getBusVoltage_V();
    float correnteAmperes = ina219_sensor->getCurrent_mA() / 1000.0; // Converte mA para A
    float potenciaWatts = ina219_sensor->getPower_mW() / 1000.0; // Converte mW para W
    int porcentagem = calcularPorcentagem(voltagemBateria);

    Serial.print("Tensão: ");
    Serial.print(voltagemBateria, 2);
    Serial.print(" V | Corrente: ");
    Serial.print(correnteAmperes, 3);
    Serial.print(" A | Potência: ");
    Serial.print(potenciaWatts, 3);
    Serial.print(" W | Bateria: ");
    Serial.print(porcentagem);
    Serial.println(" %");
    
    // Aviso de bateria baixa
    if (porcentagem <= 20) {
      Serial.println("⚠️  AVISO: Bateria baixa!");
    }
  }
  
  // Métodos para obter valores individuais
  float getTensao() { return ina219_sensor->getBusVoltage_V(); }
  float getCorrente() { return ina219_sensor->getCurrent_mA() / 1000.0; }
  float getPotencia() { return ina219_sensor->getPower_mW() / 1000.0; }
  int getPorcentagem() { return calcularPorcentagem(getTensao()); }

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
