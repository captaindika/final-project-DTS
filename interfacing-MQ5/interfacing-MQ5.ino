
int buzzer = 26;
//digital
int MQ5=34;
int gas_value;



//analog
//float MQ52 = A2; //pin 12 -> A5
float gas_value2;


void setup()
  {
  
    pinMode(MQ5,INPUT);
    pinMode(buzzer,OUTPUT);
    Serial.begin(115200);
  
  }

void loop()
  {
      //for digital
      gas_value=digitalRead(MQ5);

//    for analog
    gas_value2 = analogRead(MQ5);
    gas_value2 = map(gas_value2,0,4095,2,150); // formula for esp32
    Serial.println(gas_value2);
    Serial.println(gas_value);
    delay(500);
    if( gas_value == 1){
      digitalWrite(buzzer,LOW);
    }
    else{
      digitalWrite(buzzer,HIGH);
    }
  }
