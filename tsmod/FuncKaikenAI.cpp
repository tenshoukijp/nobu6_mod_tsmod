#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"


//---------------------��̂`�h�𓾂�-------------------------
int pCallFromFuncGetKaikenAIAsm = 0x47028B; // ���XTENSHOU.EXE���ɂ�����CALL��

int iParamGetKaikenAIBushouID = -1;
int iReturnGetKaikenAIID = -1;

void FuncGetKaikenAIAsm() {

	__asm {
		// ����1�`3 2�o�C�g����
		push iParamGetKaikenAIBushouID;
		// ����4 ������ւ̃|�C���^(�|�C���^=4�o�C�g)
		call pCallFromFuncGetKaikenAIAsm

		mov iReturnGetKaikenAIID, EAX // EAX���Ώە�����AI
		// �X�^�b�N�����ꍞ�񂾕�
		add  esp, 4
	}

}

// �Ώە����̉�ӌ��`�h�𓾂�B
int getKaikenIkouAI(int iBushouID) {
	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
		iParamGetKaikenAIBushouID = iBushouID + 1;

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
		FuncGetKaikenAIAsm();

		__asm {
			pop edi
			pop esi
			pop esp
			pop edx
			pop ecx
			pop ebx
			pop eax
		}

		return iReturnGetKaikenAIID;
	} else {
		return 0; // �������C���M�����[�Ȃ�A�Êϑ�����Ԃ��Ă����B
	}
}

int WINAPI Extern_getKaikenIkouAI(int iBushouID) {
	return getKaikenIkouAI(iBushouID);
}

// �Ώە����̉�ӌ��`�h��ݒ肷��B
extern int iAddressOfGetPointerKaikenHeapMemoryExecute;
void setKaikenIkouAI(int iBushouID, int iAI) {
	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
		getKaikenIkouAI(iBushouID); // ��x����s���ăq�[�v�������̈ʒu(iAddressOfGetPointerKaikenHeapMemoryExecute)���m�肳����B

		NB6KAIKENIKOU *nb6kaiken = (NB6KAIKENIKOU *)iAddressOfGetPointerKaikenHeapMemoryExecute;
		nb6kaiken->ai = iAI;
	}
}

void WINAPI Extern_setKaikenIkouAI(int iBushouID, int iAI) {
	setKaikenIkouAI(iBushouID, iAI);
}