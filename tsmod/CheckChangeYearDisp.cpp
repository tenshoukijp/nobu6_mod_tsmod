#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"


bool CheckChangeYearDisplay() {

	// �X�^�[�g�N�������ς���Ă���Ȃ�΁A�g���ݒ�̢�����\�L��͢����\�L��ɋ����I�ɐ؂�ւ���
	if ( getStartYearAD() != 1454 ) {
		// �����\���ł͂Ȃ��A����\��
		nb6settingsex.yeardisp = 0;

		return true;
	}
	return false;
}