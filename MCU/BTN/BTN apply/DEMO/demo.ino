#define DEBUG
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
SoftwareSerial gpsSerial3(9, 8);
SoftwareSerial lidarSerial(11, 10); // RX, TX
SoftwareSerial xbee(13, 12);

bool BTN_toggle();
void readGPS1();
void readGPS2();
void readGPS3();
void readIMU();
void readLidar();
void sensor_monitor();
int parsingGPS(char c, int *nowflag, double *latitude, float *longitude);
int EBimuAsciiParser(float *item, int number_of_item);

// GPS Var
int state = GPS1;
int flag[5] = {1, 0, 0, 0, 0};

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

//xbee
int i = 0; //test state

//BTN
bool BTN_STATE = 1;
bool current_btn = 0;
bool prev_btn = 0;

void setup() {
    Serial.begin(9600);
    Serial.println("Start GPS...");
    gpsSerial1.begin(9600);
    gpsSerial2.begin(9600);
    gpsSerial3.begin(9600);
    lidar.begin(lidarSerial);
    imuSerial.begin(115200);
    xbee.begin(9600); //시리얼 통신 초기화
    gpsSerial1.listen();
}

void loop() {
    if(BTN_toggle() == 1){
        Serial.print("TEST ");
        Serial.println(i);
        readGPS1();
        readGPS2();
        readGPS3();
        readIMU();
        readLidar();           
        xbeeTX();
        #ifdef DEBUG
            sensor_monitor();
        #endif
    }
    BTN_STATE = 0;
}

bool BTN_toggle(){
    current_btn = digitalRead(A0);
    if ( (prev_btn ^ current_btn) == 1){
        prev_btn = current_btn;
        return 1;
    }
    else {
        prev_btn = current_btn;
        return 0;
    }
}

void readGPS1() {
    #ifdef DEBUG
        Serial.println("GPS1");
    #endif
    while (flag[0]) {
        if (gpsSerial1.available()) {
            c = gpsSerial1.read(); // 센서의 값 읽기
            if (parsingGPS(c, &flag[0], &latitude[0], &longitude[0]) == 0) {
                str = "";
            }
        }
    }
    #ifdef DEBUG
        Serial.print("Lat : ");
        Serial.println(latitude[0]);
        Serial.print("Long : ");
        Serial.println(longitude[0]);
    #endif
    flag[1] = 1;
    gpsSerial2.listen();
    state = GPS2;
}

void readGPS2() {
    #ifdef DEBUG
        Serial.println("GPS2");
    #endif
    while (flag[1]) {
        if (gpsSerial2.available()) {
            c = gpsSerial2.read(); // 센서의 값 읽기
            if (parsingGPS(c, &flag[1], &latitude[1], &longitude[1]) == 0) {
                str = "";
            }
        }
    }
    #ifdef DEBUG
        Serial.print("Lat : ");
        Serial.println(latitude[1]);
        Serial.print("Long : ");
        Serial.println(longitude[1]);
    #endif
    flag[2] = 1;
    gpsSerial3.listen();
    state = GPS3;
}

void readGPS3() {
    #ifdef DEBUG
        Serial.println("GPS3");
    #endif
    while (flag[2]) {
        if (gpsSerial3.available()) {
            c = gpsSerial3.read(); // 센서의 값 읽기
            if (parsingGPS(c, &flag[2], &latitude[2], &longitude[2]) == 0) {
                str = "";
            }
        }
    }
    #ifdef DEBUG
        Serial.print("Lat : ");
        Serial.println(latitude[2]);
        Serial.print("Long : ");
        Serial.println(longitude[2]);
    #endif
    flag[3] = 1;
    imuSerial.listen();
    state = IMU;
}


void readIMU() {
    #ifdef DEBUG
        Serial.println("IMU");
    #endif
    while (flag[3]) {
        if(EBimuAsciiParser(euler, 3)){
            if( euler[0] != 0 && euler[1] != 0 && euler[2] != 0)
                flag[3] = 0;
        }
    }
    #ifdef DEBUG
        Serial.print(euler[0], 10);
        Serial.print("  /  ");
        Serial.print(euler[1], 10);
        Serial.print("  /  ");
        Serial.println(euler[2], 10);
    #endif
    flag[4] = 1;
    lidarSerial.listen();
    state = LIDAR;
}

void readLidar() {
    #ifdef DEBUG
        Serial.println("Lidar");
    #endif
    while (flag[4]) { 
        //if (lidar.measure()) {                  // 거리와 신호의 강도를 측정합니다. 성공하면 을 반환하여 if문이 작동합니다.
        //    distance = lidar.getDistance();       // 거리값을 cm단위로 불러옵니다.
        //    strength = lidar.getStrength();       // 신호의 강도를 불러옵니다. 측정 대상이 넓으면 강도가 커집니다.
            flag[4] = 0;
        //}
    }
    flag[5] = 1;
    gpsSerial1.listen();
    state = TX;
}

void xbeeTX() {
    #ifdef DEBUG
        Serial.println("XBEE");
    #endif
    xbee.print("37.6645");
    xbee.print(" ");
    xbee.print("126.7417");
    xbee.print(" ");
    xbee.print("37.6645");
    xbee.print(" ");
    xbee.print("126.7417");
    xbee.print(" ");
    xbee.print("37.6645");
    xbee.print(" ");
    xbee.print("126.7417");
    xbee.print(" ");
    xbee.print(euler[0], 10);
    xbee.print(" ");
    xbee.print(euler[1], 10);
    xbee.print(" ");
    xbee.print(euler[2], 10);
    xbee.print(" ");
    xbee.println("40");
    flag[5] = 0;
    flag[0] = 1;
    gpsSerial1.listen();
    state = GPS1;

    i++;
}

int parsingGPS(char c, int *nowflag, double *latitude, float *longitude) {
    if (c == '\n') { // \n 값인지 구분.
        if (targetStr.equals(str.substring(1, 6))) {
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

void sensor_monitor(){
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
    Serial.println(longitude[2], 15);
    Serial.print("IMU : ");
    Serial.print(euler[0], 10);
    Serial.print("  /  ");
    Serial.print(euler[1], 10);
    Serial.print("  /  ");
    Serial.println(euler[2], 10);
    /*
    Serial.print("Distance = ");
    Serial.print(distance);
    Serial.print("cm  /   ");
    Serial.print("Strength = ");
    Serial.println(strength);
    */
}
