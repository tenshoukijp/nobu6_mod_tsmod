#include "TSModCommand.h"
#include "CommonGlobalHandle.h"


/*
 �b�o�t�ɂ�鋺���𖳌��Ƃ��邩�ۂ��B�f�t�H���g�́A�����𖳌��Ƃ͂��Ȃ�(�����͗L��)�B
 �_�p�b�`���̈ڐA�B
 */


void WritePatchOtherDaimyoNeverThreat( int iAddress, char Value ) {
	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(iAddress), &Value, 1, NULL);
}

#define CMD_OTHER_DAIMYO_NEVER_THREAT_ADDRESS_1 0x4A74FC
#define CMD_OTHER_DAIMYO_NEVER_THREAT_ADDRESS_2 0x4A750C
#define CMD_OTHER_DAIMYO_NEVER_THREAT_ADDRESS_3 0x4A7528


void SetOtherDaimyoNeverThreat() {

	/* �b�o�t�ɂ�鋺���B�����Ȃ��A�E������B

	000A68FC: 00 2D
	000A690C: 00 1D
	000A6928: EB 77

	�����Ȃ�
	004A74FB   74 00            JE      SHORT TENSHOU.004A74FD
	004A750B   75 00            JNZ     SHORT TENSHOU.004A750D
	004A7528   EB 0E            JMP     SHORT TENSHOU.004A7538

	��������
	004A74FB   74 2D            |JE      SHORT TENSHOU_.004A752A
	004A750B   75 1D            |JNZ     SHORT TENSHOU_.004A752A
	004A7528   77 0E            |JA      SHORT TENSHOU_.004A7538
	*/

	// �����������n�m�ɂ��Ă���΁c
	if ( TSModCommand::OtherDaimyo::cmdNeverThreat ) {
		WritePatchOtherDaimyoNeverThreat( CMD_OTHER_DAIMYO_NEVER_THREAT_ADDRESS_1, (char)0x00);
		WritePatchOtherDaimyoNeverThreat( CMD_OTHER_DAIMYO_NEVER_THREAT_ADDRESS_2, (char)0x00);
		WritePatchOtherDaimyoNeverThreat( CMD_OTHER_DAIMYO_NEVER_THREAT_ADDRESS_3, (char)0xEB);

	// �f�t�H���g�Ȃ��
	} else {
		WritePatchOtherDaimyoNeverThreat( CMD_OTHER_DAIMYO_NEVER_THREAT_ADDRESS_1, (char)0x2D);
		WritePatchOtherDaimyoNeverThreat( CMD_OTHER_DAIMYO_NEVER_THREAT_ADDRESS_2, (char)0x1D);
		WritePatchOtherDaimyoNeverThreat( CMD_OTHER_DAIMYO_NEVER_THREAT_ADDRESS_3, (char)0x77);
	}
}