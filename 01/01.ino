void setup() {
  // put your setup code here, to run once:
 // pinMode(P1_0, OUTPUT);
  pinMode(P1_6, OUTPUT);

  Serial.begin(9600); // скорость передачи данных в бит/с PC
}

void loop() {
  // put your main code here, to run repeatedly: 
  digitalWrite(P1_0, HIGH);//off red led
  digitalWrite(P1_6, HIGH);//on green led
  delay(1000);// sleep 1000 (mk)s

  digitalWrite(P1_6, LOW);
  digitalWrite(P1_0, LOW);
  delay(2000);
}
