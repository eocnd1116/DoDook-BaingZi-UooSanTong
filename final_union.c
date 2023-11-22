// 라이브러리 선언 //
#include <Keypad.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>


// ====================================================================


// 압력판 변수 초기화 //
int pp_list[]={0,0,0,0,0,0};
int ppAfter[]={0,0,0,0,0,0};

// 디스플레이 변수 초기화 //
LiquidCrystal_I2C lcd(0x27,16,2);
int dpType=-1; //현재 출력타입
int dpTC=0;    //출력타입 변경 요청
char ct_music [] = {
  B00000,
	B00100,
  B00110,
  B00101,
  B00101,
  B11100,
  B11100,
  B00000
};
char ct_arrow [] = {
  B00000,
  B01000,
  B01100,
  B01110,
  B01110,
  B01100,
  B01000,
  B00000
};


// ====================================================================


void setup() {
  // 시리얼 모니터 //
  Serial.begin(9600);

  // 압력센서 pinMode //
  for ( int i=54; i<=59; i++) { pinMode(i, INPUT); } 

  // 디스플레이 세팅 //
  lcd.init();
  lcd.backlight();
  dpTC=3;
  lcd.createChar(0, ct_music);
  lcd.createChar(1, ct_arrow);
}


// ====================================================================


void loop() {
  // 압력센서 //
  for ( int i=54; i<=59; i++) {
    if ( ppAfter[i-54] == 0 ) {
      pp_list[i-54]=0;
    }
    else{
      pp_list[i-54]=1;
    }
    ppAfter[i-54]=map(analogRead(i), 0, 1024, 0, 255);
  }


  // 디스플레이 //
  if ( dpType != dpTC ) {
    switch(dpTC) {
      case 0:
        lcd.setCursor(2, 0);
        lcd.write(0);
        lcd.print(" Welcome! ");
        lcd.write(0);
        lcd.setCursor(0, 1);
        lcd.print("Press A To Start");
        break;
      case 1:
        lcd.setCursor(0, 0);
        lcd.print("Select aaaa");
        lcd.setCursor(1, 1);
        lcd.write(1);
        lcd.print("Yes  No");
        break;
      case 2:
        lcd.setCursor(1, 0);
        lcd.write(1);
        lcd.print("Put in");
        lcd.setCursor(1, 1);
        lcd.print("Take out");
        break;
      case 3:
        lcd.setCursor(1, 0);
        lcd.print("Put in");
        lcd.setCursor(1, 1);
        lcd.write(1);
        lcd.print("Take out");
        break;
    }
    dpType = dpTC;
  }


  // 키패드 //
  

}
