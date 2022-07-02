#include <SoftwareSerial.h>
#define GPS1 0
#define GPS2 1
#define GPS3 2

SoftwareSerial gpsSerial1(4,5);
SoftwareSerial gpsSerial2(6,7);
SoftwareSerial gpsSerial3(8,9);

char c = "";
String str = "";
String targetStr = "GPGGA";
int state = GPS1;
int GPS_flag[3] = {1,0,0};

void setup(){
    Serial.begin(9600);
    Serial.println("Start GPS...");
    gpsSerial1.begin(9600);
    gpsSerial2.begin(9600);
    gpsSerial3.begin(9600);
    gpsSerial1.listen();
}

void readGPS1();
void readGPS2();
void readGPS3();
void parsingGPS();

void loop() {
    if(state == GPS1){
        while(GPS_flag[0]){
            if(gpsSerial1.available()){
                c=gpsSerial1.read(); // 센서의 값 읽기
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

                        // 좌표 계산.
                        double LatF = Lat1.toDouble() + Lat2.toDouble()/60;
                        float LongF = Long1.toFloat() + Long2.toFloat()/60;

                        // 좌표 출력.
                        Serial.print("Lat1 : ");
                        Serial.println(LatF, 15);
                        Serial.print("Long1 : ");
                        Serial.println(LongF, 15);
                        GPS_flag[0] = 0;
                    }
                    // str 값 초기화
                    str = "";
                }
                else{ // \n 아닐시, str에 문자를 계속 더하기
                    str += c;
                }
            }
        }
        state = GPS2;
        GPS_flag[1] = 1;
        gpsSerial2.listen();
    }
    else if(state == GPS2){
        while(GPS_flag[1]){
            if(gpsSerial2.available()){
                c=gpsSerial2.read(); // 센서의 값 읽기
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

                        // 좌표 계산.
                        double LatF = Lat1.toDouble() + Lat2.toDouble()/60;
                        float LongF = Long1.toFloat() + Long2.toFloat()/60;

                        // 좌표 출력.
                        Serial.print("Lat2 : ");
                        Serial.println(LatF, 15);
                        Serial.print("Long2 : ");
                        Serial.println(LongF, 15);
                        GPS_flag[1] = 0;
                    }
                    // str 값 초기화
                    str = "";
                }
                else{ // \n 아닐시, str에 문자를 계속 더하기
                    str += c;
                }
            }
        }
        state = GPS3;
        GPS_flag[2] = 1;
        gpsSerial3.listen();
    }

    else if(state == GPS3){
        while(GPS_flag[2]){
            if(gpsSerial3.available()){
                c=gpsSerial3.read(); // 센서의 값 읽기
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

                        // 좌표 계산.
                        double LatF = Lat1.toDouble() + Lat2.toDouble()/60;
                        float LongF = Long1.toFloat() + Long2.toFloat()/60;

                        // 좌표 출력.
                        Serial.print("Lat3 : ");
                        Serial.println(LatF, 15);
                        Serial.print("Long3 : ");
                        Serial.println(LongF, 15);
                        GPS_flag[2] = 0;
                    }
                    // str 값 초기화
                    str = "";
                }
                else{ // \n 아닐시, str에 문자를 계속 더하기
                    str += c;
                }
            }
        }
        state = GPS1;
        GPS_flag[0] = 1;
        gpsSerial1.listen();
    }
}