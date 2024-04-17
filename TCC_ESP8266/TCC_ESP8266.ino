/******************************************************************************
This sketch connects the ESP8266 to a MQTT broker and subcribes to the topic
******************************************************************************/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

const char *ssid = "brisa-1470450";   // name of your WiFi network
const char *password = "rmepsjew"; // password of the WiFi network

const char* mqttUser = "bruno";
const char* mqttPassword = "bruno";

const char *ID = "ESP8266-8e588452-4602-4d1c-aa49-9cd06bc44e7c";  // Name of our device, must be unique
const char *TOPIC = "smart_control_api";  // Topic to subcribe to

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
        digitalWrite(LED_BUILTIN, LOW);
      }
      else if(value.equals("TURN_OFF")) { // Turn the light off
        Serial.println("LED TURN_OFF");
        digitalWrite(LED_BUILTIN, HIGH);
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


void setup() {
  Serial.begin(9600); // Start serial communication at 115200 baud
  delay(100);
  setup_wifi(); // Connect to network
  client.setServer(broker, 1883);
  client.setCallback(callback);// Initialize the callback routine
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