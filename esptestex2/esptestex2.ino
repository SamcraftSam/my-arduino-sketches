

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* ssid = "TP-Link_FC64";
const char* password =  "66558576";
const char* mqtt_server = "broker.emqx.io";
const int mqtt_port = 1883;
WiFiClient espClient;
PubSubClient client(espClient);

void setup() 
{
//  dht.begin();
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
//  client.setCallback(MQTTcallback);
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

void loop(){
  //int chk = DHT11.read(DHT11PIN);
  //float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  //float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  //float f = dht.readTemperature(true);
  Serial.print("Humidity (%): ");
  //Serial.println(h);
  client.publish("Humidity %: ");
  //client.publish(h);
  Serial.print("Temperature (C): ");
  //Serial.println(t);
  client.publish("Temperature C: ");
  //client.publish(t);
  delay(1000);
  client.loop();
}
