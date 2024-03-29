// 라이브러리 선언 //
#include <Keypad.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>


// ====================================================================


// 압력판 변수 초기화 //
bool pp_list[]={false,false,false,false,false,false};
String pp_pwd[]={"","","","","",""};
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
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1','2','3', 'A'},
  {'4','5','6', 'B'},
  {'7','8','9', 'C'},
  {'*','0','#', 'D'}
};
byte rowPins[ROWS] = {7,6,5,4};
byte colPins[COLS] = {3,2,1,0};
/*e
const byte ROWS = 4;
const byte COLS = 3;
char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};
byte rowPins[ROWS] = {5, 0, 1, 3};
byte colPins[COLS] = {4, 6, 2};*/
const char key_l = '8';
const char key_sel = '*';
const char key_r = '2';
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

// ETC. //
int timer = 0;
int rTimer = 0;
int selecter = 0;
int targetNum = -1;
int pwdCheak=0;

int afterPpT=0;
int afterTN=0;

String outPwd="";


// ====================================================================


void setup() {
  // 시리얼 모니터 //
  Serial.begin(9600);

  // 부저 //
  pinMode(8, OUTPUT);

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

  if ( dpType==2 ) { // In
    for ( int i=54; i<=59; i++) {
      if ( pp_onf[i-54] != pp_list[i-54] && pp_onf[i-54]==false ) {
        selecter=0;
        dpTC=3;
        targetNum=i-54;
        timer=10;
        rTimer=0;
        pp_pwd[targetNum]="";
        Serial.println("PRESS:dpTC chaged 3.  ( '"+String(i-54)+"' cheaking )");
        Serial.println("PRESS:selecter chaged 0.  ( '"+String(i-54)+"' cheaking )");
      }
    }
  }
  if ( dpType!=4 ) { //Out
    for ( int i=54; i<=59; i++) {
      if ( pp_onf[i-54] != pp_list[i-54] && pp_onf[i-54]==true ) {
        tone(8, 240);
        timer=999;
        rTimer=0;
        dpTC=9;
        afterPpT=dpType;
        afterTN=i-54;
      }
    }
  }
  if ( dpType==9 ) {
    if ( pp_list[afterTN] ) {
      Serial.println("adasdadssad");
      noTone(8);
      timer=10;
      rTimer=0;
      dpTC=afterPpT;
      afterPpT=0;
      afterTN=0;
    }
  }
  


  // 디스플레이 //
  if ( dpType != dpTC ) {
    lcd.clear();
    switch(dpTC) {
      case 0:
        selecter=0;
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
      case 6:
        lcd.setCursor(0, 0);
        lcd.print("Password.("+String(targetNum+1)+")");
        lcd.setCursor(1, 1);
        lcd.print("               ");
        break;
      case 4:
        lcd.setCursor(0, 0);
        lcd.print("Good Bye!!");
        break;
      case 5:
        selecter=0;
        lcd.setCursor(0, 0);
        lcd.print("Want Umbrella.");
        lcd.setCursor(1, 1);
        lcd.write(1);
        lcd.print("1 2 3 4 5 6");
        break;
      case 7:
        lcd.setCursor(0, 0);
        lcd.print("Fail Passward..");
        break;
      case 8:
        lcd.setCursor(0, 0);
        lcd.print("No Umbrella..");
        break;
      case 9:
        lcd.setCursor(0, 0);
        lcd.print("FUCKING LOST");
        lcd.setCursor(0, 1);
        lcd.print("UMBRELLA " + String(targetNum+1));
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
      if ( dpType==3 ) {
        pp_pwd[targetNum]="";
      }
      if ( dpType==6 ) {
        outPwd="";
      }
      targetNum=-1;
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
    switch ( dpType ) {
      case 0:
        if ( key==key_sel ) {
          Serial.println("KEYPAD:dpTC chaged 1.  ( press 'select key' )");
          dpTC=1;
          selecter=0;
          timer=10;
          rTimer=0;
        }
        break;
      case 1:
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
          Serial.println(selecter);
          if ( !selecter ) { //IN
            Serial.println("KEYPAD:dpTC chaged 2.  ( press 'select key' )");
            dpTC=2;
            timer=10;
            rTimer=0;
          }
          else { //OUT
            Serial.println("KEYPAD:dpTC chaged 5.  ( press 'select key' )");
            dpTC=5;
            timer=10;
            rTimer=0;
          }
        }
        break;
      case 3:
        if ( pwdCheak == 0 ) {
          if ( key==key_l || key==key_r ) { //back speace
            timer=10;
            rTimer=0;
            pp_pwd[targetNum].remove(pp_pwd[targetNum].length()-1, 1);
            Serial.println("KEYPAD:backspeace checking.  ( "+String(key)+"/"+String(pp_pwd[targetNum])+" )");
            lcd.setCursor(1, 1);
            lcd.print("               ");
            lcd.setCursor(1, 1);
            lcd.print(pp_pwd[targetNum]);
          }
          else if ( String(key).toInt()!=0 && pp_pwd[targetNum].length()<8) { //number
            timer=10;
            rTimer=0;
            pp_pwd[targetNum]+=String(key);
            Serial.println("KEYPAD:pwd checking.  ( "+String(key)+"/"+String(pp_pwd[targetNum])+" )");
            lcd.setCursor(1, 1);
            lcd.print(pp_pwd[targetNum]);
          }
        }
        if ( key==key_sel ) {
          if ( pwdCheak == 0 ) {
            if ( pp_pwd[targetNum].length()>3 ) {
              pp_onf[targetNum]=true;
              dpTC=4;
              timer=2;
              rTimer=0;
            }
            else {
              lcd.setCursor(1, 1);
              lcd.print("               ");
              lcd.setCursor(1, 1);
              lcd.print("4-8 letters plz.");
              pwdCheak=300;
            }
          }
        }
        break;
      case 5:
        if ( key==key_l && selecter>0 ) {
          selecter--;
          lcd.setCursor(3+selecter*2, 1);
          lcd.print(" ");
          lcd.setCursor(1+selecter*2, 1);
          lcd.write(1);
          Serial.println("KEYPAD:selecter chaged 0.  ( press 'left key' )");
        }
        if ( key==key_r && selecter<5 ) {
          selecter++;
          lcd.setCursor(-1+selecter*2, 1);
          lcd.print(" ");
          lcd.setCursor(1+selecter*2, 1);
          lcd.write(1);
          Serial.println("KEYPAD:selecter chaged 1.  ( press 'right key' )");
        }
        if ( key==key_sel ) {
          Serial.println(selecter);
          if ( pp_onf[selecter] ) { //있음
            outPwd="";
            dpTC=6;
            targetNum=selecter;
            timer=10;
            rTimer=0;
            selecter=0;
            Serial.println("PRESS:dpTC chaged 6.  ( '"+String(selecter)+"' cheaking )");
          }
          else { //없음
            dpTC=8;
            timer=2;
            rTimer=0;
          }
        }
        break;
      case 6:
        if ( pwdCheak == 0 ) {
          if ( key==key_l || key==key_r ) { //back speace
            timer=10;
            rTimer=0;
            outPwd.remove(outPwd.length()-1, 1);
            Serial.println("KEYPAD:backspeace checking.  ( "+String(key)+"/"+String(outPwd)+" )");
            lcd.setCursor(1, 1);
            lcd.print("               ");
            lcd.setCursor(1, 1);
            lcd.print(outPwd);
          }
          else if ( String(key).toInt()!=0 && outPwd.length()<8) { //number
            timer=10;
            rTimer=0;
            outPwd+=String(key);
            Serial.println("KEYPAD:pwd checking.  ( "+String(key)+"/"+String(outPwd)+" )");
            lcd.setCursor(1, 1);
            lcd.print(outPwd);
          }
        }
        if ( key==key_sel ) {
          if ( pwdCheak == 0 ) {
            if ( outPwd.length()>3 ) {
              if ( outPwd==pp_pwd[targetNum] ) {
                pp_onf[targetNum]=false;
                pp_pwd[targetNum]="";
                dpTC=4;
                timer=2;
                rTimer=0;
              }
              else {
                dpTC=7;
                timer=2;
                rTimer=0;
              }
            }
            else {
              lcd.setCursor(1, 1);
              lcd.print("               ");
              lcd.setCursor(1, 1);
              lcd.print("4-8 letters plz.");
              pwdCheak=300;
            }
          }
        }
        break;
    }
    Serial.println(key);
  }
  if ( pwdCheak ) { 
    pwdCheak--;
    if ( pwdCheak==0 ) {
      lcd.setCursor(1, 1);
      lcd.print("               ");
      lcd.setCursor(1, 1);
      lcd.print(pp_pwd[targetNum]);
    }
  }
}
