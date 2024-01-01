#include <avr/wdt.h>

//Pin definition
int cpuLoad = 3;
int cpuShutdown = 4;
int tempA = 5;
int tempB = 6;
int fanOut = 7;
int resetButton = 8;
int buzz = 9;
bool blinkOn = false;
bool safeShutdown = false;

//button state
bool lastButtonState = false;
long long lastButtonPress = millis();
//serial comms
long long lastSerialComm = millis();
int serialTimeout = 30000;
int alarmTiming = 300000;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(cpuLoad,OUTPUT);
  pinMode(cpuShutdown,OUTPUT);
  pinMode(tempA,OUTPUT);
  pinMode(tempB,OUTPUT);
  pinMode(fanOut,OUTPUT);
  pinMode(resetButton,INPUT);
  pinMode(buzz,OUTPUT);
  //Lamp test
  digitalWrite(cpuLoad,LOW);
  digitalWrite(cpuShutdown,LOW);
  digitalWrite(tempA,LOW);
  digitalWrite(tempB,LOW);
  digitalWrite(fanOut,LOW);
  tone(buzz,2000,1000);
  delay(1000);
  noTone(buzz);
  digitalWrite(cpuLoad,HIGH);
  digitalWrite(cpuShutdown,HIGH);
  digitalWrite(tempA,HIGH);
  digitalWrite(tempB,HIGH);
  digitalWrite(fanOut,HIGH);
}

void loop() {
  // put your main code here, to run repeatedly:
  //start time gap for buttons
  lastButtonState=digitalRead(resetButton);
  //keep checking for serial usage
  if(!Serial.available()&&millis()-lastSerialComm>serialTimeout&&!safeShutdown){
    if(millis()%1000==0&&blinkOn == false){  
      digitalWrite(cpuShutdown,LOW);
      blinkOn = true;
    } else if(millis()%1000==0&&blinkOn == true){
      digitalWrite(cpuShutdown,HIGH);
      blinkOn = false;
    }
  } else if(!safeShutdown){
    digitalWrite(cpuShutdown,HIGH);
  }
  char cmd[1]; 
  Serial.readBytes(cmd,1);
  switch (cmd[0]){
    case 'c': // CPU usage
      analogWrite(cpuLoad,255-Serial.parseInt());
      break;
    case 'b': // PC booted
      digitalWrite(cpuShutdown,HIGH);
      safeShutdown = false;
      break;
    case 's': // PC safely shut down
      digitalWrite(cpuShutdown,LOW);
      digitalWrite(cpuLoad,HIGH);
      digitalWrite(cpuShutdown,HIGH);
      digitalWrite(tempA,HIGH);
      digitalWrite(tempB,HIGH);
      digitalWrite(fanOut,HIGH);
      safeShutdown = true;
      break;
    case 'f': // Fan has gone out
      digitalWrite(fanOut,LOW);
      tone(buzz,1000,1000);
      break;
    case 't': //TempA exceeded
      digitalWrite(tempA,LOW);
      tone(buzz,1500,1000);
      break;
    case 'y': //TempB exceeded
      digitalWrite(tempB,LOW);
      tone(buzz,2000,1000);
      break;
    case 'r': //Force reset
      lastButtonPress=millis();
      noTone(buzz);
      digitalWrite(cpuLoad,HIGH);
      digitalWrite(cpuShutdown,HIGH);
      digitalWrite(tempA,HIGH);
      digitalWrite(tempB,HIGH);
      digitalWrite(fanOut,HIGH);
      break;
    case '\0':
      break;
    default: //Errors
      Serial.print('E');
      break;
  }
  if(lastButtonState == false &&digitalRead(resetButton)==true){//Reset alarms
    lastButtonPress=millis();
    noTone(buzz);
    digitalWrite(cpuLoad,HIGH);
    digitalWrite(cpuShutdown,HIGH);
    digitalWrite(tempA,HIGH);
    digitalWrite(tempB,HIGH);
    digitalWrite(fanOut,HIGH);
  }
  if(digitalRead(resetButton)==true&&lastButtonState==true&&millis()-lastButtonPress>5000){//Reset MCU
    wdt_disable();
    wdt_enable(WDTO_15MS);
    while (1) {}
  }
}
