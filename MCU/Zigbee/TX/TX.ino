SoftwareSerial xbee(13, 12);

void setup() {
  Serial.begin(9600); //시리얼 통신 초기화
  xbee.begin(9600); //시리얼 통신 초기화
}

void loop() { //1초 간격으로 'H'와 'L'을 주기적으로 전송한다
  Serial.println("TEST");
  xbee.println("TEST");
  delay(300);
}