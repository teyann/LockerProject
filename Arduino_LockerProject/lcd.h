#ifndef LCD_H
#define LCD_H

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include "locker.h"
#include "key.h"
#include "lockDevice.h"

class lcd
{
public:
	lcd(); //for Serial IO
	lcd(LiquidCrystal_I2C* liquidCrystalPtr);
	void begin(locker* myLocker, key* myKey);
	void writeLcd();
	void writeErrLcd(int errCnt);
private:
	enum {
		INITAL_LCD = 0, SELECT_LCD = 2, PASSWORD_LCD = 3,
		OPEN_LCD = 4, CLOSE_LCD = 6, ERROR_LCD = 8
	};
	LiquidCrystal_I2C* _lcd;
	//string Class::arr = {"one", "two", "three"}; 

	static String lcdMsg[];
	static String lcdKeyword[];

	void initalLcd();
	void selectLcd();
	void passwordLcd();
	void openLcd();
	void closeLcd();
	void errorLcd(int errCnt);
	void adminLcd();

	locker* _myLocker;
	key* _myKey;
};

#endif
