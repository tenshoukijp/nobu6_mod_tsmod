#include "TSModCommand.h"
#include "CommonGlobalHandle.h"

/*
 ��Q�R�c�ȉ��ɁA�R�}���h�̎w�����\�B�_�p�b�`���̈ڐA�B
 */


void WritePatchCanInstructDirectOtherGundan( int iAddress, char Value ) {
	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(iAddress), &Value, 1, NULL); //1�o�C�g�̂ݏ�������
}

#define CMD_CAN_INSTRUCT_DIRECT_OTHER_GUNDAN_ADDRESS_1 0x476880
#define CMD_CAN_INSTRUCT_DIRECT_OTHER_GUNDAN_ADDRESS_2 0x476898



void SetCanInstructDirectOtherGundan() {

	/* �w���s�\�Ǝw���\�̔�r(�����w���s�\)

	�w���\
	00476880  |. EB 02          JMP     SHORT TENSHOU.00476884
	00476898  |. EB 02          JMP     SHORT TENSHOU.0047689C

	�w���s�\
	00476880  |. 75 02          JNZ     SHORT TENSHOU.00476884
	00476898  |. 75 02          JNZ     SHORT TENSHOU.0047689C
	*/

	// �w���\�ł���΁c
	if ( TSModCommand::MyselfDaimyo::cmdCanInstructDirectOtherGundan ) {
		WritePatchCanInstructDirectOtherGundan( CMD_CAN_INSTRUCT_DIRECT_OTHER_GUNDAN_ADDRESS_1, (char)0xEB);
		WritePatchCanInstructDirectOtherGundan( CMD_CAN_INSTRUCT_DIRECT_OTHER_GUNDAN_ADDRESS_2, (char)0xEB);

	// �s�\�ł���΁c
	} else {
		WritePatchCanInstructDirectOtherGundan( CMD_CAN_INSTRUCT_DIRECT_OTHER_GUNDAN_ADDRESS_1, (char)0x75);
		WritePatchCanInstructDirectOtherGundan( CMD_CAN_INSTRUCT_DIRECT_OTHER_GUNDAN_ADDRESS_2, (char)0x75);
	}
}