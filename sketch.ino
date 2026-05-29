#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>
#include <ESP32Servo.h>

// --- Definición de Pines de Actuadores ---
#define SERVO_PIN      4
#define BUZZER_PIN    13
#define RELAY_PIN     25
#define LED_AZUL      14
#define LED_ROJO      18

// --- Definición de Pines de Sensores ---
#define TRIG_PIN      12
#define ECHO_PIN      15
#define PIR_PIN       27
#define DHT_PIN       16
#define DHT_TYPE       DHT22
#define GAS_PIN       35
#define LDR_PIN       34

// --- Objetos ---
DHT dht(DHT_PIN, DHT_TYPE);
Servo portonServo;
WiFiClient espClient;
PubSubClient client(espClient);

// --- Credenciales WiFi y MQTT ---
const char* ssid = "Wokwi-GUEST";
const char* password = "";
const char* mqtt_server = "broker.hivemq.com";

// --- Variables de temporización ---
unsigned long lastMsg = 0;

void setup_wifi() {
  delay(10);
  Serial.println("\nConectando a WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n¡WiFi Conectado!");
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Mensaje recibido en [");
  Serial.print(topic);
  Serial.print("]: ");

  String message = "";
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  Serial.println(message);

  // --- Control del Relay (ventilador) ---
  if (String(topic) == "udb/domotica/control/ventilador") {
    if (message == "ON")  digitalWrite(RELAY_PIN, HIGH);
    else if (message == "OFF") digitalWrite(RELAY_PIN, LOW);
  }

  // --- Control del LED Azul (luces) ---
  if (String(topic) == "udb/domotica/control/luces") {
    if (message == "ON")  digitalWrite(LED_AZUL, HIGH);
    else if (message == "OFF") digitalWrite(LED_AZUL, LOW);
  }

  // --- Control del LED Rojo (alerta) ---
  if (String(topic) == "udb/domotica/control/alerta") {
    if (message == "ON")  digitalWrite(LED_ROJO, HIGH);
    else if (message == "OFF") digitalWrite(LED_ROJO, LOW);
  }

  // --- Control del Buzzer ---
  if (String(topic) == "udb/domotica/control/buzzer") {
    if (message == "ON") {
      digitalWrite(BUZZER_PIN, HIGH);
      delay(200);
      digitalWrite(BUZZER_PIN, LOW);
    }
  }

  // --- Control del Servo (portón) ---
  if (String(topic) == "udb/domotica/control/servo") {
    if (message == "ABRIR") portonServo.write(90);
    else if (message == "CERRAR") portonServo.write(0);
  }
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Intentando conexión MQTT...");
    String clientId = "ESP32Client-" + String(random(0, 10000));
    if (client.connect(clientId.c_str())) {
      Serial.println("¡Conectado al Broker!");
      // Suscribirse a TODOS los tópicos de control
      client.subscribe("udb/domotica/control/ventilador");
      client.subscribe("udb/domotica/control/luces");
      client.subscribe("udb/domotica/control/alerta");
      client.subscribe("udb/domotica/control/buzzer");
      client.subscribe("udb/domotica/control/servo");
    } else {
      Serial.print("Falló, rc=");
      Serial.print(client.state());
      Serial.println(" Intentando de nuevo en 5 segundos");
      delay(5000);
    }
  }
}

long leerDistancia() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  return pulseIn(ECHO_PIN, HIGH) * 0.034 / 2;
}

void setup() {
  Serial.begin(115200);
  dht.begin();
  portonServo.attach(SERVO_PIN, 500, 2400);
  portonServo.write(0);

  pinMode(RELAY_PIN, OUTPUT);
  pinMode(LED_AZUL, OUTPUT);
  pinMode(LED_ROJO, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(PIR_PIN, INPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  unsigned long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;

    // 1. Lectura de Clima
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    if (!isnan(h) && !isnan(t)) {
      client.publish("udb/domotica/sensor/temperatura", String(t).c_str());
      client.publish("udb/domotica/sensor/humedad", String(h).c_str());
      if (t > 30.0) digitalWrite(RELAY_PIN, HIGH);
      else          digitalWrite(RELAY_PIN, LOW);  // FIX: apagar relay si baja la temperatura
    }

    // 2. Lectura de Iluminación — FIX: agregado else para apagar el LED
    int luzRaw = analogRead(LDR_PIN);
    client.publish("udb/domotica/sensor/luz", String(luzRaw).c_str());
    if (luzRaw > 2000) digitalWrite(LED_AZUL, HIGH);
    else               digitalWrite(LED_AZUL, LOW);  // FIX: apaga cuando hay luz

    // 3. Sistema de Presencia (Intrusos)
    int presencia = digitalRead(PIR_PIN);
    client.publish("udb/domotica/sensor/presencia", String(presencia).c_str());
    if (presencia == HIGH) {
      digitalWrite(BUZZER_PIN, HIGH);
      delay(200);
      digitalWrite(BUZZER_PIN, LOW);
    }

    // 4. Control de Acceso (Vehículos)
    long distancia = leerDistancia();
    client.publish("udb/domotica/sensor/distancia", String(distancia).c_str());
    if (distancia < 20 && distancia > 0) {
      portonServo.write(90);
    } else {
      portonServo.write(0);
    }

    // 5. Monitoreo de Gas Crítico
    int nivelGas = analogRead(GAS_PIN);
    client.publish("udb/domotica/sensor/gas", String(nivelGas).c_str());
    if (nivelGas > 2500) {
      digitalWrite(LED_ROJO, HIGH);
    } else {
      digitalWrite(LED_ROJO, LOW);
    }

    Serial.printf("Temp: %.1f°C | Hum: %.1f%% | Luz: %d | Dist: %ldcm | Gas: %d | PIR: %d\n",
                  t, h, luzRaw, distancia, nivelGas, presencia);
  }
}
