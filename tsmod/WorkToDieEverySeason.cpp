#include "TSModCommand.h"
#include "CommonGlobalHandle.h"

/*
 ���S�p�x�ɂ�����炸���G���S�������u����v�u���Ȃ��v�B�_�p�b�`���̈ڐA�B
 */


void WritePatchWorkToDieEverySeason( int iAddress, char Value ) {
	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(iAddress), &Value, 1, NULL); //1�o�C�g�̂ݏ�������
}

#define CMD_WORK_TO_DIE_EVERY_SEASON_ADDRESS_1 0x441C2D



void SetWorkToDieEverySeason() {

	/*

	���S�p�x�ɂ�����炸���G���S�������u����v�u���Ȃ��v

	0004102D: EB 75

	���S��������
	00441C2D   . EB 12          JMP     SHORT TENSHOU.00441C41

	���S�������Ȃ�
	00441C2D   . 75 12          JNZ     SHORT TENSHOU.00441C41

	*/

	// ���G���S����������c
	if ( TSModCommand::AllBushou::cmdSetWorkToDieEverySeason ) {

		WritePatchWorkToDieEverySeason( CMD_WORK_TO_DIE_EVERY_SEASON_ADDRESS_1, (char)0xEB);

	// �f�t�H���g
	} else {
		WritePatchWorkToDieEverySeason( CMD_WORK_TO_DIE_EVERY_SEASON_ADDRESS_1, (char)0x75);
	}
}