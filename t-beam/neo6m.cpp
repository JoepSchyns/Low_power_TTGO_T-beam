#include "neo6m.h"

HardwareSerial neoSerial(1);


Neo6m::Neo6m() {
  


  neoSerial.begin(9600, SERIAL_8N1, GPS_TX, GPS_RX);
}

size_t Neo6m::turnOff() { //TODO implement  UBX-ACK
  Neo6m::softwareReset(); //sleep_mode can only be activated at start up?
  delay(300); //give some time to restart //TODO wait for ack
  return neoSerial.write(RXM_PMREQ, sizeof(RXM_PMREQ));
}

size_t Neo6m::softwareReset() {
  return neoSerial.write(CFG_RST, sizeof(CFG_RST));
}
