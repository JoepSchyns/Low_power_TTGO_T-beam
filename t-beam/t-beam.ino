#include <esp_wifi.h>
#include <esp_bt.h>
#include <esp_bt_main.h>
#include <esp_bt.h>
#include "driver/rtc_io.h"
//#include "driver/adc.h"  //adc_power_off(); no effect

//brown out   //WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); no effect
//#include "soc/soc.h"
//#include "soc/rtc_cntl_reg.h"

#include "neo6m.h"
#include "ttgolora.h"

#define LED_PIN 14

#define uS_TO_S_FACTOR 1000000  /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP 360        /* Time ESP32 will go to sleep (in seconds) */

RTC_DATA_ATTR int bootCount = 0;


void print_wakeup_reason() {
  esp_sleep_wakeup_cause_t wakeup_reason;

  wakeup_reason = esp_sleep_get_wakeup_cause();

  switch (wakeup_reason)
  {
    case 1  : Serial.println("Wakeup caused by external signal using RTC_IO"); break;
    case 2  : Serial.println("Wakeup caused by external signal using RTC_CNTL"); break;
    case 3  : Serial.println("Wakeup caused by timer"); break;
    case 4  : Serial.println("Wakeup caused by touchpad"); break;
    case 5  : Serial.println("Wakeup caused by ULP program"); break;
    default : Serial.printf("Wakeup was not caused by deep sleep: %d\n", wakeup_reason); break;
  }
}

//method to turn off wifi and bluetooth of esp32
void turnOff(String namer, esp_err_t disable(), esp_err_t dinit()) {
  esp_err_t disableError = disable();
  esp_err_t dinitError = dinit();
  //Serial.println(String(dinit));
  Serial.println("TurnOff: " + namer);
  Serial.println("Disable: " + String(esp_err_to_name(disableError)));
  Serial.println("Dinit: " + String(esp_err_to_name(dinitError)));
}

bool disableGPIO() {
  int resultError = 0;
  for (int i = 0; i < 40; i++) {
    gpio_num_t pin = gpio_num_t(i);
    
    if (rtc_gpio_is_valid_gpio(pin)) {
      esp_err_t error = rtc_gpio_isolate(pin);
      resultError += error;
      //Serial.println("isolate GPIO_NUM_" + String(pin) + ": " + String(esp_err_to_name(error)));
    }

  }
  return resultError == 0;
}


void setup() {
  Serial.begin(115200);
  delay(1000); //Take some time to open up the Serial Monitor

  //connect to gps (Neo-6m) chip and start power savings
  Neo6m gps = Neo6m();
  gps.turnOff();

  //connect to lora (sx1276) chip and start power savings
  TTGOLoRa lora = TTGOLoRa();
  lora.turnOff();

  //turn off blue LED
  pinMode(LED_PIN, OUTPUT); 
  digitalWrite(LED_PIN, LOW);

  //disable GPIO
  bool gpio_error = disableGPIO();
  Serial.println("GPIO pins disabled: " + String(gpio_error));


  //turn off bluetooth and wifi
  turnOff("bluedroid", esp_bluedroid_disable, esp_bluedroid_deinit);
  turnOff("wifi", esp_wifi_stop, esp_wifi_deinit);
  turnOff("bt_controller", esp_bt_controller_disable, esp_bt_controller_deinit);

  //Power-down of RTC peripherals
  Serial.println("ESP_PD_DOMAIN_RTC_PERIPH: " + String(esp_err_to_name(esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_OFF))));
  Serial.println("ESP_PD_DOMAIN_RTC_SLOW_MEM: " + String(esp_err_to_name(esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_SLOW_MEM, ESP_PD_OPTION_OFF))));
  Serial.println("ESP_PD_DOMAIN_RTC_FAST_MEM: " + String(esp_err_to_name(esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_FAST_MEM, ESP_PD_OPTION_OFF))));
  Serial.println("ESP_PD_DOMAIN_XTAL: " + String(esp_err_to_name(esp_sleep_pd_config(ESP_PD_DOMAIN_XTAL, ESP_PD_OPTION_OFF))));
  
  //Increment boot number and print it every reboot
  ++bootCount;
  Serial.println("Boot number: " + String(bootCount));

  //Print the wakeup reason for ESP32
  print_wakeup_reason();

  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  Serial.println("Setup ESP32 to sleep for every " + String(TIME_TO_SLEEP) +
                 " Seconds");

  Serial.println("Going to sleep now");
  Serial.flush();
  esp_deep_sleep_start();
  
  Serial.println("This will never be printed");
}


void loop() {
  //This is not going to be called
}
