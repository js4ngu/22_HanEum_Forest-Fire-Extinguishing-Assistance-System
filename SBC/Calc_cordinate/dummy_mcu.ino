#include <SoftwareSerial.h>
SoftwareSerial RPI(4, 5);

void setup() {
    Serial.begin(9600);
    RPI.begin(9600);

    double latitude[3]  = {37.2221,     37.2221,    37.2221};
    float longitude[3]  = {127.1876,    127.1876,   127.1876,}
    float euler[3]      = {60,          0,          210};
    uint16_t distance   = 2000;
}

void loop() {
    monitor();
    rpiTX();
    delay(2000);
}

void monitor(){
    Serial.print(latitude[0], 15); Serial.print(","); Serial.print(longitude[0], 15); Serial.print(",");
    Serial.print(latitude[1], 15); Serial.print(","); Serial.println(longitude[1], 15); Serial.print(",");
    Serial.print(latitude[2], 15); Serial.print(","); Serial.println(longitude[1], 15); Serial.print(",");
    Serial.print(euler[0], 10); Serial.print(","); Serial.print(euler[1], 10); Serial.print(","); Serial.println(euler[2], 10); Serial.print(",");
    Serial.println(distance, 5);
}

void rpiTX(){
    RPI.print(latitude[0], 15); RPI.print(","); RPI.print(longitude[0], 15); RPI.print(",");
    RPI.print(latitude[1], 15); RPI.print(","); RPI.println(longitude[1], 15); RPI.print(",");
    RPI.print(latitude[2], 15); RPI.print(","); RPI.println(longitude[1], 15); RPI.print(",");
    RPI.print(euler[0], 10); RPI.print(","); RPI.print(euler[1], 10); RPI.print(","); RPI.println(euler[2], 10); RPI.print(",");
    Serial.println(distance, 5);
}