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
char cmd[1]; // Command
bool fanUnreachable = false;

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
  Serial.println("Ready!");
}

void loop() {
  // put your main code here, to run repeatedly:
  //start time gap for buttons
  lastButtonState=digitalRead(resetButton);
  //Global blinking lights clock
  if(millis()%1000==0&&blinkOn == false){  
    blinkOn = true;
  } else if(millis()%1000==0&&blinkOn == true){
    blinkOn = false;
  }
  //keep checking for serial usage
  if((!Serial.available()||millis()-lastSerialComm>serialTimeout)&&!safeShutdown){
    if(blinkOn == false){  
      digitalWrite(cpuShutdown,LOW);
    } else if(blinkOn == true){
      digitalWrite(cpuShutdown,HIGH);
    }
  } else if(!safeShutdown){
    digitalWrite(cpuShutdown,HIGH);
    blinkOn = false;
  }
  if(Serial.available()){ // Stop blinking because data has enetered.
    digitalWrite(cpuShutdown,HIGH);
    blinkOn = false;
  }
  // Fan unreachable blink
  if(blinkOn == false && fanUnreachable==true){  
    digitalWrite(fanOut,LOW);
  } else if(blinkOn == true && fanUnreachable==true){
    digitalWrite(fanOut,HIGH);
    Serial.println("fd");
  }
  // Apparently VSCode's Arduino implementation doesnt know about .read(), so arrays it is! 
  cmd[0] = '\0'; // Clear data
  Serial.readBytes(cmd,1);
  switch (cmd[0]){
    case 'c': // CPU usage
      analogWrite(cpuLoad,255-Serial.parseInt());
      lastSerialComm = millis();
      Serial.println(cmd);
      break;
    case 'b': // PC booted
      digitalWrite(cpuShutdown,HIGH);
      safeShutdown = false;
      lastSerialComm = millis();
      Serial.println(cmd);
      break;
    case 's': // PC safely shut down
      digitalWrite(cpuShutdown,LOW);
      digitalWrite(cpuLoad,HIGH);
      digitalWrite(cpuShutdown,HIGH);
      digitalWrite(tempA,HIGH);
      digitalWrite(tempB,HIGH);
      digitalWrite(fanOut,HIGH);
      safeShutdown = true;
      lastSerialComm = millis();
      Serial.println(cmd);
      break;
    case 'f': // Fan has gone out
      fanUnreachable = false; // Clearly false since we're getting detains about the fan
      digitalWrite(fanOut,LOW);
      tone(buzz,1000,1000);
      lastSerialComm=millis();
      Serial.println(cmd);
      break;
    case 'd': //Fan info unreachable for driver/ACPI disabled.
      fanUnreachable = true;
      lastSerialComm=millis();
      Serial.println(cmd);
      break;
    case 't': //TempA exceeded
      digitalWrite(tempA,LOW);
      tone(buzz,1500,1000);
      lastSerialComm=millis();
      Serial.println(cmd);
      break;
    case 'y': //TempB exceeded
      digitalWrite(tempB,LOW);
      tone(buzz,2000,1000);
      lastSerialComm=millis();
      Serial.println(cmd);
      break;
    case 'r': //Force lamp reset
      lastButtonPress=millis();
      noTone(buzz);
      digitalWrite(cpuLoad,HIGH);
      digitalWrite(cpuShutdown,HIGH);
      digitalWrite(tempA,HIGH);
      digitalWrite(tempB,HIGH);
      digitalWrite(fanOut,HIGH);
      lastSerialComm=millis();
      Serial.println(cmd);
      break;
    case 'l': //Foce lamp test/whole arduino reset
      wdt_disable();
      Serial.println(cmd);
      wdt_enable(WDTO_15MS);
      while (1) {}
      break;
    case 'v':
      Serial.println(cmd);
      Serial.println("1.0.5");
      lastSerialComm=millis();
      break;
    case '\n':
    case '\0':
      break;
    default: //Errors
      Serial.println(cmd);
      Serial.println('E');
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
    fanUnreachable = false;
    Serial.println("rb");
  }
  if(digitalRead(resetButton)==true&&lastButtonState==true&&millis()-lastButtonPress>5000){//Reset MCU
    Serial.println("lb");
    wdt_disable();
    wdt_enable(WDTO_15MS);
    while (1) {}
  }
}
