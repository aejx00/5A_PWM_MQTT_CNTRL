#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "configuration.h"

/* By default PWM frequency is 1000Hz and we are using same 
   for this application hence no need to set */

WiFiClient espClient;
PubSubClient client(espClient);

int pwmToPercent(int p)
{
 return map(p, 0,100,0,1023); 
} 

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    //if (client.connect(clientId.c_str())) {
    if (client.connect(device_id, mqtt_user, mqtt_pass)) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      // client.publish(device_id, "hello world");
      // ... and resubscribe
      client.subscribe(device_id);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) 
{
  Serial.print("Message received in topic: ");
  Serial.println(topic);
  Serial.print("Message:");
  String message;
  int converted_message;
  for (int i = 0; i < length; i++) 
  {
    message = message + (char)payload[i];
  }

  // Serial.print(message);
  // Serial.println("\n");
  if (message == "on") 
  {
    Serial.println("ON RECIEVED\n");
    analogWrite(LEDpin, 1023);
  }
  else if (message == "off") 
  {
    Serial.println("OFF RECIEVED\n");
    analogWrite(LEDpin, 0);
  }
  else
  {
    Serial.println("VALUE RECIEVED\n");
    char* str = (char* )payload; //conversion from a byte array to a character array
    str[length]= '\0'; 
    int mess_int = atoi(str);
    Serial.print("Percent Val: ");
    Serial.print(mess_int);
    Serial.println("\n");
    int translated_val = pwmToPercent(mess_int);
    Serial.print("Converted Val: ");
    Serial.print(translated_val);
    Serial.println("\n");
    analogWrite(LEDpin, translated_val);
  }
}

void setup(){
  Serial.begin(115200);
  analogWrite(LEDpin, 0);  /* set initial 50% duty cycle */
  WiFi.setHostname(device_id);
  WiFi.begin(WIFI_SSID, WIFI_PASSWD);
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.print("Connected to WiFi :");
  Serial.println(WiFi.SSID());
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
  while (!client.connected()) 
  {
    Serial.println("Connecting to MQTT...");
    if (client.connect("ESP8266"))
    {
      Serial.println("connected");
      Serial.println(WiFi.localIP());
    }
    else
    {
      Serial.print("failed with state ");
      Serial.println(client.state());
      delay(2000);
    }
    client.subscribe(device_id);
}
}

void loop(){
  if (!client.connected()) {
      reconnect();
    }
  client.loop();
}
