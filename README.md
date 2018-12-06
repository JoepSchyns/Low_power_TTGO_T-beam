# Low power TTGO T-beam 

The TTGO T-beam is a microcontrollers with a ESP32, WiFi, Bluetooth, GPS, LORA and Battery handling. Therefore it is especially suitable for off grid applications. However, little documentation can be found on reducting battery consumption.

This sketch demonstrates how battery consumption can be reduced to 10ma during sleep, see image below. Please feel free to push more power reduction methods.

<table class="image">
  <tr><td><img src="https://joep.space/img/ttgo_t-beam_power-consumption_2.jpg" width="250" alt="TTGO T-Beam 10ma power comsumption"/></td></tr>
  <figcaption align="bottom"><sub><sup>TTGO T-Beam 10ma power comsumption</sup></sub></figcaption>
</table>

## Components
| Chip        | Application   | Documentation                                                            | Power reduction methods  |
| ----------- |-------------  | ------------                                                             |-------                   |
| Esp 32      | Processor     | <https://docs.espressif.com/projects/esp-idf/en/latest/>                 | Deep sleep               |
| -           |      -        |                           -                                              | Power-down of RTC peripherals and memories  |
| -           |      -        |                           -                                              | GPIO isolation            |
| -           |      -        |                           -                                              | LED 14 low                |
| Neo-6m      | GPS           | <https://www.u-blox.com/en/product/neo-6-series>                         | Power Save Mode           |
| SX1276      | Lora          | <https://www.semtech.com/products/wireless-rf/lora-transceivers/SX1276>  | Sleep mode                | 
| CP2104      | USN to UART bridge | <https://www.silabs.com/products/interface/usb-bridges/classic-usb-bridges/device.cp2104> | TODO/not possible |
| w25q32 | Flash memory       | <https://www.winbond.com/resource-files/w25q32jv%20dtr%20revf%2002242017.pdf> | TODO/not possible |
| tp5400 | Battery management | <http://www.tpwic.com/index.php?m=content&c=index&a=show&catid=172&id=71> | TODO/not possible |
| 4a2d   | Voltage regulator  | <https://www.st.com/resource/en/datasheet/ld3985.pdf> | TODO/not possible |

## Neo-6m UBX communication
The t-beam gps chip (Neo-6m) can be put into sleep by commisioning a power save command using proprietary UBX protocol over Neo's RX TX pins. In normal mode of operation the chip sends out location using NMEA protocol. Switching between these protocols can be done via pin 14 and 15 (see Data Sheet 1.15). However, as can be seen in the image below, these pins are not connected on the T-beam. During testing is established that the Neo does execute UBX commands in NMEA mode but does not send back acknowledgements. Therefore UBX commands, such as the sleep command, that do not require acknowledgements work out of the box. 
Nonetheless, protocols switching pins 14 and 15 are exposed on the t-beam. Thus the pins could be soldered to two GPIO pins by hand. By connectings these pins UBX acknowledgements could be enabled, this is however not an necessity. 
<table class="image">
  <tr><td><img src="https://joep.space/img/t-beam_neo.jpg" width="150" alt="TTGO T-Beam Neo-6m hardware configuration"/></td></tr>
  <figcaption align="bottom"><sub><sup>TTGO T-Beam Neo-6m hardware configuration</sup></sub></figcaption>
</table>

## Dependencies
* esp32 Arduino core: <https://github.com/espressif/arduino-esp32>
* Arduino LoRa: <https://github.com/sandeepmistry/arduino-LoRa>
## Useful links
* Official Github: <http://github.com/LilyGO/TTGO-T-Beam>
* TTGO T-beam wiki: <http://tinymicros.com/wiki/TTGO_T-Beam>
