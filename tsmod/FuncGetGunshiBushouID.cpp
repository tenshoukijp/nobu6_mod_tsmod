#include "WinTarget.h"

#include "GameDataStruct.h"

/*
�R�t�֐��̌Ăяo����
00427768  |. 50             PUSH    EAX							// �R�c�ԍ�
00427769  |. E8 7BD40200    CALL    TENSHOU.00454BE9			�� // �R�tGet�֐�
0042776E  |. 66:A3 907C4D00 MOV     WORD PTR DS:[4D7C90], AX	// �ǂ������݂̌R�c�̌R�t�ԍ���0x4D7C90�Ɋi�[�����炵���B�uEAX�v�ł͂Ȃ��uAX�v�̕��������g���B
00427774  |. 83C4 04        ADD     ESP, 4

*/


/*
 �R�c�ԍ��������Ƃ��āA�R�t�̕����ԍ��𓾂�
*/
int pCallFromFuncGetGunshiBushouID = 0x454BE9; //

int iReturnOfBushouIDGetGunshiBushou = 0xFFFF;
int iSigOfGundanIDGetGunshiBushou = 0xFFFF;

void FuncGetGunshiBushouIDAsm() {
	__asm {
		// �R�c�ԍ�������
		MOV     ESI, iSigOfGundanIDGetGunshiBushou

		// �R�c�ԍ��������Ƃ��āA�R�t�ԍ��𓾂邽�߂̊֐����Ăяo��
		PUSH    ESI
		CALL    pCallFromFuncGetGunshiBushouID
		ADD     ESP, 4

		MOV iReturnOfBushouIDGetGunshiBushou, EAX // ���ʂ̂����AAX�̔ԍ����R�t�����ԍ��Ƃ��ĕۑ�

	}
}

// �R�c�̌R�t�����𓾂�B�R�t����҂����Ȃ����0xFFFF���Ԃ��Ă���B
int getGunshiBushouID(int iGundanID) {

	if ( 0 <= iGundanID && iGundanID < GAMEDATASTRUCT_GUNDAN_NUM ) {

		iSigOfGundanIDGetGunshiBushou = iGundanID+1; // �z��p���R�c�ɕύX

		iReturnOfBushouIDGetGunshiBushou = 0xFFFF; // �܂��́A���x���̒l�Ń��Z�b�g�B

		__asm {
			push eax
			push ebx
			push ecx
			push edx
			push esp
			push esi
			push edi
		}

		// TENSHOU.EXE���̊֐��Ăяo��
		FuncGetGunshiBushouIDAsm();

		__asm {
			pop edi
			pop esi
			pop esp
			pop edx
			pop ecx
			pop ebx
			pop eax
		}

		iReturnOfBushouIDGetGunshiBushou = iReturnOfBushouIDGetGunshiBushou & 0xFFFF; // EAX�̂����AAX�̕����������p����B

		// ���̌R�c�ɌR�t�������Ȃ��
		if ( 0 <= iReturnOfBushouIDGetGunshiBushou-1 && iReturnOfBushouIDGetGunshiBushou-1 < GAMEDATASTRUCT_BUSHOU_NUM ) {
			return iReturnOfBushouIDGetGunshiBushou-1; // �����ԍ��������ԍ��z��p�ɂ��ĕԂ�
		}
	}

	return 0xFFFF;
}