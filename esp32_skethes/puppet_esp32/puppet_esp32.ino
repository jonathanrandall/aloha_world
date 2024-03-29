
#include "wifi_stuff.h"

#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"

long cmd[3];


void setup(){
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); //disable brownout detector
  // put your setup code here, to run once:
  Serial.begin(115200);
  init_wifi();
  delay(200);

  start_wheel_encoder();

  robot_setup();
  cmd_queue = xQueueCreate(10, sizeof(cmd));

  delay(200);

  xTaskCreatePinnedToCore(
       robot_move_loop, /* Function to implement the task */
       "rml",    /* Name of the task */
       1024*8,            /* Stack size in words */
       NULL,            /* Task input parameter */
       2,               /* Priority of the task */
       NULL,     /* Task handle. */
       1);              /* Core where the task should run */
  
  xTaskCreatePinnedToCore(
       http_get_loop, /* Function to implement the task */
       "ghl",    /* Name of the task */
       1024*8,            /* Stack size in words */
       NULL,            /* Task input parameter */
       2,               /* Priority of the task */
       NULL,     /* Task handle. */
       1);              /* Core where the task should run */
  /*
  update_speed();
  robot_fwd();
  delay(2000);
    Serial.print("counter 1: ");
  Serial.print(counter1);
  Serial.print(", counter 2: ");
  Serial.println(counter2);
  robot_back();
  delay(2000);
  robot_left();
    Serial.print("counter 1: ");
  Serial.print(counter1);
  Serial.print(", counter 2: ");
  Serial.println(counter2);
  delay(4000);
  robot_right();
    Serial.print("counter 1: ");
  Serial.print(counter1);
  Serial.print(", counter 2: ");
  Serial.println(counter2);
  delay(4000);
    Serial.print("counter 1: ");
  Serial.print(counter1);
  Serial.print(", counter 2: ");
  Serial.println(counter2);
  robot_stop();
  */

}

void loop() {
  vTaskDelete(NULL);
  // put your main code here, to run repeatedly:
  // delay(1000);
  // String enc_vals;
  // long cnt1, cnt2;
  // long cnt_all[2];
  // enc_vals = httpGETRequest(serverEncoder);
  // sscanf(enc_vals.c_str(), "%d %d", &cnt1, &cnt2);
  // Serial.print("counter 1: ");
  // Serial.print(cnt1);
  // Serial.print(", counter 2: ");
  // Serial.println(cnt2);
  // cnt_all[0]=cnt1;
  // cnt_all[1]=cnt2;
  // robot_move(cnt_all);


}
