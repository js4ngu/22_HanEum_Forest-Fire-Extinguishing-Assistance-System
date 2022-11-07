#include <SoftwareSerial.h>
#define FILTER_SIZE 1
#define SBUF_SIZE 64

SoftwareSerial imuSerial(2,3);

float raw_array[3][FILTER_SIZE] = {{0,},{0,},{0,}};
int raw_array_index = 0;

char sbuf[SBUF_SIZE];
signed int sbuf_cnt=0;
float euler[3];
float filteredEuler[3];

float movingAVG(int array_num); // 0~2  : Pitch , roll , yaw
void insertArray(int array_num, float value);
int EBimuAsciiParser(float *item, int number_of_item);

void setup() {
    Serial.begin(9600);
    Serial.println("Start...");
    imuSerial.begin(115200);
}
    
void loop() {
    if(EBimuAsciiParser(euler,3)) {
        for (int i = 0; i < 3; i++) {
            insertArray(i, euler[i]);
            filteredEuler[i] = movingAVG(i);
        }
        Serial.print("\n\r");
        Serial.print(filteredEuler[0]);    Serial.print(" ");
        Serial.print(filteredEuler[1]);    Serial.print(" ");
        Serial.print(filteredEuler[2]);    Serial.print(" ");
    }
    delay(50);
}

float movingAVG(int array_num) {
    int i = 0;
    float sum = 0;
    for (i = 0; i < FILTER_SIZE; i++) {
      sum += raw_array[array_num][i];
    }
    return ((float)sum / FILTER_SIZE);
}

void insertArray(int array_num, float value) {
    raw_array[array_num][raw_array_index] = value;
    raw_array_index++;
    if (raw_array_index >= FILTER_SIZE) {
      raw_array_index = 0;
    }
}

int EBimuAsciiParser(float *item, int number_of_item) {
    int n,i;
    int rbytes;
    char *addr; 
    int result = 0;

    rbytes = imuSerial.available();
    for(n=0;n<rbytes;n++) {
        sbuf[sbuf_cnt] = imuSerial.read();
        if(sbuf[sbuf_cnt]==0x0a) {
            addr = strtok(sbuf,",");
            for(i=0;i<number_of_item;i++) {
                item[i] = atof(addr);
                addr = strtok(NULL,",");
            }
            result = 1;
        }
        else if(sbuf[sbuf_cnt]=='*') {
            sbuf_cnt=-1;
        }
        sbuf_cnt++;
        if(sbuf_cnt>=SBUF_SIZE) sbuf_cnt=0;
    }
    return result;
}
