#include <SoftwareSerial.h>
#include <DFRobot_TFmini.h>

#define GPS1 0
#define GPS2 1
#define GPS3 2
#define IMU 3
#define LIDAR 4
#define TX 5

#define SBUF_SIZE 64 // about IMU

SoftwareSerial imuSerial(2, 3);
SoftwareSerial gpsSerial1(4, 5);
SoftwareSerial gpsSerial2(6, 7);
SoftwareSerial gpsSerial3(8, 9);
SoftwareSerial lidarSerial(11, 10); // RX, TX
SoftwareSerial xbee(13, 12);

void readGPS1(bool print_value, bool print_raw);
void readGPS2(bool print_value, bool print_raw);
void readGPS3(bool print_value, bool print_raw);
void readIMU(bool print);
void readLidar(bool print);

int parsingGPS(char c, int *nowflag, double *latitude, float *longitude, bool print_raw);
int EBimuAsciiParser(float *item, int number_of_item);

// GPS Var
int state = GPS1;
int flag[6] = {1, 0, 0, 0, 0, 0};

double latitude[3];
float longitude[3];

// parsing GPS Var
char c = "";
String str = "";
String targetStr = "GPGGA";

// IMU var
char sbuf[SBUF_SIZE];
signed int sbuf_cnt = 0;
float euler[3];

//lidar
DFRobot_TFmini  lidar;
uint16_t distance, strength;   // 거리와 강도를 담는 변수

void setup() {
    Serial.begin(9600);
    Serial.println("Start GPS...");
    gpsSerial1.begin(9600);
    gpsSerial2.begin(9600);
    gpsSerial3.begin(9600);
    lidarSerial.begin(9600);
    xbee.begin(9600); //시리얼 통신 초기화

    lidar.begin(lidarSerial);
    imuSerial.begin(115200);
    gpsSerial1.listen();
}

void loop() {
    if (state == GPS1) {
        readGPS1(0, 0);
        state = GPS2;
    }
    else if (state == GPS2) {
        readGPS2(0, 0);
        state = GPS3;
    }
    else if (state == GPS3) {
        readGPS3(0, 0);
        state = IMU;
    }
    else if (state == IMU) {
        readIMU(0);
        state = LIDAR;
    }
    else if (state == LIDAR) {
        readLidar(0);
        state = TX;
    }
    else if (state == TX) {
        xbeeTx();
        Serial.print("GPS1 : ");
        Serial.print(latitude[0], 15);
        Serial.print("  /  ");
        Serial.println(longitude[0], 15);
        Serial.print("GPS2 : ");
        Serial.print(latitude[1], 15);
        Serial.print("  /  ");
        Serial.println(longitude[1], 15);
        Serial.print("GPS3 : ");
        Serial.print(latitude[2], 15);
        Serial.print("  /  ");
        Serial.println(longitude[1], 15);
        Serial.print("IMU : ");
        Serial.print(euler[0], 10);
        Serial.print("  /  ");
        Serial.print(euler[1], 10);
        Serial.print("  /  ");
        Serial.println(euler[2], 10);
        Serial.print("Distance = ");
        Serial.print(distance);
        Serial.print("cm  /   ");
        Serial.print("Strength = ");
        Serial.println(strength);
        state = GPS1;
    }
}

void readGPS1(bool print_value, bool print_raw) {
    while (flag[0]) {
        if (gpsSerial1.available()) {
            c = gpsSerial1.read(); // 센서의 값 읽기
            if (parsingGPS(c, &flag[0], &latitude[0], &longitude[0], print_raw) == 0) {
                if (print_value) {
                    Serial.print("Lat1 : ");
                    Serial.println(latitude[0], 15);
                    Serial.print("Long1 : ");
                    Serial.println(longitude[0], 15);
                }
                str = "";
            }
        }
    }
    flag[1] = 1;
    gpsSerial2.listen();
}

void readGPS2(bool print_value, bool print_raw) {
    while (flag[1]) {
        if (gpsSerial2.available()) {
            c = gpsSerial2.read(); // 센서의 값 읽기
            if (parsingGPS(c, &flag[1], &latitude[1], &longitude[1], print_raw) == 0) {
                if (print_value) {
                    Serial.print("Lat2 : ");
                    Serial.println(latitude[1], 15);
                    Serial.print("Long2 : ");
                    Serial.println(longitude[1], 15);
                }
                str = "";
            }
        }
    }
    flag[2] = 1;
    gpsSerial3.listen();
}

void readGPS3(bool print_value, bool print_raw) {
    while (flag[2]) {
        if (gpsSerial3.available()) {
            c = gpsSerial3.read(); // 센서의 값 읽기
            if (parsingGPS(c, &flag[2], &latitude[2], &longitude[2], print_raw) == 0) {
                if (print_value) {
                    Serial.print("Lat3 : ");
                    Serial.println(latitude[2], 15);
                    Serial.print("Long3 : ");
                    Serial.println(longitude[2], 15);
                }
                str = "";
            }
        }
    }
    flag[3] = 1;
    imuSerial.listen();
}

void readIMU(bool print) {
    while (flag[3]) {
        if(EBimuAsciiParser(euler, 3)){
            if (print) {
                Serial.print("\n\r");
                Serial.print(euler[0]);
                Serial.print(" ");
                Serial.print(euler[1]);
                Serial.print(" ");
                Serial.print(euler[2]);
                Serial.println(" ");
            }
            flag[3] = 0;
        }
    }
    flag[4] = 1;
    lidarSerial.listen();
}

void readLidar(bool print) {
    while (flag[4]) { 
        if (lidar.measure()) {                  // 거리와 신호의 강도를 측정합니다. 성공하면 을 반환하여 if문이 작동합니다.
            distance = lidar.getDistance();       // 거리값을 cm단위로 불러옵니다.
            strength = lidar.getStrength();       // 신호의 강도를 불러옵니다. 측정 대상이 넓으면 강도가 커집니다.
            if (print) {
                Serial.print("Distance = ");
                Serial.print(distance);
                Serial.println("cm");
                Serial.print("Strength = ");
                Serial.println(strength);
            }
            flag[4] = 0;
        }
    }
    flag[5] = 1;
    gpsSerial1.listen();
}

void xbeeTx() {
    while (flag[5]) { 
        xbee.print(latitude[0], 15);
        xbee.print("  /  ");
        xbee.print(longitude[0], 15);
        xbee.print("  /  ");
        xbee.print(latitude[1], 15);
        xbee.print("  /  ");
        xbee.print(longitude[1], 15);
        xbee.print("  /  ");
        xbee.print(latitude[2], 15);
        xbee.print("  /  ");
        xbee.print(longitude[2], 15);
        xbee.print("  /  ");
        xbee.print(euler[0], 10);
        xbee.print("  /  ");
        xbee.print(euler[1], 10);
        xbee.print("  /  ");
        xbee.print(euler[2], 10);
        xbee.print("  /  ");
        xbee.print(distance);
        xbee.println("\r\n");
    }
    flag[0] = 1;
    gpsSerial1.listen();
}
int parsingGPS(char c, int *nowflag, double *latitude, float *longitude, bool print_raw) {
    if (c == '\n') { // \n 값인지 구분.
        if (targetStr.equals(str.substring(1, 6))) {
            if (print_raw)
                Serial.println(str);
            int first = str.indexOf(",");
            int two = str.indexOf(",", first + 1);
            int three = str.indexOf(",", two + 1);
            int four = str.indexOf(",", three + 1);
            int five = str.indexOf(",", four + 1);
            // Lat과 Long 위치에 있는 값들을 index로 추출
            String Lat = str.substring(two + 1, three);
            String Long = str.substring(four + 1, five);
            
            // Lat의 앞값과 뒷값을 구분
            String Lat1 = Lat.substring(0, 2);
            String Lat2 = Lat.substring(2);

            // Long의 앞값과 뒷값을 구분
            String Long1 = Long.substring(0, 3);
            String Long2 = Long.substring(3);

            // 좌표 계산.
            *latitude = Lat1.toDouble() + Lat2.toDouble() / 60;
            *longitude = Long1.toFloat() + Long2.toFloat() / 60;
            *nowflag = 0;
            return 0;
        }
        // str 값 초기화
        str = "";
    }
    else { // \n 아닐시, str에 문자를 계속 더하기
        str += c;
    }
}

int EBimuAsciiParser(float *item, int number_of_item) {
    int n, i;
    int rbytes;
    char *addr;
    int result = 0;

    rbytes = imuSerial.available();
    for (n = 0; n < rbytes; n++) {
        sbuf[sbuf_cnt] = imuSerial.read();
        if (sbuf[sbuf_cnt] == 0x0a) {
            addr = strtok(sbuf, ",");
            for (i = 0; i < number_of_item; i++) {
                item[i] = atof(addr);
                addr = strtok(NULL, ",");
            }
            result = 1;
        }
        else if (sbuf[sbuf_cnt] == '*') {
            sbuf_cnt = -1;
        }
        sbuf_cnt++;
        if (sbuf_cnt >= SBUF_SIZE)
            sbuf_cnt = 0;
    }
    return result;
}