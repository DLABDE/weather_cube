void setup() {
  Serial.begin(9600);
}

void loop() {
  Serial.print("ADC Value: ");Serial.println(map(analogRead(A0),0,1024,0,330)/100.0);
  delay(300);
}