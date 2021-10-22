/*
   ~ HID-RAT-KEY ~
  rattish lang implementation for badUSB's payloads
  Sett Sarverott
  season 3 of 2021
*/

#include <Keyboard.h>
#include <SPI.h>
#include <SD.h>

//PANIC CODES FOR ERRORS

#define NO_CARD_PANIC 0 //if sd card is not detected: char(SDC)=morse(.../-../-.-.)
#define NO_PAYLOAD_PANIC 1 //if payload.txt is not detected on sd card: char(PLD)=morse(.--./.-../-..)
#define NO_MORE_WORK_PANIC 2 //if payload execution is finished: char(FIN)=morse(..-./../-.)
#define WRONG_SYNTAX_PANIC 3 //if code throws error: char(ERR)=morse(./.-./.-.)
#define TOO_MANY_RECURENT_CALLS_PANIC 4 //if code throws error: char(REQ)=morse(.-././--.-)
#define LOST_FILE_PANIC 5 //if code throws error: char(NOE)=morse(-./---/.)
//#define LOST_FILE_PANIC 5 //if code throws error: char(LOS)=morse(.-../---/...)
#define CRITICAL_PANIC 999 //for all else issues: char(SOS)=morse(.../---/...)

//LOGIC STATES OVERRIDE

#define ON_SIGNAL LOW
#define OFF_SIGNAL HIGH

//MORSE SIGNALS SETUP

#define MORSE_LONG 1000,300
#define MORSE_SHORT 300,300
#define MORSE_CHAR_PAUSE 1000

//MORSE ALFABET

#define MORSE_A false,true //morse .-
#define MORSE_B true,false,false,false //morse -...
#define MORSE_C true,false,true,false //morse -.-.
#define MORSE_D true,false,false //morse -..
#define MORSE_E false //morse .
#define MORSE_F false,false,true,false //morse ..-.
#define MORSE_G true,true,false //morse ...
#define MORSE_H false,false,false,false //morse ....
#define MORSE_I false,false //morse ..
#define MORSE_J false,true,true,true //morse .---
#define MORSE_K true,false,true //morse -.-
#define MORSE_L false,true,false,false //morse .-..
#define MORSE_M true,true //morse --
#define MORSE_N true,false //morse -.
#define MORSE_O true,true,true //morse ---
#define MORSE_P false,true,true,false //morse .--.
#define MORSE_Q true,true,false,true //morse --.-
#define MORSE_R false,true,false //morse .-.
#define MORSE_S false,false,false //morse ...
#define MORSE_T true //morse -
#define MORSE_U false,false,true //morse ..-
#define MORSE_V false,false,false,true //morse ...-
#define MORSE_W false,true,true //morse .--
#define MORSE_X true,false,false,true //morse -..-
#define MORSE_Y true,false,true,true //morse -.--
#define MORSE_Z true,true,false,false //morse --..
#define MORSE_0 true,true,true,true,true //morse -----
#define MORSE_1 false,true,true,true,true //morse .----
#define MORSE_2 false,false,true,true,true //morse ..---
#define MORSE_3 false,false,false,true,true //morse ...--
#define MORSE_4 false,false,false,false,true //morse ....-
#define MORSE_5 false,false,false,false,false //morse .....
#define MORSE_6 true,false,false,false,false //morse -....
#define MORSE_7 true,true,false,false,false //morse --...
#define MORSE_8 true,true,true,false,false //morse ---..
#define MORSE_9 true,true,true,true,false //morse ----.

/*
LETTERS IN MORSE CODE

A = .-
B = -...
C = -.-.
D = -..
E = .
F = ..-.
G = --.
H = ....
I = ..
J = .---
K = -.-
L = .-..
M = --
N = -.
O = ---
P = .--.
Q = --.-
R = .-.
S = ...
T = -
U = ..-
V = ...-
W = .--
X = -..-
Y = -.--
Z = --..

0 = -----
1 = .----
2 = ..---
3 = ...--
4 = ....-
5 = .....
6 = -....
7 = --...
8 = ---..
9 = ----.
*/

void blinkSignal(int onTime, int offTime){
  digitalWrite(LED_BUILTIN, ON_SIGNAL);
  delay(onTime);
  digitalWrite(LED_BUILTIN, OFF_SIGNAL);
  delay(offTime);
}

void morseLetter(bool signal0){
  if(signal0){
    blinkSignal(MORSE_LONG);
  }else{
    blinkSignal(MORSE_SHORT);
  }
}
void morseLetter(
  bool signal0,
  bool signal1
){
  morseLetter(signal0);
  morseLetter(signal1);
}
void morseLetter(
  bool signal0,
  bool signal1,
  bool signal2
){
  morseLetter(signal0,signal1);
  morseLetter(signal2);
}
void morseLetter(
  bool signal0,
  bool signal1,
  bool signal2,
  bool signal3
){
  morseLetter(signal0,signal1,signal2);
  morseLetter(signal3);
}
void morseLetter(
  bool signal0,
  bool signal1,
  bool signal2,
  bool signal3,
  bool signal4
){
  morseLetter(signal0,signal1,signal2,signal3);
  morseLetter(signal4);
}

void panicSignal(unsigned short panicCode){
  while(true){
    switch(panicCode){
      case NO_CARD_PANIC: //char(SDC)=morse(.../-../-.-.)
        morseLetter(MORSE_S);
        delay(MORSE_CHAR_PAUSE);
        morseLetter(MORSE_D);
        delay(MORSE_CHAR_PAUSE);
        morseLetter(MORSE_C);
      break;
      case NO_PAYLOAD_PANIC: //char(PLD)=morse(.--./.-../-..)
        morseLetter(MORSE_P);
        delay(MORSE_CHAR_PAUSE);
        morseLetter(MORSE_L);
        delay(MORSE_CHAR_PAUSE);
        morseLetter(MORSE_D);
      break;
      case NO_MORE_WORK_PANIC: //char(FIN)=morse(..-./../-.)
        morseLetter(MORSE_F);
        delay(MORSE_CHAR_PAUSE);
        morseLetter(MORSE_I);
        delay(MORSE_CHAR_PAUSE);
        morseLetter(MORSE_N);
      break;
      case WRONG_SYNTAX_PANIC: //char(ERR)=morse(./.-./.-.)
        morseLetter(MORSE_E);
        delay(MORSE_CHAR_PAUSE);
        morseLetter(MORSE_R);
        delay(MORSE_CHAR_PAUSE);
        morseLetter(MORSE_R);
      break;
      case TOO_MANY_RECURENT_CALLS_PANIC: //char(REQ)=morse(.-././--.-)
        morseLetter(MORSE_R);
        delay(MORSE_CHAR_PAUSE);
        morseLetter(MORSE_E);
        delay(MORSE_CHAR_PAUSE);
        morseLetter(MORSE_Q);
      break;
      case LOST_FILE_PANIC: //char(NOE)=morse(-./---/.)
        morseLetter(MORSE_N);
        delay(MORSE_CHAR_PAUSE);
        morseLetter(MORSE_O);
        delay(MORSE_CHAR_PAUSE);
        morseLetter(MORSE_E);
      break;
      case CRITICAL_PANIC: //char(SOS)=morse(.../---/...)
      default:
        morseLetter(MORSE_S);
        delay(MORSE_CHAR_PAUSE);
        morseLetter(MORSE_O);
        delay(MORSE_CHAR_PAUSE);
        morseLetter(MORSE_S);
    }
    delay(2500);
  }
}

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
