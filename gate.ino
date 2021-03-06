// Laser Gate Code for 90SU Hackathon
// (C) 2016 Dave Collins

int laser = 3;
int sensor = 19;
int triggerPin = 7;

int lapCount = 0;
int sensorOff = 0;
int sensorOffTime = 0;
int triggerOff = 0;
int triggerOffTime = 0;

int lastBool = 0;
int lastTrigger = 0;

int lapMin[4];
int lapSec[4];
int lapTen[4];
float splitSpeed[4] = {0, 0, 0, 0};


int splitDist = 1;

void setup(){
  
 pinMode(laser, OUTPUT); 
 pinMode(sensor, INPUT);
 pinMode(triggerPin, INPUT);
 
 Serial.begin(115200);
 
 digitalWrite(laser, HIGH);
  
  
}

void loop(){
  
  int sensorValue = analogRead(sensor);
  
  int sensorBool = 0;
  
  int triggerValue = digitalRead(triggerPin);
  
  if (sensorValue < 100){
    
    sensorBool = 0;
    
  }
  else{
    
    sensorBool = 1;
    
  }
  
  if (sensorBool == 1 && sensorOff == 0){
    if (lastBool == 0){
    
      // Rising Edge
      
      lapCount++;
      
      sensorOff = 1;
      sensorOffTime = 0;
      
    }
  }
  
  if (triggerValue == 1 && triggerOff == 0){
    if (lastTrigger == 0){
   
     trigger();
           
      triggerOff = 1;
      triggerOffTime = 0;
      
    }
  }
  
  sensorOffTime++;
  
  if(sensorOffTime > 50){
    
    sensorOff = 0;
    
  }
  
    triggerOffTime++;
  
  if(triggerOffTime > 50){
    
    triggerOff = 0;
    
  }
  
  
  
  lapTen[lapCount]++;
  
  if(lapTen[lapCount] == 10){
   
    lapTen[lapCount] = 0;
    lapSec[lapCount]++;
    
    if(lapSec[lapCount] == 60){
      
      lapSec[lapCount] = 0;
      lapMin[lapCount]++;
    }
    
  }
  
  lastBool = sensorBool;
  lastTrigger = triggerValue;
  
  delay(100);
  
  printScreen();
  
  
}

void printScreen(){
  
  Serial.write(27); 
  Serial.print("[2J"); // clear screen 
  Serial.write(27); // ESC 
  Serial.print("[H"); // cursor to home 
  
  String lapString = "Lap ";
  
  for(int n = 1; n < 4; n++){
   
    
    lapString = lapString + n;
    lapString = lapString + ": ";
    lapString = lapString + lapMin[n];
    if(lapSec[n] < 10){
      lapString = lapString + ":0";
    }
    else{
      lapString = lapString + ":";
    }
    lapString = lapString + lapSec[n];
    lapString = lapString + ".";
    lapString = lapString + lapTen[n];
    lapString = lapString + "   Top Speed: ";
    
    
    Serial.print(lapString);
    Serial.print(splitSpeed[n]);
    Serial.println("m/s");
    
    lapString = "Lap ";
    
      
  }
  
  
}

void trigger(){
 
 float tenths = (float) lapTen[lapCount];
 float splitTime = lapSec[lapCount] + tenths/10;
 
 splitSpeed[lapCount] = splitDist / splitTime;
 
  
}
