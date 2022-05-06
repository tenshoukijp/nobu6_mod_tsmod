#include "TSModCommand.h"
#include "CommonGlobalHandle.h"

/*
 �Q�l�ɂȂ������ɁA�M���l���O�Ƃ��Ȃ��B
 */


void WritePatchKunkouIsNotResetOnRonin5Byte( int iAddress, char *pValue ) {
	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(iAddress),pValue, 5, NULL); //int 1����5�o�C�g��������
}
void WritePatchKunkouIsNotResetOnRonin4Byte( int iAddress, char *pValue ) {
	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(iAddress),pValue, 4, NULL); //int 1����4�o�C�g��������
}

#define CMD_KUNKOU_IS_NOT_RESET_ON_RONIN_ADDRESS_1 0x40B423
#define CMD_KUNKOU_IS_NOT_RESET_ON_RONIN_ADDRESS_2 0x40BC4C
#define CMD_KUNKOU_IS_NOT_RESET_ON_RONIN_ADDRESS_3 0x42943A
#define CMD_KUNKOU_IS_NOT_RESET_ON_RONIN_ADDRESS_4 0x442913
#define CMD_KUNKOU_IS_NOT_RESET_ON_RONIN_ADDRESS_5 0x4C8EC0 // �M���̕�����̃A�h���X
#define CMD_KUNKOU_IS_NOT_RESET_ON_RONIN_ADDRESS_6 0x4CC154 // �M���̕�����̃A�h���X
#define CMD_KUNKOU_IS_NOT_RESET_ON_RONIN_ADDRESS_7 0x4CC160 // �M���̕�����̃A�h���X





void InitKunkouIsNotResetOnRonin() {

	/*  �Q�l�ɂȂ������ɁA�M���l���O�Ƃ��Ȃ��B
		�_�p�b�`�ŉ���ݒ肵�Ă��悤�Ƃ������B

		[�f�t�H���g]
		0040B423  |. E8 7DE5FFFF    CALL    TENSHOU.004099A5	�� �Q�l����


		0040BC4C  |. E8 54DDFFFF    CALL    TENSHOU.004099A5	�� �o�p��

		0042943A  |. E8 6605FEFF    CALL    TENSHOU.004099A5	�� ���ŘQ�l

		00442913  |. E8 8D70FCFF    CALL    TENSHOU.004099A5	�� ���g��(�喼����)


		[�_�p�b�`��]
		0040B423  |. 9090909090    NOP x 5

		0040BC4C  |. 9090909090    NOP x 5

		0042943A  |. 9090909090    NOP x 5

		00442913  |. 9090909090    NOP x 5

	*/

	char patch1[] = "\xE8\x7D\xE5\xFF\xFF";
	WritePatchKunkouIsNotResetOnRonin5Byte( CMD_KUNKOU_IS_NOT_RESET_ON_RONIN_ADDRESS_1, patch1);

	char patch2[] = "\xE8\x54\xDD\xFF\xFF";
	WritePatchKunkouIsNotResetOnRonin5Byte( CMD_KUNKOU_IS_NOT_RESET_ON_RONIN_ADDRESS_2, patch2);

	char patch3[] = "\xE8\x66\x05\xFE\xFF";
	WritePatchKunkouIsNotResetOnRonin5Byte( CMD_KUNKOU_IS_NOT_RESET_ON_RONIN_ADDRESS_3, patch3);

	char patch4[] = "\xE8\x8D\x70\xFC\xFF";
	WritePatchKunkouIsNotResetOnRonin5Byte( CMD_KUNKOU_IS_NOT_RESET_ON_RONIN_ADDRESS_4, patch4);

	return;

	// �_�p�b�`�Łu�M���v���u�����v�Ƃ��ꂽ��������A�u�M���v�ɒ����B
	char szKunkou[] = "�M��";
	WritePatchKunkouIsNotResetOnRonin4Byte( CMD_KUNKOU_IS_NOT_RESET_ON_RONIN_ADDRESS_5, szKunkou);
	WritePatchKunkouIsNotResetOnRonin4Byte( CMD_KUNKOU_IS_NOT_RESET_ON_RONIN_ADDRESS_6, szKunkou);
	WritePatchKunkouIsNotResetOnRonin4Byte( CMD_KUNKOU_IS_NOT_RESET_ON_RONIN_ADDRESS_7, szKunkou);

}