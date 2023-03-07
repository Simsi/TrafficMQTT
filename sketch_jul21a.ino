#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <stdlib.h>
#include <string>

const char* ssid = "PrinterNetwork";
const char* password =  "robotics";
const char* mqtt_server = "192.168.0.111";
const int mqtt_port = 1883;
//const char* host = "192.168.1.93"; // only electronics-fun.com not https://electronics-fun.com
int REDLED = 13;   // led connected to GPIO2 (D4)
int YELLOWLED = 12;
int GREENLED = 14;

struct State
{
  bool g;
  bool y;
  bool r;
  int duration;
  int id;
};

State nothing = {0,0,0,1};
State green6 = {1,0,0,6};
State green1 = {1,0,0,1};
State yellow3 = {0,1,0,3};
State yellow1 = {0,1,0,1};
State red6 = {0,0,1,6};
State red1 = {0,0,1,1};
State yellow_red = {0,1,1,3};

State mode[16] = {green6, nothing, green1, nothing, green1, nothing, green1, yellow3, red6, red1, red1, red1, red1, red1, red1, yellow_red};

const int httpPort = 80; // 80 is for HTTP / 443 is for HTTPS!
int current_state = 0;

WiFiClient espClient;
PubSubClient client(espClient);

// 192.168.1.118

void handle_state(State s)
{
  digitalWrite(GREENLED, s.g);
  digitalWrite(YELLOWLED, s.y);
  digitalWrite(REDLED, s.r);
  delay(s.duration * 1000);
}

void setup() 
{
  pinMode(REDLED, OUTPUT);
  pinMode(YELLOWLED, OUTPUT);
  pinMode(GREENLED, OUTPUT);
  digitalWrite(REDLED, LOW);
  digitalWrite(GREENLED, HIGH);
  digitalWrite(YELLOWLED, LOW);
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
  client.setCallback(MQTTcallback);
  while (!client.connected()) 
  {
    Serial.println("Connecting to MQTT...");
    if (client.connect("ESP8266_2"))
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









void MQTTcallback(char* topic, byte* payload, unsigned int length) 
{
  Serial.println("callback");
  Serial.print("Message received in topic: ");
  Serial.println(topic);
  Serial.print("Message:");
  String message;
  for (int i = 0; i < length; i++) 
  {
    message = message + (char)payload[i];
  }
  Serial.print(message);
  int f = message.toInt();
  handle_state(mode[(f + 8) % 16]);
  Serial.println();
  Serial.println("-----------------------");
}


void loop()
{

  Serial.println("loop");
  client.loop();

/////////////////////////////////////////////////////////////////////////////////////////
// char g[3];
// itoa(current_state, g, 10);
// client.publish("esp/test", g);
// handle_state(mode[current_state]);
// current_state = (current_state + 1) % 16;


  

  ////////////////////////////////////////////////////////////////////////////////////////

  // WiFiClient client = server.available();

  // if (!client){ 
  //   return;
  // }
  // Serial.println("Waiting for new client");

  // while(!client.available()){
  //   delay(1);
  // }

  // String request = client.readStringUntil('\r');
  // Serial.println(request);
  // client.flush();
  
  //  digitalWrite(LED,LOW);
  //  delay(200);
  //  digitalWrite(LED, HIGH);
  //  delay(200);
  //  digitalWrite(LED, LOW);
  //  delay(200);
  //  digitalWrite(LED, HIGH);
  //  delay(200);

  // int value = LOW;

  // if(request.indexOf("/LED=ON") != -1){
  //   digitalWrite(LED, LOW); // Turn LED ON
  //   value = HIGH;
  // }

  // if(request.indexOf("/LED=OFF") != -1)

  // {

  //   digitalWrite(LED, LOW); // Turn LED OFF

  //   value = LOW;

  // }
}