#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#define LED D1
const char* ssid = "TP-Link_FC64";
const char* password =  "66558576";
const char* mqtt_server = "broker.emqx.io";
const int mqtt_port = 1883;
WiFiClient espClient;
PubSubClient client(espClient);
void setup() 
{
  pinMode(LED, OUTPUT);
  pinMode(D2,INPUT_PULLUP);
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
  //client.setCallback(MQTTcallback);
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
  client.publish("HI!");
  Serial.println("OUT");
  delay(1000);
  client.loop();
}
