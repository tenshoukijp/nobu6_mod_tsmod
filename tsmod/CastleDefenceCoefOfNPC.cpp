#include "TSModCommand.h"
#include "CommonGlobalHandle.h"

/*
 �b�o�t��ł̏�h��͌W���B�_�p�b�`���̈ڐA�B
 */


void WritePatchCastleDefenceCoefOfNPC( int iAddress, char Value ) {
	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(iAddress), &Value, 1, NULL); //1�o�C�g�̂ݏ�������
}

#define CMD_CASTLE_DEFENCE_COEF_OF_NPC_1 0x432FC3



void SetCastleDefenceCoefOfNPC() {

	/* 
	�b�o�t��ł̏�h��́B����(3), ����(4)

	000323C3: 03 04

	����(3)
	00432FC2  |. B9 03000000    MOV     ECX, 3

	����(4)
	00432FC2  |. B9 04000000    MOV     ECX, 4

	*/

	// �b�o�t��ł̏�h��͌W����ύX���Ă���΁c
	if ( TSModCommand::AllCastle::cmdCastleDefenceCoefOfNPC ) {

		char iCoef = (char)TSModCommand::AllCastle::cmdCastleDefenceCoefOfNPC;
		if ( iCoef <= 0 ) { iCoef = 1; }
		if ( iCoef >= 7 ) { iCoef = 7; }

		WritePatchCastleDefenceCoefOfNPC( CMD_CASTLE_DEFENCE_COEF_OF_NPC_1, (char)iCoef);

	// �f�t�H���g(=3)
	} else {
		WritePatchCastleDefenceCoefOfNPC( CMD_CASTLE_DEFENCE_COEF_OF_NPC_1, (char)0x3);
	}
}