#include <SoftwareSerial.h>
SoftwareSerial imuSerial(2,3);

#define SBUF_SIZE 64

char sbuf[SBUF_SIZE];
signed int sbuf_cnt=0;

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

void setup() {
    Serial.begin(9600);
    Serial.println("Start GPS... ");
    imuSerial.begin(115200);
}
    
void loop() {
    float euler[3];
    if(EBimuAsciiParser(euler,3)) {
        Serial.print("\n\r");
        Serial.print(euler[0]);    Serial.print(" ");
        Serial.print(euler[1]);    Serial.print(" ");
        Serial.print(euler[2]);    Serial.print(" ");
    }
}