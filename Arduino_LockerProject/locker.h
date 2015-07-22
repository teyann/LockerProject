#ifndef _BOX_H_
#define _BOX_H_

#include <Arduino.h>
#include "key.h"
#include "lockDevice.h"

#define _ADMIN_PASSWORD_ 0000

// IDLE, SELECE, OPEN, CLOSE, ADMIN
enum state_t{
	IDLE_STATE,
	SELECT_STATE,
	PASSWORD_STATE,
	OPEN_STATE,
	CLOSE_STATE,
	ADMIN_STATE
};
// FIND, KEEP
enum mode_t{
	KEEP_MODE,
	FIND_MODE
};

class locker
{
public:
	// ������
	locker(int maxNum);
	void begin();
	int updateLocker(key* myKey, lockDevice* myLock);
	// ���� ����
	state_t currentState();
	// ���õ� ���
	mode_t selectedMode();
	boolean hasItem(int lockerNum);
private:
	struct lockerInfo_t{
		int password;
		boolean hasItem;
	};

	// ���� ������Ʈ
	void updateState(state_t newState);

	
	// ��� ����
	void selectMode(mode_t newMode);

	void setLockerNum(int newNum);

	int password(int lockerNum);

	int findItem(int lockerNum, int password);
	int keepItem(int lockerNum, int password);

	mode_t a2Mode(char c);

	// ��� �ٲ�
	state_t _state;
	// �ʹݿ� �ԷµǴ� ���� ���� �ٲ�
	lockerInfo_t *_info = new lockerInfo_t[_lockerMaxNum];
	mode_t _mode;

	int _lockerNum;

	// �ð�����
	uint32_t closeStartTime;
	uint32_t adminStartTime;
	// �ʱⰪ
	const int _lockerMaxNum;
};

#endif
