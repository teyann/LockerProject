#ifndef CONFIG_H
#define CONFIG_H

//////////////////////////////
// 1/ 시리얼 설정
////////////////////

// 입출력을 시리얼로 함(시리얼 테스트 모드), 비활성화되면 I2C LCD로 출력
#define SERIAL_IO 
// 시리얼로 여러 값들을 자세희 살펴보는 모드
//#define SERIAL_DEBUG 

// 시리얼 포트에서 사용할 baudrate
#define SERIAL_BAUDRATE 115200 


//////////////////////////////
// 2/ Locker 설정
////////////////////

// 시리얼에선 바꿀수 있지만, LCD에서는 4개가 최대
#define LOCKER_SIZE 4
// 아이들상태에서 관리자 모드로 접근하기 위한 4자리 비밀번호
#define ADMIN_PASSWORD 9999 


//////////////////////////////
// 3/ Keypad 설정
////////////////////

// 각각 키패드 값과 매치되는 아날로그 값들 [0v;5v] -> [0;1023]
const uint16_t analogKeyValue[3][4] = { 523, 623, 773,
										353, 393, 443,
										258, 283, 313,
										208, 223, 240, };

// analogKeyValue-7 <= x <= analogKeyValue+7 일때의 키패드 값을 출력
#define VAL_RANGE 7


//////////////////////////////
// 4/ 입출력 핀 설정
////////////////////

// 잠금장치(솔레노이드)와 연결된 핀 설정
const int LOCK_PIN[LOCKER_SIZE] = {
	9, 8, 7, 6,
};
// 리미트 스위치와 연결된 핀 설정
const int LIMIT_SWITCH_PIN[LOCKER_SIZE] = { 
	5, 4, 3, 2,
};

// 키패드와 연결된 핀 설정
#define KEY_PIN A0 


#endif