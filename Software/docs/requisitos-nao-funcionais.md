# Elicitação de Requisitos Não Funcionais 

## Introdução

<div align="justify">&emsp;&emsp;
A elicitação de requisitos não funcionais tem como propósito identificar e documentar as restrições, padrões de qualidade e condições de operação que o sistema web deve atender. Esses requisitos garantem que o software não apenas execute suas funções corretamente no que diz respeito à transmissão de comandos para o carrinho, mas também o faça de maneira eficiente, segura, estável e acessível ao usuário final do site. Tais requisitos definem critérios de desempenho, confiabilidade, usabilidade e compatibilidade tecnológica, assegurando que o sistema mantenha um comportamento previsível e adequado mesmo sob condições variadas de uso.
</div>

## Objetivo

<div align="justify">&emsp;&emsp;
O objetivo deste documento é apresentar os requisitos não funcionais levantados durante o processo de levantamento de requisitos do sistema. Esses requisitos servirão como base para o desenvolvimento, implementação e validação da solução, garantindo que o produto final atenda aos padrões de desempenho e qualidade esperados. Além disso, eles auxiliarão a equipe de desenvolvimento a antecipar riscos e definir métricas claras para avaliação do software que consistirá o projeto da disciplina.
</div>

## Metodologia

<div align="justify">&emsp;&emsp;
A elicitação dos requisitos não funcionais foi realizada a partir da análise das necessidades do projeto e das limitações tecnológicas envolvidas na comunicação entre a interface e o carrinho controlado por esp32. Foram consideradas boas práticas de engenharia de software, bem como fatores como desempenho, confiabilidade, segurança e usabilidade. A partir dessa análise, foi elaborada a tabela a seguir, contendo os requisitos identificados, cada um classificado com seu respectivo código e prioridade.
</div>

## Resultados

<div align="justify">&emsp;&emsp;
A tabela abaixo apresenta os requisitos não funcionais identificados, numerados de RNF05 a RNF16 devido a alguns já terem sido modelados através da priorização MoSCoW. Cada requisito foi definido com base na necessidade de garantir um funcionamento eficiente, seguro e intuitivo do sistema, abrangendo aspectos de desempenho, comunicação, integridade de dados e experiência do usuário.
</div>

| Código | Descrição | Prioridade |
| ------- | ---------- | ----------- |
| RNF05 | A transferência das instruções não pode ultrapassar 10 segundos. | Alta |
| RNF06 | A consolidação do trajeto realizado não pode ultrapassar 10 segundos. | Alta |
| RNF07 | A resposta com o serviço (requisições internas) não pode ultrapassar 5 segundos. | Alta |
| RNF08 | O sistema deve suportar no mínimo 5 trajetórias armazenadas simultaneamente sem degradação perceptível de desempenho. | Alta |
| RNF09 | O sistema deve ser capaz de detectar perda de conexão com o carrinho. | Alta |
| RNF10 | A conexão end-to-end entre o serviço e a ESP deve ser mantida de forma contínua. | Alta |
| RNF11 | O software deve garantir integridade dos dados durante a comunicação e armazenamento. | Alta |
| RNF12 | A interface deve ser intuitiva e responsiva, adaptando-se a mobile e desktop. | Alta |
| RNF13 | As mensagens de erro e status devem ser claras e informativas, evitando termos técnicos desnecessários. | Alta |
| RNF14 | O sistema deve permitir configuração dos parâmetros de comunicação com o carrinho sem necessidade de recompilar. | Alta |
| RNF15 | O software deve validar as instruções antes de enviá-las ao carrinho, prevenindo comandos incorretos ou malformados. | Alta |
| RNF16 | A interface deve ser compatível com os principais navegadores caso utilize uma camada web local. | Alta |
