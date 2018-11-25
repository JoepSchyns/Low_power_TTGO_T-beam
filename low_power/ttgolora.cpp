#include "ttgolora.h" //https://github.com/sandeepmistry/arduino-LoRa

TTGOLoRa::TTGOLoRa() {
  SPI.begin(SCK,MISO,MOSI,SS); 
  LoRa.setPins(SS, RST, DI0);
  start();

}

void TTGOLoRa::enable_sleep(){
  LoRa.sleep();
}

bool TTGOLoRa::start(){
    return LoRa.begin(868E6);
}
