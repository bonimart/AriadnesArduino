#include <LiquidCrystal.h>
#include "Wire.h"

//pins for lcd
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
  //initialize LCD display
  lcd.begin(16, 2);
  Wire.begin();
  

}

void loop() {

}
