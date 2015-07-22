#ifndef _LOCKDEVICE_H_
#define _LOCKDEVICE_H_

#include <Arduino.h>

class lockDevice
{
public:
	lockDevice(int* lockPtr, int* limSwPtr);

	void lockOn(int boxNum);
	void lockOff(int boxNum);
	boolean isLimSwOn(int boxNum);
	int limSwOnTime(int boxNum);
private:
	void lockInit();
	void limSwInit();

	int* const _lockPtr;
	int* const _limSwPtr;
};

#endif