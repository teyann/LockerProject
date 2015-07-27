#include "key.h"

char KeypadClass::analogKeyData[3][4] = { '1', '2', '3',
										'4', '5', '6',
										'7', '8', '9',
										'*', '0', '#', };

KeypadClass::KeypadClass() {
	_data = '\0';
	_bufIndex = 0;
	bufFull = false;
}

void KeypadClass::updateKeypad() {
#if(!defined SERIAL_IO)
	// key matrix�� ����� ���� ������ �о��
	uint16_t keyADC = analogRead(KEY_PIN);

	// analogKeyValue[i][j] - VAL_RANGE : �����ص� ���� VAL_RANGE�� ����
	// analogKeyValue[i][j] + VAL_RANGE : �����ص� ���� VAL_RANGE�� ���Ѱ�
	// �� �ΰ� �����϶� �̸� �����ص� ������ ������
	for (uint8_t i = 0; i < 3; i++) {
		for (uint8_t j = 0; j < 4; j++) {
			if (keyADC >= (analogKeyValue[i][j] - VAL_RANGE) &&
				keyADC <= (analogKeyValue[i][j] + VAL_RANGE)) {
				_data = analogKeyData[i][j];
				break;
			}
		}
	}
#else
	if (Serial.available() > 0) {
		_data = Serial.read();
	}
	else {
		_data = NO_CHAR;
	}
#endif
	// 선형 큐(FIFO)
	if (isNumber() == true) {
		for (int i = 1; i < 4; i++) {
			_buf[i - 1] = _buf[i];
		}
		_buf[3] = _data;
		++_bufIndex;
		if (_bufIndex == 4) {
			bufFull = true;
		}
		_bufIndex = _bufIndex % 4;
	}

#if(!defined SERIAL_IO && defined SERIAL_DEBUG)
	Serial.println(keyADC + "/" + _data);
#endif
#ifdef SERIAL_DEBUG
	if (_data != NO_CHAR) {
		Serial.print("In Char = ");
		Serial.println(_data);
		Serial.println();
	}
#endif
}

/*
 * @retval ascii character
 */
char KeypadClass::getData() {
	return _data;
}

/*
 * @retval [0;9]
 */
int KeypadClass::getNum() {
// WARNING | 입력된 값이 숫자가 아니면 값에 오류 발생
	return (_data - 48) % 10;
}

/*
 * @retval [0;9999]
 */
int KeypadClass::get4DigitNum() {
	int val = atoi(_buf);
	clearBuffer();
	return val;
}

void KeypadClass::clearBuffer() {
	memset(_buf, 0, 4);
	_bufIndex = 0;
	bufFull = false;
}

boolean KeypadClass::isBufferFull() {
	return bufFull;
}

int KeypadClass::getBufferCount() {
	return _bufIndex;
}

boolean KeypadClass::isData() {
	return _data != NO_CHAR;
}

boolean KeypadClass::isNumber() {
	return (_data >= '0' && _data <= '9');
}

boolean KeypadClass::isChar() {
	char c = _data;
	return ((c >= 'A' && c <= 'D') || (c == '*') || (c == '#'));
}

boolean KeypadClass::isAsterisk() {
	return (_data == '*');
}
