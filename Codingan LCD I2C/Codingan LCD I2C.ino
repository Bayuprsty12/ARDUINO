#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  lcd.begin();
}

void loop(){
  lcd.setCursor(1,0);
  lcd.print("TEST LCD i2C");
  lcd.setCursor(1,1);
  lcd.print("N2F|WingkyStY");
}