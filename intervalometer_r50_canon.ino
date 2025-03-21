
//wifi libs
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>

// OLED config
#include <Wire.h>
#include "OLED.h"
OLED display(4, 5);  //if using a custom oled display, set proper pins for SCL and SCA pins.
//WIFI_Kit_8's OLED connection:
//SDA -- D4
//SCL -- D5
//RST -- D2

//Basic params 
const int TIMEOUT_MENU_OFF = 150;        // Timeout secs to poweroff the board (going to deepsleep to save battery. To wakeup, push RESET button)
const int TIMEOUT_CAMERA_CONNECT = 50;   // Timeout secs trying to connect to the CAMARA CCAPI.
const bool bypassCCAPIurl = false;       // Bypass the initial connection to the camera at setup (set true only for debug purpose).

//Programmable button params
const int buttonPin = D3;           // the number of the pushbutton pin:
const long longPressTime = 1200;    // time in msecs to determine long press button.

// camera api params
String CAMERA_IP =  "http://192.168.4.2";
String CAMERA_PORT = "8080";
String SERVER_IP = "http://192.168.4.1";  // webserver IP

String CAMERA_SHOOT_URL = "/ccapi/ver100/shooting/control/shutterbutton";
String CAMERA_TEST_URL  = "/ccapi";
String CAMERA_SHOT_BODY = "{\"af\": true}";

// wifi params
#ifndef APSSID
#define APSSID "CCAPI_OLED_ESP8266"   // wifi network name
#define APPSK "1234"                  // wifi password
#endif



// ---- Program variables -----
int buttonState = 0;                // variable for reading the pushbutton status
int buttonTimer = 0;
boolean buttonActive = false;
boolean longPressActive = false;
int optionMenu = 0;                 // The option menu selected: 0=intervalometer 1=num shoots, 2=single shoot test,  3=intervalometer active

bool START_SHOOT = false;
int NUM_SHOOTS = 2;
int CURRENT_SHOOT = 0;
int FREQUENCY = 10;
bool AF = true;    // for future use (option to set AF/MF by menu option)
String CAM_STATUS_CONNECTED = "Not Connected";


const char *ssid = APSSID;
const char *password = APPSK;

ESP8266WebServer server(80);

int i = 0;             // cycle counter.
int waitingSteps = 0;  // counter for timeout menu

void setup() {
  delay(200);
  Serial.begin(115200);
  //pinMode(LED_BUILTIN, OUTPUT);  // Initialize the LED_BUILTIN pin as an output
  Serial.println();
  congifureOLED();

  Serial.print("Configuring access point...");
  WiFi.softAP(ssid, password);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);

  // web server config
  setupHttpHandlers();
  server.begin();
  Serial.println("HTTP server started: Waiting incoming connections...");

  GetCCAPIinitialPage();
  drawMenuOLED();
}

void loop() {

  readButton();
  server.handleClient();
  
  if (START_SHOOT == false){

    delay(100);   //allow the cpu to switch to other tasks
    if (i % 100 == 0) {
        Serial.println("Waiting orders..." + String(i));
        Serial.println("Timeout for deeeSleep..." + String(waitingSteps));
    }
    if (waitingSteps > (TIMEOUT_MENU_OFF * 10)){
      GotoDeepSleep("Timeout menu !");
    }
    waitingSteps++;
    i++;

  } else {
    waitingSteps = 0;

    Serial.println("Executing shooting command..." + String(CURRENT_SHOOT) + " of " + String(NUM_SHOOTS));
    
    if (CURRENT_SHOOT < NUM_SHOOTS){

        int timer = millis();
        camera_shoot();
        CURRENT_SHOOT++;
        drawProgress();
        wait_time(millis() - timer);    
    } else {
      START_SHOOT = false;
      CURRENT_SHOOT = 0;
      drawMenuOLED();
    }
  }
}
