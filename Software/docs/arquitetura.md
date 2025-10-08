# Descrição da Arquitetura

<!-- Descrição da arquitetura da solução de software proposta:
– Propósito do software (qual o seu papel no sistema);
– Estilo arquitetural contendo:
∗ Padrão adotado: MVC, MVP, Microsserviços, Monolítico, etc (Justificar);
∗ Linguagens de programação: Java, Python, C#, JavaScript, etc.
∗ Frameworks e bibliotecas: Spring Boot, .NET Core, React, Angular, Django,
etc.
∗ Banco de dados: Relacional (PostgreSQL, MySQL, etc) X NãoSQL (Mon-
goDB,etc).
Capítulo 4. Projeto Conceitual do Produto 18
– Diagrama de alto nível: representação esquemática dos componentes da arqui-
tetura, suas responsabilidades e comunicações, esclarecendo a composição do
front-end e do back-end. -->

## Propósito do Software

<div align="justify">&emsp;&emsp;
O Software proposto é uma plataforma web que atua como interface central para o controle, monitoramento e análise do carrinho autônomo equipado com um sistema embarcado ESP32.  
A comunicação ocorre via protocolo HTTP sobre TCP/IP, permitindo o planejamento detalhado de rotas, o envio de missões ao veículo, o acompanhamento de sua posição e a análise dos dados coletados durante as missões.
</div>

---

### Funcionalidades Principais

#### 1. Envio de Instruções de Rotas (Gerenciamento de Missões)

Esta é a funcionalidade de comando da plataforma. A interface permitirá que o usuário crie, salve e envie **missões** para o carrinho.

**Criação de Rotas:**  
O usuário poderá definir percursos por meio de uma interface visual composta por blocos de ações.  
Cada rota é uma sequência de pontos de passagem (*waypoints*) ou comandos específicos, como:

- "Vá para a coordenada X, Y"
- "Gire 90°"
- "Ative o atuador A"

**Envio da Missão via HTTP:**  
Ao clicar em **"Iniciar Missão"**, a plataforma traduz a rota em um formato de dados, como JSON, e a envia ao sistema embarcado.

```bash
{
  "mission_id": "M101",
  "action": "a103da0112"
}
```

> O campo `"action"` representa uma sequência de comandos, por exemplo:  
> “ande 103 cm, vire à direita, ande 112 cm”.

---

#### 2. Histórico de Missões

O sistema registrará todas as missões executadas, fornecendo um log detalhado para consulta e auditoria.

**Log de Execução:**  
Para cada missão, o sistema armazenará informações como:

- Tempo de execução  
- Status final (Concluída, Falhou)

**Interface de Consulta:**  
A plataforma contará com uma tela dedicada para filtrar e pesquisar missões por **data**, **status** ou **ID do carrinho**, exibindo todos os detalhes registrados.

---

#### 3. Análise de Dados

**Dashboard Analítico:**  
Uma tela interativa apresentará gráficos e indicadores de desempenho do sistema.

**Métricas Avaliadas:**

- **Eficiência:** tempo médio para completar missões e distância percorrida  
- **Saúde do Veículo:** consumo e vida útil da bateria, entre outros parâmetros

## Estilo Arquitetural

- **Padrão adotado**: Cliente-Servidor

Justificativa: o padrão Cliente-Servidor foi adotado porque define uma clara separação de responsabilidades, centralizando a lógica de negócio na API e gerenciando a comunicação entre a interface web e o hardware.

- **Linguagens de programação**: TypeScript, Python, C++

Justificativa: TypeScript porque agrega robustez ao código por meio da tipagem estática, prevenindo erros. Python porque permite desenvolvimento rápido e integração eficiente com bibliotecas, além de suportar lógica de negócio complexa no backend. C++ porque permite controle de baixo nível do ESP32 e execução precisa em tempo real.

- **Frameworks e bibliotecas**: React, FastAPI

Justificativa: React porque facilita a construção de uma interface interativa e baseada em componentes. FastAPI porque oferece alta performance, simplicidade no desenvolvimento de APIs e suporte eficiente a conexões assíncronas em tempo real (WebSocket/SSE).

- **Banco de dados**: PostgreSQL

Justificativa: PostgreSQL porque garante armazenamento confiável e estruturado dos dados, oferece bom desempenho e extensibilidade, permitindo análises futuras das trajetórias.

## Diagrama de alto nível

<center>

![DiagramaAltoNivel](assets/diagrama-arquitetura.png)

</center>