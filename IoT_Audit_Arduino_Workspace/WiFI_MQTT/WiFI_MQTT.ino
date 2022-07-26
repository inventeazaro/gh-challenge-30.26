/*********
  Rui Santos
  Complete project details at https://randomnerdtutorials.com
*********/

#include <WiFi.h>
#include <PubSubClient.h>

// Replace the next variables with your SSID/Password combination
const char* ssid = "your_wifi_network"; 
const char* password = "your_wifi_password"; 

// Add your MQTT Broker IP address, example:
const char* mqtt_server = "server address";
//const char* mqttUser = "your mqtt user";
//const char* mqttPassword = "your mqtt password";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

// LED Pin
const int ledPin = 4;

void setup() {
  Serial.begin(115200);

  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  pinMode(ledPin, OUTPUT);
}

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
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageReceived;

  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageReceived += (char)message[i];
  }
  Serial.println();

  // If a message is received on the topic test, print it
  if (String(topic) == "test") {
    Serial.println(messageReceived);
  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
  //  if (client.connect("ESP32Client",mqttUser,mqttPassword)) {
    if (client.connect("ESP32Client")) {
      Serial.println("connected");
      // Subscribe
      client.subscribe("test");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  long now = millis();
  if (now - lastMsg > 5000) {
    lastMsg = now;

    // Convert the value to a char array
    char messageSend[] = "[{\"degrees\":23,\"powerConsumption\":123}]";
    // dtostrf(temperature, 1, 2, tempString);
    Serial.println(messageSend);
    client.publish("the topic you wish to publish to", messageSend);
  }
}
