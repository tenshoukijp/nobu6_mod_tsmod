#include "TSModCommand.h"
#include "CommonGlobalHandle.h"

/*
 ���S�p�x�̎����ւ̉e�����J�b�g���邩�ǂ����B�_�p�b�`���̈ڐA�B
 */


void WritePatchDeathFrequencyNotInfluenceLife( int iAddress, char Value ) {
	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(iAddress), &Value, 1, NULL); //1�o�C�g�̂ݏ�������
}


#define CMD_DEATH_FREQUENCY_NOT_INFLUENCE_LIFE_ADDRESS 0x441C0B

void SetDeathFrequencyNotInfluenceLife() {

	// 0004100B: 02 01
	// ���S�p�x�̎����ւ̉e���B�e�������Ȃ�u�Q�v�B�L��Ȃ�u�P�v�B�f�t�H���g�͗L��B

	/*
	// �e���͂������(=�f�t�H���g)
	00441C06   83C4 04          ADD     ESP, 4
	00441C09   83F8 01          CMP     EAX, 1
	00441C0C   1BDB             SBB     EBX, EBX

	// �e���͂�������(=��r�l���Q�ɕύX���āA���킵�Ă�)
	00441C06   . 83C4 04        ADD     ESP, 4
	00441C09   . 83F8 02        CMP     EAX, 2
	00441C0C   . 1BDB           SBB     EBX, EBX
	*/

	// �e�����J�b�g����A�Ƃ����t���O�������Ă���Ȃ�΁c
	if ( TSModCommand::AllBushou::cmdDeathFrequencyNotInfluenceLife == 1 ) {
		WritePatchDeathFrequencyNotInfluenceLife(CMD_DEATH_FREQUENCY_NOT_INFLUENCE_LIFE_ADDRESS, 2 );
	} else {
		WritePatchDeathFrequencyNotInfluenceLife(CMD_DEATH_FREQUENCY_NOT_INFLUENCE_LIFE_ADDRESS, 1 );
	}
}