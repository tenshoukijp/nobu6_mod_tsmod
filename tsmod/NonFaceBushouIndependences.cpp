#include "TSModCommand.h"
#include "CommonGlobalHandle.h"

/*
 ��Ȃ����������g�����邩�ۂ��B
 */


void WritePatchNonFaceBushouIndependences( int iAddress, char *pValue ) {
	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(iAddress), pValue, 6, NULL); //1�o�C�g�̂ݏ�������
}

#define CMD_NON_FACE_BUSHOU_INDEPENDENCES_ADDRESS_1 0x4424C9



void WriteAsmJumperOnNonFaceBushouIndependences() {

	/*
	�f�t�H���g(�疳���͊��g�����Ȃ�)
	004424C9  |. 0F84 B7010000  JE      TENSHOU.00442686

	���g������
	004424C9  |. 74 04          JE      SHORT TENSHOU.004424CF
	004424CB  |? 90             NOP
	004424CC  |? 90             NOP
	004424CD  |? 90             NOP
	004424CE  |? 90             NOP
	*/

	// ���g������
	if ( TSModCommand::AllBushou::cmdNonFaceBushouIndependences ) {
		char szCmd1[] = "\x74\x04\x90\x90\x90\x90";
		WritePatchNonFaceBushouIndependences( CMD_NON_FACE_BUSHOU_INDEPENDENCES_ADDRESS_1, szCmd1);

	}
}