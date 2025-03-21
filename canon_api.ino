

//  curl 'http://192.168.1.138:8080/ccapi/ver100/shooting/control/shutterbutton' -d '{"af": true}' --verbose -H 'Content-Type: application/json

void camera_shoot(){

  
    WiFiClient client;
    HTTPClient http;

    String url = CAMERA_IP + ":" + CAMERA_PORT + CAMERA_SHOOT_URL;
    String body = CAMERA_SHOT_BODY;

    Serial.println("[HTTP] begin..." + url);
    // configure traged server and url
    http.begin(client, url);  // HTTP
    http.addHeader("Content-Type", "application/json");

    Serial.println("[HTTP] POST..." + url + " with body: " + body);
    // start connection and send HTTP header and body
    int httpCode = http.POST(body);

    // httpCode will be negative on error
    if (httpCode > 0) {
      // HTTP header has been send and Server response header has been handled
      Serial.printf("[HTTP] POST... code: %d\n", httpCode);

      // file found at server
      if (httpCode == HTTP_CODE_OK) {
        const String& payload = http.getString();
        Serial.println("received payload:\n<<");Serial.println(payload);Serial.println(">>");
      }
    } else {
      Serial.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }
    http.end();
}


void camera_switchoff(){}   // for future features.


bool getURL(String url){

  WiFiClient client;
  HTTPClient http;
  bool exitstatus = false;

    Serial.println("[HTTP] begin..." + url);
    if (http.begin(client, url)) {  // HTTP


      Serial.println("[HTTP] GET..." + url);
      // start connection and send HTTP header
      int httpCode = http.GET();

      // httpCode will be negative on error
      if (httpCode > 0) {
        // HTTP header has been send and Server response header has been handled
        Serial.printf("[HTTP] GET... code: %d\n", httpCode);

        // file found at server
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
          String payload = http.getString();
          Serial.println(payload);
          exitstatus = true;
        }
      } else {
        Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
      }

      http.end();
    } else {
      Serial.println("[HTTP] Unable to connect");
    }

    return exitstatus;

}


