#include "WinTarget.h"

#include "GameDataStruct.h"

/*
00457D90  |. 83C4 04        ADD     ESP, 4
00457D93     E8 56B5FCFF    CALL    TENSHOU.004232EE                    ���t�F�C�h�A�E�g

00457D98     6A 01          PUSH    1
00457D9A     B9 18C24D00    MOV     ECX, TENSHOU.004DC218
00457D9F  |. 6A 01          PUSH    1                                ; |Arg2 = 00000001
00457DA1  |. 56             PUSH    ESI                              ; |Arg1
00457DA2  |. E8 E8F40200    CALL    TENSHOU.0048728F                 ; \TENSHOU.0048728F�@�@�@�J�����ړ�

00457DA7     E8 28B5FCFF    CALL    TENSHOU.004232D4�@�@�@�@�@�@�@�@�@�@���t�F�C�h�C��
*/

int pCallFromFuncMoveCamera = 0x48728F; // ���XTENSHOU.EXE���ɂ�����CALL��

WORD iCastleIdInFuncMoveCamera = 1L;

// ���C����ʂł̃J�����ړ�
void FuncMoveCameraAsm() {
	__asm {
		push ecx  // ecx �̒l��ۑ�

		// ����1�`3 2�o�C�g����
		mov ecx, 0x4DC218  // (���g��2710=10000���B�Ȃɂ��K�v�炵��);
		push 1
		push 1
		push dword ptr iCastleIdInFuncMoveCamera;
		call pCallFromFuncMoveCamera

		pop ecx  // ecx�̒l�����ւƖ߂�
	}
}


// �J�������w��̏�ID�ւƈړ�
void FuncMoveCamera(int iCastleID) {

	if ( 0 <= iCastleID && iCastleID < GAMEDATASTRUCT_CASTLE_NUM ) {
		// �֐��p�̂�́A1�����̂ŁA��������
		iCastleIdInFuncMoveCamera = (WORD)iCastleID+1;
	
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
		FuncMoveCameraAsm();

		__asm {
			pop edi
			pop esi
			pop esp
			pop edx
			pop ecx
			pop ebx
			pop eax
		}

	}
}
