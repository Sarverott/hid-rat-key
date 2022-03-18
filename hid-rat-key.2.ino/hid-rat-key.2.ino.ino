/*
   ~ HID-RAT-KEY ~
  rattish lang implementation for badUSB's payloads
  Sett Sarverott
  2021
*/

#include <Keyboard.h>
#include <SPI.h>
#include <SD.h>

#define DEFAULT_PAYLOAD_FILE "payload.rat"
#define DEFAULT_RESULT_FILE "result.txt"
#define RECURENT_ITERATIONS_LIMIT 4

struct payloader{
  File file;
  payloader* NEXT=NULL;
};

short int includeIterationCounter=RECURENT_ITERATIONS_LIMIT;

typedef int (*ratCommand) (char* arguments);


int commentaryBlank(char* arguments){return 0;}

int printOutput(char* arguments){
  Keyboard.println(arguments);
  return 0;
}
int readInput(char* arguments){
  
  return 0;
}
int waitTimePause(char* arguments){
  
  return 0;
}
int pressKeyS(char* arguments){
  
  return 0;
}
int releaseKeyS(char* arguments){
  
  return 0;
}



ratCommand rattishDictionary[256];

char argumentsBuffer[1000];
void resetArgBuffer(){
  argumentsBuffer[0]=0;
}

void setEnviroment(){
  Serial.begin(9600);
  Keyboard.begin();
}

void setNamespaces(){
  rattishDictionary['#']=commentaryBlank;

  rattishDictionary['<']=printOutput;
  rattishDictionary['>']=readInput;
  rattishDictionary['.']=waitTimePause;
  rattishDictionary['_']=pressKeyS;
  rattishDictionary['^']=releaseKeyS;
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Keyboard.begin();
  if(!SD.begin(3)) {
    Keyboard.end();
//    panicSignal(NO_CARD_PANIC);
  }
  delay(1500);
 // executePayload("payload.rat");
  delay(1000);
  Keyboard.end();
}

void loop() {
 // panicSignal(NO_MORE_WORK_PANIC);
}
