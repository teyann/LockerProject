/**
 *	kor/한글테스트
 *	eng/Korean Test
 *
 *	[ARDUINO_LOCKER]  PROJECT
 *
 */

// TODO: 관리자 모드
// TODO: 리미트 스위치가 눌린후 다시 떼어질경우 시간 초기화 (인터럽트 고려)

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "locker.h"
#include "key.h"
#include "lcd.h"
#include "lockDevice.h"
#include "config.h"

// 0x27의 주소를 가지는, 16문자, 2줄 화면으로 설정 / set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C		i2cLcd(0x27, 16, 2);
// 4개의 락커를 지니도록 설정 / set to has 4 Locker
LockerClass				Locker(LOCKER_SIZE);
// 각각 배열에 있는 값들을 가지는 잠금 장치를 설정 / set lockdevice by each pin for array 
LockDeviceClass			Lock(LOCK_PIN, LIMIT_SWITCH_PIN);
KeypadClass				Key;

#if(defined SERIAL_IO)
LcdClass				LCD;
#else
LcdClass				LCD(&i2cLcd);
#endif

void setup() {
	Serial.begin(SERIAL_BAUDRATE);
	// Lcd, Locker 초기화 / initalize lcd and locker
	LCD.begin(&Locker, &Key);
	Locker.begin(&Key, &Lock);
}

void loop() {
	// 현재 입력되는 값을 읽음 / read current key value
	Key.updateKeypad();
	// Locker를 업데이트 하고 에러 값을 받음 / update locker and get error code
	int errCode = Locker.updateLocker();
	// 에러코드가 0이 아니면 에러 화면을 출력 / if error code isn't 0, displaied error screen
	if (errCode != 0) {
		LCD.writeErrLcd(errCode);
	}
	else {
		LCD.writeLcd();
	}
}