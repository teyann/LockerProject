#ifndef LOCKDEVICE_H
#define LOCKDEVICE_H

#include <Arduino.h>
#include "config.h"

class LockDeviceClass
{
public:
	LockDeviceClass(const int* const lockPtr, const int* const limSwPtr); // 생성자

	void	lockOn(int lockerNum);			// 락커 잠금 설정
	void	lockOff(int lockerNum);			// 락커 잠금 해제

	boolean isLimSwOn(int lockerNum);		// 락커 문이 닫혔는지 여부
	int		getLimSwOnTime(int lockerNum);	// 락커 문이 닫힌채 유지된 시간

private:
	void	lockInit();						// 잠금장치(솔레노이드) 초기화
	void	limSwInit();					// 리미트 스위치 초기화

	const int* const _lockPtr;				// 잠금장치(솔레노이드)핀 포인터
	const int* const _limSwPtr;				// 리미트 스위치 핀 포인터
};

#endif