#ifndef KEYPAD_H
#define KEYPAD_H

#include <Arduino.h>
#include "config.h"

#define NO_CHAR '\0' // NULL, 0

class KeypadClass
{
public:
	KeypadClass(); // 생성자
	void	updateKeypad();		// 키패드 업데이트

	char	getData();			// 키패드 값 (아스키)
	int		getNum();			// 키패드 값 (숫자, 아스키-48)

	int		get4DigitNum();		// 키패드 버퍼에 저장된 4개의 숫자를 4자리 숫자로 출력
	int		getBufferCount();	// 키패드 버퍼 카운터 [0;3]
	void	clearBuffer();		// 키패드 버퍼 비우기
	boolean isBufferFull();		// 키패드 버퍼 비었는지 여부

	boolean isData();			// 키패드 값이 NO_CHAR이 아닌지 여부
	boolean isNumber();			// 키패드 값이 '0' - '9' 인지 여부
	boolean isChar();			// 키패드 값이 'A' - 'z' + '/', '*'... 인지 여부
	boolean isAsterisk();		// 키패드 값이 '*'인지 여부

private:
	char		_data;					// 현재 입력된 키패드 값

	char		_buf[5];				// 키패드 버퍼, 마지막 바이트는 '\0'
	int			_bufIndex;				// 키패드 버퍼 카운터 [0;3]
	boolean		bufFull;				// 키패드 버퍼가 꽉찼을때(버퍼 카운터가 3일때)

	static char analogKeyData[3][4];	// 아날로그 데이터를 키패드 값으로 바꾸기위한 맵핑값
};

#endif