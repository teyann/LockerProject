#ifndef LOCKER_H
#define LOCKER_H

#include <Arduino.h>
#include "key.h"
#include "lockDevice.h"
#include "config.h"

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

class LockerClass
{
public:
	LockerClass(int maxNum);	// 생성자
	~LockerClass();				// 소멸자

	void	begin(KeypadClass* const myKeypad, LockDeviceClass* const myLock); // 초기설정
	int		updateLocker();					// 상태 업데이트

	state_t	getCurrentState();				// 현재 상태
	mode_t	getSelectMode();				// 선택한 모드
	boolean	getHasItem(int lockerNum);		// 보관중인 물품 여부
	int		getPassword(int lockerNum);		// 비밀번호

private:
	struct lockerInfo_t{
		uint16_t password;
		boolean hasItem;
	};

	int		keepItem(int lockerNum, int password);		// 물품 보관
	int		findItem(int lockerNum, int password);		// 물품 찾기

	void	setCurrentState(state_t newState);			// 현재 상태 설정
	void	setSelectMode(mode_t newMode);				// 모드 설정
	void	setHasItem(int lockerNum, boolean state);	// 보관중인 물품 여부 설정
	void	setPassword(int lockerNum, int password);	// 비밀번호 설정

	// 타이머 설정
	void setCloseStartTime() { _closeStartTime = millis(); }
	void setAdminStartTime() { _adminStartTime = millis(); }
	void setErrorStartTime() { _errorStartTime = millis(); }
	// 현재 시간과 타이머 시간 간격
	uint32_t getDeltaCloseTime() { return millis() - _closeStartTime; }
	uint32_t getDeltaAdminTime() { return millis() - _adminStartTime; }
	uint32_t getDeltaErrorTime() { return millis() - _errorStartTime; }

	// 에러 설정 [0:정상, 1:범위초과, 2:사용불가, 3:비밀번호오류]
	void setErrorCode(int errCode) { _errCode = errCode; setErrorStartTime(); }
	int getErrorCode() { return _errCode; }

	KeypadClass*			_myKeypad;		// 키패드 클래스 포인터
	LockDeviceClass*		_myLock;		// 잠금장치 클래스 포인터
	const int				_lockerMaxNum;	// 락커 사이즈
	lockerInfo_t *			_lockerInfo;	// 락커 정보(비밀번호, 보관여부)

	state_t					_currentState;	// 현재 상태 [IDLE, SELECE, OPEN, CLOSE, ADMIN]
	mode_t					_mode;			// 현재 선택된 모드 [FIND, KEEP]

	int						_errCode;		// 현재 에러값 [0:정상, 1:범위초과, 2:사용불가, 3:비밀번호오류]

	// 타이머
	uint32_t				_closeStartTime; // 보관함이 닫힌 시간
	uint32_t				_adminStartTime; // 관리자 모드로 들어간 시간
	uint32_t				_errorStartTime; // 에러가 발생한 시간
};

#endif
