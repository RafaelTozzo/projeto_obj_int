#include <ESP8266WiFi.h>
#include <Ticker.h>
#include <AsyncMqtt_Generic.h>
#include <DHT.h>

// Definição dos pinos e configurações básicas
#define DHT_PIN D1
#define DHT_TYPE DHT11
#define LED_RED_PIN D5
#define LED_YELLOW_PIN D6
#define LED_GREEN_PIN D7

// Configurações de Wi-Fi e MQTT
const char* ssid = "Wi-Fi network"; // Adicionar sua rede Wi-Fi
const char* password = "Wi-Fi password"; // Adicionar a senha da rede Wi-Fi
#define MQTT_HOST "test.mosquitto.org" // Adiciona servidor MQTT público, no caso utilizei o Mosquitto
#define MQTT_PORT 1883 // Porta padrão do MQTT

// Tópicos MQTT
const char *PubTempTopic = "mts/dht/temp";
const char *PubHumTopic = "mts/dht/hum";

// Inicialização dos objetos e variáveis
DHT dht(DHT_PIN, DHT_TYPE);
AsyncMqttClient mqttClient;

Ticker mqttReconnectTimer;
Ticker wifiReconnectTimer;

float temperature;
float humidity;

unsigned long lastPublishTime = 0;
const long publishInterval = 2000; // Publicação a cada 2 segundos

// Conecta ao Wi-Fi
void connectToWifi() {
  Serial.println("Connecting to Wi-Fi...");
  WiFi.begin(ssid, password); // Inicia a conexão com a rede Wi-Fi
  while (WiFi.status() != WL_CONNECTED) { // Aguarda a conexão ser estabelecida
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to Wi-Fi"); // Exibe a confirmação da conexão
}

// Conecta ao broker MQTT
void connectToMqtt() {
  Serial.println("Connecting to MQTT...");
  mqttClient.connect(); // Inicia a conexão com o broker MQTT
}

// Evento chamado quando o Wi-Fi é conectado com sucesso
void onWifiConnect(const WiFiEventStationModeGotIP& event) {
  Serial.print("Connected to Wi-Fi. IP address: ");
  Serial.println(WiFi.localIP()); // Exibe o IP obtido
  connectToMqtt(); // Conecta ao broker MQTT
}

// Evento chamado quando o Wi-Fi é desconectado
void onWifiDisconnect(const WiFiEventStationModeDisconnected& event) {
  Serial.println("Disconnected from Wi-Fi.");
  mqttReconnectTimer.detach(); // Para tentativas de reconexão MQTT
  wifiReconnectTimer.once(2, connectToWifi); // Reagenda a conexão Wi-Fi
}

// Evento chamado quando o MQTT se conecta com sucesso
void onMqttConnect(bool sessionPresent) {
  Serial.println("Connected to MQTT broker."); // Confirma conexão com o broker
}

// Evento chamado quando o MQTT é desconectado
void onMqttDisconnect(AsyncMqttClientDisconnectReason reason) {
  Serial.println("Disconnected from MQTT. Reconnecting...");
  if (WiFi.isConnected()) { // Se ainda estiver conectado ao Wi-Fi, tenta reconectar ao MQTT
    mqttReconnectTimer.once(2, connectToMqtt);
  }
}

// Evento chamado quando uma publicação MQTT é reconhecida pelo broker
void onMqttPublish(uint16_t packetId) {
  Serial.printf("Publish acknowledged. PacketId: %d\n", packetId);
}

// Lê os dados do sensor e publica via MQTT
void readAndPublishSensorData() {
  Serial.println("Reading sensor data...");
  temperature = dht.readTemperature(); // Lê a temperatura do sensor
  humidity = dht.readHumidity(); // Lê a umidade do sensor

  if (!isnan(temperature) && !isnan(humidity)) { // Verifica se as leituras são válidas
    // Publica a temperatura
    char tempMsg[8];
    dtostrf(temperature, 6, 2, tempMsg); // Formata o valor para string
    uint16_t packetIdPubTemp = mqttClient.publish(PubTempTopic, 1, false, tempMsg);
    Serial.printf("Published temperature: %s °C with PacketId: %d\n", tempMsg, packetIdPubTemp);

    // Publica a umidade
    char humMsg[8];
    dtostrf(humidity, 6, 2, humMsg); // Formata o valor para string
    uint16_t packetIdPubHum = mqttClient.publish(PubHumTopic, 1, false, humMsg);
    Serial.printf("Published humidity: %s %% with PacketId: %d\n", humMsg, packetIdPubHum);

    // Controle dos LEDs baseado na temperatura
    if (temperature > 30) { // Se temperatura acima de 30°C
      digitalWrite(LED_RED_PIN, HIGH);
      digitalWrite(LED_YELLOW_PIN, LOW);
      digitalWrite(LED_GREEN_PIN, LOW);
    } else if (temperature > 25) { // Se temperatura entre 25°C e 30°C
      digitalWrite(LED_RED_PIN, LOW);
      digitalWrite(LED_YELLOW_PIN, HIGH);
      digitalWrite(LED_GREEN_PIN, LOW);
    } else { // Se temperatura abaixo de 25°C
      digitalWrite(LED_RED_PIN, LOW);
      digitalWrite(LED_YELLOW_PIN, LOW);
      digitalWrite(LED_GREEN_PIN, HIGH);
    }
  } else {
    Serial.println("Failed to read from DHT sensor."); // Alerta em caso de falha na leitura
  }
}

// Configuração inicial
void setup() {
  Serial.begin(115200); // Inicia a comunicação serial

  // Configura os pinos dos LEDs
  pinMode(LED_RED_PIN, OUTPUT);
  pinMode(LED_YELLOW_PIN, OUTPUT);
  pinMode(LED_GREEN_PIN, OUTPUT);

  dht.begin(); // Inicia o sensor DHT

  Serial.println("Starting ESP8266 with DHT11 and MQTT");

  // Configura eventos de conexão e desconexão do Wi-Fi
  WiFi.onStationModeGotIP(onWifiConnect);
  WiFi.onStationModeDisconnected(onWifiDisconnect);

  // Configura eventos de conexão, desconexão e publicação no MQTT
  mqttClient.onConnect(onMqttConnect);
  mqttClient.onDisconnect(onMqttDisconnect);
  mqttClient.onPublish(onMqttPublish);
  mqttClient.setServer(MQTT_HOST, MQTT_PORT); // Define o servidor MQTT

  connectToWifi(); // Inicia a conexão Wi-Fi
}

// Loop principal
void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - lastPublishTime >= publishInterval) { // Verifica intervalo de publicação
    lastPublishTime = currentMillis;
    Serial.println("Publishing data...");
    readAndPublishSensorData(); // Publica os dados
  }

  // Verifica a conexão MQTT e tenta reconectar se necessário
  if (!mqttClient.connected()) {
    Serial.println("MQTT disconnected, attempting to reconnect...");
    connectToMqtt();
  }
}
