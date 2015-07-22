#ifndef _LCD_H_
#define _LCD_H_

#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "locker.h"
#include "key.h"
#include "lockDevice.h"

class lcd
{
public:
	lcd(LiquidCrystal_I2C* liquidCrystalPtr);
	void writeLcd(locker* locker, key* key);
	void writeErrLcd(int errCnt);
private:
	enum {
		INITAL_LCD = 0, SELECT_LCD = 2, PASSWORD_LCD = 3,
		OPEN_LCD = 4, CLOSE_LCD = 6, ERROR_LCD = 8
	};
	LiquidCrystal_I2C* _lcdI2c;

	String lcdMsg[9] = {
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

	String lcdKeyword[4] = {
		"Keep ",
		"Find ",
		"Out Range" // Err 1
		"Don't Use", // Err 2
		"Password" // Err 3
	};

	void initalLcd();
	void selectLcd(locker* locker);
	void passwordLcd(key* key);
	void openLcd();
	void closeLcd();
	void errorLcd(int errCnt);
	void adminLcd();
};

#endif
