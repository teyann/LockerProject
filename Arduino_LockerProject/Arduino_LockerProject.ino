/*	kor/한글테스트
	eng/Korean Test
	kor/lcd 출력으로 변경하려면, 시리얼로 출력되어 있는걸 바탕으로 소스 변경이 필요함
	eng/if you change serial output to lcd output, you must to change lcd code based on serial source code.
*/

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
#ifndef _SEIRLA_IO_
lcd					LCD(&i2cLcd);
#endif
#ifdef _SEIRLA_IO_
lcd					LCD;
#endif
lockDevice			Lock(LOCK_PIN, LIMIT_SWITCH_PIN);

void setup() {
	Serial.begin(115200);
	LCD.begin(&Locker, &Key);
	Locker.begin(&Key, &Lock);
}

void loop() {
	// 
	Key.updateKey();
	// ��� ������Ʈ
	int errCode = Locker.updateLocker();
	if (errCode != 0) {
		LCD.writeErrLcd(errCode);
	}
	else {
		// LCD ���
		LCD.writeLcd();
	}
}
