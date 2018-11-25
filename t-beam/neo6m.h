#ifndef Neo6m_h
#define Neo6m_h

#include "HardwareSerial.h"

#define byte uint8_t
#define GPS_TX 12
#define GPS_RX 15

class Neo6m
{
  public:
    Neo6m();
    void turnOff();
    void softwareReset();
    
  private:
  bool getUBX_ACK(const uint8_t *MSG);
  //u-blox 6 Receiver Description 9.5 Power On/Off command and 36.3 RXM-PMREQ
  const byte RXM_PMREQ[16] = {0xb5, 0x62, 0x02, 0x41, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x4d, 0x3b};   //power off until wakeup call
  //31.19 CFG-RST (0x06 0x04) checksum calculation https://gist.github.com/tomazas/3ab51f91cdc418f5704d
  const byte CFG_RST[12] = {0xb5, 0x62, 0x06, 0x04, 0x04, 0x00, 0x00, 0x00, 0x01,0x00, 0x0F, 0x66};//Controlled Software reset
};

#endif
