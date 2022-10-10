#include <SoftwareSerial.h>
SoftwareSerial gpsSerial(4,5);

char c = ""; // Wn 인지 구분 및 str에 저장.
String str = ""; // \n 전까지 c 값을 저장.
String targetStr = "GPGGA"; // str의 값이 NMEA의 GPGGA 값인지 타겟

void setup() {
    Serial.begin(9600);
    Serial.println("Start GPS... ");
    gpsSerial.begin(9600);
}

void loop() {
    if(gpsSerial.available()) { // gps 센서 통신 가능?
        c=gpsSerial.read(); // 센서의 값 읽기
        if(c == '\n'){ // \n 값인지 구분.
            if(targetStr.equals(str.substring(1, 6))){
                Serial.println(str);
                int first = str.indexOf(",");
                int two = str.indexOf(",", first+1);
                int three = str.indexOf(",", two+1);
                int four = str.indexOf(",", three+1);
                int five = str.indexOf(",", four+1);
                
                // Lat과 Long 위치에 있는 값들을 index로 추출
                String Lat = str.substring(two+1, three);
                String Long = str.substring(four+1, five);

                // Lat의 앞값과 뒷값을 구분
                String Lat1 = Lat.substring(0, 2);
                String Lat2 = Lat.substring(2);

                // Long의 앞값과 뒷값을 구분
                String Long1 = Long.substring(0, 3);
                String Long2 = Long.substring(3);

                // 좌표 계산
                double LatF = Lat1.toDouble() + Lat2.toDouble()/60;
                float LongF = Long1.toFloat() + Long2.toFloat()/60;

                // 좌표 출력
                Serial.print("Lat : ");
                Serial.println(LatF, 15);
                Serial.print("Long : ");
                Serial.println(LongF, 15);
            }
            // str 값 초기화 
            str = "";
        }
        else { // \n 아닐시, str에 문자를 계속 더하기
        str += c;
        }
    }
}
