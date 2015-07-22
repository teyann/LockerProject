#include "lcd.h"

lcd::lcd() {
}

lcd::lcd(LiquidCrystal_I2C* liquidCrystalPtr) {
	_lcd = liquidCrystalPtr;
	_lcd->init();
	_lcd->backlight();
}

void lcd::initalLcd() {
	/* Locker System!
	 * 1. Keep 2. Find
	 */
#ifndef _SEIRLA_IO_
	_lcd->clear();
	_lcd->print(lcdMsg[INITAL_LCD]);
	_lcd->setCursor(0, 1);
	_lcd->print(lcdMsg[INITAL_LCD+1]);
#endif
	Serial.println(lcdMsg[INITAL_LCD]);
	Serial.println(lcdMsg[INITAL_LCD + 1]);
	Serial.print("_");
	Serial.println();
}
void lcd::selectLcd(locker* locker) {
	/* Find Locker Number / Keep Locker Number
	 * 1:O 2:X 3:X 4:O
	 */
#ifndef _SEIRLA_IO_
	_lcd->clear();
	if (locker->selectedMode() == FIND_MODE)
		_lcd->print(lcdKeyword[0]);
	else 
		_lcd->print(lcdKeyword[1]);

	_lcd->print(lcdMsg[SELECT_LCD]);
	_lcd->setCursor(0, 1);

	for (int i = 0; i < 4; i++) {
		_lcd->print(i+1);
		_lcd->print(":");
		if (locker->selectedMode() == FIND_MODE) {
			if (locker->hasItem(i) != true) 
				_lcd->print("X");
			else 
				_lcd->print("O");
		}
		else {
			if (locker->hasItem(i) != false)
				_lcd->print("X");
			else
				_lcd->print("O");
		}
		_lcd->print(" ");
	}
#endif
	if (locker->selectedMode() == KEEP_MODE)
		Serial.print(lcdKeyword[0]);
	else
		Serial.print(lcdKeyword[1]);

	Serial.println(lcdMsg[SELECT_LCD]);

	for (int i = 1; i <= 4; i++) {
		Serial.print(i);
		Serial.print(":");
		if (locker->selectedMode() == KEEP_MODE) {
			if (locker->hasItem(i) != false)
				Serial.print("X");
			else
				Serial.print("O");
		}
		else {
			if (locker->hasItem(i) != true)
				Serial.print("X");
			else
				Serial.print("O");
		}
		Serial.print(" ");
	}
	Serial.println();
	Serial.println();
}
void lcd::passwordLcd(key* key) {
	/* Password
	 * **__
	 */
#ifndef _SEIRLA_IO_
	_lcd->clear();
	_lcd->print(lcdMsg[PASSWORD_LCD]);
	_lcd->setCursor(0, 1);

	static int preBufCnt = 0;
	int bufCnt = key->bufferCount();
	if (key->isBufferFull() != false) {
		bufCnt = 4;
	}
	for (int i = 0; i < 4; i++) {
		if (bufCnt >= i)
			_lcd->print("*");
		else
			_lcd->print("_");
	}
	preBufCnt = bufCnt;
#endif
	Serial.println(lcdMsg[PASSWORD_LCD]);
	int bufCnt = key->bufferCount();
	if (key->isBufferFull() != false) {
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
}
void lcd::openLcd() {
	/* Locker Opened
	 * # Button
	 */
#ifndef _SEIRLA_IO_
	_lcd->clear();
	_lcd->print(lcdMsg[OPEN_LCD]);
	_lcd->setCursor(0, 1);
	_lcd->print(lcdMsg[OPEN_LCD+1]);
#endif
	Serial.println(lcdMsg[OPEN_LCD]);
	Serial.println(lcdMsg[OPEN_LCD + 1]);
	Serial.println();
}
void lcd::closeLcd() {
	/* Locker Closed
	 * Thank You
	 */
#ifndef _SEIRLA_IO_
	_lcd->clear();
	_lcd->print(lcdMsg[CLOSE_LCD]);
	_lcd->setCursor(0, 1);
	_lcd->print(lcdMsg[CLOSE_LCD+1]);
#endif
	Serial.println(lcdMsg[CLOSE_LCD]);
	Serial.println(lcdMsg[CLOSE_LCD + 1]);
	Serial.println();
}
void lcd::errorLcd(int errCnt) {
	/* Error
	 * Out Range / Don't Use / Password
	 */
#ifndef _SEIRLA_IO_
	_lcd->clear();
	_lcd->print(lcdMsg[ERROR_LCD]);
	_lcd->setCursor(0, 1);
	_lcd->print(lcdKeyword[errCnt+1]);
#endif
	Serial.println(lcdMsg[ERROR_LCD]);
	Serial.println(lcdKeyword[errCnt + 1]);
	Serial.println();
}

void lcd::writeLcd(locker* locker, key* key) {
	state_t currentState = locker->currentState();
	static state_t preState = CLOSE_STATE;

	static int preBufCnt = 0;
	int bufCnt = key->bufferCount();

	if ((bufCnt != preBufCnt && currentState == PASSWORD_STATE) ||
		currentState != preState) {
		switch (currentState)
		{
		case IDLE_STATE:
			initalLcd();
			break;
		case SELECT_STATE:
			selectLcd(locker);
			break;
		case PASSWORD_STATE:
			passwordLcd(key);
			break;
		case OPEN_STATE:
			openLcd();
			break;
		case CLOSE_STATE:
			closeLcd();
			break;
		default:
			break;
		}
	}
	preState = currentState;
	preBufCnt = bufCnt;
}
void lcd::writeErrLcd(int errCnt) {
	errorLcd(errCnt);
}
