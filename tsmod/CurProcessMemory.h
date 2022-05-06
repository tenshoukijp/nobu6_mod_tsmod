#pragma once

#include "OutputDebugString.h"
#include "OutputDebugStream.h"

enum DataLocation { LOC1=0, LOC2=1, LOC3=2, LOC4=3, LOC5=4, LOC6=5, LOCEND }; // ������g���ăA�N�Z�X���邱��

class CurProcessMemory {
	public:
		CurProcessMemory();

		int m_LAdress[LOCEND]; // ��̓I��(const�ȈӖ���)�Ӗ������������A�h���X���i�[����ꏊ

		char szCurMemoryData[LOCEND][512]; // �e�������A�h���X��DataLocation�ɑΉ����镶������i�[����ꏊ
		char szPreMemoryData[LOCEND][512]; // �P�O�̊e�������A�h���X��DataLocation�ɑΉ����镶������i�[����ꏊ(�L�Ӌ`�ȕω����������ꍇ�̂�Cur��Pre�ւ̊i�[�������)

		void setVariableFromMemory(void);  // ������Ăяo�����ƂŃ������˃N���X�ւƒl���i�[����

		char* getCurVarPtr( DataLocation eLOC ); // ���݂�LOC[1�`?]�̒l�𓾂�
		char* getPreVarPtr( DataLocation eLOC ); // �P�O��LOC[1�`?]�̒l�𓾂�

		int getMemoryAddress( DataLocation eLOC );
};

extern CurProcessMemory CPM;
