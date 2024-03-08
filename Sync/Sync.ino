#include <SoftwareSerial.h>
#include "LobotServoController.h"
#include "LobotSerialServoControl.h"
#include "HWSerialModuleControl.h"
#include "I2Cdev.h"
#include "Wire.h"

#define BTH_RX 11
#define BTH_TX 12

#define SERVO_SERIAL_RX 10
#define SERVO_SERIAL_TX 3
#define SERVO_SERIAL_RX_CON 4
#define SERVO_SERIAL_TX_CON 2

#define LED         6
#define KEY1        8
#define KEY2        7
#define LED_ON      0
#define LED_OFF     1

#define   SERVO_TYPE_PWM      0
#define   SERVO_TYPE_BUS      1


enum{
 /* MODE_xArm = 1,
  MODE_LeArm,
  MODE_Tankbot,*/
  MODE_Tankbot = 1,
  MODE_xArm,
  MODE_LeArm,
};
uint8_t WorkMode = MODE_xArm;//MODE_Tankbot;
bool WorkStart = true; //false;

SoftwareSerial Bth(BTH_RX, BTH_TX);
LobotServoController lsc(Bth);

unsigned long ptime;
unsigned long pinterval = 40;


SoftwareSerial ServoSerial(SERVO_SERIAL_RX, SERVO_SERIAL_TX);

HWSerialModuleControl sync(ServoSerial,SERVO_SERIAL_RX_CON,SERVO_SERIAL_TX_CON,1);

int16_t pos_out[7];

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);   
  sync.OnInit();

  //蓝牙配置
  Bth.begin(9600);
  delay(100);
  Bth.print("AT+ROLE=M");  //蓝牙配置为主模式
  delay(100);
  Bth.print("AT+RESET");  //软重启蓝牙模块
  delay(250);

//  sync.setSyncID(6);//设置模块ID，群发设置，请一次只连接一个模块。

  for(uint8_t i=1; i<=6;i++)
  {
    pos_out[i]=500;
  }
  ptime=millis();

  
  pinMode(LED,OUTPUT);
  pinMode(KEY1, INPUT_PULLUP);
  pinMode(KEY2, INPUT_PULLUP);

}

void loop() {
  if(WorkStart){
    car_control();
    arm_control(20);
  }

  if (ptime - millis()>pinterval){
    ptime = millis();
    for(uint8_t i=1; i<=6;i++)
  {
    Serial.print(pos_out[i]);
    Serial.print("\t");

  }
  // Serial.println("");
  }
  
  KeyHandle();
  LedHandle();
  
}

int16_t PosConvert(int16_t pos, uint8_t servo_type = SERVO_TYPE_BUS, int16_t middle_pos = 500, bool flip = false, float rate = 1.0, uint16_t offset = 0)
{
  float p = middle_pos + (pos - middle_pos)*rate + offset;
  if(servo_type == SERVO_TYPE_BUS){
    if(flip)
      return 1000 - int16_t(p);
    return int16_t(p);
  }else if(servo_type == SERVO_TYPE_PWM){
    if(flip)
      return 3000 - int16_t(p);
    return int16_t(p);
  }

}

void arm_control(uint16_t t)
{
  int16_t pos[7], p;
  sync.getSyncPositionAll(pos);
  
  for(uint8_t i=1; i<=6;i++)
  {
    delay(1);
    // Serial.print(pos[i]);
    Serial.print("\t");
    if(pos[i] > 0)
    {
      switch(WorkMode){
        case MODE_xArm:
          if(i == 1){
            lsc.moveServo(i, PosConvert(pos[i], SERVO_TYPE_BUS, 500, true, 4, -230), t);
          }else if(i == 4){
              lsc.moveServo(i, PosConvert(pos[i], SERVO_TYPE_BUS, 500, false, 1.0), t);
          }else if(i == 5){
            lsc.moveServo(i, PosConvert(pos[i], SERVO_TYPE_BUS, 500, false, 1.0), t);
          }else{
            lsc.moveServo(i, PosConvert(pos[i], SERVO_TYPE_BUS, 500), t);
          }
          break;
        case MODE_LeArm:
          pos[i] = pos[i] * 2 + 500;
          if(i == 1){
            lsc.moveServo(i, PosConvert(pos[i], SERVO_TYPE_PWM, 1500, true, 4, -100), t);
          }else if(i == 4){
              lsc.moveServo(i, PosConvert(pos[i], SERVO_TYPE_PWM, 1500, true, 1.6), t);
          }else if(i == 5){
            lsc.moveServo(i, PosConvert(pos[i], SERVO_TYPE_PWM, 1500, true, 1.6), t);
          }else{
            lsc.moveServo(i, PosConvert(pos[i], SERVO_TYPE_PWM, 1500), t);
          }
          break;
        case MODE_Tankbot:
          if(i == 1){
            lsc.moveServo(i, PosConvert(pos[i], SERVO_TYPE_BUS, 500, true, 4, -230), t);
          }else if(i == 4){
              lsc.moveServo(i, PosConvert(pos[i], SERVO_TYPE_BUS, 500, true, 1.3), t);
          }else if(i == 5){
            lsc.moveServo(i, PosConvert(pos[i], SERVO_TYPE_BUS, 500, true, 1.2), t);
          }else{
            lsc.moveServo(i, PosConvert(pos[i], SERVO_TYPE_BUS, 500), t);
          }
          break;
      }
    pos_out[i] = 0.2*pos_out[i]+0.8*pos[i];
    }
  }
  Serial.println(" ");
}

//具体向小车发送数据
void car_control()
{
  uint16_t x,y;
  byte left, right;
  x = analogRead(A2);
  y = analogRead(A3);
  if(y <= 5){
    left = 80,right = 80;
    Serial.print("1 ");
  }
  else if(y > 900){
    left = -80,right = -80;
    Serial.print("2 ");
  }
  else if(x <= 5){
    left = 80,right = -80;
    Serial.print("4 ");
  }
  else if(x > 900){
    left = -80,right = 80;
    Serial.print("3 ");
  }
  else
    left = 0,right = 0;
    
  byte buf[10];
  buf[0] = buf[1] = 0x55;
  buf[2] = 0x04;
  buf[3] = 0x32;
  buf[4] = (byte)left;
  buf[5] = (byte)right;
  Bth.write(buf, 6);
}



void KeyHandle()
{
  static uint32_t time_count = 0;
  static uint16_t time_key1 = 0, time_key2 = 0;                        

  if(millis()-time_count > 100){
    time_count = millis();
    if(digitalRead(KEY1) == 1){
      if(time_key1 >= 1 && time_key1 <= 6){
        time_key1 = 0;
        
        if(++WorkMode > MODE_LeArm){
          WorkMode = MODE_Tankbot;
        }
        WorkStart = false;
      }
      else
        time_key1 = 0;
    }
    else{
      time_key1 += 1;
      if(time_key1 == 10){
        

      }
    }

    if(digitalRead(KEY2) == 1){
      if(time_key2 >= 1 && time_key2 <= 6){
        time_key2 = 0;
        
        WorkStart = true;
        arm_control(1000);
        arm_control(1000);
        delay(1000);
      }
      else
        time_key2 = 0;
    }
    else{
      time_key2 += 1;
      if(time_key2 == 10){
        

      }
    }

    
  }
}

void LedHandle()
{
  static uint32_t time_count = 0;
  static uint8_t blink_period = 12, blink_time_now = 0, blink_times = WorkMode;
  if(millis()-time_count > 100){
    time_count = millis();

    if(blink_times != WorkMode)
      blink_time_now = 0;
    blink_times = WorkMode;

    if(blink_time_now > blink_period)
      blink_time_now = 0;

    if(blink_time_now <= blink_times*2){
      if(blink_time_now % 2 == 1)
        digitalWrite(LED, LED_ON);
      else
        digitalWrite(LED, LED_OFF);
    }else{
      digitalWrite(LED, LED_OFF);
    }
      
    blink_time_now++;
  }
}
