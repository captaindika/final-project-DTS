void setup() {
  // put your setup code here, to run once:
  pinMode(14,OUTPUT);
  pinMode(26,OUTPUT);
  Serial.begin(115200);
  Serial.println("Low early");

}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(14,HIGH);
  digitalWrite(26,HIGH);
  Serial.println("HIGH");
  delay(5000);
  digitalWrite(14,LOW);
  digitalWrite(26,LOW);
  Serial.println("LOW");
  delay(5000);\

}
