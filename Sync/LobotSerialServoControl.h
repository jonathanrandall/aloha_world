
#ifndef LOBOTSERIALSERVOCONTROL_H
#define LOBOTSERIALSERVOCONTROL_H

#include <Arduino.h>
#include <SoftwareSerial.h>

#define GET_LOW_BYTE(A) (uint8_t)((A))
//宏函数 获得A的低八位
#define GET_HIGH_BYTE(A) (uint8_t)((A) >> 8)
//宏函数 获得A的高八位
#define BYTE_TO_HW(A, B) ((((uint16_t)(A)) << 8) | (uint8_t)(B))
//宏函数 以A为高八位 B为低八位 合并为16位整形



#define LOBOT_SERVO_FRAME_HEADER         0x55

#define LOBOT_SERVO_MOVE_TIME_WRITE      1
#define LOBOT_SERVO_MOVE_TIME_READ       2
#define LOBOT_SERVO_MOVE_TIME_WAIT_WRITE 7
#define LOBOT_SERVO_MOVE_TIME_WAIT_READ  8
#define LOBOT_SERVO_MOVE_START           11
#define LOBOT_SERVO_MOVE_STOP            12
#define LOBOT_SERVO_ID_WRITE             13
#define LOBOT_SERVO_ID_READ              14
#define LOBOT_SERVO_ANGLE_OFFSET_ADJUST  17
#define LOBOT_SERVO_ANGLE_OFFSET_WRITE   18
#define LOBOT_SERVO_ANGLE_OFFSET_READ    19
#define LOBOT_SERVO_ANGLE_LIMIT_WRITE    20
#define LOBOT_SERVO_ANGLE_LIMIT_READ     21
#define LOBOT_SERVO_VIN_LIMIT_WRITE      22
#define LOBOT_SERVO_VIN_LIMIT_READ       23
#define LOBOT_SERVO_TEMP_MAX_LIMIT_WRITE 24
#define LOBOT_SERVO_TEMP_MAX_LIMIT_READ  25
#define LOBOT_SERVO_TEMP_READ            26
#define LOBOT_SERVO_VIN_READ             27
#define LOBOT_SERVO_POS_READ             28
#define LOBOT_SERVO_OR_MOTOR_MODE_WRITE  29
#define LOBOT_SERVO_OR_MOTOR_MODE_READ   30
#define LOBOT_SERVO_LOAD_OR_UNLOAD_WRITE 31
#define LOBOT_SERVO_LOAD_OR_UNLOAD_READ  32
#define LOBOT_SERVO_LED_CTRL_WRITE       33
#define LOBOT_SERVO_LED_CTRL_READ        34
#define LOBOT_SERVO_LED_ERROR_WRITE      35
#define LOBOT_SERVO_LED_ERROR_READ       36


class LobotSerialServoControl {
  public:
    LobotSerialServoControl(SoftwareSerial &A);
    LobotSerialServoControl(SoftwareSerial &A,int receiveEnablePin, int transmitEnablePin);
	  LobotSerialServoControl(HardwareSerial &A);
    LobotSerialServoControl(HardwareSerial &A,int receiveEnablePin, int transmitEnablePin);
    void OnInit(void);
    void LobotSerialServoMove(uint8_t id, int16_t position, uint16_t time);
    void LobotSerialServoStopMove(uint8_t id);
    void LobotSerialServoSetID(uint8_t oldID, uint8_t newID);
    void LobotSerialServoSetMode(uint8_t id, uint8_t Mode, int16_t Speed);
    void LobotSerialServoLoad(uint8_t id);
    void LobotSerialServoUnload(uint8_t id);
    int LobotSerialServoReceiveHandle(byte *ret);
    int LobotSerialServoReadPosition(uint8_t id);
    int LobotSerialServoReadVin(uint8_t id);



  protected:
    byte LobotCheckSum(byte buf[]);
    inline void RxEnable(void);
    inline void TxEnable(void);
      
  protected:
	  bool isUseHardwareSerial;
    Stream *SerialX;
    bool isAutoEnableRT;
    int receiveEnablePin;
    int transmitEnablePin;
  
};

#endif
