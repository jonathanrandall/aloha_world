
#include "wifi_stuff.h"

void setup() {
  // put your setup code here, to run once:
  init_wifi();

  start_wheel_encoder();
  Serial.begin(115200);

}

void loop() {
  // put your main code here, to run repeatedly:

  delay(100);
  Serial.print("counter 1: ");
  Serial.print(counter1);
  Serial.print(", counter 2: ");
  Serial.println(counter2);

}
