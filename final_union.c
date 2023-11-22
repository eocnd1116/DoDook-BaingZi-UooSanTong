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
char ct_cd [] = {
	B10101,
	B01110,
	B11011,
	B00100,
	B00100,
	B11011,
	B01110,
	B10101
}

// ====================================================================


void setup() {
  // 시리얼 모니터 //
  Serial.begin(9600);

  // 압력센서 pinMode //
  for ( int i=54; i<=59; i++) { pinMode(i, INPUT); } 

  // 디스플레이 세팅 //
  lcd.init();
  lcd.backlight();
  
  lcd.print("★ Welcome ★");
  lcd.write(0);
  lcd.setCursor(0, 1);
  lcd.print("press # to start");
  lcd.creatChar(0, ct_cd)
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

  //debuging
  for(int i=0; i<6; i++) {
    Serial.print(pp_list[i]);
  }
  Serial.println("");
}
