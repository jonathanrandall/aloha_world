
#include "HWSerialModuleControl.h"




int16_t HWSerialModuleControl::getSyncPosition(uint8_t id)
{//Position：0~1000
  int count = 300;
  int ret;
  byte buf[8];

  buf[0] = buf[1] = LOBOT_SERVO_FRAME_HEADER;
  buf[2] = HIWONDER_SERVO_IS_MODULE_ID;
  buf[3] = 5;
  buf[4] = moduleID;
  buf[5] = id;
  buf[6] = SYN_POS_READ;
  buf[7] = LobotCheckSum(buf);
  

  while (SerialX->available())
    SerialX->read();
    
  if(!isUseHardwareSerial)
    ((SoftwareSerial*)(SerialX))->listen();
  if(isAutoEnableRT == false)
    TxEnable();
  SerialX->write(buf, buf[3] +3);
  if(isUseHardwareSerial)
  {
    delayMicroseconds((buf[3] +3)*110);
  }
  if(isAutoEnableRT == false)
    RxEnable();
  while (!SerialX->available()) {
    count -= 1;
    if (count < 0)
      return -1;
  }

  if (LobotSerialServoReceiveHandle(buf) > 0)
    ret = BYTE_TO_HW(buf[4], buf[3]);
  else
    ret = -2048;

#ifdef LOBOT_DEBUG
  Serial.println(ret);
#endif
  return ret;
  
}

void HWSerialModuleControl::getSyncPositionAll(int16_t *pos)
{
  int16_t p;
  for(uint8_t i=1; i<=6;i++)
  {
    if((p = getSyncPosition(i))<0)
    {
      if((p = getSyncPosition(i))<0)
      {
        p = getSyncPosition(i);
      }
    }
    pos[i] = p;
  }
  
}
void HWSerialModuleControl::setSyncID(uint8_t id)
{
  byte buf[9];
  buf[0] = buf[1] = LOBOT_SERVO_FRAME_HEADER;
  buf[2] = HIWONDER_SERVO_IS_MODULE_ID;
  buf[3] = 6;
  buf[4] = moduleID;
  buf[5] = 0xFE;
  buf[6] = SYN_ID_WRITE;
  buf[7] = id;
  buf[8] = LobotCheckSum(buf);
  if(isAutoEnableRT == false)
    TxEnable();
  SerialX->write(buf, buf[3] +3);
  delay(15);
}

int HWSerialModuleControl::getSyncID(void)
{
  int count = 10000;
  int ret;
  byte buf[8];

  buf[0] = buf[1] = LOBOT_SERVO_FRAME_HEADER;
  buf[2] = HIWONDER_SERVO_IS_MODULE_ID;
  buf[3] = 5;
  buf[4] = moduleID;
  buf[5] = 0xFE;
  buf[6] = SYN_ID_READ;
  buf[7] = LobotCheckSum(buf);
  

  while (SerialX->available())
    SerialX->read();
    
  if(!isUseHardwareSerial)
    ((SoftwareSerial*)(SerialX))->listen();
  if(isAutoEnableRT == false)
    TxEnable();
  SerialX->write(buf, buf[3] +3);
  if(isUseHardwareSerial)
  {
    delayMicroseconds((buf[3] +3)*110);
  }
  if(isAutoEnableRT == false)
    RxEnable();
  while (!SerialX->available()) {
    count -= 1;
    if (count < 0)
      return -1;
  }

  if (LobotSerialServoReceiveHandle(buf) > 0)
    ret = buf[3];
  else
    ret = -2048;

#ifdef LOBOT_DEBUG
  Serial.println(ret);
#endif
  return ret;
 
}
