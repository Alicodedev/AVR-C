
#include <ArduinoJson.h>
//#include <ArduinoJson.hpp>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const int led = 12; // Define the LED pin
const int digitalPin = D7; // KY-028 digital interface (D3 on NodeMCU)
const int analogPin = A0; // KY-028 analog interface (A0 on NodeMCU)
int digitalVal; // Digital readings
int analogVal; // Analog readings



// Change the credentials below, so your ESP8266 connects to your router
const char* ssid = "BELL444";
const char* password = "7A196A9CFEE5";

// Change the variable to your Raspberry Pi IP address, so it connects to your MQTT broker
const char* mqtt_server = "192.168.2.247";

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE  (50)
char msg[MSG_BUFFER_SIZE];
int value = 0;


// Don't change the function below. This functions connects your ESP8266 to your router
void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("WiFi connected - ESP IP address: ");
  Serial.println(WiFi.localIP());
  client.setServer(mqtt_server, 1883);

}


// This functions is executed when some device publishes a message to a topic that your ESP8266 is subscribed to
// Change the function below to add logic to your program, so when a device publishes a message to a topic that 
// your ESP8266 is subscribed you can actually do something

void callback(char* topic, byte* payload, unsigned int length) {

}



// This functions reconnects your ESP8266 to your MQTT broker
// Change the function below if you want to subscribe to more topics with your ESP8266 



// The setup function sets your ESP GPIOs to Outputs, starts the serial communication at a baud rate of 115200
// Sets your mqtt broker and sets the callback function
// The callback function is what receives messages and actually controls the LEDs
void setup() {
  pinMode(led,OUTPUT);
  pinMode(digitalPin,INPUT);
  delay(100);
  Serial.begin(9600);
  setup_wifi();
  //client.setCallback(callback);
}

// For this project, you don't need to change anything in the loop function. 
// Basically it ensures that you ESP is connected to your broker
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  StaticJsonDocument<32> doc;
  char output[55];


  long now = millis();
  if (now - lastMsg > 5000) {

    lastMsg = now;


    digitalVal = digitalRead(digitalPin);
    if (digitalVal == HIGH) // If temperature threshold reached
    {
      digitalWrite(led, HIGH); // Turn ON the LED
    }

    else
    {
      digitalWrite(led, LOW); // Turn OFF the LED
    }

  // Read the analog interface
    analogVal = analogRead(analogPin);
    doc["t"] = analogVal;
    Serial.println(analogVal); // Print analog value to seria
    delay(100);
  
    serializeJson(doc, output);
    Serial.println(output);
    client.publish("/home/sensors", output);
    Serial.println("Sent");
  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
     /*
     YOU  NEED TO CHANGE THIS NEXT LINE, IF YOU'RE HAVING PROBLEMS WITH MQTT MULTIPLE CONNECTIONS
     To change the ESP device ID, you will have to give a unique name to the ESP8266.
     Here's how it looks like now:
       if (client.connect("ESP8266Client")) {
     If you want more devices connected to the MQTT broker, you can do it like this:
       if (client.connect("ESPOffice")) {
     Then, for the other ESP:
       if (client.connect("ESPGarage")) {
      That should solve your MQTT multiple connections problem

     THE SECTION IN loop() function should match your device name
    */
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      delay(5000);
    }
  }
}