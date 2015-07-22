#include "lockDevice.h"

lockDevice::lockDevice(int* lockPtr, int* limSwPtr) 
: _lockPtr(lockPtr), _limSwPtr(limSwPtr) {
	lockInit();
	limSwInit();
}

void lockDevice::lockInit() {
	for (uint8_t i = 0; i < 4; i++) {
		pinMode(_lockPtr[i], OUTPUT);
		lockOn(_lockPtr[i]);
	}
}

void lockDevice::lockOff(int boxNum) {
	digitalWrite(_lockPtr[boxNum-1], HIGH);
}

void lockDevice::lockOn(int boxNum) {
	digitalWrite(_lockPtr[boxNum - 1], LOW);
}

void lockDevice::limSwInit() {
	for (uint8_t i = 0; i < 4; i++) {
		pinMode(_limSwPtr[i], INPUT_PULLUP); //���� Ǯ�� ���
	}
}

boolean lockDevice::isLimSwOn(int boxNum) {
	static uint32_t startTime = 0;
	static bool flag = false;
	if (digitalRead(_limSwPtr[boxNum - 1]) == 0) {
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
}

int lockDevice::limSwOnTime(int boxNum) {
	// INT
}
