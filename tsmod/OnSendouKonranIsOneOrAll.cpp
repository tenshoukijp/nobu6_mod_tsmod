#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"


/*
 * ���������łP�l�������A���l��������
 */

// �������s�������s�����h�c
int iSendouKonranIsOneOrAllBushouID = -1;

// �������������l���ɂȂ邩�ǂ����t���O
int iSendouKonranIsOneOrAllMultiNumber = -1;

// �O��̐������ɍ��������J�E���g
extern int iSendouKonranTargetConranCnt;

void OnTenshouExeSendouKonranIsOneOrAllExecute() {

	// �O��̐����ō��������l���������ŔO�̂��߃��Z�b�g
	iSendouKonranTargetConranCnt = 0;

	// �����l�ȏ�ł���΁A���������Ώۂ����l���ƂȂ�B
	// ����ID�����͈̔�(����ȊO�̒l�������Ă��邱�Ƃ����邽�߃`�F�b�N)
	if (0 <= iSendouKonranIsOneOrAllBushouID-1 && iSendouKonranIsOneOrAllBushouID-1 < GAMEDATASTRUCT_BUSHOU_NUM) {
		if (getBushouInt(iSendouKonranIsOneOrAllBushouID-1) >= 90 &&   // �q�d��90(=180)�ȏ�
			nb6bushouref[iSendouKonranIsOneOrAllBushouID-1].maxint >= 100 &&	 // �q�˂�100(=200)�ȏ�
			nb6bushouref[iSendouKonranIsOneOrAllBushouID-1].ambition >= 90) {	// ��]��90�ȏ�

			// �}���`�l������
			iSendouKonranIsOneOrAllMultiNumber = 1;
		}
	}
}


/*
00433A9B  |. E8 EB940700    CALL    TENSHOU.004ACF8B									
				�������� JMP TSMod.OnTSExeSendouKonranIsOneOrAll �Ə��������Ă����ۂ̏���
00433AA0  |. 83C4 04        ADD     ESP, 4									
00433AA3  |. 85C0           TEST    EAX, EAX		������EAX��0���Ǝ���������
*/
int pTenshouExeJumpFromToOnTenshouExeSendouKonranIsOneOrAll	  =0x433A9B; // �֐��͂��̃A�h���X����AOnTenshouExeSendouKonranIsOneOrAll�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeSendouKonranIsOneOrAll =0x4ACF8B; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeSendouKonranIsOneOrAll  =0x433AA0; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeSendouKonranIsOneOrAll() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {

		// ���XTENSHOU.EXE�ɂ����������������Ŏ��s
		call pTenshouExeJumpCallFromToOnTenshouExeSendouKonranIsOneOrAll

		// ���s������ID
		mov iSendouKonranIsOneOrAllBushouID, edi

		// eax��0���Ƒ��l���ɂȂ炸�������� 
		mov iSendouKonranIsOneOrAllMultiNumber, eax;

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}
	
	// �Q�[���f�[�^���v���C���[�^�[�����ς�����i�K�œǂݍ��ݍX�V���Ă���
	OnTenshouExeSendouKonranIsOneOrAllExecute();

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

		// ���l���ɂȂ邩�ǂ����̏����㏑��
		mov eax, iSendouKonranIsOneOrAllMultiNumber

		jmp pTenshouExeReturnLblFromOnTenshouExeSendouKonranIsOneOrAll
	}
}

/*
0041B892   . E8 5A270100    CALL    TENSHOU.0042DFF1
				�������� JMP TSMod.OnTSExeSendouKonranIsOneOrAll �Ə��������Ă����ۂ̏���
0041B897   . 83C4 04        ADD     ESP, 4
0041B89A     85C0           TEST    EAX, EAX
0041B89C     74 24          JE      SHORT TENSHOU.0041B8C2
*/
char cmdOnTenshouExeJumpFromSendouKonranIsOneOrAll[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeSendouKonranIsOneOrAll() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeSendouKonranIsOneOrAll;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeSendouKonranIsOneOrAll + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeSendouKonranIsOneOrAll  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromSendouKonranIsOneOrAll+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeSendouKonranIsOneOrAll), cmdOnTenshouExeJumpFromSendouKonranIsOneOrAll, 5, NULL); //5�o�C�g�̂ݏ�������
}




