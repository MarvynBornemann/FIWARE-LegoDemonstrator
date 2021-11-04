void setup() {
  // put your setup code here, to run once:
  pinMode(2, OUTPUT);
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(2,HIGH);
  Serial.println("HIGH");
  delay(5000);
  digitalWrite(2,LOW);
  Serial.println("LOW");
  delay(5000);
}
