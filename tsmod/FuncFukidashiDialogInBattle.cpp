#define _CRT_SECURE_NO_WARNINGS
#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "GameDataStruct.h"
#include "ScenarioMod.h"
#include "MessageN6PPatch.h"
#include "FuncFukidashiDialog.h"


/*
�퓬���̐^�񒆂̃_�C�A���O
00434D3A  |. 50             PUSH    EAX					�� ���b�Z�[�W�̊i�[���ꂽ�|�C���^(4D97C0���ʏ�)
00434D3B  |. 56             PUSH    ESI					�� �����ԍ�
00434D3C     E8 06590600    CALL    TENSHOU.0049A647
00434D41  |. 83C4 08        ADD     ESP, 8
*/



int pCallFromFuncSenjoFukidashiDialog = 0x49A647;
char pszMessageOfSenjoFukidashiMessage[512] = "";
int iAddressMessageOfSenjoFukidashiMessage = (int)0x4D97C0;
int iBushouIDOfSenjoFukidasiDialog = -1;
void FuncSenjoFukidashiDialogAsm() {
	__asm {
		mov eax, iAddressMessageOfSenjoFukidashiMessage
		mov esi, iBushouIDOfSenjoFukidasiDialog
		push eax
		push esi
		call pCallFromFuncSenjoFukidashiDialog
		add esp, 8
	}
}








BOOL isOnSenjoFukidashiDialog = FALSE;
void FuncSenjoFukidashiDialog(int iBushouID, char *szMessage) {
	iBushouIDOfSenjoFukidasiDialog = iBushouID+1;
	strcpy( (char *)iAddressMessageOfSenjoFukidashiMessage, szMessage);

	MessageN6PPatch::isOnTenshouExeMessageCopyUsed = false;
	nb6fukidashirelatepersons->i1stPersonID = iBushouID;

	isOnSenjoFukidashiDialog = TRUE;

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
	FuncSenjoFukidashiDialogAsm();

	__asm {
		pop edi
		pop esi
		pop esp
		pop edx
		pop ecx
		pop ebx
		pop eax
	}

	isOnSenjoFukidashiDialog = FALSE;
}






// �O���Q�Ɨp
void WINAPI Extern_FuncSenjoFukidashiDialog(int iBushouID, char *szMessage) {
	FuncSenjoFukidashiDialog(iBushouID, szMessage);
}
