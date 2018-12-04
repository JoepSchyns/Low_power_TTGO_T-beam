#include "neo6m.h"

HardwareSerial neoSerial(1);


Neo6m::Neo6m() {
  neoSerial.begin(9600, SERIAL_8N1, GPS_TX, GPS_RX);
  neoSerial.setTimeout(2);

  tGps = new TinyGPSPlus();
}

TinyGPSLocation Neo6m::getLocation(){
  receiveData();
  return tGps->location;
}
TinyGPSLocation Neo6m::waitForLocation(){
  wakeup();
  Serial.print("get location");
  TinyGPSLocation result;
  do{
    Serial.print("read location");
    result = getLocation();
    Serial.print(F("INVALID"));
    Serial.print(result.lat(), 6);
    Serial.print(F(","));
    Serial.print(result.lng(), 6);
    Serial.println();
  }while(!hasFix()); 
  return result;
}
bool Neo6m::hasFix(){
  if (tGps->location.isValid() && 
      tGps->location.age() < 2000 &&
      tGps->hdop.isValid() &&
      tGps->hdop.value() <= 300 &&
      tGps->hdop.age() < 2000 &&
      tGps->altitude.isValid() && 
      tGps->altitude.age() < 2000 ){
    return true;
  }
  return false;
}
void Neo6m::receiveData() {
  int data;
  int previousMillis = millis();
  while ((previousMillis + 1000) > millis())
  {
    while (neoSerial.available() )
    {
      char data = neoSerial.read();
      tGps->encode(data);
      Serial.print(data);
    }
  }
  //Serial.println("");
}

//Protocol Specification V14 11.2.3.2 Wake-up
void Neo6m::wakeup(){
  Serial.println("Wake");
  int data = -1;
  do{
    for(int i = 0; i < 20; i++){ //send random to trigger respose
        neoSerial.write(0xFF);
      }
    data = neoSerial.read();
  }while(data == -1);
  Serial.println("not sleeping");

}
void Neo6m::enable_sleep() { //TODO implement  UBX-ACK
  do{ //We cannot read UBX ack therefore try to sleep gps until it does not send data anymore
    Serial.println("try to sleep gps!");
    Neo6m::softwareReset(); //sleep_mode can only be activated at start up
    delay(600); //give some time to restart //TODO wait for ack
    neoSerial.write(RXM_PMREQ, sizeof(RXM_PMREQ));
    unsigned long startTime = millis();
    unsigned long offTime = 1;
    Serial.println(offTime);
    
    while(millis() - startTime < 1000){ //wait for the last command to finish
      int c = neoSerial.read();
      if(offTime == 1 && c == -1){ //check  if empty
        offTime = millis();
      }else if(c != -1){
        offTime = 1;
      }
      if(offTime != 1 && millis() - offTime > 100){ //if gps chip does not send any commands for .. seconds it is sleeping
        Serial.println("sleeping gps!");
        return;
      }
    }
  }while(1);
}

void Neo6m::softwareReset() {
  neoSerial.write(CFG_RST, sizeof(CFG_RST));
}
