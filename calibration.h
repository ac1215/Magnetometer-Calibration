#ifndef __CALIBRATION_H__
#define __CALIBRATION_H__

#include <NXPMotionSense.h>
#include <Wire.h>
#include <EEPROM.h>
#include <imuread.h>

#define MAG_CAL_EEADDR  60
#define MAG_CAL_SIZE    76
#define MAG_CAL_STATUS  59

#define GAPS_THRESHOLD 4.0f
#define VARIANCE_THRESHOLD 4.5f
#define WOBBLE_THRESHOLD 4.0f
#define FITERROR_THRESHOLD 5.0f
#define QUALITY_UPDATE 3

class calibration
{
public:
    calibration();
    uint8_t initCalibration();
    void magneticCalibration();
    void writeCalibration(const void *);
    bool calibrationStatus();
    void printCalibrationData(void);
    ~calibration();
private:
    NXPMotionSense imuCalibration;
    bool show_calibration_confirmed = false;
    float fGaps, fVariance, fWobble, fFiterror;
};

#endif //__CALIBRATION_H__
