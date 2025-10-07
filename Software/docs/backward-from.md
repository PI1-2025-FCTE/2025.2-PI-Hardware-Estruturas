# Backward From

## Introdução

<div align="justify">&emsp;&emsp;
A matriz de rastreabilidade Backward From é uma ferramenta utilizada no planejamento de projetos ou processos, com foco em começar pelo objetivo final e, a partir dele, mapear as etapas anteriores necessárias para alcançá-lo. Essa abordagem garante que todas as atividades estejam alinhadas diretamente ao resultado esperado, evitando esforços dispersos ou não prioritários.
</div>

## Objetivo

<div align="justify">&emsp;&emsp;
O objetivo deste diagrama é representar de forma clara como o método Backward From organiza atividades e metas a partir do resultado final desejado, permitindo visualizar as relações de dependência entre entregáveis e as etapas necessárias para alcançá-los.
</div>

## Metodologia

<div align="justify">&emsp;&emsp;
A metodologia Backward From parte da definição do estado final ou objetivo estratégico. Em seguida, identifica-se quais marcos intermediários são essenciais, e depois, quais atividades permitem atingir cada marco. Essa construção é realizada em sentido reverso (do fim para o início), mas serve para guiar a execução no sentido direto (do início para o fim).
</div>

## Resultados

<div align="justify">&emsp;&emsp;
O resultado é um diagrama que mostra, de forma hierárquica e encadeada, as atividades necessárias para atingir o objetivo final. Essa visão ajuda a equipe a priorizar tarefas e a compreender a importância de cada entrega parcial no caminho para o resultado completo.
</div>

| ID    | Descrição                                                                 | Origem | Implementado | Prioridade |
| ------ | ------------------------------------------------------------------------- | ------- | ------------- | ----------- |
| RF01 | Fornecer uma interface gráfica para o usuário. | [MoSCoW](https://github.com/EricAraujoBsB/PI1-Grupo2/blob/main/Software/docs/moscow.md) | A fazer | Alta |
| RF02 | Estabelecer e manter comunicação sem fio com o hardware do carrinho (ESP32). | [MoSCoW](https://github.com/EricAraujoBsB/PI1-Grupo2/blob/main/Software/docs/moscow.md) | A fazer | Alta |
| RF03 | Permitir que o usuário inicie conexão com um carrinho específico na rede. | [MoSCoW](https://github.com/EricAraujoBsB/PI1-Grupo2/blob/main/Software/docs/moscow.md) | A fazer | Alta |
| RF04 | Criar novas trajetórias combinando comandos pré-definidos (ex.: "Avançar x cm", "Virar à esquerda", "Virar à direita"). | [MoSCoW](https://github.com/EricAraujoBsB/PI1-Grupo2/blob/main/Software/docs/moscow.md) | A fazer | Alta |
| RF07 | Enviar a sequência de comandos planejada para o carrinho e iniciar a execução da trajetória. | [MoSCoW](https://github.com/EricAraujoBsB/PI1-Grupo2/blob/main/Software/docs/moscow.md) | A fazer | Alta |
| RF08 | Disponibilizar comando de parada de emergência para interromper a trajetória em execução. | [MoSCoW](https://github.com/EricAraujoBsB/PI1-Grupo2/blob/main/Software/docs/moscow.md) | A fazer | Alta |
| RF09 | Receber e processar dados do carrinho em tempo real durante a execução da trajetória. | [MoSCoW](https://github.com/EricAraujoBsB/PI1-Grupo2/blob/main/Software/docs/moscow.md) | A fazer | Alta |
| RF11 | Persistir dados das trajetórias concluídas em um banco de dados. | [MoSCoW](https://github.com/EricAraujoBsB/PI1-Grupo2/blob/main/Software/docs/moscow.md) | A fazer | Alta |
| RF12 | Listar todas as trajetórias salvas anteriormente. | [MoSCoW](https://github.com/EricAraujoBsB/PI1-Grupo2/blob/main/Software/docs/moscow.md) | A fazer | Alta |
| RF13 | Visualizar detalhadamente uma trajetória antiga, incluindo a exibição do percurso em gráfico. | [MoSCoW](https://github.com/EricAraujoBsB/PI1-Grupo2/blob/main/Software/docs/moscow.md) | A fazer | Alta |
| RF14 | Executar trajetórias de forma autônoma após o envio dos comandos, sem intervenção humana. | [MoSCoW](https://github.com/EricAraujoBsB/PI1-Grupo2/blob/main/Software/docs/moscow.md) | A fazer | Alta |
| RF10 | Exibir graficamente a trajetória atual do carrinho em tempo real. | [MoSCoW](https://github.com/EricAraujoBsB/PI1-Grupo2/blob/main/Software/docs/moscow.md) | A fazer | Média |
| RNF02 | Tornar a interface web responsiva, adaptando-se a diferentes tamanhos de tela. | [MoSCoW](https://github.com/EricAraujoBsB/PI1-Grupo2/blob/main/Software/docs/moscow.md) | A fazer | Média |
| RNF04 | Manter comunicação contínua e estável entre a interface e o carrinho durante a operação. | [MoSCoW](https://github.com/EricAraujoBsB/PI1-Grupo2/blob/main/Software/docs/moscow.md) | A fazer | Média |
| RF15 | Exibir informações básicas do carrinho conectado, como nível da bateria e estado atual. | [MoSCoW](https://github.com/EricAraujoBsB/PI1-Grupo2/blob/main/Software/docs/moscow.md) | A fazer | Baixa |