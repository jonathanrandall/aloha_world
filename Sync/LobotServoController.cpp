/******************************************************
 * FileName:      LobotServoController.cpp
 ** Company:       Lewan Soul 
 * Date:           2016/07/02  16:53
 *Last Modification Date: 202008281106
* www.lewansoul.com
 *****************************************************/

#include "LobotServoController.h"
#include <Stream.h>

#define GET_LOW_BYTE(A) (uint8_t)((A))

#define GET_HIGH_BYTE(A) (uint8_t)((A) >> 8)

#define BYTE_TO_HW(A, B) ((((uint16_t)(A)) << 8) | (uint8_t)(B))


LobotServoController::LobotServoController(SoftwareSerial &A)
{
	numOfActinGroupRunning = 0xFF;
	actionGroupRunTimes = 0;
	isGetBatteryVolt = false;
	isRunning_ = false;
	batteryVoltage = 0;
	isUseHardwareSerial = false;
	A.listen();
	SerialX = (Stream*)(&A);
}

LobotServoController::LobotServoController(HardwareSerial &A)
{
	numOfActinGroupRunning = 0xFF;
	actionGroupRunTimes = 0;
	isGetBatteryVolt = false;
	isRunning_ = false;
	batteryVoltage = 0;
	isUseHardwareSerial = true;
	SerialX = (Stream*)(&A);
}

void LobotServoController::moveServo(uint8_t servoID, uint16_t Position, uint16_t Time)
{
	uint8_t buf[11];
	if (servoID > 31 || !(Time > 0) || Position > 32767) {
		return;
	}
	buf[0] = FRAME_HEADER;             
	buf[1] = FRAME_HEADER;
	buf[2] = 8;                             
	buf[3] = CMD_SERVO_MOVE;              
	buf[4] = 1;                         
	buf[5] = GET_LOW_BYTE(Time);        
	buf[6] = GET_HIGH_BYTE(Time);        
	buf[7] = servoID;             
	buf[8] = GET_LOW_BYTE(Position);        
	buf[9] = GET_HIGH_BYTE(Position);        

	SerialX->write(buf, 10);
}

void LobotServoController::moveServos(LobotServo servos[], uint8_t Num, uint16_t Time)
{
	uint8_t buf[103]; 
	if (Num < 1 || Num > 32 || !(Time > 0)) {
		return; 
	}
	buf[0] = FRAME_HEADER; 
	buf[1] = FRAME_HEADER;
	buf[2] = Num * 3 + 5; 
	buf[3] = CMD_SERVO_MOVE; 
	buf[4] = Num;  
	buf[5] = GET_LOW_BYTE(Time); 
	buf[6] = GET_HIGH_BYTE(Time);
	uint8_t index = 7;
	for (uint8_t i = 0; i < Num; i++) {
		buf[index++] = servos[i].ID;
		buf[index++] = GET_LOW_BYTE(servos[i].Position);
		buf[index++] = GET_HIGH_BYTE(servos[i].Position);
	}
	SerialX->write(buf, buf[2] + 2); 
}

void LobotServoController::moveServos(uint8_t Num, uint16_t Time, ...)
{
	uint8_t buf[128];
	va_list arg_ptr = NULL;
	va_start(arg_ptr, Time);
	if (Num < 1 || Num > 32 || (!(Time > 0)) || arg_ptr == NULL) {
		return; 
	}
	buf[0] = FRAME_HEADER;  
	buf[1] = FRAME_HEADER;
	buf[2] = Num * 3 + 5; 
	buf[3] = CMD_SERVO_MOVE;
	buf[4] = Num;        
	buf[5] = GET_LOW_BYTE(Time); 
	buf[6] = GET_HIGH_BYTE(Time);
	uint8_t index = 7;
	for (uint8_t i = 0; i < Num; i++) { 
		uint16_t tmp = va_arg(arg_ptr, uint16_t);
		buf[index++] = GET_LOW_BYTE(tmp); 

		uint16_t pos = va_arg(arg_ptr, uint16_t);
		buf[index++] = GET_LOW_BYTE(pos); 
		buf[index++] = GET_HIGH_BYTE(pos);
	}
	va_end(arg_ptr);
	SerialX->write(buf, buf[2] + 2);
}

void LobotServoController::runActionGroup(uint8_t numOfAction, uint16_t Times)
{
	uint8_t buf[7];
	buf[0] = FRAME_HEADER;
	buf[1] = FRAME_HEADER;
	buf[2] = 5;
	buf[3] = CMD_ACTION_GROUP_RUN; 
	buf[4] = numOfAction;
	buf[5] = GET_LOW_BYTE(Times); 
	buf[6] = GET_HIGH_BYTE(Times); 
	isRunning_ = true;
	SerialX->write(buf, 7);
}

void LobotServoController::stopActionGroup(void)
{
	uint8_t buf[4];
	buf[0] = FRAME_HEADER; 
	buf[1] = FRAME_HEADER;
	buf[2] = 2; 
	buf[3] = CMD_ACTION_GROUP_STOP; 

	SerialX->write(buf, 4);
}

void LobotServoController::setActionGroupSpeed(uint8_t numOfAction, uint16_t Speed)
{
	uint8_t buf[7];
	buf[0] = FRAME_HEADER; 
	buf[1] = FRAME_HEADER;
	buf[2] = 5; 
	buf[3] = CMD_ACTION_GROUP_SPEED;
	buf[4] = numOfAction; 
	buf[5] = GET_LOW_BYTE(Speed);
	buf[6] = GET_HIGH_BYTE(Speed);

	SerialX->write(buf, 7); 
}

void LobotServoController::setAllActionGroupSpeed(uint16_t Speed)
{
	setActionGroupSpeed(0xFF, Speed);
}

void LobotServoController::sendCMDGetBatteryVolt()
{
	uint8_t buf[4];
	buf[0] = FRAME_HEADER;
	buf[1] = FRAME_HEADER;
	buf[2] = 2;                 
	buf[3] = CMD_GET_BATTERY_VOLTAGE; 
	if(!isUseHardwareSerial)
		((SoftwareSerial*)(SerialX))->listen();
	isGetBatteryVolt = false;
	SerialX->write(buf, 4); 
}

uint16_t LobotServoController::getBatteryVolt(void)
{
	if(isGetBatteryVolt)
	{
		isGetBatteryVolt = false;
		return batteryVoltage;
	}else{
		return -1;
	}
}

uint16_t LobotServoController::getBatteryVolt(uint32_t timeout)
{
	isGetBatteryVolt = false;
	sendCMDGetBatteryVolt();
	timeout += millis();
	while(!isGetBatteryVolt)
	{
		if(timeout < millis())
		{
			return -1;
		}
		receiveHandle();
	}
	return batteryVoltage;
}
bool LobotServoController::isRunning()
{
	return isRunning_;
}

bool LobotServoController::waitForStopping(uint32_t timeout)
{
	while(SerialX->available())
		SerialX->read();
	timeout += millis();
	while(isRunning_)
	{
		if(timeout < millis())
		{
			return false;
		}
		receiveHandle();
	}
	return true;
}

void LobotServoController::receiveHandle()
{
	uint8_t rx;
	static uint8_t buf[16];
	static bool isGetFrameHeader = false;
	static uint8_t frameHeaderCount = 0;
	static uint8_t dataLength = 2;
	static uint8_t dataCount = 0;
	
	while(SerialX->available() > 0)
	{
		rx = SerialX->read();
		if(!isGetFrameHeader)
		{
			if(rx == 0x55)
			{
				frameHeaderCount++;
				if(frameHeaderCount == 2)
				{
					frameHeaderCount = 0;
					isGetFrameHeader = true;
					dataCount = 1;
				}
			} else {
				isGetFrameHeader = false;
				dataCount = 0;
				frameHeaderCount = 0;
			}
		}
		if(isGetFrameHeader)
		{
			buf[dataCount] = rx;
			if(dataCount == 2)
			{
				dataLength = buf[dataCount];
				if(dataLength < 2 || dataLength > 8)
				{
					dataLength = 2;
					isGetFrameHeader = false;
				}
			}
			dataCount++;
			if(dataCount == dataLength + 2)
			{
				isGetFrameHeader = false;
				switch(buf[3])
				{
					case BATTERY_VOLTAGE:
						batteryVoltage = BYTE_TO_HW(buf[5], buf[4]);
						isGetBatteryVolt = true;
						break;
					case ACTION_GROUP_RUNNING:
						isRunning_ = true;
						break;
					case ACTION_GROUP_COMPLETE:
					case ACTION_GROUP_STOPPED:
						isRunning_ = false;
						break;
					default:
						break;
				}
			}
		}
	}
}
