/******************************************************************************
This sketch connects the ESP32 to a MQTT broker and subcribes to the topic
******************************************************************************/

#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

const char *ssid = "brisa-1470450";   // name of your WiFi network
const char *password = "rmepsjew"; // password of the WiFi network

const char* mqttUser = "bruno";
const char* mqttPassword = "bruno";

const char *ID = "ESP32-a17f185b-dd59-43be-91a8-226e1236b1ba";  // Name of our device, must be unique
const char *TOPIC = "smart_control_api";  // Topic to subcribe to

int temperature = 0;
char message[8];

IPAddress broker(192,168,0,8); // IP address of your MQTT broker eg. 192.168.1.50
WiFiClient wclient;
PubSubClient client(wclient); // Setup MQTT client

// Parse incomming messages from the broker
void parseMessage(String json) {
  StaticJsonDocument<300> doc;
  DeserializationError error = deserializeJson(doc, json);

  bool hasTurnOn = false;

  if (error) {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.f_str());
  }

  for (size_t i = 0; i < doc.size(); i++) {
    String ip = doc[i]["ip"];
    String value = doc[i]["value"];

    if(ip.equals(WiFi.localIP().toString())){
      if(value.equals("TURN_ON")) { // Turn the light on
        Serial.println("LED TURN_ON");
        digitalWrite(LED_BUILTIN, HIGH);
      }
      else if(value.equals("TURN_OFF")) { // Turn the light off
        Serial.println("LED TURN_OFF");
        digitalWrite(LED_BUILTIN, LOW);
      } else { // Command not found
        Serial.println("Command not found for your device");
      }
      
      return;
    }
  }
}

// Handle incomming messages from the broker
void callback(char* topic, byte* payload, unsigned int length) {
  String response;

  for (int i = 0; i < length; i++) {
    response += (char)payload[i];
  }
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  
  parseMessage(response);
}

// Connect to WiFi network
void setup_wifi() {
  Serial.print("\nConnecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password); // Connect to network

  while (WiFi.status() != WL_CONNECTED) { // Wait for connection
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

// Reconnect to client
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if(client.connect(ID, mqttUser, mqttPassword )) {
      client.subscribe(TOPIC);
      Serial.println("connected");
      Serial.print("Subcribed to: ");
      Serial.println(TOPIC);
      Serial.println('\n');

    } else {
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void verify_temperature(){
  while (!client.connected()) {
    delay(5000);
    temperature += random(-10,10);

    if (temperature < -5){
      Serial.print("Temperatura Atual: ");
      Serial.println(temperature);
      Serial.println("Desligando Resfriador Industrial");
      dtostrf('{"topic":"smart_control_api","message":[{"ip":"192.168.0.9","value":"TURN_OFF"}]}', 1, 2, message);
      client.publish(TOPIC, message);
    } else if (temperature <= 8){
      Serial.print("Temperatura Atual: ");
      Serial.println(temperature);
      Serial.println("Ligando Resfriador Industrial");
      dtostrf('{"topic":"smart_control_api","message":[{"ip":"192.168.0.9","value":"TURN_ON"}]}', 1, 2, message);
      client.publish(TOPIC, message);

    }
  }
}

void setup() {
  Serial.begin(115200); // Start serial communication at 115200 baud
  delay(100);
  setup_wifi(); // Connect to network
  client.setServer(broker, 1883);
  client.setCallback(callback);// Initialize the callback routine
  verify_temperature();
  // Set pin mode
  pinMode(LED_BUILTIN,OUTPUT);
}

void loop() {
  if (!client.connected())  // Reconnect if connection is lost
  {
    reconnect();
  }
  client.loop();
}