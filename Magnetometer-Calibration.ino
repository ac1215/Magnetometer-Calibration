#include"calibration.h"

calibration magnetoCal;

const int ledPin = 13;

void setup() {
  Serial.begin(115200);
  while (!Serial) ; // wait for serial port open
  pinMode(ledPin, OUTPUT);
  if ((magnetoCal.initCalibration()))
  {
    Serial.println("Magnetometer Calibration is not initalized!!!");
    while (1);
  }
  digitalWrite(ledPin, HIGH);
  Serial.println("Magnetometer Calibration is initalized!!!");
}

void loop() {

  magnetoCal.magneticCalibration();
  if (magnetoCal.calibrationStatus())
  {
    Serial.println("Magnetometer Calibration Successfully Done!!!!");
    digitalWrite(ledPin, LOW);
    while (1);
  }
  magnetoCal.printCalibrationData();
}
