#ifndef __DFRobot_TFmini_H__
#define __DFRobot_TFmini_H__

#include <Arduino.h>
#include <stdint.h>
#include <Wire.h>
#include "stdio.h"
#include <SoftwareSerial.h>

class DFRobot_TFmini
{
public:
    void                begin(Stream &s_);
    bool                measure(void);
    uint16_t            getDistance(void);
    uint16_t            getStrength(void);
private:
    SoftwareSerial*     TFSerial;
    uint16_t            distance = 0;
    uint16_t            strength = 0;
};

#endif