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

#ifndef PANIC_ON_SIGNAL
  #define PANIC_ON_SIGNAL LOW
#endif
#ifndef PANIC_OFF_SIGNAL
  #define PANIC_OFF_SIGNAL HIGH
#endif

//MORSE SIGNALS SETUP
#ifndef MORSE_LONG
  #define MORSE_LONG 1000,300
#endif
#ifndef MORSE_SHORT
  #define MORSE_SHORT 300,300
#endif
#ifndef MORSE_CHAR_PAUSE
  #define MORSE_CHAR_PAUSE 1000
#endif
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

int morseLedPin=LED_BUILTIN;

void blinkSignal(int onTime, int offTime){
  digitalWrite(morseLedPin, PANIC_ON_SIGNAL);
  delay(onTime);
  digitalWrite(morseLedPin, PANIC_OFF_SIGNAL);
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
