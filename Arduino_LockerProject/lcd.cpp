#include "lcd.h"

String LcdClass::lcdMsg[] = {
	"Locker System!",
	"1. Keep 2. Find",

	"Locker Number",

	"Password",

	"Locker Opened",
	"# Button",

	"Locker Closed",
	"Thank You",

	"Error",
};

String LcdClass::lcdKeyword[] = {
	"Keep ",
	"Find ",
	"Out Range", // Err 1
	"Don't Use", // Err 2
	"Password" // Err 3
};

LcdClass::LcdClass() { // serial
}

LcdClass::LcdClass(LiquidCrystal_I2C* const liquidCrystalPtr) { // i2c lcd
	_myLcd = liquidCrystalPtr;
	_myLcd->init();
	_myLcd->backlight();
}

void LcdClass::begin(LockerClass* const myLocker, KeypadClass* const myKeypad) {
	_myLocker = myLocker;
	_myKeypad = myKeypad;
	wasErrorState = false;
	writeLcd();
}

void LcdClass::initalLcd() {
	/* Locker System!
	 * 1. Keep 2. Find
	 */
#if(!defined SERIAL_IO)
	_myLcd->clear();
	_myLcd->print(lcdMsg[INITAL_LCD]);
	_myLcd->setCursor(0, 1);
	_myLcd->print(lcdMsg[INITAL_LCD+1]);
#else
	Serial.println(lcdMsg[INITAL_LCD]);
	Serial.println(lcdMsg[INITAL_LCD + 1]);
	Serial.println();
#endif
}
void LcdClass::selectLcd() {
	/* Find Locker Number / Keep Locker Number
	 * 1:O 2:X 3:X 4:O
	 */
#if(!defined SERIAL_IO)
	_myLcd->clear();
	if (_myLocker->getSelectMode() == KEEP_MODE)
		_myLcd->print(lcdKeyword[0]);
	else 
		_myLcd->print(lcdKeyword[1]);

	_myLcd->print(lcdMsg[SELECT_LCD]);
	_myLcd->setCursor(0, 1);

	for (int i = 1; i <= LOCKER_SIZE; i++) {
		_myLcd->print(i);
		_myLcd->print(":");
		if (_myLocker->getSelectMode() == KEEP_MODE) {
			if (_myLocker->getHasItem(i) != false) 
				_myLcd->print("X");
			else 
				_myLcd->print("O");
		}
		else {
			if (_myLocker->getHasItem(i) != true)
				_myLcd->print("X");
			else
				_myLcd->print("O");
		}
		_myLcd->print(" ");
	}
#else
	if (_myLocker->getSelectMode() == KEEP_MODE)
		Serial.print(lcdKeyword[0]);
	else
		Serial.print(lcdKeyword[1]);

	Serial.println(lcdMsg[SELECT_LCD]);

	for (int i = 1; i <= LOCKER_SIZE; i++) {
		Serial.print(i);
		Serial.print(":");
		if (_myLocker->getSelectMode() == KEEP_MODE) {
			if (_myLocker->getHasItem(i) != false)
				Serial.print("X");
			else
				Serial.print("O");
		}
		else {
			if (_myLocker->getHasItem(i) != true)
				Serial.print("X");
			else
				Serial.print("O");
		}
		Serial.print(" ");
	}
	Serial.println();
	Serial.println();
#endif
}
void LcdClass::passwordLcd() {
	/* Password
	 * **__
	 */
#if(!defined SERIAL_IO)
	_myLcd->clear();
	_myLcd->print(lcdMsg[PASSWORD_LCD]);
	_myLcd->setCursor(0, 1);

	int bufCnt = _myKeypad->getBufferCount();
	if (_myKeypad->isBufferFull() != false) {
		bufCnt = 4;
	}
	for (int i = 0; i < 4; i++) {
		if (i <= (bufCnt-1))
			_myLcd->print("*");
		else
			_myLcd->print("_");
	}
#else
	Serial.println(lcdMsg[PASSWORD_LCD]);
	int bufCnt = _myKeypad->getBufferCount();
	if (_myKeypad->isBufferFull() != false) {
		bufCnt = 4;
	}
	for (int i = 0; i < 4; i++) {
		if (i <= (bufCnt-1))
			Serial.print("*");
		else
			Serial.print("_");
	}
	Serial.println();
	Serial.println();
#endif
}
void LcdClass::openLcd() {
	/* Locker Opened
	 * # Button
	 */
#if(!defined SERIAL_IO)
	_myLcd->clear();
	_myLcd->print(lcdMsg[OPEN_LCD]);
	_myLcd->setCursor(0, 1);
	_myLcd->print(lcdMsg[OPEN_LCD+1]);
#else
	Serial.println(lcdMsg[OPEN_LCD]);
	Serial.println(lcdMsg[OPEN_LCD + 1]);
	Serial.println();
#endif
}
void LcdClass::closeLcd() {
	/* Locker Closed
	 * Thank You
	 */
#if(!defined SERIAL_IO)
	_myLcd->clear();
	_myLcd->print(lcdMsg[CLOSE_LCD]);
	_myLcd->setCursor(0, 1);
	_myLcd->print(lcdMsg[CLOSE_LCD+1]);
#else
	Serial.println(lcdMsg[CLOSE_LCD]);
	Serial.println(lcdMsg[CLOSE_LCD + 1]);
	Serial.println();
#endif
}
void LcdClass::adminLcd() {
	/* 1: 0000 2: ____
	 * 3: ____ 4: 1234
	 */
#if(!defined SERIAL_IO)
	_myLcd->clear();
#else
	Serial.println("-test-");
	Serial.println("ADMIN_STATE");
	Serial.println();
#endif
}
void LcdClass::errorLcd(int errCnt) {
	/* Error
	 * Out Range / Don't Use / Password
	 */
#if(!defined SERIAL_IO)
	_myLcd->clear();
	_myLcd->print(lcdMsg[ERROR_LCD]);
	_myLcd->setCursor(0, 1);
	_myLcd->print(lcdKeyword[errCnt+1]);
#else
	Serial.println(lcdMsg[ERROR_LCD]);
	Serial.println(lcdKeyword[errCnt + 1]);
	Serial.println();
#endif
}

void LcdClass::writeLcd() {
	state_t currentState = _myLocker->getCurrentState();
	static state_t preState = CLOSE_STATE;

	static int preBufCnt = 0;
	int bufCnt = _myKeypad->getBufferCount();
	/*
	 * 패스워드 입력 상태일때 숫자가 입력될경우
	 * 또는 상태가 바뀐경우
	 * 또는 이전상태가 에러였던경우
	 */
	if ((bufCnt != preBufCnt && currentState == PASSWORD_STATE) ||
		currentState != preState ||
		getWasErrorState() != false) {
		switch (currentState)
		{
		case IDLE_STATE:
			initalLcd();
			break;
		case SELECT_STATE:
			selectLcd();
			break;
		case PASSWORD_STATE:
			passwordLcd();
			break;
		case OPEN_STATE:
			openLcd();
			break;
		case CLOSE_STATE:
			closeLcd();
			break;
		case ADMIN_STATE:
			adminLcd();
			break;
		default:
			break;
		}
	}
	preState = currentState;
	preBufCnt = bufCnt;
	setWasErrorState(false);
}
void LcdClass::writeErrLcd(int errCnt) {
	if (getWasErrorState() != true) {
		setWasErrorState(true);
		errorLcd(errCnt);
	}
}
