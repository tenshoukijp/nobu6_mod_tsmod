#include "WinTarget.h"

#include "TSModCommand.h"
#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"

/*
 �G���f�B���O�ɓ˓�����
 */



/*
0047EC3E  |. 56             PUSH    ESI
0047EC3F  |.-E9 4E627D01    JMP     TSMOD.JmpOnTSExeEndingConditionT>
0047EC44  |. 83C4 04        ADD     ESP, 4
0047EC47  |. 85C0           TEST    EAX, EAX
0047EC49  |. 75 34          JNZ     SHORT TENSHOU.0047EC7F
0047EC4B  |. 68 00400000    PUSH    4000
0047EC50  |. E8 5E83FEFF    CALL    TENSHOU.00466FB3
0047EC55  |. 83C4 04        ADD     ESP, 4
0047EC58  |. 56             PUSH    ESI
0047EC59  |. E8 33FFFFFF    CALL    TENSHOU.0047EB91
0047EC5E  |. 83C4 04        ADD     ESP, 4
0047EC61  |. 56             PUSH    ESI
0047EC62  |. E8 006A0200    CALL    TENSHOU.004A5667
0047EC67  |. 83C4 04        ADD     ESP, 4
0047EC6A  |. 85C0           TEST    EAX, EAX
0047EC6C  |. 74 0A          JE      SHORT TENSHOU.0047EC78
0047EC6E  |. E8 7B46FAFF    CALL    TENSHOU.004232EE  �� �G���f�B���O�ɓ����Ă�B
0047EC73  |. E8 4D7F0100    CALL    TENSHOU.00496BC5  �� �G���f�B���O�ɓ����Ă�B
0047EC78  |> B8 01000000    MOV     EAX, 1

*/


extern void TenshouExeEndingTitlePushAddressClear();


void OnTenshouExeEnteringDoEndingExecute() {
	CommonGlobalHandle::isEndingMode = true;

	CommonGlobalHandle::isEndingModeAfter = true;

	// �G���f�B���O�̐����ȂǂŒl���ς���Ă��܂�����������Ԃ��N���A���Ă���
	// �V�ċL�A�v���P�[�V�������I�����邱�ƂȂ��A�ēx�Q�[�����o����悤�ɁB
	TenshouExeEndingTitlePushAddressClear();
}


/*
0047EC6C  |. 74 0A          JE      SHORT TENSHOU.0047EC78
0047EC6E  |. E8 7B46FAFF    CALL    TENSHOU.004232EE  �� �G���f�B���O�ɓ����Ă�B
0047EC73  |. E8 4D7F0100    CALL    TENSHOU.00496BC5  �� �G���f�B���O�ɓ����Ă�B
�������� JMP TSMod.OnTSExeEnteringDoEnding �Ə��������Ă����ۂ̏���
0047EC78  |> B8 01000000    MOV     EAX, 1
*/
int pTenshouExeJumpFromToOnTenshouExeEnteringDoEnding	   =0x47EC6E; // �֐��͂��̃A�h���X����AOnTenshouExeEnteringDoEnding�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeEnteringDoEnding  =0x4232EE; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeEnteringDoEnding   =0x47EC73; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeEnteringDoEnding() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}
	
	// �G���f�B���O���[�h��
	OnTenshouExeEnteringDoEndingExecute();

	// �X�^�b�N�Ɉ����o��
	__asm {

		pop edi
		pop esi
		pop ebp
		pop esp
		pop edx
		pop ecx
		pop ebx
		pop eax

		// ���XTENSHOU.EXE���ɂ��������̂�������
		call pTenshouExeJumpCallFromToOnTenshouExeEnteringDoEnding

		jmp pTenshouExeReturnLblFromOnTenshouExeEnteringDoEnding
	}
}

/*
*/
char cmdOnTenshouExeJumpFromEnteringDoEnding[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeEnteringDoEnding() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeEnteringDoEnding;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeEnteringDoEnding + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeEnteringDoEnding  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromEnteringDoEnding+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeEnteringDoEnding), cmdOnTenshouExeJumpFromEnteringDoEnding, 5, NULL); //5�o�C�g�̂ݏ�������
}




