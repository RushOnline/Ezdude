#include <Arduino.h>

#include "buwizz.h"

void(* resetFunc) (void) = 0;

void setup() {
  Serial.begin(115200);
  Serial.println("Ha4uHaeM Ha4uHaTb...");
  
  Buwizz.setup();
  Buwizz.set_mode(BW_MODE_NORMAL);
}

void loop() {

  Buwizz.set_speed(1, 127);
  sleep(1);
  Buwizz.set_speed(1, -127);
  sleep(1);

  if (Serial.available()) {
    Serial.println("Resetting...");
    sleep(3);
    resetFunc();
  }
}
