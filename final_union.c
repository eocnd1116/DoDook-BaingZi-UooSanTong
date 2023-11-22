// 라이브러리 선언 //
#include <Keypad.h>
#include <LiquidCrystal.h>


// ====================================================================


// 압력판 변수 초기화 //
int pp_list[]={0,0,0,0,0,0};
int ppAfter[]={0,0,0,0,0,0};

// 디스플레이 변수 초기화 //
LiquidCrystal lcd(15, 14, 12, 11, 10, 9);

// ====================================================================


void setup() {
  // 시리얼 모니터 //
  Serial.begin(9600);

  // 압력센서 pinMode //
  for ( int i=54; i<=59; i++) { pinMode(i, INPUT); } 

  // 디스플레이 세팅 //
  lcd.begin(16, 2);
  lcd.clear();
  lcd.print("LiquidCrystal");
  lcd.setCursor(0,1);
  lcd.print("Hello rnjswns");
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
