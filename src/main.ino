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

void loop() {
  switch (STATE)
  {
    case MENU:
      lcd.clear();
      lcd.print("Press button!");
      if(risingEdge(STEP, st)){
        STATE = READ;
      }
      end = digitalRead(END);
      st = digitalRead(STEP);
      break;
      
    
    case READ:
      lcd.clear();
      lcd.print("Reading");
      if(risingEdge(END, end)){
        STATE = NAVIGATE;
      }
      else if(risingEdge(STEP, st)){
        //make a step
      }
      end = digitalRead(END);
      st = digitalRead(STEP);
      break;
      
    case NAVIGATE:
      lcd.clear();
      lcd.print("Navigating");
      if(risingEdge(END, end)){
        STATE = MENU;
      }
      else if(risingEdge(STEP, st)){
        //make a step
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
