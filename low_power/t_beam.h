#ifndef t_beam_h
#define t_beam_h

//wifi and bluetooth libraries
#include <esp_wifi.h>
#include <esp_bt.h>
#include <esp_bt_main.h>
#include <esp_bt.h>

//rtc libraries
#include "driver/rtc_io.h"

//onboard GPS chip management
#include "neo6m.h"

//onboard Lora chip management
#include "ttgolora.h"


#define LED_PIN 14

class T_beam
{
  public:
    T_beam();
    void low_power_deep_sleep_timer(uint64_t time_in_us);
    TinyGPSLocation getLocation();
  private:
    Neo6m *gps;
    TTGOLoRa *lora;
    void isolateGPIO();
    void turnOffRTC();
    void turnOffBluetooth();
    void turnOffWifi();
};

#endif
