#include <Keypad.h>
#include <Servo.h> 
#include <LiquidCrystal_I2C.h>

//디스플레이 변수
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.clear();
  lcd.print("LiquidCrystal");
  lcd.setCursor(0,1);
  lcd.print("Hello rnjswns");
}

void loop() {
  
}
