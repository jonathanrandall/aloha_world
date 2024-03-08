/******************************************************
* FileName:      LobotSerialServoControl.h
* Company:     Hiwonder
* Date:          2020/05/13  16:53
 *Last Modification Date: 202005131938
* www.hiwonder.com
*****************************************************/

#include "LobotSerialServoControl.h"
#include <Stream.h>



LobotSerialServoControl::LobotSerialServoControl(SoftwareSerial &A)
{
  isAutoEnableRT = true;
	isUseHardwareSerial = false;
//	A.listen();
	SerialX = (Stream*)(&A);

}
LobotSerialServoControl::LobotSerialServoControl(SoftwareSerial &A,int receiveEnablePin, int transmitEnablePin)
{
  isAutoEnableRT = false;
  this->receiveEnablePin = receiveEnablePin;
  this->transmitEnablePin = transmitEnablePin;
  
  isUseHardwareSerial = false;
//  A.listen();
  SerialX = (Stream*)(&A);
//  A.begin(115200);
}
LobotSerialServoControl::LobotSerialServoControl(HardwareSerial &A)
{
  isAutoEnableRT = true;
	isUseHardwareSerial = true;
	SerialX = (Stream*)(&A);
}
LobotSerialServoControl::LobotSerialServoControl(HardwareSerial &A,int receiveEnablePin, int transmitEnablePin)
{
  isAutoEnableRT = false;
  this->receiveEnablePin = receiveEnablePin;
  this->transmitEnablePin = transmitEnablePin;
  
  isUseHardwareSerial = true;
  SerialX = (Stream*)(&A);
}

void LobotSerialServoControl::OnInit(void)
{
  if(isUseHardwareSerial)
  {
    ((HardwareSerial*)SerialX)->begin(115200);
  }
  else
  {
    ((SoftwareSerial*)SerialX)->listen();
    ((SoftwareSerial*)SerialX)->begin(115200);
  }

  if(!isAutoEnableRT)
  {
    pinMode(receiveEnablePin, OUTPUT);
    pinMode(transmitEnablePin, OUTPUT);
    RxEnable();
  }

}

inline void LobotSerialServoControl::RxEnable(void)
{
  digitalWrite(receiveEnablePin, HIGH);
  digitalWrite(transmitEnablePin, LOW);
}
inline void LobotSerialServoControl::TxEnable(void)
{
  digitalWrite(receiveEnablePin, LOW);
  digitalWrite(transmitEnablePin, HIGH);
}

byte LobotSerialServoControl::LobotCheckSum(byte buf[])
{
  byte i;
  uint16_t temp = 0;
  for (i = 2; i < buf[3] + 2; i++) {
    temp += buf[i];
  }
  temp = ~temp;
  i = (byte)temp;
  return i;
}

void LobotSerialServoControl::LobotSerialServoMove(uint8_t id, int16_t position, uint16_t time)
{
  byte buf[10];
  if(position < 0)
    position = 0;
  if(position > 1000)
    position = 1000;
  buf[0] = buf[1] = LOBOT_SERVO_FRAME_HEADER;
  buf[2] = id;
  buf[3] = 7;
  buf[4] = LOBOT_SERVO_MOVE_TIME_WRITE;
  buf[5] = GET_LOW_BYTE(position);
  buf[6] = GET_HIGH_BYTE(position);
  buf[7] = GET_LOW_BYTE(time);
  buf[8] = GET_HIGH_BYTE(time);
  buf[9] = LobotCheckSum(buf);
  if(isAutoEnableRT == false)
    TxEnable();
  SerialX->write(buf, 10);
}

void LobotSerialServoControl::LobotSerialServoStopMove(uint8_t id)
{
  byte buf[6];
  buf[0] = buf[1] = LOBOT_SERVO_FRAME_HEADER;
  buf[2] = id;
  buf[3] = 3;
  buf[4] = LOBOT_SERVO_MOVE_STOP;
  buf[5] = LobotCheckSum(buf);
  if(isAutoEnableRT == false)
    TxEnable();
  SerialX->write(buf, 6);
}

void LobotSerialServoControl::LobotSerialServoSetID(uint8_t oldID, uint8_t newID)
{
  byte buf[7];
  buf[0] = buf[1] = LOBOT_SERVO_FRAME_HEADER;
  buf[2] = oldID;
  buf[3] = 4;
  buf[4] = LOBOT_SERVO_ID_WRITE;
  buf[5] = newID;
  buf[6] = LobotCheckSum(buf);
  if(isAutoEnableRT == false)
    TxEnable();
  SerialX->write(buf, 7);
  
#ifdef LOBOT_DEBUG
  Serial.println("LOBOT SERVO ID WRITE");
  int debug_value_i = 0;
  for (debug_value_i = 0; debug_value_i < buf[3] + 3; debug_value_i++)
  {
    Serial.print(buf[debug_value_i], HEX);
    Serial.print(":");
  }
  Serial.println(" ");
#endif

}

void LobotSerialServoControl::LobotSerialServoSetMode(uint8_t id, uint8_t Mode, int16_t Speed)
{
  byte buf[10];

  buf[0] = buf[1] = LOBOT_SERVO_FRAME_HEADER;
  buf[2] = id;
  buf[3] = 7;
  buf[4] = LOBOT_SERVO_OR_MOTOR_MODE_WRITE;
  buf[5] = Mode;
  buf[6] = 0;
  buf[7] = GET_LOW_BYTE((uint16_t)Speed);
  buf[8] = GET_HIGH_BYTE((uint16_t)Speed);
  buf[9] = LobotCheckSum(buf);

#ifdef LOBOT_DEBUG
  Serial.println("LOBOT SERVO Set Mode");
  int debug_value_i = 0;
  for (debug_value_i = 0; debug_value_i < buf[3] + 3; debug_value_i++)
  {
    Serial.print(buf[debug_value_i], HEX);
    Serial.print(":");
  }
  Serial.println(" ");
#endif
  if(isAutoEnableRT == false)
    TxEnable();
  SerialX->write(buf, 10);
}

void LobotSerialServoControl::LobotSerialServoLoad(uint8_t id)
{
  byte buf[7];
  buf[0] = buf[1] = LOBOT_SERVO_FRAME_HEADER;
  buf[2] = id;
  buf[3] = 4;
  buf[4] = LOBOT_SERVO_LOAD_OR_UNLOAD_WRITE;
  buf[5] = 1;
  buf[6] = LobotCheckSum(buf);
  if(isAutoEnableRT == false)
    TxEnable();
  SerialX->write(buf, 7);
  
#ifdef LOBOT_DEBUG
  Serial.println("LOBOT SERVO LOAD WRITE");
  int debug_value_i = 0;
  for (debug_value_i = 0; debug_value_i < buf[3] + 3; debug_value_i++)
  {
    Serial.print(buf[debug_value_i], HEX);
    Serial.print(":");
  }
  Serial.println(" ");
#endif

}

void LobotSerialServoControl::LobotSerialServoUnload(uint8_t id)
{
  byte buf[7];
  buf[0] = buf[1] = LOBOT_SERVO_FRAME_HEADER;
  buf[2] = id;
  buf[3] = 4;
  buf[4] = LOBOT_SERVO_LOAD_OR_UNLOAD_WRITE;
  buf[5] = 0;
  buf[6] = LobotCheckSum(buf);
  if(isAutoEnableRT == false)
    TxEnable();
  SerialX->write(buf, 7);
  
#ifdef LOBOT_DEBUG
  Serial.println("LOBOT SERVO LOAD WRITE");
  int debug_value_i = 0;
  for (debug_value_i = 0; debug_value_i < buf[3] + 3; debug_value_i++)
  {
    Serial.print(buf[debug_value_i], HEX);
    Serial.print(":");
  }
  Serial.println(" ");
#endif
}

int LobotSerialServoControl::LobotSerialServoReceiveHandle(byte *ret)
{
  bool frameStarted = false;
  bool receiveFinished = false;
  byte frameCount = 0;
  byte dataCount = 0;
  byte dataLength = 2;
  byte rxBuf;
  byte recvBuf[32];
  byte i;

  while (SerialX->available()) {
    rxBuf = SerialX->read();
    delayMicroseconds(100);
    if (!frameStarted) {
      if (rxBuf == LOBOT_SERVO_FRAME_HEADER) {
        frameCount++;
        if (frameCount == 2) {
          frameCount = 0;
          frameStarted = true;
          dataCount = 1;
        }
      }
      else {
        frameStarted = false;
        dataCount = 0;
        frameCount = 0;
      }
    }
    if (frameStarted) {
      recvBuf[dataCount] = (uint8_t)rxBuf;
      if (dataCount == 3) {
        dataLength = recvBuf[dataCount];
        if (dataLength < 3 || dataCount > 7) {
          dataLength = 2;
          frameStarted = false;
        }
      }
      dataCount++;
      if (dataCount == dataLength + 3) {
        
#ifdef LOBOT_DEBUG
        Serial.print("RECEIVE DATA:");
        for (i = 0; i < dataCount; i++) {
          Serial.print(recvBuf[i], HEX);
          Serial.print(":");
        }
        Serial.println(" ");
#endif

        if (LobotCheckSum(recvBuf) == recvBuf[dataCount - 1]) {
          
#ifdef LOBOT_DEBUG
          Serial.println("Check SUM OK!!");
          Serial.println("");
#endif

          frameStarted = false;
          memcpy(ret, recvBuf + 4, dataLength);
          return 1;
        }
        return -1;
      }
    }
  }
}

int LobotSerialServoControl::LobotSerialServoReadPosition(uint8_t id)
{
  int count = 10000;
  int ret;
  byte buf[6];

  buf[0] = buf[1] = LOBOT_SERVO_FRAME_HEADER;
  buf[2] = id;
  buf[3] = 3;
  buf[4] = LOBOT_SERVO_POS_READ;
  buf[5] = LobotCheckSum(buf);

#ifdef LOBOT_DEBUG
  Serial.println("LOBOT SERVO Pos READ");
  int debug_value_i = 0;
  for (debug_value_i = 0; debug_value_i < buf[3] + 3; debug_value_i++)
  {
    Serial.print(buf[debug_value_i], HEX);
    Serial.print(":");
  }
  Serial.println(" ");
#endif

  while (SerialX->available())
    SerialX->read();
    
  if(!isUseHardwareSerial)
    ((SoftwareSerial*)(SerialX))->listen();
  if(isAutoEnableRT == false)
    TxEnable();
  SerialX->write(buf, 6);
  if(isUseHardwareSerial)
  {
    delayMicroseconds(700);
  }
  if(isAutoEnableRT == false)
    RxEnable();
  while (!SerialX->available()) {
    count -= 1;
    if (count < 0)
      return -1;
  }

  if (LobotSerialServoReceiveHandle(buf) > 0)
    ret = BYTE_TO_HW(buf[2], buf[1]);
  else
    ret = -2048;

#ifdef LOBOT_DEBUG
  Serial.println(ret);
#endif
  return ret;
}

int LobotSerialServoControl::LobotSerialServoReadVin(uint8_t id)
{
  int count = 10000;
  int ret;
  byte buf[6];

  buf[0] = buf[1] = LOBOT_SERVO_FRAME_HEADER;
  buf[2] = id;
  buf[3] = 3;
  buf[4] = LOBOT_SERVO_VIN_READ;
  buf[5] = LobotCheckSum(buf);

#ifdef LOBOT_DEBUG
  Serial.println("LOBOT SERVO VIN READ");
  int debug_value_i = 0;
  for (debug_value_i = 0; debug_value_i < buf[3] + 3; debug_value_i++)
  {
    Serial.print(buf[debug_value_i], HEX);
    Serial.print(":");
  }
  Serial.println(" ");
#endif

  while (SerialX->available())
    SerialX->read();

  if(!isUseHardwareSerial)
    ((SoftwareSerial*)(SerialX))->listen();
  if(isAutoEnableRT == false)
    TxEnable();
    
  SerialX->write(buf, 6);

  if(isUseHardwareSerial)
  {
    delayMicroseconds(700);
  }
  if(isAutoEnableRT == false)
    RxEnable();
  
  while (!SerialX->available()) {
    count -= 1;
    if (count < 0)
      return -2048;
  }

  if (LobotSerialServoReceiveHandle(buf) > 0)
    ret = (int16_t)BYTE_TO_HW(buf[2], buf[1]);
  else
    ret = -2048;

#ifdef LOBOT_DEBUG
  Serial.println(ret);
#endif
  return ret;
}
