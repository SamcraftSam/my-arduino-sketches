#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "DHT.h"
#define DHTPIN D2
#define DHTTYPE DHT11
const char* ssid = "TP-Link_FC64";
const char* password =  "66558576";
const char* mqtt_server = "broker.emqx.io";
const int mqtt_port = 1883;
WiFiClient espClient;
PubSubClient client(espClient);
DHT dht(DHTPIN, DHTTYPE);
void setup() 
{
  dht.begin();
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.print("Connected to WiFi :");
  Serial.println(WiFi.SSID());
  client.setServer(mqtt_server, mqtt_port);
  //client.setCallback(callback);
  while (!client.connected()) 
  {
    Serial.println("Connecting to MQTT...");
    if (client.connect("ESP8266"))
    {
      Serial.println("connected");
    }
    else
    {
      Serial.print("failed with state ");
      Serial.println(client.state());
      delay(2000);
    }
  }
  client.subscribe("esp/test");
}
void loop() 
{
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  client.publish("esp/test1", "Humidity %: ");
  Serial.println("Humidity %: ");
  //client.publish("esp/test1", h);
  Serial.println(h);
  client.publish("esp/test1", "Temp C: ");
  Serial.println("Temp C: ");
  //client.publish("esp/test1", h);
  Serial.println(t);
  delay(3000);
  client.loop();
}
