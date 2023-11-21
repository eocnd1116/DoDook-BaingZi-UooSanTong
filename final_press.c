#include <Keypad.h>

int pp_list[]={0,0,0,0,0,0};
int ppAfter[]={0,0,0,0,0,0};

void setup() {
  Serial.begin(9600);
  for ( int i=54; i<=59; i++) { pinMode(i, INPUT); }
}

void loop() {
  //압력센서 및 부저
  for ( int i=54; i<=59; i++) {
    if ( ppAfter[i-54] == 0 ) {
      pp_list[i-54]=1;
    }
    else{
      pp_list[i-54]=0;
    }
    ppAfter[i-54]=map(analogRead(i), 0, 1024, 0, 255);
  }

  //debuging
  for(int i=0; i<6; i++) {
    Serial.print(pp_list[i]);
  }
  Serial.println("");
}
