#include <LiquidCrystal.h>
#include "Wire.h"
#include <MPU6050_light.h>
#include "stack/stack.h"
#include "stack/stack.cpp"

#define LCD_WAIT 500
#define BTN_WAIT 20
#define ANGLE_ERROR 5
#define END 6
#define STEP 7
//#define DEBUG

unsigned long lcd_timer = 0;
unsigned long btn_timer = 0;
bool end;
bool step;
float angleDiff;
Stack angles;

//mpu set-up
MPU6050 mpu(Wire);

//pins for lcd
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

enum states {MENU, READ, NAVIGATE};
enum states STATE = MENU;

void setup() {
  //initialize serial monitor
  Serial.begin(9600);
  
  //initialize LCD display
  lcd.begin(16, 2);
  Wire.begin();

  //initialize buttons
  pinMode(END, INPUT_PULLUP);
  pinMode(STEP, INPUT_PULLUP);

  //initialize mpu sensor
  byte status = mpu.begin();
  Serial.print(F("MPU6050 status: "));
  Serial.println(status);
  while (status != 0) { }
  //recalculates both gyro and accelerometer offsets
  mpu.calcOffsets(true, true);
  

}

bool risingEdge(int pin, bool val){
  return !digitalRead(pin) && val == true;
}

float trimAngle(float angle){
  //modulus for small floats -> output in range <0; 360)
  while(angle < -180){
    angle += 360;
  }
  while(angle >= 180){
    angle -= 360;
  }
  return angle;
}

void makeStep(float angle){
  if(!angles.size){
    angles.push(angle);
    return;
  }
  //check if we are moving back, if so, pop from stack
  float prev = angles.top();
  angleDiff = trimAngle(angles.top() - mpu.getAngleZ() + 180);
  if(angleDiff < ANGLE_ERROR
    && angleDiff > -ANGLE_ERROR){
      angles.pop();
      return;
  }
  angles.push(angle);

}

void handleLcd(){
  lcd.clear();
  switch(STATE){
    case MENU:
      lcd.print("Press left");
      lcd.setCursor(0, 1);
      lcd.print("button to start!");
      break;
    case READ:
      lcd.print("Reading");
      lcd.setCursor(0, 1);
      lcd.print("Step count: ");
      lcd.print(angles.size);
      break;
    case NAVIGATE:
      angleDiff = trimAngle(angles.top() - mpu.getAngleZ() + 180);
#ifdef DEBUG
      lcd.print(angleDiff, 0);
#endif
#ifndef DEBUG
      if(angleDiff > 0 && angleDiff > ANGLE_ERROR){
        lcd.print("Turn left");
      }
      else if(angleDiff < 0 && angleDiff < -ANGLE_ERROR){
        lcd.print("Turn right");
      }
      else{
        lcd.print("Make a step!");
      }
#endif
      lcd.setCursor(0, 1);
      lcd.print("Steps left: ");
      lcd.print(angles.size);
      break;
  }
}

void handleBtn(){
  switch(STATE){
    case MENU:
      if(risingEdge(STEP, step)){
        STATE = READ;
      }
      break;
    case READ:
      if(risingEdge(END, end)){
        STATE = NAVIGATE;
      }
      else if(risingEdge(STEP, step)){
        //make a step, angles can be out of range <0;360), therefore they need to be trimmed
        makeStep(trimAngle(mpu.getAngleZ()));
      }
      break;
    case NAVIGATE:
      if(risingEdge(END, end) || angles.size == 0){
        angles.clear();
        STATE = MENU;
      }
      else if(risingEdge(STEP, step)){
        //make a step
        angleDiff = trimAngle(angles.top() - mpu.getAngleZ() + 180);
        if(angleDiff > -ANGLE_ERROR && angleDiff < ANGLE_ERROR){
          makeStep(trimAngle(mpu.getAngleZ()));
        }
      }
      break;
  }
}

void loop() {
  //keep sensor updated for higher precision
  mpu.update();
  if(millis()-lcd_timer > LCD_WAIT){
    lcd_timer = millis();
    handleLcd();
  }
  if(millis()-btn_timer > BTN_WAIT){
    btn_timer = millis();
    handleBtn();
    end = digitalRead(END);
    step = digitalRead(STEP);
  }
}
