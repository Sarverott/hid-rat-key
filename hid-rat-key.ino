/*
   ~ HID-RAT-KEY ~
  rattish lang implementation for badUSB's payloads
  Sett Sarverott
  2021
*/

#include <Keyboard.h>
#include <SPI.h>
#include <SD.h>

#include "./paniccodes.h"

bool setPause(char inputKey, int *delayPeriod){
  if(inputKey=='\n'){
    delay(*delayPeriod);
    return false;
  }else if(inputKey=='\r'){
    return true;
  }else if(inputKey>='0'&&inputKey<='9'){
    (*delayPeriod)*=10;
    (*delayPeriod)-='0';
    (*delayPeriod)+=inputKey;
    return true;
  }else{
    panicSignal(WRONG_SYNTAX_PANIC);
    return true;
  }
}

bool pressKeyCode(char inputKey, char *pressingKey){
  if(inputKey=='\n'){
    Keyboard.press(*pressingKey);
    return false;
  }else if(inputKey=='\r'){
    return true;
  }else if(inputKey>='0'&&inputKey<='9'){
    (*pressingKey)*=10;
    (*pressingKey)-='0';
    (*pressingKey)+=inputKey;
    return true;
  }else{
    panicSignal(WRONG_SYNTAX_PANIC);
    return true;
  }
}

bool releaseKeys(char inputKey){
  Keyboard.releaseAll();
  return inputKey!='\n';
}

bool keyboardWrite(char inputKey){
  if(inputKey==0){
    return true;
  }else if(inputKey=='\n'){
    Keyboard.press(KEY_RETURN);
    delay(50);
    Keyboard.releaseAll();
    delay(50);
    return false;
  }else{
    Keyboard.print(inputKey);
    return true;
  }
}
void executePayload(char*,bool,int);

bool getIncludingFile(
  char inputKey,
  char *filepath,
  int *counter,
  int recurentionsLimit
){
  if(inputKey==0){
    return true;
  }else if(inputKey=='\r'){
    return true;
  }else if(inputKey=='\n'){
    while(*counter<100){
      filepath[*counter]='\0';
      *counter+=1;
    }
    executePayload(filepath, true, recurentionsLimit-1);
    return false;
  }else{
    filepath[*counter]=inputKey;
    *counter+=1;
    return true;
  }
}

bool commandSwitch(
  char command,
  char inputKey,
  int *lineDelay=NULL,
  char *pressingKey=NULL,
  char *subfilename=NULL,
  int *subfilenamecharpointer=NULL,
  int recurentionsLimit=10
){
  switch(command){
    case '^':
      return releaseKeys(inputKey);
    case '_':
      return pressKeyCode(inputKey, pressingKey);
    break;
    case '.':
      return setPause(inputKey, lineDelay);
    break;
    case '<':
      return keyboardWrite(inputKey);
    break;
    case '@':
      return getIncludingFile(
        inputKey,
        subfilename,
        subfilenamecharpointer,
        recurentionsLimit
      );
    break;
    case '#':
      return inputKey!='\n';
  }
  return false;
}

void executePayload(
  char *filename,
  bool includingMode=false,
  int recurentionsLimit=10
){
  if(recurentionsLimit==0){
    panicSignal(TOO_MANY_RECURENT_CALLS_PANIC);
  }
  File payloadFile=SD.open(filename);
  if(payloadFile){

    char currentCommand=0;

    char subfilename[100];
    char pressingKey=0;
    int lineDelay=0;
    int subfilenamecharpointer=0;

    bool lineFlag=true;

    while(payloadFile.available()){
      char inputKey=payloadFile.read();
      if(currentCommand==0){
        currentCommand=inputKey;
      }else{
        lineFlag=commandSwitch(
          currentCommand,
          inputKey,
          &lineDelay,
          &pressingKey,
          subfilename,
          &subfilenamecharpointer,
          recurentionsLimit
        );
        if(!lineFlag){
          currentCommand=0;
          pressingKey=0;
          lineDelay=0;
          subfilenamecharpointer=0;
          while(subfilenamecharpointer<100){
            subfilename[subfilenamecharpointer]='\0';
            subfilenamecharpointer+=1;
          }
          subfilenamecharpointer=0;
        }
      }
    }
    payloadFile.close();
  }else if(includingMode){
    panicSignal(LOST_FILE_PANIC);
  }else{
    panicSignal(NO_PAYLOAD_PANIC);
  }
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Keyboard.begin();
  if(!SD.begin(3)) {
    Keyboard.end();
    panicSignal(NO_CARD_PANIC);
  }
  delay(1500);
  executePayload("payload.rat");
  delay(1000);
  Keyboard.end();
}

void loop() {
  panicSignal(NO_MORE_WORK_PANIC);
}
