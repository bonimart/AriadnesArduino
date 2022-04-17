#include <LiquidCrystal.h>
#include "Wire.h"
#include <MPU6050_light.h>

//mpu set-up
MPU6050 mpu(Wire);

//pins for lcd
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
  //initialize serial monitor
  Serial.begin(9600);
  
  //initialize LCD display
  lcd.begin(16, 2);
  Wire.begin();

  //initialize mpu sensor
  byte status = mpu.begin();
  Serial.print(F("MPU6050 status: "));
  Serial.println(status);
  while (status != 0) { }
  //recalculates both gyro and accelerometer offsets
  mpu.calcOffsets(true, true);
  

}

void loop() {

}
