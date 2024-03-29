
#include "wifi_stuff.h"



void setup() {
  // put your setup code here, to run once:
  init_wifi();

  start_wheel_encoder();
  Serial.begin(115200);

  pinMode(button_pin, INPUT_PULLUP);
  pinMode(2, OUTPUT);

  digitalWrite(2, LOW);
  delay(2000);


}

void loop() {
  // put your main code here, to run repeatedly:

  delay(100);
  uint8_t buttonState = digitalRead(button_pin);

  if(buttonState){
    digitalWrite(2, LOW);
  } else {
    digitalWrite(2, HIGH);
  }

  Serial.print("counter 1: ");
  Serial.print(counter1);
  Serial.print(", counter 2: ");
  Serial.println(counter2);

}
