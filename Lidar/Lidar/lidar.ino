/*
   @File  : DFRobot_TFmini_test.ino
   @Brief : This example use TFmini to measure distance
           With initialization completed, we can get distance value and signal strength
   @Copyright   [DFRobot](http://www.dfrobot.com), 2016
               GNU Lesser General Public License
   @version  V1.0
   @date  2018-1-10
*/
#include <DFRobot_TFmini.h>

SoftwareSerial lidarSerial(10, 11); // RX, TX

DFRobot_TFmini  lidar;
uint16_t distance, strength;   // 거리와 강도를 담는 변수

void setup() {
  Serial.begin(115200);
  Serial.print("asdf");
  lidarSerial.begin(9600);
  lidar.begin(lidarSerial);
}

void loop() {
  if (lidar.measure()) {                  // 거리와 신호의 강도를 측정합니다. 성공하면 을 반환하여 if문이 작동합니다.
    distance = lidar.getDistance();       // 거리값을 cm단위로 불러옵니다.
    //strength = lidar.getStrength();       // 신호의 강도를 불러옵니다. 측정 대상이 넓으면 강도가 커집니다.
    Serial.print("Distance = ");
    Serial.print(distance);
    Serial.println("cm");
    //Serial.print("Strength = ");
    //Serial.println(strength);
  }
  delay(100);
}
