#include <SoftwareSerial.h>
SoftwareSerial xbee(12, 13);

void setup() {
  xbee.begin(9600); //시리얼 통신 초기화
}

void loop() { //1초 간격으로 'H'와 'L'을 주기적으로 전송한다
  xbee.write("Test\n\r");
  delay(500);
}