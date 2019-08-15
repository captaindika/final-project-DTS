  // Blynk specific
  #define BLYNK_PRINT Serial
  #define PIN_UPTIME V5
  
  #include <WiFi.h>
  #include <PubSubClient.h>
  
  #include <BlynkSimpleEsp32.h>

  // Credentials
  char auth[] = "ksJ275N1xwiQwSurp8juWykDRK8uOc-t";
  const char* ssid = "0xABADBEEF";
  const char* password = "fuckmewkwkwk1234";
  const char* mqtt_server = "dika.exbi.xyz";
  const char* mqtt_user = "dika";
  const char* mqtt_pass = "DKfrandita23";

  WiFiClient espClient;
  PubSubClient client(espClient);
  
  char msg[50];
  float gas_value;

  boolean relayState = false;

  boolean gasDetected = false;
  long lastMillisGasWarning = 0;

  long lastMillisGasRead = 0;

  // PIN DEFINITION
  const int MQ5 = 34;
  const int buzzer = 26;
  const int relay = 14; // LOW for relay is on

void setup_wifi(){
  delay(10);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }

  //connected status
  Serial.println("Wifi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* message, unsigned int length){
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
  String messageTemp2;

  for(int i = 0; i < length; i++){
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  
  Serial.println();
  if (String(topic) == "esp32/fan" && !gasDetected){
    if(messageTemp == "1"){
      relayState = true;
      digitalWrite(relay, LOW); 
    } else {
      relayState = false;
      digitalWrite(relay, HIGH); 
    }
  }
}

void reconnect(){
  while (!client.connected()){
    Serial.print("Attempting MQTT connection...");
    if(client.connect("ESP32Client",mqtt_user,mqtt_pass)){
      Serial.println("Connected");
      client.subscribe("esp32/fan");
    }
    else{
      Serial.print("Failed, rc=");
      Serial.print(client.state());
      Serial.println("try again in 5 seconds");
      delay(2000);
    }
  }
}
  

void setup() {
  pinMode(MQ5, INPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(relay, OUTPUT);

  digitalWrite(relay,HIGH);
  
  Serial.begin(115200);
  
  setup_wifi();
  
  Blynk.begin(auth, ssid, password);
  
  client.setServer(mqtt_server,1883);
  client.setCallback(callback);
}


void loop() {
  client.loop();
  Blynk.run();
  if (!client.connected()){
    reconnect();
  }

  if ((millis() - lastMillisGasRead) > 5000){
    char tempString[8];
    
    lastMillisGasRead = millis();
    
    gas_value = map(analogRead(MQ5),0,4095,2,150);
    dtostrf(gas_value, 1, 2, tempString);
    
    Serial.println("=====_Gas Monitoring_=====");
    Serial.println(gas_value);
    if(gas_value >= 50){
      gasDetected = true;
    } else {
      gasDetected = false;
    }
    client.publish("esp32/gas", tempString);
  }

  if ((millis() - lastMillisGasWarning) > 5000){
    lastMillisGasWarning = millis();
    if(gasDetected) {
      digitalWrite(relay,LOW);
      digitalWrite(buzzer,HIGH);
    } else {
      if(!relayState) { 
        digitalWrite(relay,HIGH);
      }
      digitalWrite(buzzer,LOW);
    }
  }
}
