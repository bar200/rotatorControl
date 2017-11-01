#pragma once
#ifndef Motor_h
#define Motor_h


#include "Arduino.h"
class Motor {
private:
	bool calibrateUp=true;//indicates whether the rotator has finished calibrating upwards
	bool calibrateDown=true;//indicates whether the rotator has finished calibrating downwards
	bool isCalibrating=false;
  bool moving;
  unsigned long counter;
  int calibrationDelay;
  int IdNum;
	int upPin;
	int downPin;
	int readPin;
	int sampleSize;
	double lastPosition;
	double currentPosition;
	double calibrationFactor;
	double targetPositionAnalog;
  double targetPositionDegrees;
	double tolerance;
	double maxPosition;
	double minPosition;
  double maxDegrees=NULL;
  double minDegrees=NULL;
public:
    Motor(double maxDegrees, double minDegrees,int tolerence,int sampleSize,int calibrationDelay); 
    double degreesToAnalog(double degree){
    double analog;
    analog=(degree/(maxDegrees-minDegrees))*(maxPosition-minPosition);
    return(analog);
  }
	void updatePosition() {
  //analogReadResolution(4095.0);//sets read resolution
		int i;
		
		currentPosition=0;
		for (i = 0; i < sampleSize; i++) {
			currentPosition = currentPosition + analogRead(readPin);//test this
		}
   currentPosition=currentPosition/sampleSize;
	}
	void moveUp() {
		digitalWrite(downPin, LOW);
		digitalWrite(upPin, HIGH);
	}
	void moveDown() {
		digitalWrite(upPin, LOW);
		digitalWrite(downPin, HIGH);
	}
	void hault() {//sets movement pins low
		digitalWrite(upPin, LOW);
		digitalWrite(downPin, LOW);
	}
	//USE CAREFULLY
	void fullStop() {//sets movement pins low and the target position to the current position
		digitalWrite(upPin, LOW);
		digitalWrite(downPin, LOW);
		updatePosition();
		targetPositionAnalog = currentPosition;
	}
	bool isMoving() {
		
    int interval = calibrationDelay;
    unsigned long currentMillis = millis();
		
  
   if ((currentMillis - counter) >= interval) {
     Serial.println(moving);
     updatePosition();//error is here
     
    counter = currentMillis;
		if ((currentPosition <= lastPosition + tolerance) && (currentPosition >= lastPosition - tolerance)) {
			moving = false;
      
		}
		else {
			moving = true;
       
		}
   lastPosition=currentPosition;
   }
   
   else if ((currentMillis - counter) < 0) {
      counter = currentMillis;
    }
    
		return(moving);
	}
	bool isInPosition() {
		bool inPosition = false;
		updatePosition();
		if (!isCalibrating&&(currentPosition <= targetPositionAnalog + tolerance) && (currentPosition >= targetPositionAnalog - tolerance)) {
			inPosition = true;
			hault();
		}
		else {
			inPosition = false;
		}
		return(inPosition);
	}
	void calibrate() {
    
   // Serial.println(isMoving());
		if (!isMoving()&&isCalibrating) {
			if (!calibrateUp) {
				moveUp();
        moving=true;
				calibrateUp = true;
			}
			else if (!calibrateDown) {
				updatePosition();
        maxPosition=currentPosition;
				moveDown();
        moving=true;
				calibrateDown = true;
			}
			else {
				//serial print max min
        updatePosition();
        minPosition=currentPosition;
        //save to sd card
        targetPositionAnalog=degreesToAnalog(targetPositionDegrees);
        isCalibrating=!calibrateDown||!calibrateUp;
			}
     
      
		}
	}
	void moveToPosition() {
		updatePosition();
    
    //Serial.print("Target: ");
    //Serial.println(targetPosition);
    //Serial.print("actual: ");
    //Serial.println(currentPosition);
		if (!isCalibrating&&!isInPosition()) {
      
			if (currentPosition > targetPositionAnalog) {
				moveDown();
       //Serial.println("Down");
			}
			else {
				moveUp();
       //Serial.println("Up");
			}
		}
	}



	double getPosition() {
		updatePosition();
		return (currentPosition);
	}
	void setUpPin(int pin) {
		upPin = pin;
	}
	void setDownPin(int pin) {
		downPin = pin;
	}
	void setReadPin(int pin) {
		readPin = pin;
	}
	int getUpPin() {
		return(upPin);
	}
	int getDownPin() {
		return(downPin);
	}
  void setCalibrate(bool calibrate){
    isCalibrating=calibrate;
    calibrateUp=false;
    calibrateDown=false;
  }
  void setTargetPosition(double newPosition){
    if(newPosition>=0){
      targetPositionDegrees=newPosition;
      targetPositionAnalog=degreesToAnalog(newPosition);
    }
    else{
      updatePosition();
      targetPositionAnalog=currentPosition;
    }
      
  }
  void setTolerence(double newTolerence){
    if(newTolerence>=0){
      tolerance=newTolerence;
    }
  }
  void setCalibrationDelay ( int newDelay){
    calibrationDelay=newDelay;
  }
  void setSampleSize(int sample){
    sampleSize=sample;
  }
};
Motor::Motor(double maxDegrees, double minDegrees,int tolerence,int sampleSize,int calibrationDelay){
  
}

#endif
