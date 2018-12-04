#include "t_beam.h"

T_beam::T_beam(){
  gps = new Neo6m();
  lora = new TTGOLoRa();
  
    //turn off blue LED
  pinMode(LED_PIN, OUTPUT); 
  digitalWrite(LED_PIN, LOW);

}
TinyGPSLocation T_beam::getLocation(){
  return gps->waitForLocation();
}


//put the t-beam sleep with the least amount off power consumption as possible
void T_beam::low_power_deep_sleep_timer(uint64_t time_in_us){
  gps->enable_sleep();
  lora->enable_sleep();
  //isolateGPIO();
  turnOffWifi();
  turnOffBluetooth();

  //enable
  esp_sleep_enable_timer_wakeup(time_in_us);
  esp_deep_sleep_start();
}

//Isolate all GPIOpins on the t-beam
void T_beam::isolateGPIO() {
  for (int i = 0; i < 40; i++) {
    gpio_num_t pin = gpio_num_t(i);
    if (rtc_gpio_is_valid_gpio(pin)) {
      rtc_gpio_isolate(pin);
    }
  }
}

//Power-down of RTC peripherals
void  T_beam::turnOffRTC(){
  esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_OFF);
  esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_SLOW_MEM, ESP_PD_OPTION_OFF);
  esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_FAST_MEM, ESP_PD_OPTION_OFF);
  esp_sleep_pd_config(ESP_PD_DOMAIN_XTAL, ESP_PD_OPTION_OFF);
}

//Turn off wifi and clear memory for wifi
void T_beam::turnOffWifi(){
  esp_wifi_stop();
  esp_wifi_deinit();
}

//Turn off bluetooth and clear memory for bluetooth
void T_beam::turnOffBluetooth(){
  esp_bluedroid_disable();
  esp_bluedroid_deinit();
  esp_bt_controller_disable();
  esp_bt_controller_deinit();
}
