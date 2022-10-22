[![Build Status](https://img.shields.io/badge/USEFUL%20ELECTRONICS-YOUTUBE-red)](https://www.youtube.com/user/wardzx1)

# Sponsors

### PCBWay
Tutorial Series Sponsor PCBWay Website. You can get extra free coupons using the link below:
[<img src="https://github.com/UsefulElectronics/esp32s3_la66_lorawan/blob/main/repo%20cover/banner320x100.gif">](https://www.pcbway.com/setinvite.aspx?inviteid=582640)

***
### DWIN
DWIN 7 inch 800X480 Aliexpress Coupon Codes:
Over 20USD , 20-2： 3CUIXVS75K8I
Over 50USD 50-5： LY03L4IINDGS
https://www.aliexpress.com/item/1005004840046174.html
***

# ESP32C3 DWIN 7 inch Display Weather Station
***
### [Tutorial Link](https://youtu.be/vfW4ydkPVy8) On [![Build Status](https://img.shields.io/badge/YouTube-FF0000?style=for-the-badge&logo=youtube&logoColor=white)](https://www.youtube.com/wardzx1) 
In this tutorial, a DIY weather station is built using ESP32C3 that is connected over WiFi through a TCP socket to OpenWeather API. ESP32C3 sends HTTP GET request every one minuet to get the current weather related information in JSON format after that the obtained data is parsed and sent the DWIN display over UART to show the obtained data on the screen. 

CJSON library is used for  parsing JSON file that is obtained from the OpenWeather API. FreeRTOS queue is used to let the tasks communicate and exchange data. DWIN compatible packets are created using the obtained data and sent over UART to the display. 3D printed case is also used to put all the project components and make everything look nice.

![Circuit Diagram](https://github.com/UsefulElectronics/esp32c3-tzigbee-dwin-weather-station/repo%20cover/video%20cover.png)

![Weather Station Case](https://github.com/UsefulElectronics/esp32c3-tzigbee-dwin-weather-station/repo%20cover/Weather%20station%20case.jpg)