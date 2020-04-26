#include <SD.h>


/*
  AK975X Human Presence and Movement Sensor Example Code
  By: Nathan Seidle
  SparkFun Electronics
  Date: March 10th, 2017
  License: This code is public domain but you buy me a beer if you use this and we meet someday (Beerware license).

  Outputs the four IR readings and internal temperature.

  IR readings increase as warm bodies enter into the observable areas.

  Hardware Connections:
  Attach a Qwiic shield to your RedBoard or Uno.
  Plug the Qwiic sensor into any port.
  Serial.print it out at 9600 baud to serial monitor.
*/

#include <Wire.h>

#include "SparkFun_AK975X_Arduino_Library.h" //Use Library Manager or download here: https://github.com/sparkfun/SparkFun_AK975X_Arduino_Library

AK975X movementSensor; //Hook object to the library

#define LOG_INTERVAL  1000 
#define ECHO_TO_SERIAL  1
#define WAIT_TO_START  0

int ir1, ir2, ir3, ir4, temperature;
const int chipSelect = 10;
const int Blue = 7;
const int Orange = 6;
const int Green = 5;
File logfile;


void setup()
{
  Serial.begin(9600);
  while(!Serial.available());
    Serial.print("Start SD");
    pinMode(10,OUTPUT);
     if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }
  Serial.println("card initialized.");

  char filename[] = "LOGGER00.CSV.";
  for(uint8_t i = 0; i< 100; i++){
    filename[6] = i/10 +'0';
    filename[7] = 1%10 + '0';
    if (!SD.exists(filename)) {
      logfile = SD.open(filename,FILE_WRITE);
      break;
    }
  }

  Serial.print("logging to: ");
  Serial.println(filename);
 
  Wire.begin();


  //Turn on sensor
  if (!movementSensor.begin())
  logfile.println("ir1,ir2,ir3,ir4,temp,millis");

  }
  

void loop()
{
  DateTime now;
  delay ((LOG_INTERVAL -1)-(millis() % LOG_INTERVAL));
  digitalWrite(Green,HIGH);
  if (movementSensor.available())
  {
    ir1 = movementSensor.getIR1();
    ir2 = movementSensor.getIR2();
    ir3 = movementSensor.getIR3();
    ir4 = movementSensor.getIR4();
    float tempF = movementSensor.getTemperatureF();

    movementSensor.refresh(); //Read dummy register after new data is read    logfile.print(,);
   
    //Note: The observable area is shown in the silkscreen.
    //If sensor 2 increases first, the human is on the left
    Serial.print("1:DWN[");
    Serial.print(ir1);
    Serial.print("]\t2:LFT[");
    Serial.print(ir2);
    Serial.print("]\t3:UP[");
    Serial.print(ir3);
    Serial.print("]\t4:RGH[");
    Serial.print(ir4);
    Serial.print("]\ttempF[");
    Serial.print(tempF);
    Serial.print("]\tmillis[");
    Serial.print(millis());
    Serial.print("]");
    Serial.println();
    if (logfile){
    logfile.print(ir1);
    logfile.print(ir2);
    logfile.print(ir3);
    logfile.print(ir4);
    logfile.print(tempF);
    logfile.println();
    logfile.close();
  }
  }

  delay(1);
}
