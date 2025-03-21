

void printOLED(String s, int line_pos){
   char line_to_print[20]="";
   sprintf(line_to_print,s.c_str());
   display.print(line_to_print,line_pos);
}

void GetCCAPIinitialPage(){
   int ii = 0;
   printOLED("Wifi AP      OK",0);
   printOLED("Server HTTP  OK",1);
   if (!bypassCCAPIurl){
      while (!getURL(CAMERA_IP + ":" + CAMERA_PORT + CAMERA_TEST_URL)){
        printOLED("Conex CCAPI ...",2);
        printOLED("         ... " + String(ii),3);
        Serial.println("Trying to connect CCAPI... " + String(ii++));
        delay(700);
        if (ii > TIMEOUT_CAMERA_CONNECT){
           GotoDeepSleep("CAMERA NOT FOUND !");
        }
      } 
   }
    printOLED("CCAPI OK !!!",2);
    delay(500);
}


void GotoDeepSleep(String reason){
  int i = 0;
    display.clear();
    for (i = 5;i>0; i--){
      printOLED(reason,1);
      printOLED("  SWITCHOFF... " + String(i),3);
      delay(1000);
    }
    display.clear();
    display.off();
    ESP.deepSleep(0);
}


void readButton(){

  if (!digitalRead(buttonPin) == HIGH) {
    if (buttonActive == false) {
      buttonActive = true;
      buttonTimer = millis();
    }
    if ((millis() - buttonTimer > longPressTime) && (longPressActive == false)) {
      longPressActive = true;
      Serial.println("LONG PRESS");
      waitingSteps = 0; /*reset the timeout value for deepsleep everytime we press the button*/ 
      optionMenu++;
      drawMenuOLED();
    }
	} else {
		if (buttonActive == true) {
			if (longPressActive == true) {
				longPressActive = false;
			} else {
          Serial.println("SHORT PRESS");
          waitingSteps = 0; /*reset the timeout value for deepsleep everytime we press the button*/
          changeMenuValues(optionMenu);
          drawMenuOLED();
			}
			buttonActive = false;
		}
	}
}

void changeMenuValues(int val){

    if (val == 0){ 
      FREQUENCY += 5; 
      if (FREQUENCY > 100){FREQUENCY += 5;} 
      if (FREQUENCY > 300){FREQUENCY = 5;}
    }
    if (val == 1){
      if   (NUM_SHOOTS >= 100){NUM_SHOOTS += 50;}
      else { if  (NUM_SHOOTS > 9){NUM_SHOOTS += 5;} else {NUM_SHOOTS++;}} 
      if (NUM_SHOOTS > 999){NUM_SHOOTS = 2;} 
    }
    if (val == 2){ camera_shoot();delay(1000);}
    if (val == 3){START_SHOOT = true;}
}

void congifureOLED(){

  pinMode(16, OUTPUT);
  digitalWrite(16, LOW);   // turn D2 low to reset OLED
  delay(50);
  digitalWrite(16, HIGH);    // while OLED is running, must set D2 in high
  
  // Initialize display
  display.begin();

  // Test message
  printOLED("Starting...",0);
}

void drawMenuOLED(){
    display.clear(); 
    if (optionMenu > 3){optionMenu = 0;}
    if (optionMenu == 0){
      printOLED("* INTERVAL: " + String(FREQUENCY),0);
      printOLED("  SHOOTS:  " + String(NUM_SHOOTS),1);
      printOLED("  REMOTE SHOOT->",2);
      printOLED("  START?  --->",3);
    }
    if (optionMenu == 1){
      printOLED("  INTERVAL: " + String(FREQUENCY),0);
      printOLED("* SHOOTS:  " + String(NUM_SHOOTS),1);
      printOLED("  REMOTE SHOOT->",2);
      printOLED("  START?  --->",3);
    }
    if (optionMenu == 2){
      printOLED("  INTERVAL: " + String(FREQUENCY),0);
      printOLED("  SHOOTS:  " + String(NUM_SHOOTS),1);
      printOLED("* REMOTE SHOOT->",2);
      printOLED("  START?  --->",3);
    }
    if (optionMenu == 3){
      printOLED("  INTERVAL: " + String(FREQUENCY),0);
      printOLED("  SHOOTS:  " + String(NUM_SHOOTS),1);
      printOLED("  REMOTE SHOOT->",2);
      printOLED("* START?  --->",3);
    }
}

void drawProgress(){
      display.clear(); 
      printOLED("ACTIVE: " + String(FREQUENCY) +" segs",0);
      printOLED("SHOOTS: " + String (CURRENT_SHOOT) + "-" + String(NUM_SHOOTS),1);
      printOLED(" [RST to stop]",3);
}