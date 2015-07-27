#include "locker.h"

LockerClass::LockerClass(int maxNum) 
: _lockerMaxNum(maxNum) {
	_lockerInfo = new lockerInfo_t[_lockerMaxNum];
}

LockerClass::~LockerClass() {
	delete[] _lockerInfo;
	_lockerInfo = NULL;
}

void LockerClass::begin(KeypadClass* const myKeypad, LockDeviceClass* const myLock) {
	for (int i = 0; i < _lockerMaxNum; i++) {
		_lockerInfo[i].hasItem = false;
		_lockerInfo[i].password = 0;
	}
	_myKeypad = myKeypad;
	_myLock = myLock;
}

state_t LockerClass::getCurrentState() {
	return _currentState;
}

void LockerClass::setCurrentState(state_t newState) {
	_currentState = newState;
	if (newState == IDLE_STATE)
		// 아이들상태으로 바뀌기 전에 ADMIN_PASSWORD에 
		// 있는 숫자가 입력되었으면 오류가 발생하므로 버퍼를 비움
		_myKeypad->clearBuffer(); 
	else if (newState == CLOSE_STATE)
		setCloseStartTime();
	else if (newState == ADMIN_STATE)
		setAdminStartTime();

}

mode_t LockerClass::getSelectMode(){
	return _mode;
}
void LockerClass::setSelectMode(mode_t newMode) {
	_mode = newMode;
}

/* 
 * @param lockerNum [1;4]
 */
boolean LockerClass::getHasItem(int lockerNum) {
	return _lockerInfo[lockerNum-1].hasItem;
}
/*
 * @param lockerNum [1;4]
 */
void LockerClass::setHasItem(int lockerNum, boolean state) {
	_lockerInfo[lockerNum-1].hasItem = state;
}

/*
 * @param lockerNum [1;4]
 */
int LockerClass::getPassword(int lockerNum) {
	return _lockerInfo[lockerNum-1].password;
}
/*
 * @param lockerNum [1;4]
 */
void LockerClass::setPassword(int lockerNum, int password) {
	_lockerInfo[lockerNum - 1].password = password;
}

/*
 * @param lockerNum [1;4] pw[0;9999]
 * @retval 1: 범위초과, 2: 보관되어있지않음, 3: 비밀번호틀림
 */
int LockerClass::findItem(int lockerNum, int pw) {
	if (!(lockerNum > 0 && lockerNum <= _lockerMaxNum))
		return 1;
	if (getHasItem(lockerNum) != true)
		return 2;
#ifdef SERIAL_DEBUG
	Serial.print("FIND | N "); Serial.print(lockerNum);
	Serial.print(" | PW "); Serial.print(getPassword(lockerNum));
	Serial.print(" | InPW "); Serial.println(pw);
#endif
	if (getPassword(lockerNum) != pw)
		return 3;
	setHasItem(lockerNum, false);
	setPassword(lockerNum, 0);
	return 0;
}

/*
 * @param lockerNum [1;4] pw[0;9999]
 * @retval 1: 범위초과, 2: 보관되어있음
 */
int LockerClass::keepItem(int lockerNum, int pw) {
	if (!(lockerNum > 0 && lockerNum <= _lockerMaxNum))
		return 1;
	if (getHasItem(lockerNum) != false)
		return 2;
#ifdef SERIAL_DEBUG
	Serial.print("KEEP | N "); Serial.print(lockerNum);
	Serial.print(" | PW "); Serial.println(pw);
#endif
	setHasItem(lockerNum, true);
	setPassword(lockerNum, pw);
	return 0;
}

int LockerClass::updateLocker() {
	static int lockerNum = -1;

	// 입력된 키패드 데이터
	char key = _myKeypad->getData();

	// 에러 상태일때
	int errCode = getErrorCode();
	if (errCode != 0) {
		if (getDeltaErrorTime() <= 2000)
			return errCode;
		else
			setErrorCode(0);
	}

	// OPEN모드가 아닐때, *가 입력된경우
	if (_myKeypad->isAsterisk() != false) { 
		if (getCurrentState() != OPEN_STATE)
			setCurrentState(IDLE_STATE);
	}

	switch (getCurrentState()) {
	case IDLE_STATE:
		if (_myKeypad->isBufferFull() != false) {
			if (_myKeypad->get4DigitNum() == ADMIN_PASSWORD) {
				setCurrentState(ADMIN_STATE);
			}
			_myKeypad->clearBuffer();
		}
		if (key == '1' || key == '2') {
			mode_t inMode;
			if (key == '1')
				inMode =  KEEP_MODE;
			else
				inMode =  FIND_MODE;
			setSelectMode(inMode);
			setCurrentState(SELECT_STATE);
		}
		break;
	case SELECT_STATE:
		if (_myKeypad->isNumber() != false) {
			lockerNum = _myKeypad->getNum(); // [0;9]
			if (!(lockerNum > 0 && lockerNum <= _lockerMaxNum)) { // [1;4]
				setErrorCode(1);
				return 1;
			}
			if (getSelectMode() == FIND_MODE) {
				if (getHasItem(lockerNum) != true) {
					setErrorCode(2);
					return 2;
				}
				else {
                    _myKeypad->clearBuffer();
					setCurrentState(PASSWORD_STATE);
				}
			}
			else if (getSelectMode() == KEEP_MODE) {
				if (getHasItem(lockerNum) != false) {
					setErrorCode(2);
					return 2;
				}
				else {
                    _myKeypad->clearBuffer();
					setCurrentState(PASSWORD_STATE);
				}
			}
        }
        break;

	case PASSWORD_STATE:
			if (_myKeypad->isBufferFull() != false) {
				int pw = _myKeypad->get4DigitNum();
				if (getSelectMode() == KEEP_MODE) {
					int temp = keepItem(lockerNum, pw);
					switch (temp) {
					case 0:
						setCurrentState(OPEN_STATE);
						break;
					case 1:
						setErrorCode(1);
						setCurrentState(IDLE_STATE);
						return 1;
						break;
					// 일반적인 상황에선 case 2가 발생하지 않음
					case 2:
						setErrorCode(2);
						return 2;
						break;
					}
				}
				else if (getSelectMode() == FIND_MODE) {
					int temp = findItem(lockerNum, pw);
					switch (temp) {
					case 0:
						setCurrentState(OPEN_STATE);
						break;
					case 1:
						setErrorCode(1);
						setCurrentState(IDLE_STATE);
						return 1;
						break;
					// 일반적인 상황에선 case 2가 발생하지 않음
					case 2:
						setErrorCode(2);
						return 2;
						break;
					case 3:
						setErrorCode(3);
						return 3;
						break;
					}
				}
				_myKeypad->clearBuffer();
			}
		  break;
	case OPEN_STATE:
		_myLock->lockOff(lockerNum);
		if (_myLock->isLimSwOn(lockerNum) != false) {
			if (key == '#') {
				setCurrentState(CLOSE_STATE);
			}
//			else if ()  // TODO: 리미트스위치가 일정시간 이상 눌려있는경우(일정시간이상 닫혀있는경우) 구현
		}
		break;
	case CLOSE_STATE:
		_myLock->lockOn(lockerNum);
		if (getDeltaCloseTime() >= 3000) {
			lockerNum = -1;
			setCurrentState(IDLE_STATE);
		}
		break;
	case ADMIN_STATE:
		// TODO: 앞에 0 채울 방법 찾아보기
		if (getDeltaAdminTime() >= 3000) {
			setCurrentState(IDLE_STATE);
		}
		break;
	}
	setErrorCode(0);
	return 0;
}
