#include "lcd.h"

lcd::lcd(LiquidCrystal_I2C* liquidCrystalPtr) {
	_lcdI2c = liquidCrystalPtr;
	_lcdI2c->init();
	_lcdI2c->backlight();
}

void lcd::initalLcd() {
	/* Locker System!
	 * 1. Keep 2. Find
	 */
	_lcdI2c->clear();
	_lcdI2c->print(lcdMsg[INITAL_LCD]);
	_lcdI2c->setCursor(0, 1);
	_lcdI2c->print(lcdMsg[INITAL_LCD+1]);
}
void lcd::selectLcd(locker* locker) {
	/* Find Locker Number / Keep Locker Number
	 * 1:O 2:X 3:X 4:O
	 */
	_lcdI2c->clear();

	if (locker->selectedMode() == FIND_MODE)
		_lcdI2c->print(lcdKeyword[0]);
	else 
		_lcdI2c->print(lcdKeyword[1]);

	_lcdI2c->print(lcdMsg[SELECT_LCD]);
	_lcdI2c->setCursor(0, 1);

	for (int i = 0; i < 4; i++) {
		_lcdI2c->print(i+1);
		_lcdI2c->print(":");
		if (locker->selectedMode() == FIND_MODE) {
			if (locker->hasItem(i) != true) 
				_lcdI2c->print("X");
			else 
				_lcdI2c->print("O");
		}
		else {
			if (locker->hasItem(i) != false)
				_lcdI2c->print("X");
			else
				_lcdI2c->print("O");
		}
		_lcdI2c->print(" ");
	}
}
void lcd::passwordLcd(key* key) {
	/* Password
	 * **__
	 */
	_lcdI2c->clear();
	_lcdI2c->print(lcdMsg[PASSWORD_LCD]);
	_lcdI2c->setCursor(0, 1);

	static int preBufCnt = 0;
	int bufCnt = key->bufferCount();
	if (key->isBufferFull() != false) {
		bufCnt = 4;
	}
	for (int i = 0; i < 4; i++) {
		if (bufCnt >= i)
			_lcdI2c->print("*");
		else
			_lcdI2c->print("_");
	}
	preBufCnt = bufCnt;
}
void lcd::openLcd() {
	/* Locker Opened
	 * # Button
	 */
	_lcdI2c->clear();
	_lcdI2c->print(lcdMsg[OPEN_LCD]);
	_lcdI2c->setCursor(0, 1);
	_lcdI2c->print(lcdMsg[OPEN_LCD+1]);
}
void lcd::closeLcd() {
	/* Locker Closed
	 * Thank You
	 */
	_lcdI2c->clear();
	_lcdI2c->print(lcdMsg[CLOSE_LCD]);
	_lcdI2c->setCursor(0, 1);
	_lcdI2c->print(lcdMsg[CLOSE_LCD+1]);
}
void lcd::errorLcd(int errCnt) {
	/* Error
	 * Out Range / Don't Use / Password
	 */
	_lcdI2c->clear();
	_lcdI2c->print(lcdMsg[ERROR_LCD]);
	_lcdI2c->setCursor(0, 1);
	_lcdI2c->print(lcdKeyword[errCnt+1]);
}

void lcd::writeLcd(locker* locker, key* key) {
	switch (locker->currentState())
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
void lcd::writeErrLcd(int errCnt) {
	errorLcd(errCnt);
}
