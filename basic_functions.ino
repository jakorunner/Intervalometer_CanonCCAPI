void wait_time(int timer){

  if (timer < 0){timer = 0;}

  Serial.print("Shoot time: ");Serial.println(timer);
  int pause_time = (FREQUENCY * 1000) - timer;
  Serial.print("Waiting time: ");Serial.println(pause_time);

  if (pause_time > 0) {delay(pause_time);}
}


// this function is not used, because my board doesnt have builtin led, but may be useful for other boards (wemos, nodemcu...).
void blinkled(int led,int ontime, int times, int offtime){
  for (int i=0;i<times;i++)
  {
    digitalWrite(led, LOW);  // Turn the LED on (Note that LOW is the voltage level
    delay(ontime);                      
    digitalWrite(led, HIGH); 
    delay(offtime);
  }
}