A ESP8266-oled app  that uses the Canon CCAPI to act as a remote intervalometer over wifi. With a display to set the intervalometer parameters and also, the option to set these parameters by a web server.
I had several DSLR cameras, with a jack input for a remote shutter/intervalometer. And i usually use it, for timelapses or astrophotography.  But my new Canon R50 (the camera i use for travel without carry a heavy load in my back) doesnt have this input jack. So i decided to make my own remote shutter/intervalometer using the features that Canon provide by CCAPI.

The ESP8266 with an OLED display integrated has the option to develop an intervalometer without attach HW components/soldering... etc. Because this board has an OLED display integrated and a programmable button (enough for this project). And also, has a battery connection to attach a battery with a battery charge module integrated (with protection for overcharging, discharging... ). So, you can use it with a small battery. With this board, you can use it "standalone", without need a phone or usb charger. 
This is a "little bit" older board (It is already difficult to find in some stores, Heltec... etc) that i already have at home useless. But for this purpose, meet the requirements i need.


## 📖 Index
1. [Hardware](#Hardware)
2. [Features](#features)
3. [Camera Configuration](#camera-configuration)
   - [Activate CCAPI](#activate-ccapi)
   - [Create a WiFi Connection](#create-a-wifi-connection)
4. [Software Installation (Arduino IDE)](#software-installation-arduino-ide)
5. [Usage](#usage)
   - [Using the OLED Display Interface](#using-the-oled-display-interface)
   - [Using the Web Server Interface](#using-the-web-server-interface)
6. [External Libraries](#external-libraries)
7. [To-Do and Improvements](#to-do-and-improvements)
8. [Future Work and Thoughts](#future-work-and-thoughts)


## Hardware
- WIFI OLED ESP8266 board. https://es.aliexpress.com/item/1005005412921002.html
- Battery (optional): https://es.aliexpress.com/item/32990147903.html

## Features
Very basic options (the options i usually need to make timelapses and astrophotography, and sometimes, as a remote shutter. So i dont need a lot of features).
Select the time and number of shoots.
Ability of make single shoots (act as a single remote shoot dongle)
Use the intervalometer with the small OLED display integrated in the ESP8266 or use the intervalometer by a phone connected to the ESP8266 wifi network (with a bigger display).
With "screensaver" if the ESP8266 is not used (in stand by) in X time (you can change it by code in the const var TIMEOUT_MENU_OFF) time the OLED display and the ESP8266 goes to deep sleep (the board has a RESET button to wake up). Designed it to be used with battery. After an "intervalometer-work" is finished, if nobody use it, the ESP8266 will switchoff after X seconds (you can change it by code in the const var TIMEOUT_MENU_OFF).
The IPs are fixed. 


## Configure Camera:

You will need to know if your camera is able to run CCAPI. Check the list in this link: https://developercommunity.usa.canon.com/s/article/CCAPI-Supported-Cameras
After that, the next step is enable the CCAPI in your camera, create a wifi connection to your ESP8266 wifi AP network.

 ### activate CCAPI
 First of all, ensure your Canon camera support CCAPI. Check these guidelines to register as a Canon Developer, and download the tool to activate the "developer mode" in your camera. It depends of your geographical area, check these links for instructions:
- North, Central, and South America: https://developercommunity.usa.canon.com/
- EMEA and Asia: https://developers.canon-europe.com/s/
- Oceania: https://www.canon.com.au/support/support-news/support-news/

If you need a "step by step" guide, i recommend this github project that is pretty well explained: https://github.com/evoth/esp-intervalometer

 ### create a Wifi connection
  Create in your camera a wifi network. Check the following link to see a step by step guide to configure CCAPI and create a wifi connection:
  https://cam.start.canon/en/C017/manual/html/UG-06_Network_0130.html
  I suggest to pre-create a wifi connection before run your ESP8266.
  
## Install software (Arduino IDE)
 This code has been developed with Arduino IDE. Open, compile and upload your code to a NodeMCU with OLED display. For details about your board see the technical specs of your board (but usually will be a ESP8266 or nodeMCU or similar). In my case, it is a NodeMCU ESP 12E.

<img src="images/ide.jpg" width="600">
 

## Usage
This app has 2 ways of use:

### by OLED display interface: 
The ESP8266 Oled display, its a board with an integrated OLED 0.95 display and a programmable button. Enough to manage an option menu to configure intervalometer params and see the progress of the shooting process. With this mode, you can use it without connect a phone/computer to the ESP8266 wifi (Sometimes it's annoying to change Wi-Fi on your mobile phone because when it's a network that doesn't have internet or you restart the ESP8266, the Wi-Fi changes easily and you're not aware of it in your phone)


When start the ESP8266, the board create a WIFI AP network and also creates a web server.
<img src="images/2025-03-21 13.11.04.jpg" width="500">

I suggest that, first of all, create a connection profile (CCAPI Wifi connection) in your camera before start and connect the ESP8266.
Then, after that, the ESP8266 will try to connect to the Camera API REST main page, for 50 secondos aprox (you can change it by code by the const var TIMEOUT_CAMERA_CONNECT).
In this time, you should turn on the camera, select the CCAPI connect profle and to connect it, to the Wifi Network  (SSID AND PASS)

I set it to "limited time" in order to save "battery". If in this time (by default 50 seconds) the ESP8266 cant connect to the camera (one try every second), the ESP8266 will switchoff the OLED display and goes to deep sleep in order to save battery. For retry, push RESET button. With a 700mah lipo battery and the "save mode" deepsleep, the battery can be attached by months without recharge or connect to an usb powersource. In "work in progress mode", with the intervalometer working, it will work by arround 3 hours with this battery.

Once, the ESP8266 connected to the CCAPI wfi API, the ESP8266 shows the menu:
- Frequency:  --> Set the time/frequency for the intervalometer.
- Number of shoots: --> Set the number of shoots.
- Single Shoot: --> Make a single shoot (use it as a remote shutter)
- Start  --> Start the intervalometer with a frequency time of "Frequency" and number of shoots of "Number of shoots"

  <img src="images/2025-03-21 13.12.20.jpg" width="500">

The ESP8266-OLED has a programmable button (GPIO D3). 
To move across options in the menu, make a long press button (more than 1.2 seconds, this time it can be changed by code in the longPressTime variable).
To change values in an option menu, make a short press button (less than 1.2 seconds).
So, with this trick, we can use a single button to move and select across a menu and configure the intervalometer.
IPs are hardcoded in this mode.
The menu has a "timeout value". If no button pressed in X seconds. The ESP8266 goes to deepsleep and switchoff the display (time can be changed in the const var TIMEOUT_MENU_OFF). To wake the board, press RESET button.

One you have configure the params (frequency and number of shoots), you can select "Start?" and make a short press to start to make shoots.
Once the intervalometer is working appears this:

   <img src="images/2025-03-21 23.36.01.jpg" width="500">
 
You can interrupt the process by pressing RESET (the program will reset and return after connect again to the menu).


### by Web server interface: 
It works similar to use the OLED display. But, when the ESP8266 conects to the Camera, you can use your phone (or computer) to connect to the Wifi of the ESP8266 (use a diferent static IP in order to avoid problems with the Camera IP setted) main page of the ESP8266 (http://19.168.4.1/ui) and appears an interface to manage the Intervalometer:

<p align="center">
<img src="images/2025-03-21 19.49.41.jpg" width="300">
<img src="images/2025-03-21 20.07.15.jpg" width="300">
</p>

And you can configure in the same way the intervalometer and run it. When the intervalometer is running, you can see in the OLED display the progress.

If you connect to the web ui by phone. Every time you connect the wifi by phone, if you restart the ESP8266, the phone disconnect and connect automatically to another wifi with internet access (or even, the phone by itself try yo change to another trusted wifi with internet access). So, sometimes its very frustrating to remember and change the wifi connection in your phone.
By web interface, you can set AF/MF and change the IP/PORT of the camera. But changes are not persistent. Every time you restart the ESP8266 these values are set to default values.
You can see the progress of the intervalometer in the OLED display.

## External libraries:
All libraries used are "standard" libraries for ESP8266, except the OLED display library (esp8266-OLED). Instructions for download and install from: https://github.com/klarsys/esp8266-OLED

## to do and improvements:
- improve UI in the web server (see the status in the web).
- Add single shoot feature to the web server.
- add bulb mode.
- improve the oled display showing remaining time for the next shoot.
- use of internal timer instead of "count milis".
- The option to cancel "intervalometer work" with the programmable button instead the RESET button.
- move texts (web and oled-menu) to a separate class, to "develop" languaje packs.
- add MF mode (but i realized that if you set MF in the camera, the intervalometer will work in MF mode).
- add the option to switch off camera (and the ESP8266) after finished the "intervalometer work".
- add an initial delay in the start.
- add elapsed time in the timeouts (to get more precise seconds in the stand by timeouts) to swith off the ESP8266.
- Better mange of some "delays" functions.
- Option to change parameters/app settings by web and set it as permanent in ESP8266 memory.
- In the future, study the option to use deep sleep "mode" (or light sleep) in the time between shoots to save more energy (but in this case the oled display and other functions like the webserver wont work in these moments). But the ESP8266 with OLED display integrated, the RESET pin is also used by the OLED display, and in this case, we need additional wiring work (in ESP8266 the wakeup signal pin need to be connected to RST pin).
- Fiinally, design a fancy 3d case to host the ESP8266 and the battery.

## Future work and other thoughts
So, im not sure, but i have already a LilyGo board (more powerful, with better display and features, but may be, more "power-hungry" for the oled screen) and may be, in the future, i will migrate this code to ESP32 boards (the code will be the same, except the Wifi library and the OLED display library, i guess). The integrated OLED display in ESP8266 is very small to add a lot of features (and the option to add some scroll is ugly with this tiny screen).
May be, in the future, i will attach a PIR sensor or distance sensor, easily, to develop a basic "trap camera mode"
At this moment, this project is not closed (a lot of things to do and improve) bus this project meets my personal requirements. So, future features will be developed not soon.

Finally, thanks to:
- @evoth, for inspiration and ideas: https://github.com/evoth/esp-intervalometer
- @klarsys, for the oled display library: https://github.com/klarsys/esp8266-OLED
