#include <WiFiNINA.h>
#include <PubSubClient.h>

const char ssid[] = "FTTH";        
const char pass[] = "0Wy)]6M5Ncw=";  
const char mqtt_server[] = "broker.hivemq.com";
int mqtt_port = 1883;

int status = WL_IDLE_STATUS;

// Wifi Client
WiFiClient wifi_client;
PubSubClient client(wifi_client);

// Humidity and Temperature Constants
#include <DHT.h>

#define DHTPIN 2
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

#define Threshold 400

float hum, temp, sensorValue;

// MQ135 Air Quality Sensor
const int MQ2_PIN = A7;

void setup() {
  pinMode(DHTPIN, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  dht.begin();
  Serial.begin(9600);

  // Attempt to connect to WiFi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);

    status = WiFi.begin(ssid, pass);

    // Wait 10 seconds for connection:
    delay(10000);
  }

  Serial.println("Connected to wifi");

  client.setServer(mqtt_server, mqtt_port);

  // Connect to HiveMQ broker
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);  // Generate a unique client ID
    if (client.connect(clientId.c_str())) {
      Serial.println("Connected");
    } else {
      Serial.print("Failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);  // Wait for 5 seconds before retrying
    }
  }
}

void loop() {
  // Read sensor data
  readSensor();

  // Read MQ2 sensor data
  sensorValue = analogRead(MQ2_PIN);

  String value = "Humidity: " + String(hum) + " Temperature: " + String(temp) + " Air Quality: " + String(sensorValue);

  // Send Data to MQTT
  client.publish("sidhant/task", value.c_str());
  delay(1000);
}

void readSensor() {
  hum = dht.readHumidity();
  temp = dht.readTemperature();
  Serial.println("Humidity: " + String(hum) + " Temperature: " + String(temp) + " Air Quality: " + String(sensorValue));
}
