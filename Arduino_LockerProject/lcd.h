#ifndef LCD_H
#define LCD_H

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include "locker.h"
#include "key.h"
#include "lockDevice.h"
#include "config.h"

class LcdClass
{
public:
	// 생성자
	LcdClass(); 
	LcdClass(LiquidCrystal_I2C* const liquidCrystalPtr);

	void	begin(LockerClass* const myLocker, KeypadClass* const myKeypad); // 초기 설정
	void	writeLcd();					// LCD 출력
	void	writeErrLcd(int errCnt);	// LCD 에러 메시지 출력
	
private:
	enum {
		INITAL_LCD = 0, SELECT_LCD = 2, PASSWORD_LCD = 3,
		OPEN_LCD = 4, CLOSE_LCD = 6, ERROR_LCD = 8
	};

	void	initalLcd();				// 1. 초기 상태 LCD
	void	selectLcd();				// 2. 선택 상태 LCD
	void	passwordLcd();				// 3. 비밀번호 입력 상태 LCD
	void	openLcd();					// 4. 락커 문이 열린 상태 LCD
	void	closeLcd();					// 5. 락커 문이 닫힌 상태 LCD
	void	adminLcd();					// 관리자 상태 LCD

	void	errorLcd(int errCnt);		// 에러 LCD

	void	setWasErrorState(boolean err) { wasErrorState = err; }
	boolean getWasErrorState() { return wasErrorState; }

	LiquidCrystal_I2C*	_myLcd;			// LiquidCrystal_I2C(라이브러리) 클래스 포인터
	LockerClass*		_myLocker;		// 락커 클래스 포인터
	KeypadClass*		_myKeypad;			// 키패드 클래스 포인터
	
	static String		lcdMsg[];		// 출력 문자열
	static String		lcdKeyword[];	// 출력 키워드

	boolean				wasErrorState;	// 이전 상태가 에러인지 여부
};

#endif
