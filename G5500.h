#pragma once
#ifndef G5500_h
#define G5500_h
#include "Motor.h" 
class G5500 {

private:
  int tolerence= 5;
  int sampleSize= 1000;
  int calibrationDelay= 2000;
  double maxAngleEl= 180;
  double minAngleEl= 0;
  double maxAngleAz= 450;
  double minAngleAz= 0;
  Motor *azimuth=new Motor(minAngleAz,maxAngleAz,tolerence,sampleSize,calibrationDelay);
  Motor *elevation=new Motor(minAngleEl,maxAngleEl,tolerence,sampleSize,calibrationDelay);
public:
  void setPositionAzimuth(double newPosition){
    azimuth->setTargetPosition(newPosition);
  }
  void setPositionElevation(double newPosition){
    elevation->setTargetPosition(newPosition);
  }
  void setToCalibrateAzimuth(){
    azimuth->setCalibrate(true);
  }
  void setToCalibrateElevation(){
    elevation->setCalibrate(true);
  }
  void calibrationCheckAzimuth(){
    azimuth->calibrate();
  }
  void calibratinCheckElevation(){
    elevation->calibrate();
  }
  void moveCheckAzimuth(){
    azimuth->moveToPosition();
  }
  void moveCheckElevation(){
    elevation->moveToPosition();
  }
  void fullStop() {
    azimuth->fullStop();
    elevation->fullStop();
  }
  void setAzimuthReadPin(int newPin){
    azimuth->setReadPin(newPin);
  }
  void setAzimuthUpPin(int newPin){
    azimuth->setUpPin(newPin);
  }
  void setAzimuthDownPin(int newPin){
    azimuth->setDownPin(newPin);
  }
  void setElevationReadPin(int newPin){
    elevation->setReadPin(newPin);
  }
  void setElevationUpPin(int newPin){
    elevation->setUpPin(newPin);
  }
  void setElevationDownPin(int newPin){
    elevation->setDownPin(newPin);
  }
  int getAzimuthUpPin(){
    return(azimuth->getUpPin());
  }
  int getAzimuthDownPin(){
    return(azimuth->getDownPin());
  }
  int getElevationUpPin(){
    return(elevation->getUpPin());
  }
  int getElevationDownPin(){
    return(elevation->getDownPin());
  }
  

};

#endif
