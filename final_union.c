// 라이브러리 선언 //
#include <Keypad.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>


// ====================================================================


// 압력판 변수 초기화 //
bool pp_list[]={false,false,false,false,false,false};
int pp_pwd[]={0,0,0,0,0,0};
bool pp_onf[]={false,false,false,false,false,false};
int ppAfter[]={0,0,0,0,0,0};

// 디스플레이 변수 초기화 //
LiquidCrystal_I2C lcd(0x27,16,2);
int dpType=-1; //현재 출력타입
int dpTC=0;    //출력타입 변경 요청
const char ct_music [] = {B00000,B00100,B00110,B00101,B00101,B11100,B11100,B00000};
const char ct_arrow [] = {B00000,B01000,B01100,B01110,B01110,B01100,B01000,B00000};

// 키패드 //
const byte ROWS = 4;
const byte COLS = 3;
char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};
const char key_l = '*';
const char key_sel = '*';
const char key_r = '#';
byte rowPins[ROWS] = {5, 0, 1, 3};
byte colPins[COLS] = {4, 6, 2};
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

// ETC. //
int timer = 0;
int rTimer = 0;
int selecter = 0;
int targetNum = -1;


// ====================================================================


void setup() {
  // 시리얼 모니터 //
  Serial.begin(9600);

  // 압력센서 pinMode //
  for ( int i=54; i<=59; i++) { pinMode(i, INPUT); } 

  // 디스플레이 세팅 //
  lcd.init();
  lcd.backlight();
  dpTC=0;
  lcd.createChar(0, ct_music);
  lcd.createChar(1, ct_arrow);
  for ( int i=0; i<4; i++) {
    char ct_timer [] = {
      B00000,
      B01110,
      (i==0?B10101:B10001),
      (i==1?B10111:(i==3?B11101:B10101)),
      (i==2?B10101:B10001),
      B01110,
      B00000,
      B00000
    };
    lcd.createChar(2+i, ct_timer);
  }
}


// ====================================================================


void loop() {
  // 압력센서 //
  for ( int i=54; i<=59; i++) {
    if ( ppAfter[i-54] == 0 ) {
      pp_list[i-54]=false;
    }
    else{
      pp_list[i-54]=true;
    }
    ppAfter[i-54]=map(analogRead(i), 0, 1024, 0, 255);
  }

  if ( dpType==2 ) {
    for ( int i=54; i<=59; i++) {
      if ( pp_onf[i-54] != pp_list[i-54] ) {
        selecter=0;
        dpTC=3;
        targetNum=i-54;
        Serial.println("PRESS:dpTC chaged 3.  ( '"+String(i-54)+"' cheaking )");
        Serial.println("PRESS:selecter chaged 0.  ( '"+String(i-54)+"' cheaking )");
      }
    }
  }
  


  // 디스플레이 //
  if ( dpType != dpTC ) {
    lcd.clear();
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
        lcd.print("Select Ac?t.");
        lcd.setCursor(1, 1);
        lcd.write(1);
        lcd.print("Put  Out");
        break;
      case 2:
        lcd.setCursor(0, 0);
        lcd.print("Put Umbrella.");
        break;
      case 3:
        lcd.setCursor(0, 0);
        lcd.print("Password.("+String(targetNum)+")");
        break;
    }
    dpType = dpTC;
  }
  if ( dpType != 0 ) { //스타트 화면 X
    lcd.setCursor(1, 0);
    if ( timer ) {
      lcd.setCursor(14, 0);
      lcd.write(2+rTimer/75);
      lcd.print(String(timer-1));
      rTimer++;
    }
    else {
      rTimer=0;
      dpTC=0;
      Serial.println("DISPLAY:dpTC chaged 0.  ( time out )");
    }
    if ( rTimer==300 ) {
      rTimer=0;
      timer--;
    }
  }

  // 키패드 //
  char key = keypad.getKey();
  //Print this button to serial monitor
  if (key != NO_KEY){
    if ( dpType==0 && key==key_sel ) {
      Serial.println("KEYPAD:dpTC chaged 1.  ( press 'select key' )");
      dpTC=1;
      selecter=0;
      timer=10;
    } 
    else if ( dpType==1 ) { //Sel Act
      if ( key==key_l && selecter ) {
        selecter=0;
        lcd.setCursor(6, 1);
        lcd.print(" ");
        lcd.setCursor(1, 1);
        lcd.write(1);
        Serial.println("KEYPAD:selecter chaged 0.  ( press 'left key' )");
      }
      if ( key==key_r && !selecter ) {
        selecter=1;
        lcd.setCursor(1, 1);
        lcd.print(" ");
        lcd.setCursor(6, 1);
        lcd.write(1);
        Serial.println("KEYPAD:selecter chaged 1.  ( press 'right key' )");
      }
      if ( key==key_sel ) {
        if ( !selecter ) {
          Serial.println("KEYPAD:dpTC chaged 2.  ( press 'select key' )");
          dpTC=2;
          timer=10;
        }
      }
    }
    Serial.println(key);
  }
}
