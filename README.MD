# Projeto: Medidor de temperatura e umidade: uma solução com interface gráfica de monitoramento 

## 1. Descrição do Funcionamento e Uso

Este projeto consiste em um sistema de monitoramento de temperatura e umidade utilizando um sensor DHT11 conectado a uma placa NodeMCU ESP8266 CP2102, os dados são coletados e enviados via protocolo MQTT para uma interface de visualização no Node-RED, o sistema inclui um indicador de status em formato de semáforo LED, que informa visualmente as condições de temperatura com base em limites pré-estabelecidos. 

Para reproduzir este projeto, é necessário um ambiente de desenvolvimento para o NodeMCU ESP8266 (Arduino IDE) e o Node-RED para a visualização dos dados, a comunicação é feita via Wi-Fi, permitindo o monitoramento remoto dos dados de temperatura e umidade.

## 2. Software e Documentação do Código

### Código do NodeMCU ESP8266

O código do microcontrolador é fornecido no arquivo [`mqtt_esp8266.ino`](https://github.com/RafaelTozzo/projeto_obj_int/blob/main/mqtt_esp8266.ino). Este código, desenvolvido na **Arduino IDE**, realiza as seguintes funções:
- Conecta o NodeMCU à rede Wi-Fi.
- Lê os valores de temperatura e umidade do sensor DHT11.
- Publica os dados de temperatura e umidade via MQTT para um broker configurado.
- Controla o módulo de LED semáforo com base nos valores de temperatura.
- Link Arduino IDE: [https://www.arduino.cc/](https://www.arduino.cc/)

### Código da Interface Node-RED

A interface de monitoramento pode ser replicada importando o arquivo [`node-red_script.json`](https://github.com/RafaelTozzo/projeto_obj_int/blob/main/node-red_script.json) no Node-RED. Essa interface exibe:
- Medidores gráficos para a temperatura e a umidade.
- Indicador visual de semáforo LED para o status da temperatura (verde, amarelo e vermelho).
- Link Node-RED: [https://nodered.org/](https://nodered.org/)

### Documentação do Código

Cada trecho de código no arquivo `mqtt_esp8266.ino` contém comentários explicativos sobre sua funcionalidade, incluindo a configuração Wi-Fi, MQTT, leitura do sensor e controle dos LEDs. A interface do Node-RED foi projetada para ser intuitiva, e os fluxos são nomeados para facilitar a compreensão.

- **Conexão Wi-Fi:** Configurada com a biblioteca `ESP8266WiFi.h`, gerencia a conexão à rede e eventos de conexão/desconexão.
- **Publicação MQTT:** Implementada com a biblioteca `AsyncMqtt_Generic.h`, que permite a publicação de dados em tópicos específicos para temperatura e umidade.
- **Leitura de Sensores:** Realizada pela biblioteca `DHT.h`, que obtém os valores do sensor DHT11.
- **Controle de LEDs:** Baseado nos valores lidos do sensor, usando a lógica para ativar os LEDs indicativos de condições térmicas.


## 3. Hardware Utilizado

### Placa de Desenvolvimento

- **NodeMCU ESP8266 CP2102**: Placa de microcontrolador com suporte a Wi-Fi, que executa o código responsável pela leitura do sensor e pela comunicação MQTT.

### Sensores e Atuadores

- **Sensor DHT11**: Sensor digital que mede a temperatura e a umidade do ambiente, com faixa de temperatura de -20°C a 60°C e umidade de 5% a 95%.
- **Módulo Semáforo LED**: Indicador com LEDs de 8mm (verde, amarelo e vermelho) que sinaliza o status da temperatura.

### Componentes Adicionais

- **Protoboard**: Utilizado para a montagem dos circuitos de forma rápida e sem necessidade de solda.
- **Cabos Jumpers**: Conectam os diferentes componentes eletrônicos na protoboard.

## 4. Documentação das Interfaces, Protocolos e Módulos de Comunicação

### Interface MQTT

- **Broker MQTT**: O sistema utiliza um broker público para transmitir dados. O endereço padrão utilizado é `test.mosquitto.org`, mas pode ser alterado conforme a necessidade.
- **Tópicos**:
  - **mts/dht/temp**: Tópico para publicação dos dados de temperatura.
  - **mts/dht/hum**: Tópico para publicação dos dados de umidade.

### Interface Node-RED

O Node-RED é utilizado como uma plataforma de visualização dos dados transmitidos pelo MQTT. A interface foi projetada para facilitar o monitoramento das variáveis de temperatura e umidade em tempo real e inclui:
- **Medidores de Temperatura e Umidade**: Gráficos que exibem os valores em tempo real.
- **Semáforo LED Virtual**: Indicadores que refletem as condições térmicas do ambiente, com base nos limites de segurança.

### Módulos de Comunicação

- **ESP8266WiFi.h**: Biblioteca para conectar o NodeMCU à rede Wi-Fi.
- **AsyncMqtt_Generic.h**: Biblioteca para comunicação MQTT assíncrona, que facilita a publicação e assinatura de tópicos de maneira eficiente.
- **DHT.h**: Biblioteca para o sensor DHT11, que fornece funções para leitura de temperatura e umidade.

---

