#include "lockDevice.h"

LockDeviceClass::LockDeviceClass(const int* lockPtr, const int* limSwPtr)
: _lockPtr(lockPtr), _limSwPtr(limSwPtr) {
	lockInit();
	limSwInit();
}

void LockDeviceClass::lockInit() {
	for (uint8_t i = 0; i < LOCKER_SIZE; i++) {
		pinMode(_lockPtr[i], OUTPUT);
		lockOn(_lockPtr[i]);
	}
}

/*
 * @param lockerNum [1;4]
 */
void LockDeviceClass::lockOff(int lockerNum) {
	digitalWrite(_lockPtr[lockerNum-1], HIGH);
}

/*
* @param lockerNum [1;4]
*/
void LockDeviceClass::lockOn(int lockerNum) {
	digitalWrite(_lockPtr[lockerNum - 1], LOW);
}

void LockDeviceClass::limSwInit() {
	for (uint8_t i = 0; i < LOCKER_SIZE; i++) {
		pinMode(_limSwPtr[i], INPUT_PULLUP); //���� Ǯ�� ���
	}
}

/*
* @param lockerNum [1;4]
*/
boolean LockDeviceClass::isLimSwOn(int lockerNum) {
#if(!defined SERIAL_IO)
	static uint32_t startTime = 0;
	static bool flag = false;
	if (digitalRead(_limSwPtr[lockerNum - 1]) == 0) {
		if (flag == false) {
			startTime = millis();
			flag = true;
		}
		if (millis() - startTime > 20) { // ��ٿ�� ����, 20�и��� �̻� ���������
			return true;
		}
	}
	else {
		flag = false;
	}
	return false;
#else
	return true;
#endif
}

int LockDeviceClass::getLimSwOnTime(int lockerNum) {
	// INT
	return 0;
}
