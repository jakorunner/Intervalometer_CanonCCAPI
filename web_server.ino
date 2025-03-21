/* Just a little test message.  Go to http://192.168.4.1 in a web browser
   connected to this access point to see it.
*/

void setupHttpHandlers(){
  server.on("/", handleRoot);
  server.on("/startIntervalometer",HTTP_POST, handleStartIntervalometer);
  server.on("/stopIntervalometer", handleStopIntervalometer);
  server.on("/singleShoot",HTTP_POST, handleSingleShoot);
  server.on("/ui", handleLoadUI);
  server.on("/ui_trampeo", handleLoadUITrampeo);
  server.on("/ui_status", handleLoadUIStatus);
  server.on("/connectCamera", handleConnectCamera);
}


void handleConnectCamera() {
  camera_shoot();
  blinkled(LED_BUILTIN,150,5,150);

  if ( server.hasArg("ip")     && server.arg("ip")       != NULL ) { CAMERA_IP = server.arg("ip"); }
  if ( server.hasArg("puerto") && server.arg("puerto")   != NULL ) { CAMERA_PORT = server.arg("puerto"); }


  // connect to camera and get results
  String url = CAMERA_IP + ":" + CAMERA_PORT + CAMERA_TEST_URL;
  if (getURL(url)==true){
     CAM_STATUS_CONNECTED = "Camera Connected OK!";
  } else {
    CAM_STATUS_CONNECTED = "Error connecting camera";
  }
  server.send(200, "text/html", generateRootUI());
}


void handleRoot() {
  server.send(200, "text/html", generateRootUI());
  blinkled(LED_BUILTIN,150,5,150);
}

void handleSingleShoot() {
  camera_shoot();
  blinkled(LED_BUILTIN,150,5,150);
  server.send(200, "text/html", "<h1>Ejecutando single Shoot!/h1>");
}

void handleLoadUI(){
  //getURL("");
   blinkled(LED_BUILTIN,150,5,150);

  if (START_SHOOT == false){
      server.send(200, "text/html", generateIntervalometerUI());
  }
  else {
      server.send(200, "text/html", "<h1>Mostrar UI  (mostrar progreso y boton de PARADA)</h1>");
  }
}

void handleStopIntervalometer(){
  blinkled(LED_BUILTIN,150,5,150);
  server.send(200, "text/html", "<h1>Parando intervalometro...</h1>");
  START_SHOOT = false;
}

void handleStartIntervalometer(){
  blinkled(LED_BUILTIN,150,5,150);
  

  Serial.println(F("Starting intervalometer..."));
  Serial.println(server.args());
  for (int i=0;i<server.args();i++){
    Serial.print(i);Serial.print(": ");Serial.println(server.arg(i));
  }
  if ( !server.hasArg("numShoots") || server.arg("numShoots") == NULL ) {  server.send(400, "text/plain", "400: numShoots no set"); return;}
  if ( !server.hasArg("intervalo")   || server.arg("intervalo") == NULL )   {  server.send(400, "text/plain", "400: interval no set"); return;}
  if ( !server.hasArg("af_shoot")    || server.arg("af_shoot") == NULL )    {  server.send(400, "text/plain", "400: modo af_shoot no set"); return;}

  NUM_SHOOTS = server.arg("numShoots").toInt();
  FREQUENCY = server.arg("intervalo").toInt();

  server.send(200, "text/html", "<h1>Starting intervalometer ...</h1>");

    Serial.print("Starting intervalometer --> Num disparos: ");Serial.print(NUM_SHOOTS);Serial.print(".     Frequency: ");Serial.println(FREQUENCY);
  START_SHOOT = true;
}

void handleNotFound() {
   blinkled(LED_BUILTIN,150,5,150);
  //digitalWrite(led, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  //digitalWrite(led, 0);
}

void handleLoadUITrampeo(){
  //getURL("");
   blinkled(LED_BUILTIN,150,5,150);

   server.send(200, "text/html", generateFotoTrampeoUI());

}

void handleLoadUIStatus(){
  //getURL("");
   blinkled(LED_BUILTIN,150,5,150);

   server.send(200, "text/html", generateStatusUI());

}