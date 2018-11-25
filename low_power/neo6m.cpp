#include "neo6m.h"

HardwareSerial neoSerial(1);


Neo6m::Neo6m() {
  neoSerial.begin(9600, SERIAL_8N1, GPS_TX, GPS_RX);
}

void Neo6m::enable_sleep() { //TODO implement  UBX-ACK
  Neo6m::softwareReset(); //sleep_mode can only be activated at start up?
  delay(600); //give some time to restart //TODO wait for ack
  neoSerial.write(RXM_PMREQ, sizeof(RXM_PMREQ));
  getUBX_ACK(CFG_RST);
}

void Neo6m::softwareReset() {
  neoSerial.write(CFG_RST, sizeof(CFG_RST));
  getUBX_ACK(CFG_RST);
}

//read ack based on https://ukhas.org.uk/guides:ublox6
bool Neo6m::getUBX_ACK(const uint8_t *MSG) {
  uint8_t b;
  uint8_t ackByteID = 0;
  uint8_t ackPacket[10];
  unsigned long startTime = millis();
  Serial.print(" * Reading ACK response: ");
 
  // Construct the expected ACK packet    
  ackPacket[0] = 0xB5;  // header
  ackPacket[1] = 0x62;  // header
  ackPacket[2] = 0x05;  // class
  ackPacket[3] = 0x01;  // id
  ackPacket[4] = 0x02;  // length
  ackPacket[5] = 0x00;
  ackPacket[6] = MSG[2];  // ACK class
  ackPacket[7] = MSG[3];  // ACK id
  ackPacket[8] = 0;   // CK_A
  ackPacket[9] = 0;   // CK_B
 
  // Calculate the checksums
  for (uint8_t i=2; i<8; i++) {
    ackPacket[8] = ackPacket[8] + ackPacket[i];
    ackPacket[9] = ackPacket[9] + ackPacket[8];
  }
 
  while (1) {
 
    // Test for success
    if (ackByteID > 9) {
      // All packets in order!
      Serial.println(" (SUCCESS!)");
      return true;
    }
 
    // Timeout if no valid response in 3 seconds
    if (millis() - startTime > 3000) { 
      Serial.println(" (FAILED!)");
      return false;
    }
 
    // Make sure data is available to read
    if (neoSerial.available()) {
      b = neoSerial.read();
      Serial.print(b, HEX);
      // Check that bytes arrive in sequence as per expected ACK packet
      if (b == ackPacket[ackByteID]) { 
        ackByteID++;
        Serial.print(b, HEX);
      } 
      else {
        ackByteID = 0;  // Reset and look again, invalid order
      }
 
    }
  }
}
