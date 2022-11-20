#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define Buzzer D5
#define Green D6
#define Sensor A0
#define ORG "wf2kmp"
#define DEVICE_TYPE "GLMASFI_IOT_Device_Cloud_Service"
#define DEVICE_ID "PNT2022TMID35867"
#define TOKEN "PNT2022TMID35867"

const char* ssid = "Airtel-Hotspot-958A";
const char* password = "9889i1bb";
const char* host = "maker.ifttt.com";
const int httpsPort = 80;
char server[] = ORG ".messaging.internetofthings.ibmcloud.com";
char topic[] = "iot-2/evt/status/fmt/json";
char authMethod[] = "use-token-auth";
char token[] = TOKEN;
char clientId[] = "d:" ORG ":" DEVICE_TYPE ":" DEVICE_ID;

void PublishData(float);
void callback(char*, byte*, unsigned int);

WiFiClient client1;
WiFiClient client2;
PubSubClient client(server, 1883, callback, client2);

void setup() {
  pinMode(Green, OUTPUT);
  pinMode(Buzzer, OUTPUT);
  pinMode(Sensor, INPUT);
  Serial.begin(115200);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.print(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  } 
  Serial.println("");
  Serial.print("WiFi connected, IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("connecting to ");
  Serial.println(host);
  if (!client1.connect(host, httpsPort)) {
    Serial.println("connection failed");
    return;
  }
  if (!client2.connect(host, httpsPort)) {
    Serial.println("connection failed");
    return;
  }
}

int msgSent = 0;
void loop() {
  Serial.print("Reconnecting client to ");
  Serial.println(server);
  while (!client.connect(clientId, authMethod, token)) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  
  int sensor = analogRead(Sensor);
  Serial.println(String(sensor));
  PublishData(sensor);
  if (sensor >= 740 && !msgSent) {
    digitalWrite(Green, HIGH);
    digitalWrite(Buzzer, HIGH);
    String url = "/trigger/gasle/json/with/key/ktkqqpO7-nkuFo1Dc-jMZX4tNAKchaWS4E6SzY7btPA"; 
    Serial.print("Requesting URL: ");
    Serial.println(url);                 
    client1.print(String("GET ") + url + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n"); 
    msgSent = 1;
    Serial.println("Gas Concentration Level: High");
  }
  else if (sensor >= 740 && msgSent) {
    digitalWrite(Green, HIGH);
    digitalWrite(Buzzer, HIGH);
    Serial.println("Gas Concentration Level: High");
  }
  else if (sensor>=720){
    digitalWrite(Buzzer, HIGH);
    digitalWrite(Green, HIGH);
    delay(750);
    digitalWrite(Buzzer,LOW);
    digitalWrite(Green, LOW);
    delay(1000);
    Serial.println("Gas Concentration Level: Moderate");  
  }
  else{
    digitalWrite(Green, LOW);
    digitalWrite(Buzzer, LOW);
    Serial.println("Gas Concentration Level: Normal");
  }
  delay(1000);
}

void callback(char* topic, byte* payload, unsigned int length) {
 Serial.println("callback invoked");
}

void PublishData(float senso){
  String payload;
  payload= "{\"Gas_Concentration\":";
  payload += senso;
  payload+="}";
  
  Serial.print("Sending payload: ");
  Serial.println(payload);
  if (client.publish(topic, (char*) payload.c_str())) {
     Serial.println("Publish ok");
  } 
  else {
     Serial.println("Publish failed");
  }
}
