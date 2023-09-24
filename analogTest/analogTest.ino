int data;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(2, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  data = digitalRead(A2);
  Serial.println(data);
  if (data < 500){
    digitalWrite(2, HIGH);
  }
  delay(50);
}
