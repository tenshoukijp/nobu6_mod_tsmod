#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "FuncSelectMenuDialog.h"
#include "GameDataStruct.h"
#include "OutputDebugStream.h"


/*
�퓬��ʁA���j�b�g�̍ĕ`��B�������������S
*


/*
00433F8C  |. 56             PUSH    ESI
00433F8D  |. E8 FE6E0600    CALL    TENSHOU.0049AE90
00433F92  |. 83C4 04        ADD     ESP, 4
*/

int pCallFromFuncBattleUnitNumRepaint = 0x49AE90; //
int iBushouIDOfBattleUnitNumRepaint = -1;

void FuncBattleUnitNumRepaintAsm() {
	__asm {
		PUSH    iBushouIDOfBattleUnitNumRepaint
		CALL    pCallFromFuncBattleUnitNumRepaint
		ADD     ESP, 4
	}
}

// �w��̕����̐��l���ĕ`�悷��B(���Ȃ��@�\����)
void FuncBattleUnitNumRepaint(int iBushouID) {
	if ( !(0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM) ) {
		return;
	}

	iBushouIDOfBattleUnitNumRepaint = iBushouID+1;

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
	FuncBattleUnitNumRepaintAsm();

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




// �͈͍U���̕���
/*
00432BA0  |. 6A 01          |PUSH    1                               ; /Arg5 = 00000001		�� �P�Œ�
00432BA2  |. 8B45 FC        |MOV     EAX, DWORD PTR SS:[EBP-4]       ; |
00432BA5  |. 48             |DEC     EAX                             ; |
00432BA6  |. 8D4D C6        |LEA     ECX, DWORD PTR SS:[EBP-3A]      ; |
00432BA9  |. 50             |PUSH    EAX                             ; |Arg4				���@�P�Œ�
00432BAA  |. 51             |PUSH    ECX                             ; |Arg3				���@�����̕����̃��X�g�������Ă���z��(�����ԍ�[+1�̕�]�j�̃��X�g�B���X�g�̍Ō��0xFFFF�ł��邱�ƁB
00432BAB  |. FF75 E8        |PUSH    DWORD PTR SS:[EBP-18]           ; |Arg2				���@�ڕW�̒��e�_�̂x���W
00432BAE  |. FF75 E4        |PUSH    DWORD PTR SS:[EBP-1C]           ; |Arg1				���@�ڕW�̒��e�_�̂w���W
00432BB1  |. E8 F4800600    |CALL    TENSHOU.0049ACAA                ; \TENSHOU.0049ACAA
00432BB6  |. 83C4 14        |ADD     ESP, 14



0049ACAA  /$ 55             PUSH    EBP
�`
0049AE8F  \. C3             RETN
*/


int pCallFromFuncBattlePosAroundUnitNumRepaint = 0x49ACAA; //
int iPoxXOfBattlePosAroundUnitNumRepaint = -1;
int iPoxYOfBattlePosAroundUnitNumRepaint = -1;

WORD arrayOfBushouIDOfBattlePosAroundUnitNumRepaint[16] = {0}; // �ő�ł��U�l�̂͂������ǈꉞ�O�̂��߂ɁA�P�U�l���̔z��B

void FuncBattlePosAroundUnitNumRepaintAsm() {
	__asm {
		PUSH    1
		PUSH    1
		PUSH    arrayOfBushouIDOfBattlePosAroundUnitNumRepaint
		PUSH	iPoxXOfBattlePosAroundUnitNumRepaint
		PUSH	iPoxYOfBattlePosAroundUnitNumRepaint
		CALL    pCallFromFuncBattleUnitNumRepaint
		ADD     ESP, 14 //4 * 5
	}
}

// �w����W���ӂ̕`����X�V����B(���@�\���Ȃ�!!)
void FuncBattlePosAroundUnitNumRepaint(HEX_POSITION pos) {

	int x = 0xFFFF;
	int y = 0xFFFF;

	if ( Is_FieldWar() ) {

		// �U��펞�̃w�b�N�X�n�̍��W���A�z��Ƃ��Ă̎��l��index�ւƕύX����B
		ConvertFieldHexPosToFieldIndexPos(pos.x, pos.y, &x, &y);

	} else if ( Is_CastleWar() ) {

		// �U��펞�̃w�b�N�X�n�̍��W���A�z��Ƃ��Ă̎��l��index�ւƕύX����B
		ConvertCastleHexPosToCastleIndexPos(pos.x, pos.y, &x, &y);

	}

	// �ǂ��炩�����łĂ���A�E�g
	if ( x >= 0xFFFF || y >= 0xFFFF ) {
		return;
	}

	// ���W��]��
	iPoxXOfBattlePosAroundUnitNumRepaint = x;
	iPoxYOfBattlePosAroundUnitNumRepaint = y;

	// 0xFFFF�Ŗ��߂Ă���
	for ( int i=0; i<int( sizeof(arrayOfBushouIDOfBattlePosAroundUnitNumRepaint)/sizeof(arrayOfBushouIDOfBattlePosAroundUnitNumRepaint[0]) ); i++ ) {
		arrayOfBushouIDOfBattlePosAroundUnitNumRepaint[i] = 0xFFFF;
	}

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
	FuncBattlePosAroundUnitNumRepaintAsm();

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

