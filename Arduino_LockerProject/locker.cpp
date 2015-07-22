#include "locker.h"

locker::locker(int maxNum)
: _lockerMaxNum(maxNum) {
	_lockerNum = -1;
}

void locker::begin() {
	for (int i = 0; i < _lockerMaxNum; i++) {
		_info[i].hasItem = false;
		_info[i].password = 0;
	}
}

// ���� ����
state_t locker::currentState() {
	return _state;
}

// ���� ������Ʈ
void locker::updateState(state_t newState) {
	_state = newState;
	if (newState == CLOSE_STATE)
		closeStartTime = millis();
	else if (newState == ADMIN_STATE)
		adminStartTime = millis();

}

// ���õ� ���
mode_t locker::selectedMode(){
	return _mode;
}
// ��� ����
void locker::selectMode(mode_t newMode) {
	_mode = newMode;
}

boolean locker::hasItem(int lockerNum) {
	lockerNum--;
	if (!(lockerNum >= 0 && lockerNum < _lockerMaxNum))
		return false;
	return _info[lockerNum].hasItem;
}
int locker::password(int lockerNum) {
	lockerNum--;
	if (!(lockerNum >= 0 && lockerNum < _lockerMaxNum))
		return false;
	return _info[lockerNum].password;
}

int locker::findItem(int lockerNum, int pw) {
	lockerNum--;
	if (!(lockerNum >= 0 && lockerNum < _lockerMaxNum))
		return 1;
	if (_info[lockerNum].hasItem != true)
		return 2;

	Serial.print("FIND | N "); Serial.print(lockerNum + 1);
	Serial.print(" | PW "); Serial.print(password(lockerNum));
	Serial.print(" | InPW "); Serial.println(pw);

	if (password(lockerNum) != pw)
		return 3;
	_info[lockerNum].hasItem = false;
	_info[lockerNum].password = 0;
	return 0;
}
int locker::keepItem(int lockerNum, int pw) {
	lockerNum--;
	if (!(lockerNum >= 0 && lockerNum < _lockerMaxNum))
		return 1;
	if (_info[lockerNum].hasItem != false)
		return 2;

	Serial.print("KEEP | N "); Serial.print(lockerNum + 1);
	Serial.print(" | PW "); Serial.println(pw);

	_info[lockerNum].hasItem = true;
	_info[lockerNum].password = pw;
	return 0;
}

mode_t locker::a2Mode(char c) {
	if (c == '1')
		return KEEP_MODE;
	else
		return FIND_MODE;
}

int locker::updateLocker(key* myKey, lockDevice* myLock) {
	char key = myKey->readKey();

	if (key != NO_CHAR) {
		Serial.print("In Char = ");
		Serial.println(key);
		Serial.println();
	}

	// OPEN���°� �ƴҶ� '*'�� �ԷµǸ� �ʱ���·� ���ư�
	if (myKey->isAsterisk() != false) { 
		if (currentState() != OPEN_STATE)
			updateState(IDLE_STATE);
	}

	switch (currentState()) {
	case IDLE_STATE:
      		// ���۰� �������� ������ ��ȣ�� ��ġ�ϴ��� Ȯ��
		if (myKey->isBufferFull() != false) {
			if (myKey->read4Num() == _ADMIN_PASSWORD_)
				updateState(ADMIN_STATE);
			myKey->clearBuffer();
		}
		// �Էµ� ���� '1','2'�϶� ��带 ����
		if (key == '1' || key == '2') {
			selectMode(a2Mode(key));
			updateState(SELECT_STATE);
		}
		break;
	case SELECT_STATE:
		if (myKey->isNumber() != false) { // �����϶���
			_lockerNum = key - 48;
			if (!(_lockerNum >= 0 && _lockerNum < _lockerMaxNum)) {
				return 1;
			}
			// ã�� ����϶� �����Ǿ� ������ ���ÿϷ�
			if (selectedMode() == FIND_MODE) {
				if (hasItem(_lockerNum) != true) {
					// ERR_02
					return 2;
				}
				else {
                    myKey->clearBuffer();
					updateState(PASSWORD_STATE);
				}
			}
			// ���� ����ϋ� ��������� ���ÿϷ�
			else if (selectedMode() == KEEP_MODE) {
				if (hasItem(_lockerNum) != false) {
					// ERR_02
					return 2;
				}
				else {
                    myKey->clearBuffer();
					updateState(PASSWORD_STATE);
				}
			}
        }
        break;

	case PASSWORD_STATE:
			// �������� ���õǾ�����, ���۰� �������� �о ��й�ȣ ��
			if (myKey->isBufferFull() != false) {
				int pw = myKey->read4Num(); // ��й�ȣ �б�
				if (selectedMode() == KEEP_MODE) {
					int temp = keepItem(_lockerNum, pw);
					switch (temp) {
					case 0:
						updateState(OPEN_STATE);
						break;
//					case 1:
//						// ERR_01 ������ ��ȣ�� �������� ���� �ʴ°��
//						return 1;
//						break;
//					case 2:
//						// ERR_02 ������ ��ǰ�� �ִ°��  -> �ٽ� ����
//						return 2;
//						break;
					}
				}
				else if (selectedMode() == FIND_MODE) {
					int temp = findItem(_lockerNum, pw);
					switch (temp) {
					case 0:
						updateState(OPEN_STATE);
						break;
//					case 1:
//						// ERR_01 ������ ��ȣ�� �������� ���� �ʴ°��
//						return 1;
//						break;
//					case 2:
//						// ERR_02 ������ ��ǰ�� ���°��  -> �ٽ� ����
//						return 2;
//						break;
					case 3:
						// ERR_03 ��й�ȣ�� ��ġ���� �ʴ°�� -> ��й�ȣ �ٽ� �Է�
						return 3;
						break;
					}
				}
				myKey->clearBuffer();
			}
		  break;
	case OPEN_STATE:
		// ������ ���� ���
		myLock->lockOff(_lockerNum);
		// �������·� ��ư�� ������
		if (myLock->isLimSwOn(_lockerNum) != false) {
			if (key == '#') {
				updateState(CLOSE_STATE);
			}
//			else if () �ð� // ���ͷ�Ʈ�� �̿��ϴ°� Ȯ���Ұ����� ����
		}
		break;
	case CLOSE_STATE:
		// ������ ���� ���
		myLock->lockOn(_lockerNum);
//		// ���� �ð��� ���ư�
		if ((millis() - closeStartTime) >= 3000) {
			_lockerNum = -1;
			updateState(IDLE_STATE);
		}
		break;
	case ADMIN_STATE:
		// ������ ���
		// 5�ʰ� ��й�ȣ ��� ���
		// 앞에 0 채울 방법 찾아보기
		if ((millis() - adminStartTime) >= 3000) {
			updateState(IDLE_STATE);
		}
		break;
	}
	return 0;
}
