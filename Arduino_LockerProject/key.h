#ifndef _KEY_H_
#define _KEY_H_

#include <Arduino.h>

//#define _SERIAL_DEBUG_
#define _SEIRLA_IO_

#define NO_CHAR '\0' // NULL, 0
#define VAL_RANGE 7
#define KEY_PIN A0

const uint16_t analogKeyValue[3][4] = { 523, 623, 773,
                                        353, 393, 443,
                                        258, 283, 313,
                                        208, 223, 240, };
const uint8_t analogKeyData[3][4] = {   '1',  '2',  '3',
                                        '4',  '5',  '6',
                                        '7',  '8',  '9',
                                        '*',  '0',  '#', };

class key
{
public:
	key();
	void updateKey();

	// All data
	char readKey();
	// atoi
	int readNum();
	// �ֱ� �Էµ� 4�� ������
	int read4Num();

	// ���� ����
	void clearBuffer();
	// ���� ������ ����
	boolean isBufferFull();
	int bufferCount();

	// Data, [Number, Char], Not \n \0..
	boolean isData();
	// Number, ['0'-'9']
	boolean isNumber();
	// char ['A' - 'z' + '/', '*'...]
	boolean isChar();
	// *, [Char]
	boolean isAsterisk();
private:
	char _keyChar;
	char _num4Buf[5]; //Last byte is '\n'
	int _num4Index; // 0-4
	boolean bufFull;
};

#endif
