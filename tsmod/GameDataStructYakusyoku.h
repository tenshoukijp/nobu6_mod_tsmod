#pragma once


namespace ��E {
	enum { ���� = 0, ���Α叫�R = 1, �Ǘ� = 2, �֓��Ǘ� = 3, ���B�T�� = 4, �����T�� = 5, ��B�T�� = 6, �H�B�T�� = 7 };
}

// ��̑S�����ł͂�������Ԃ���P�P�����o�����߂ɕK�v�ƂȂ鑊�΃A�h���X�錾 
int GAMEDATASTRUCT_POSITIONTYPELIST_POINTER[];

// ��E�^�C�v��ID���當����𓾂�
char* getPositionTypeName(byte type);


// ���K�����ꂽ��E�ԍ�����喼�p�̖�E�ԍ��𓾂�B
int DeNormalizeDaimyoYakusyokuValue(int iYakusyokuValue);

// �喼ID���琳�K�����ꂽ��EID�𓾂�
int getNormalizedDaimyoPosition(int iDaimyoID);

// �R�cID���琳�K�����ꂽ��EID�𓾂�
int getNormalizedGundanPosition(int iGundanID);

// �喼�̖�E�̕t���ւ������݂�
bool tryChangeDaimyoYakusyoku(int iDaimyoID, int iYakusyokuValue);

// �R�c�̖�E�̕t���ւ������݂�
bool tryChangeGundanYakusyoku(int iGundanID, int iYakusyokuValue);

