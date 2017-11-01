/*
Name:    rotator_oo.ino
Created: 9/14/2017 2:22:35 PM
Author:  Blake
*/
#include "G5500.h"
#include "Arduino.h"	
//todo set moving tolerence for calibration and regular tolerence
#define NUMBEROFROTATORS 1
#define BIT 4095.0
//TODO Manuel overRide
	void setup() {
		
		Serial.begin(9600);

	}

	void loop() {
		int i = 0;
		int j = 0;
		int k = 2;//0 and 1 are taken up by serial communication
		String command;
		G5500 rotator[NUMBEROFROTATORS];
		//this will only work if pins are used in order azimuth first
		for (i = 0; i < NUMBEROFROTATORS; i++) {
			rotator[i].setAzimuthReadPin(j);
			j++;
			rotator[i].setElevationReadPin(j);
			j++;
			rotator[i].setAzimuthUpPin(k);
			k++;
			rotator[i].setAzimuthDownPin(k);
			k++;
			rotator[i].setElevationUpPin(k);
			k++;
			rotator[i].setElevationDownPin(k);
			k++;
			pinMode(rotator[i].getAzimuthUpPin(), OUTPUT);
			pinMode(rotator[i].getAzimuthDownPin(), OUTPUT);
			pinMode(rotator[i].getElevationUpPin(), OUTPUT);
			pinMode(rotator[i].getElevationDownPin(), OUTPUT);
     rotator[i].fullStop();
     //TODO define tolerences 
     
		}
   
		while (true) {
			if (Serial.available()) {
        
				command =serialRead();//gets input from serial port
				parseInput(command, &rotator[0]); //takes that input and splits it up and handles the command
       
			}
			for (i = 0; i < NUMBEROFROTATORS; i++) {
				//initiate movement
				//the fact this function is called even when the rotator is moving helps ensure the rotator will correct if it passed its position for any reason
				rotator[i].moveCheckAzimuth();
				rotator[i].moveCheckElevation();
				//check calibration
				rotator[i].calibrationCheckAzimuth();
				rotator[i].calibratinCheckElevation(); 
			}
		}

	}

	void parseInput(String command, G5500 *rotator) {
    int i;
		int endIndex;
		String nextValue;
		if (command.startsWith("position")) {
			//print position
		}
		else if (command.startsWith("setRotator")) {
			//get nesscary positions
			command = command.substring(10); //Removes "setRotator"
											 //-1 will signify that there is not change, this will not be provided by the user, it must be done on the pi
			for (i = 0; i < NUMBEROFROTATORS; i++) {//parsing the input into the positions
				endIndex = command.indexOf(',');
				(rotator + i)->setPositionAzimuth(command.substring(0, endIndex).toFloat());//FIXME
				command = command.substring(endIndex + 1);
				endIndex = command.indexOf(',');
				(rotator + i)->setPositionElevation(command.substring(0, endIndex).toFloat());//FIXME
				command = command.substring(endIndex + 1);
			}
		}
		else if (command.startsWith("calibrate")) {
			//calibrate
			//what needs to be calibrated
			//set the calibration to false
			command = command.substring(10);
			for (i = 0; i < NUMBEROFROTATORS; i++) {//parsing the input into two arrays

				endIndex = command.indexOf(',');
				nextValue = command.substring(0, endIndex);
				if (nextValue.startsWith("true") || nextValue.startsWith("1")) {
					(rotator + i)->setToCalibrateAzimuth();//FIXME
				}
				
				command = command.substring(endIndex + 1);
				endIndex = command.indexOf(',');
				nextValue = command.substring(0, endIndex);
				if (nextValue.startsWith("true") || nextValue.startsWith("1")) {
					(rotator + i)->setToCalibrateElevation();//FIXME
					
				}
				command = command.substring(endIndex + 1);
			}
		}
		else {
			//fail
		}
	}
 String serialRead() {
  String command;//incoming string
  char input;//incoming charcters
  while (Serial.available()) {
    input = Serial.read();
    Serial.print(input);
    command = command + input; //takes incoming charaters and creates a string
    delay(10);//keeps junk out
  }
  
  return (command);
}
