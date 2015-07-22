#include "locker.h"

locker::locker(int maxNum)
: _lockerMaxNum(maxNum) {
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
	if (!(lockerNum >= 0 && lockerNum < _lockerMaxNum))
		return false;
	return _info[lockerNum].hasItem;
}
int locker::password(int lockerNum) {
	if (!(lockerNum >= 0 && lockerNum < _lockerMaxNum))
		return false;
	return _info[lockerNum].password;
}

int locker::findItem(int lockerNum, int pw) {
	if (!(lockerNum >= 0 && lockerNum < _lockerMaxNum))
		return 1;
	if (_info[lockerNum].hasItem != true)
		return 2;
	if (password(lockerNum) != pw)
		return 3;
	_info[lockerNum].hasItem = false;
	_info[lockerNum].password = 0;
}
int locker::keepItem(int lockerNum, int pw) {
	if (!(lockerNum >= 0 && lockerNum < _lockerMaxNum))
		return 1;
	if (_info[lockerNum].hasItem != false)
		return 2;
	_info[lockerNum].hasItem = true;
	_info[lockerNum].password = pw;
}

mode_t locker::a2Mode(char c) {
	if (c == '1')
		return FIND_MODE;
	else
		return KEEP_MODE;
}

int locker::updateLocker(key* myKey, lockDevice* myLock) {
	char key = myKey->readKey();
	int lockerNum;

	// OPEN���°� �ƴҶ� '*'�� �ԷµǸ� �ʱ���·� ���ư�
	if (myKey->isAsterisk() != false) { 
		if (this->currentState() != OPEN_STATE)
			this->updateState(IDLE_STATE);
	}

	switch (this->currentState()) {
	case IDLE_STATE:
		// ���۰� �������� ������ ��ȣ�� ��ġ�ϴ��� Ȯ��
		if (myKey->isBufferFull() != false) {
			if (myKey->read4Num() == _ADMIN_PASSWORD_)
				this->updateState(ADMIN_STATE);
		}
		// �Էµ� ���� '1','2'�϶� ��带 ����
		if (key == '1' || key == '2') {
			this->selectMode(this->a2Mode(key));
			this->updateState(SELECT_STATE);
		}
		break;
	case SELECT_STATE:
		if (myKey->isNumber() != false) { // �����϶���
			boolean selected = false;
			lockerNum = key;
			// ã�� ����϶� �����Ǿ� ������ ���ÿϷ�
			if (this->selectedMode() == FIND_MODE) {
				if (this->hasItem(lockerNum) != true) {
					// ERR_01 ������ ��ȣ�� �������� ���� �ʴ°��
					return 1;
				}
				else {
					selected = true;
					myKey->clearBuffer();
				}
			}
			// ���� ����ϋ� ��������� ���ÿϷ�
			else if (this->selectedMode() == KEEP_MODE) {
				if (this->hasItem(lockerNum) != false) {
					// ERR_01 ������ ��ȣ�� �������� ���� �ʴ°��
					return 1;
				}
				else {
					selected = true;
					myKey->clearBuffer();
				}
			}
			if (selected != false)
				this->updateState(PASSWORD_STATE);
			break;
        }

	case PASSWORD_STATE:
{
			boolean matched = false;
			// �������� ���õǾ�����, ���۰� �������� �о ��й�ȣ ��
			if (myKey->isBufferFull() != false) {
				int pw = myKey->read4Num(); // ��й�ȣ �б�
				if (this->selectedMode() == FIND_MODE) {
					int temp = this->findItem(lockerNum, pw);
					switch (temp) {
					case 0:
						matched = true;
						break;
//					case 1:
//						// ERR_01 ������ ��ȣ�� �������� ���� �ʴ°��
//						break;
					case 2:
						// ERR_02 ������ ��ǰ�� ���°��  -> �ٽ� ����
						return 2;
						break;
					case 3:
						// ERR_03 ��й�ȣ�� ��ġ���� �ʴ°�� -> ��й�ȣ �ٽ� �Է�
						return 3;
						break;
					}
				}
				else if (this->selectedMode() == KEEP_MODE) {
					int temp = this->keepItem(lockerNum, pw);
					switch (temp) {
					case 0:
						matched = true;
						break;
//					case 1:
//						// ERR_01 ������ ��ȣ�� �������� ���� �ʴ°��
//						break;
					case 2:
						// ERR_02 ������ ��ǰ�� �ִ°��  -> �ٽ� ����
						return 2;
						break;
					}
				}
			}
			if (matched != false)
				this->updateState(OPEN_STATE);
}
		  break;
	case OPEN_STATE:
		// ������ ���� ���
		myLock->lockOff(lockerNum);
		// �������·� ��ư�� ������
		if (myLock->isLimSwOn(lockerNum) != false) {
			if (key == ' #') {
				this->updateState(CLOSE_STATE);
			}
//			else if () �ð� // ���ͷ�Ʈ�� �̿��ϴ°� Ȯ���Ұ����� ����
		}
		break;
	case CLOSE_STATE:
		// ������ ���� ���
		myLock->lockOn(lockerNum);
//		// ���� �ð��� ���ư�
		if ((millis() - closeStartTime) >= 3000) {
			this->updateState(IDLE_STATE);
		}
		break;
	case ADMIN_STATE:
		// ������ ���
		// 5�ʰ� ��й�ȣ ��� ���
		if ((millis() - adminStartTime) >= 3000) {
			this->updateState(IDLE_STATE);
		}
		break;
	}
}
