void setup() {
  Serial.begin(9600);
  pinMode(A0, INPUT);
}

void loop() {
    int a = digitalRead(A0);
    Serail.println(a);
}