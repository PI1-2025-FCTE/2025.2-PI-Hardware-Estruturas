# Roadmap
---

## Visão Geral das Entregas

| Semana | Foco Principal | Requisitos Atendidos |
|--------|----------------|---------------------|
| 1 | Infraestrutura e Configuração Base | RNF01, RNF16 |
| 2 | Comunicação TCP e Backend Core | RF02, RF03, RNF10, RNF11 |
| 3 | Interface de Comandos e Validação | RF04, RF07, RNF15, RNF12 |
| 4 | Execução e Monitoramento em Tempo Real | RF09, RF14, RF08, RNF05, RNF06 |
| 5 | Visualização e Histórico de Trajetórias | RF10, RF11, RF12, RF13, RNF07, RNF08 |
| 6 | Parada de Emergência, Refinamentos e Testes | RF08, RF15, RNF09, RNF13, RNF14 |

---

## Semana 1: Infraestrutura e Configuração Base
**Objetivo**: Estabelecer a base técnica do projeto

### Atividades

#### Backend (Python + FastAPI)
- [ ] Configurar ambiente de desenvolvimento Python
- [ ] Inicializar projeto FastAPI com estrutura de pastas
- [ ] Configurar CORS para permitir requisições do frontend
- [ ] Criar esqueleto das rotas principais (**/connect**, **/mission**, **/missions**)
- [ ] Configurar variáveis de ambiente (.env)

#### Frontend (React + TypeScript)
- [ ] Criar projeto React com TypeScript
- [ ] Configurar Tailwind CSS para estilização
- [ ] Estruturar componentes base (layout, navegação)
- [ ] Implementar roteamento (react-router-dom)
- [ ] Criar componente de status de conexão global

#### Database (PostgreSQL)
- [ ] Instalar e configurar PostgreSQL
- [ ] Criar database do projeto
- [ ] Script físico
- [ ] Configurar SQLAlchemy no backend
- [ ] Criar modelos ORM iniciais

#### Testes
- [ ] Configurar ambiente de testes unitários (pytest para backend)
- [ ] Verificar que API responde com rotas básicas
- [ ] Testar conexão com banco de dados

**Entrega**: Aplicação rodando localmente com backend, frontend e database conectados

---

## Semana 2: Comunicação TCP e Backend Core
**Objetivo**: Estabelecer comunicação confiável com ESP32

### Atividades

#### Backend - Serviço TCP
- [ ] Implementar **ESP32Service** com socket TCP client
- [ ] Criar método **connect(ip, port)** com tratamento de timeout
- [ ] Criar método **sendCommand(commandString)** com buffer management
- [ ] Implementar método **disconnect()**
- [ ] Adicionar detecção de perda de conexão (RNF09)
- [ ] Criar sistema de retry para comandos falhados

#### Backend - Encoding/Decoding
- [ ] Implementar **encodeCommands()** para converter JSON → protocolo TCP
  - Formato: **a** + 4 dígitos para frente, **e** para esquerda, **d** para direita
- [ ] Implementar **decodeCommands()** para parsear resposta do ESP32
- [ ] Implementar parser para extrair tempo de execução (**t** + 4 dígitos)
- [ ] Validar integridade dos comandos (checksum opcional - RNF11)

#### Backend - Rotas API
- [ ] **POST /api/esp32/connect** - Conectar ao ESP32
- [ ] **GET /api/esp32/status** - Verificar status da conexão
- [ ] **POST /api/esp32/disconnect** - Desconectar do ESP32

#### Frontend - Conexão
- [ ] Criar componente **ESP32Connection**
- [ ] Input para IP do ESP32
- [ ] Botão "Conectar/Desconectar"
- [ ] Indicador visual de status de conexão (conectado/desconectado)
- [ ] Mensagens de erro claras (RNF13)

#### Mockar ESP32 para Testes
- [ ] Criar servidor TCP mock em Python que simula ESP32
- [ ] Mock deve responder comandos com delay simulado
- [ ] Mock retorna formato correto: **[comandos]t[tempo]**

**Entrega**: Frontend consegue conectar ao mock ESP32 via backend, status é exibido corretamente

---

## Semana 3: Interface de Comandos e Validação
**Objetivo**: Permitir que usuário crie e valide trajetórias

### Atividades

#### Frontend - Planejamento de Trajetória
- [ ] Criar componente **CommandBuilder**
- [ ] Interface para adicionar comandos:
  - Botão "Avançar" → Input de distância (cm)
  - Botão "Virar à Esquerda"
  - Botão "Virar à Direita"
- [ ] Lista visual de comandos adicionados (sequência)
- [ ] Botão para remover comando individual
- [ ] Botão "Limpar tudo"
- [ ] Preview visual simples da trajetória (top-down básico)

#### Frontend - Validação de Comandos
- [ ] Validar distância (mínimo 1cm, máximo configurável)
- [ ] Mostrar erro se comando inválido (RNF15)
- [ ] Impedir envio se não houver comandos
- [ ] Impedir envio se não estiver conectado (US3)
- [ ] Confirmação antes de enviar ("Tem certeza?")

#### Backend - Validação
- [ ] Endpoint **POST /api/commands/validate**
- [ ] Validar formato dos comandos no backend
- [ ] Validar limites de distância
- [ ] Retornar erros específicos para cada tipo de problema
- [ ] Implementar validação de sequência lógica (opcional)

#### Frontend - Responsividade
- [ ] Adaptar interface para mobile (RNF12, RNF02)
- [ ] Testar em diferentes tamanhos de tela
- [ ] Garantir botões e inputs acessíveis no mobile

#### Testes
- [ ] Testar validação com comandos inválidos
- [ ] Testar limite de caracteres no protocolo
- [ ] Testar interface em Chrome, Firefox, Safari (RNF16)

**Entrega**: Usuário consegue criar sequência de comandos válida com interface intuitiva

---

## Semana 4: Execução e Monitoramento em Tempo Real
**Objetivo**: Executar trajetória e receber feedback em tempo real

### Atividades

#### Backend - Execução de Missão
- [ ] Endpoint **POST /api/mission/execute**
- [ ] Enviar comandos para ESP32 via TCP
- [ ] Aguardar resposta completa (até encontrar **t[4 dígitos]**)
- [ ] Implementar timeout de 30 segundos (RNF05)
- [ ] Tratar erros de comunicação
- [ ] Parsear comandos executados e tempo de execução

#### Backend - WebSocket/SSE para Tempo Real
- [ ] Implementar WebSocket ou Server-Sent Events (SSE)
- [ ] Enviar updates de status durante execução:
  - "Conectando..."
  - "Enviando comandos..."
  - "Executando..."
  - "Concluído" / "Erro"
- [ ] Broadcast de dados recebidos do ESP32 em tempo real (RF09)

#### Frontend - Execução
- [ ] Botão "Iniciar Trajetória" (US5)
- [ ] Conectar ao WebSocket/SSE
- [ ] Mostrar status atual da execução
- [ ] Barra de progresso ou indicador de loading
- [ ] Mostrar qual comando está sendo executado no momento
- [ ] Disable comandos enquanto execução está ativa

#### Frontend - Feedback em Tempo Real
- [ ] Componente **ExecutionMonitor**
- [ ] Exibir comandos executados até o momento
- [ ] Contador de tempo decorrido
- [ ] Animação simples mostrando movimento (opcional)

#### Backend - Performance
- [ ] Garantir resposta de API < 5 segundos (RNF07)
- [ ] Otimizar queries ao banco se necessário
- [ ] Implementar connection pooling para PostgreSQL

**Entrega**: Usuário envia comandos, sistema executa no ESP32 e exibe feedback em tempo real

---

## Semana 5: Visualização e Histórico de Trajetórias
**Objetivo**: Visualizar trajetórias e acessar histórico

### Atividades

#### Backend - Persistência
- [ ] Implementar serviço **MissionService**
- [ ] Método **saveMission()** - salvar no PostgreSQL (RF11)
- [ ] Calcular dados da trajetória:
  - Coordenadas (x, y) baseadas em comandos
  - Distância total percorrida
  - Pontos de virada
- [ ] Endpoint **GET /api/missions** - listar todas as missões (RF12)
- [ ] Endpoint **GET /api/missions/:id** - detalhes de uma missão (RF13)
- [ ] Garantir performance com 5+ trajetórias (RNF08)

#### Backend - Cálculo de Trajetória
- [ ] Implementar **calculateTrajectory(commands)**:
  - Iniciar em (0,0) apontando para norte (0°)
  - Para cada **a[distância]**: mover na direção atual
  - Para cada **e**: rotacionar -90°
  - Para cada **d**: rotacionar +90°
  - Retornar array de pontos **{x, y, angle}**

#### Frontend - Visualização de Trajetória
- [ ] Criar componente **TrajectoryViewer** com Canvas ou SVG
- [ ] Desenhar grid de fundo
- [ ] Desenhar path da trajetória (linha conectando pontos)
- [ ] Marcar ponto inicial (verde) e final (vermelho)
- [ ] Mostrar setas indicando direção
- [ ] Opção de zoom in/out
- [ ] Legendas com escala (ex: 1cm = 1px)

#### Frontend - Histórico
- [ ] Criar página **MissionHistory** (US1)
- [ ] Listar todas as trajetórias em cards
- [ ] Cada card mostra: data, duração, distância, status
- [ ] Mensagem "Nenhuma trajetória encontrada" se vazio
- [ ] Botão "Ver Detalhes" em cada card

#### Frontend - Detalhes da Missão
- [ ] Criar página **MissionDetails** (US2)
- [ ] Exibir comandos enviados vs executados
- [ ] Exibir tempo de execução
- [ ] Exibir trajetória no **TrajectoryViewer**
- [ ] Mostrar estatísticas: distância total, número de comandos, etc.
- [ ] Botão "Voltar" para lista

#### Testes
- [ ] Testar cálculo de trajetória com diversos comandos
- [ ] Verificar que dados salvos no DB estão corretos
- [ ] Testar performance com 10+ trajetórias

**Entrega**: Sistema completo de visualização e histórico funcionando

---

## Semana 6: Parada de Emergência, Refinamentos e Testes Finais
**Objetivo**: Funcionalidades críticas de segurança e polish do sistema

### Atividades

#### Backend - Parada de Emergência
- [ ] Criar protocolo de parada (ex: comando **s** - stop)
- [ ] Endpoint **POST /api/mission/stop** (RF08, US6)
- [ ] Enviar comando de parada via TCP imediatamente
- [ ] Interromper execução atual
- [ ] Salvar trajetória parcialmente executada
- [ ] Processar e gerar análises da trajetória interrompida

#### Frontend - Parada de Emergência
- [ ] Botão grande "PARAR" visível durante execução
- [ ] Estilo destacado (vermelho, grande)
- [ ] Confirmação visual após parada
- [ ] Mostrar que trajetória foi salva parcialmente

#### Frontend - Informações do Carrinho (Opcional - Could Have)
- [ ] Componente para mostrar status do ESP32 (RF15)
- [ ] Exibir nível de bateria (se disponível)
- [ ] Mostrar temperatura ou outros sensores

#### Backend - Configurações
- [ ] Endpoint **GET /api/config** e **PUT /api/config** (RNF14)
- [ ] Permitir configurar:
  - IP e porta padrão do ESP32
  - Timeout de conexão
  - Limites de distância
  - Unidade de medida (cm/m)
- [ ] Salvar configurações em arquivo JSON ou tabela config

#### Frontend - Configurações
- [ ] Página de configurações
- [ ] Formulário para editar parâmetros
- [ ] Salvar e aplicar sem recompilar (RNF14)

#### Refinamentos Gerais
- [ ] Melhorar mensagens de erro (RNF13)
  - Remover jargões técnicos
  - Sugerir soluções para erros comuns
- [ ] Adicionar loading states em todas as ações
- [ ] Implementar tratamento global de erros
- [ ] Adicionar logs no backend para debug
- [ ] Verificar responsividade em todos os componentes (RNF02)

#### Testes Finais
- [ ] Teste end-to-end completo:
  1. Conectar ao ESP32
  2. Criar trajetória
  3. Executar
  4. Visualizar em tempo real
  5. Parar (emergência)
  6. Ver histórico
- [ ] Teste de performance (RNF05, RNF06, RNF07)
- [ ] Teste de reconexão após perda de conexão
- [ ] Teste de múltiplos usuários (se aplicável)
- [ ] Teste de compatibilidade entre navegadores (RNF16)

#### Documentação
- [ ] README.md com instruções de instalação
- [ ] Documentação da API (Swagger/OpenAPI via FastAPI)
- [ ] Guia do usuário (como conectar, criar trajetória, etc.)
- [ ] Documentação do protocolo de comunicação ESP32
- [ ] Diagramas de arquitetura atualizados

**Entrega**: Sistema completo, testado, documentado e pronto para uso
