#include <ESP8266WiFi.h>
#include <PubSubClient.h>
//kikiwifi
const char* ssid = "kikiwifi"; // Enter your WiFi name
const char* password =  "kikiwifi"; // Enter WiFi password
const char* mqttServer = "parkom.mobilisis.hr";
const int mqttPort = 1883;
const char* mqttUser = "guest";
const char* mqttPassword = "guest";

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  pinMode(D7, OUTPUT);
  pinMode(D8, OUTPUT);

  Serial.begin(115200);

  connectToWifi();
}

void connectToWifi() {
  Serial.println("WiFi.begin");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(2000);
    Serial.println("Connecting to WiFi... ");
    //Serial.println(ssid);
  }
  Serial.println("Connected to the WiFi network");
  connectToMQTT();
}

void connectToMQTT() {
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);

  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");

    if (client.connect("ESP8266Client", mqttUser, mqttPassword )) {
      Serial.println("Connected to MQTT");
    } else {
      Serial.print("failed with state " + client.state());
      delay(2000);
    }
  }

  //client.publish("pojer/test", "hello"); //Topic name
  client.subscribe("pojer/test");
}

void callback(char* topic, byte* payload, unsigned int length) {

  Serial.print("Message arrived in topic: ");
  Serial.println(topic);

  Serial.print("Message:");

  String mess = "";
  for (int i = 0; i < length; i++) {
    //Serial.print((char)payload[i]);
    mess += (char)payload[i];
  }

  String msgString = mess;
  Serial.println(msgString);


  if (msgString == "ON7")
  {
    digitalWrite(D7, HIGH);
  }
  if (msgString == "OFF7")
  {
    digitalWrite(D7, LOW);
  }
  if (msgString == "ON8")
  {
    digitalWrite(D8, HIGH);
  }
  if (msgString == "OFF8")
  {
    digitalWrite(D8, LOW);
  }

  //Serial.println();
  Serial.println("-----------------------");

}

void loop() {
  if (WiFi.status() != WL_CONNECTED)
  {
    connectToWifi();
  }

  client.loop();
}
