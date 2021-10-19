/*
  payload-read-key-sd-seeduino-xiao.ino
  proof-of-work of badusb with reading payload from micro sd on seeeeduino xiao
  Sett Sarverott
  2021
*/

#include <SPI.h>
#include <SD.h>
#include "Keyboard.h"

File payloadFile;
File delaysFile;

bool altF4_after_payload=true;

void shortMorse(){
  digitalWrite(LED_BUILTIN, LOW);
  delay(500);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(200);
}

void longMorse(){
  digitalWrite(LED_BUILTIN, LOW);
  delay(1000);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(500);
}

void sosLedBlink(){
  shortMorse();
  shortMorse();
  shortMorse();
  delay(1000);
  longMorse();
  longMorse();
  longMorse();
  delay(700);
  shortMorse();
  shortMorse();
  shortMorse();
  delay(1800);
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Keyboard.begin();
  if (!SD.begin(3)) {
    while(true){
      sosLedBlink();
    }
  }
  delay(1500);
  Keyboard.press(KEY_LEFT_GUI);
  Keyboard.press(114);
  delay(100);
  Keyboard.releaseAll();
  delay(500);
  Keyboard.print("cmd.exe");
  delay(100);
  Keyboard.press(KEY_RETURN);
  delay(50);
  Keyboard.releaseAll();
  delay(1000);
  payloadFile=SD.open("payload.txt");
  if (payloadFile) {
    int readCommand=0;
    int lineDelay=0;
    String setupLabel="";
    while(payloadFile.available()){
      char key=payloadFile.read();
      if(readCommand==0){
        switch(key){
          case '.':
            readCommand=1;
          break;
          case '#':
            readCommand=2;
          break;
          case '@':
            readCommand=3;
          break;
        }
      }else if(readCommand==1){
        if(key=='\n'){
          readCommand=0;
          delay(lineDelay);
          lineDelay=0;
        }else if(key>='0'&&key<='9'){
          lineDelay+=lineDelay*10+key-'0';
        }
      }else if(readCommand==2){
        if(key==0){
          NULL;
        }else if(key=='\n'){
          readCommand=0;
          Keyboard.press(KEY_RETURN);
          delay(50);
          Keyboard.releaseAll();
          delay(50);
        }else{
          Keyboard.print(key);
        }
      }else if(readCommand==3){
        if(key==0){
          NULL;
        }else if(key=='\n'){
          readCommand=0;
          if(setupLabel=="noexit"){
            altF4_after_payload=false;
          }
          setupLabel="";
        }else{
          setupLabel+=key;
        }
      }
    }
    payloadFile.close();

    delay(1000);
    Keyboard.press(KEY_LEFT_ALT);
    delay(50);
    Keyboard.press(KEY_F4);
    delay(50);
    Keyboard.releaseAll();
  }else{
    Keyboard.press(KEY_LEFT_ALT);
    delay(50);
    Keyboard.press(KEY_F4);
    delay(50);
    Keyboard.releaseAll();
    while(true){
      shortMorse();
      longMorse();
    }
  }
}

void loop() {
  longMorse();
}
