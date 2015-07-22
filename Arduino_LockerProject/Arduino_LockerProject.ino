#include <EEPROM.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "locker.h"
#include "key.h"
#include "lcd.h"
#include "lockDevice.h"

int LOCK_PIN[4] = {
	9, 8, 7, 6,
};
int LIMIT_SWITCH_PIN[4] = {
	5, 4, 3, 2,
};

LiquidCrystal_I2C	i2cLcd(0x27, 16, 2);  // set the LCD address to 0x27 for a 20 chars and 4 line display
locker				Locker(4);
key					Key;
lcd					LCD(&i2cLcd);
lockDevice			Lock(LOCK_PIN, LIMIT_SWITCH_PIN);

void setup() {
	Serial.begin(9600);
}

void loop() {
	// Ű��Ʈ ������ �б�
	Key.updateKey();
	// ��� ������Ʈ
	Locker.updateLocker(&Key, &Lock);
	// LCD ���
	LCD.writeLcd(&Locker, &Key);
}
