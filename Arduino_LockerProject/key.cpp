#include "key.h"

key::key() {
	_keyChar = '/0';
	_num4Index = 0;
	bufFull = false;
}

void key::updateKey() {
	// key matrix�� ����� ���� ������ �о��
	uint16_t keyADC = analogRead(KEY_PIN);

	// analogKeyValue[i][j] - VAL_RANGE : �����ص� ���� VAL_RANGE�� ����
	// analogKeyValue[i][j] + VAL_RANGE : �����ص� ���� VAL_RANGE�� ���Ѱ�
	// �� �ΰ� �����϶� �̸� �����ص� ������ ������
	for (uint8_t i = 0; i < 3; i++) {
		for (uint8_t j = 0; j < 4; j++) {
			if (keyADC >= (analogKeyValue[i][j] - VAL_RANGE) &&
				keyADC <= (analogKeyValue[i][j] + VAL_RANGE)) {
				_keyChar = analogKeyData[i][j];
				break;
			}
		}
	}
	if (isNumber() == true) {
		// ���ۿ� ����
		for (int i = 1; i < 4; i++) {
			_num4Buf[i - 1] = _num4Buf[i];
		}
		_num4Buf[4] = _keyChar;
		if (++_num4Index == 4) {
			bufFull = true;
		}
		_num4Index = _num4Index % 4;
	}
#ifdef _SERIAL_DEBUG_
	Serial.println(keyADC + "/" + _keyChar);
#endif
}

char key::readKey() {
	return _keyChar;
}

int key::read4Num() {
	int val = atoi(_num4Buf);
	clearBuffer();
	return val;
}

void key::clearBuffer() {
	memset(_num4Buf, 0, 4);
	bufFull = false;
}

boolean key::isBufferFull() {
	return bufFull;
}

int key::bufferCount() {
	return _num4Index;
}

boolean key::isData() {
	return _keyChar != NO_CHAR;
}

boolean key::isNumber() {
	return (_keyChar >= '0' && _keyChar <= '9');
}

boolean key::isChar() {
	char c = _keyChar;
	return ((c >= 'A' && c <= 'D') || (c == '*') || (c == '#'));
}

boolean key::isAsterisk() {
	return (_keyChar == '*');
}
