

#ifndef HWSERIALMODULECONTROL_H
#define HWSERIALMODULECONTROL_H
#include "LobotSerialServoControl.h"

#define HIWONDER_SERVO_IS_MODULE_ID       0xFC
#define SYN_ID_WRITE        1
#define SYN_ID_READ         2
#define SYN_POS_READ        3


class HWSerialModuleControl: public LobotSerialServoControl
{
  public:
    HWSerialModuleControl(SoftwareSerial &A, uint8_t moduleID):LobotSerialServoControl(A),moduleID(moduleID){};
    HWSerialModuleControl(SoftwareSerial &A,int receiveEnablePin, int transmitEnablePin, uint8_t moduleID):LobotSerialServoControl(A,receiveEnablePin,transmitEnablePin),moduleID(moduleID){};
    HWSerialModuleControl(HardwareSerial &A, uint8_t moduleID):LobotSerialServoControl(A),moduleID(moduleID){};
    HWSerialModuleControl(HardwareSerial &A,int receiveEnablePin, int transmitEnablePin, uint8_t moduleID):LobotSerialServoControl(A,receiveEnablePin,transmitEnablePin),moduleID(moduleID){};

    int16_t getSyncPosition(uint8_t id);
    void getSyncPositionAll(int16_t *pos);
    void setSyncID(uint8_t id);
    int getSyncID(void);
    
  private:
    uint8_t moduleID;
};


#endif
