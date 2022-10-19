#include "Servo.h"
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

unsigned long then, timeout=0;
int degArray[181] = {180,179,178,177,176,175,174,173,172,171,170,169,168,167,166,165,164,163,162,161,160,159,158,157,156,155,154,153,152,151,150,149,148,147,146,145,144,143,142,141,140,139,138,137,136,135,134,133,132,131,130,129,128,127,126,125,124,123,122,121,120,119,118,117,116,115,114,113,112,111,110,109,108,107,106,105,104,103,102,101,100,99,98,97,96,95,94,93,92,91,90,89,88,87,86,85,84,83,82,81,80,79,78,77,76,75,74,73,72,71,70,69,68,67,66,65,64,62,63,61,60,59,58,57,56,55,54,53,52,51,50,49,48,47,46,44,45,43,42,41,40,39,38,37,36,35,34,33,32,31,30,28,29,27,26,25,24,23,22,21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0};
int speedArray[181] = {0,0,0,0,0,0,0,0,0,0,0,0,0,2,5,7,9,10,13,15,18,20,21,22,23,25,26,27,28,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,50,50,51,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,75,76,77,78,79,80,81,82,83,84,85,87,88,89,90,91,92,93,94,95,96,97,98,99,99,99,102,103,105,106,107,108,109,110,111,112,113,114,115,116,117,119,120,121,122,122,123,125,126,127,128,129,130,131,132,133,134,136,137,138,139,140,141,143,144,144,144,146,148,149,151,153,153,154,155,156,157,159,161,162,163,164,165,166,167,168,169,170,171,172,173,175,177,179,180,182,183,184,185,186,187,189,190,191,192,193,194,195,196,198};
int rpmArray[143] = {0,0,0,0,0,0,0,0,0,0,0,200,300,400,500,600,700,900,1000,1050,1100,1200,1250,1300,1350,1400,1500,1550,1600,1650,1700,1750,1800,1850,1900,2000,2050,2100,2150,2200,2250,2300,2350,2450,2500,2550,2600,2700,2750,2800,2850,2900,2950,3000,3050,3150,3200,3250,3300,3350,3400,3450,3500,3550,3600,3700,3750,3800,3850,3900,4000,4050,4100,4150,4200,4250,4300,4350,4400,4450,4500,4550,4560,4650,4700,4750,4800,4850,4900,4950,5000,5050,5100,5200,5250,5300,5350,5450,5500,5550,5600,5650,5700,5750,5800,5850,5900,5950,6000,6050,6150,6200,6250,6300,6350,6450,6500,6550,6600,6650,6700,6750,6800,6850,6900,6950,7000,7050,7150,7200,7250,7300,7350,7450,7500,7550,7600,7650,7700,7750,7850,7950,8000};

int fuelArray[48] = {62,64,66,70,72,74,76,78,80,82,84,86,88,91,93,95,97,99,101,103,105,108,110,112,115,117,119,121,123,126,128,130,132,134,135,137,139,141,143,145,147,149,151,153,155,157,159,161};
int fuelDegArray[48] = {100,95,97,94,92,89,86,83,80,79,76,74,72,70,68,66,64,62,60,58,56,54,52,50,48,46,43,41,39,37,35,33,31,29,26,24,22,20,18,16,14,12,10,8,6,4,2,0};

Servo rpmsServo;
Servo speedServo;
Servo fuelServo;
LiquidCrystal_I2C lcd(0x27, 16, 2);

struct carData
{
  int spd;
  int rpm;
  int right;
  int left;
  int fuel;
};

char spd_arr[10];
char rpm_arr[10];
char right_arr[10];
char left_arr[10];
char fuel_arr[10];


// Following functions convert telemetry data to calibrated Servo values.
// eg. If the speed is 105Km/h we need to write 81 to the servo.
// HOW DO THEY WORK?  They scan data arrays and find the exact/the closest value of speed/rpm/fuel and they return the "to_write" value
// Firstly, some edge points are covered. Secondly, function scans the arrays for the exact speed/rpm/fuel value.
// If there is an exact match, function returns the "to_write" value. If not, that means that there isn't a calibrated value for the wanted speed/rpm/fuel.
// What it does then is that it scans the arrays for and finds the closest value eg. if speed is 100Km/h, the closest calibrated value is 99 which has "to_write" value of 84.
// Speed gauge in this case won't show exact 100Km/h but it will be as close as it possible can.
// These "deviations" are due to bad servos..

int convertSpeedToDegree(int spd){
  //Min. and Max. value
  if(spd>=198){return 2;}
  if(spd==1||spd==3){return 167;}
  if(spd==4){return 166;}
  if(spd<=0){return 168;}

  //Look in calibrated data array 
  for(int i=10;i<180;i++){
  if(speedArray[i]==spd){return degArray[i];}
  }

  //If this point is reached, speed is not in calibrated data -> Look for the closest speed in the array and get its degree value
  int index = 0;
  int val = abs(speedArray[14]-spd);
  for(int i=10;i<180;i++){
    int d = abs(speedArray[i]-spd);
    if(d<val){ index=i; val = d;}
  }
  return degArray[index];
}


int convertRpmsToDegree(int rpm){
  //Min & Max value
  if(rpm<=0){return 170;}
  if(rpm>8000){return 38;}

  //Look in calibrated data array
  for(int i=12;i<143;i++){
  if(rpmArray[i]==rpm){return degArray[i];}
  } 

  //If this point is reached, speed is not in calibrated data -> Look for the closest speed in the array and get its degree value
  int index = 0;
  int val = abs(rpmArray[12]-rpm);
  for(int i=12;i<143;i++){
    int d = abs(rpmArray[i]-rpm);
    if(d<val){ index=i; val=d;}
  }
  return degArray[index];
}

int convertFuelToDegree(int fuel){
    //Min & Max value
  if(fuel<=0){return 161;}
  if(fuel>=100){return 62;}

    //Look in calibrated data array
  for(int i=0;i<48;i++){
    if(fuelArray[i]==fuel){return fuelDegArray[i];}
  }

  //If this point is reached, speed is not in calibrated data -> Look for the closest speed in the array and get its degree value
  int index = 0;
  int val = abs(fuelArray[0]-fuel);
  for(int i=0;i<48;i++){
    int d = abs(fuelArray[i]-fuel);
    if(d<val){ index=i; val=d; }
  }
  return fuelDegArray[index];
}

//Emptying charArrays is necessary bcs of the following may Occur.
// eg. At first, let's say the starting rpms are 720RPM
// After pressing the throttle pedal, RPMS are rising and let's say they are now 3222
// We release the throttle pedal, RPMS are now back at 720 but without emptying the array data.rpm value is 7202 
// because the first 3 digits were substituted but, not the 4th one. Thats why, emptying char arrays is neccessary
void emptyCharArrays(){
  
  for(int i=0;i<10; i++){
    spd_arr[i]='\0';
    rpm_arr[i]='\0';
    left_arr[i]='\0';
    right_arr[i]='\0';
    fuel_arr[i]='\0';
  }
}

// Telemetry is received in the following format "speed;rpm;right_is_on;left_is_on;fuel_percentage:" e.g. "120;2500;0;0;100"
// readFunction reads and parses incoming telemetry data
//SimHub Custom Formula:
/*
format([DataCorePlugin.GameData.NewData.SpeedKmh],'0') + ';' +
format([DataCorePlugin.GameData.NewData.Rpms],'0') + ';' +
format([DataCorePlugin.GameData.NewData.TurnIndicatorRight],'0') + ';' +
format([DataCorePlugin.GameData.NewData.TurnIndicatorLeft],'0') + ';' + 
format([DataCorePlugin.GameData.NewData.FuelPercent],'0') + ':'
*/
carData readFunction(){
  carData data;
  size_t spd_read = Serial.readBytesUntil(';',spd_arr, sizeof(spd_arr)-1);
  size_t rpm_read = Serial.readBytesUntil(';',rpm_arr, sizeof(rpm_arr)-1);
  size_t right_read = Serial.readBytesUntil(';',right_arr, sizeof(right_arr)-1);
  size_t left_read = Serial.readBytesUntil(';',left_arr, sizeof(left_arr)-1);
  size_t fuel_read = Serial.readBytesUntil(':',fuel_arr,sizeof(fuel_arr)-1);
    
  data.spd = atoi(spd_arr);
  data.rpm = atoi(rpm_arr);
  data.left = atoi(left_arr);
  data.right = atoi(right_arr);
  data.fuel = atoi(fuel_arr);

   
  emptyCharArrays();
  
  return data;
}

void boardComputer(int spd, int rpm){
    //Print speed and rpms to boards
  lcd.setCursor(0,0);
  lcd.print("Speed: ");
  if(spd%2==0 || spd%5==0) {lcd.print(String(spd));}
  if(spd<100){lcd.setCursor(9,0); lcd.print("  ");}
  if(spd<10){lcd.setCursor(8,0); lcd.print("  ");}
  
  lcd.setCursor(0,1);
  lcd.print("Rpms: ");
  if(rpm%10==0){lcd.print(String(rpm));}
  if(rpm<1000){lcd.setCursor(9,1); lcd.print("  ");}
  return;
}



void setup() {
  // put your setup code here, to run once:
  // Attach servos and set them to 0 (0 on the cluster)
  fuelServo.attach(8);
  rpmsServo.attach(9);
  speedServo.attach(10);
  fuelServo.write(161);
  rpmsServo.write(170);
  speedServo.write(168);

  //Initialize Turn Indicators
  pinMode(6,OUTPUT);
  pinMode(7,OUTPUT);

  //Welcome note on the screen
  lcd.begin();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Welcome Driver");
  then = millis();
  Serial.begin(9600);
  while (!Serial)
    delay(1);    
  Serial.println("SHBorna connected");
  lcd.clear();
}

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available()>0){

  //Get telemetry data
  carData data = readFunction();

  //Convert data to calibrated servo values
  int speedDegree = convertSpeedToDegree(data.spd);
  int rpmsDegree = convertRpmsToDegree(data.rpm);
  int fuelDegree = convertFuelToDegree(data.fuel);
  int left = data.left;
  int right = data.right;
  
  //Turn Indicators operations
  if(left&&right)
  {digitalWrite(6,HIGH);
   digitalWrite(7,HIGH);}
  if(right)
  {digitalWrite(6,HIGH);}
  else{digitalWrite(6,LOW);}
  if(left)
  {digitalWrite(7,HIGH);}
  else{digitalWrite(7,LOW);}

  //Write to servos
  speedServo.write(speedDegree);
  rpmsServo.write(rpmsDegree);
  fuelServo.write(fuelDegree);

  //LCD operator
  boardComputer(data.spd, data.rpm);

   timeout=0;
  
  }
  else timeout++;
}
