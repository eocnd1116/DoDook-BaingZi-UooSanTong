#include <Keypad.h>
#include <Servo.h> 
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

//디스플레이 변수
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
}

void loop() {
  lcd.setCursor(0,1);
  lcd.print("wa sans");
  lcd.clear();
  
}
