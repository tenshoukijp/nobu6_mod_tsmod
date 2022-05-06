#define _CRT_SECURE_NO_WARNINGS

#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"

CurProcessMemory::CurProcessMemory() {
	m_LAdress[LOC1] = (0x4D9B78); // (���b�Z�[�W��)��̂��颕������������₷��
	m_LAdress[LOC2] = (0x4D9AF8); // �E�B���h�E���b�Z�[�W������₷��
	m_LAdress[LOC3] = (0x4D91B4);
	m_LAdress[LOC4] = (0x4D97C0); // �������`��⢉ƕ󖼣�⢉ƕ�ڍף�Ȃǂ�����ꏊ�B����ɏo��E�B���h�E�g�͂�����̏ꍇ������B
	m_LAdress[LOC5] = (0x4D9A40);
	m_LAdress[LOC6] = (0x4D97B7); // ���l�Ƃ̎�����ȂǁA�Q�l��b�ŗ��p�����B
};

void CurProcessMemory::setVariableFromMemory(void) {
	// ��̓I�ȃA�h���X���Z�b�g����Ă�͈͂Ń�������ǂ�
	for ( int iLOCIX=0; iLOCIX < LOCEND ; iLOCIX++ ) {
		// ����������ǂݍ���
		ReadProcessMemory( 
			CommonGlobalHandle::curProcess,
			(LPCVOID)(m_LAdress[iLOCIX]), 
			szCurMemoryData[iLOCIX], 
			sizeof(szCurMemoryData[iLOCIX]),
			NULL);

		// �P�O�̕����ƍ��̕������r���邱�Ƃ�����̂ŁA�L���ȕω������鎞�A�X�V
		if (strlen(szCurMemoryData[iLOCIX]) > 1 && strcmp(szPreMemoryData[iLOCIX], szCurMemoryData[iLOCIX])) {
			strcpy(szPreMemoryData[iLOCIX], szCurMemoryData[iLOCIX]);
		}
	}
}

// �Ώۂ̃������|�C���^�̌��݂̒l�𓾂�
char* CurProcessMemory::getCurVarPtr( DataLocation eLOC ) {
	return szCurMemoryData[eLOC];
}

// �Ώۂ̃������|�C���^�̂P�O�̒l�𓾂�
char* CurProcessMemory::getPreVarPtr( DataLocation eLOC ) {
	return szPreMemoryData[eLOC];
}

// �Ώۂ̃������|�C���^�̃A�h���X�𓾂�
int CurProcessMemory::getMemoryAddress( DataLocation eLOC ) {
	return m_LAdress[eLOC];
}