#include "WinTarget.h"
#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "TSModCommand.h"
#include "EXBushou_All.h"

/*
�����̑O�ɖ�O�������ǂ����B
00402897  |. 85C0           TEST    EAX, EAX


0040288F  |. E8 8D1B0500    CALL    TENSHOU.00454421�@�� ��������s������AESI�Ɏd�|�������AEBX�Ɏd�|����ꂽ���̕����ԍ��B
													  �@ ����́APC/NPC�ŕς��Ȃ��B
00402894  |. 83C4 04        ADD     ESP, 4


004028BF  |. 53             PUSH    EBX
004028C0  |. 56             PUSH    ESI
004028C1  |. E8 62FDFFFF    CALL    TENSHOU.00402628 PC�̏ꍇ   ���̊֐����Ăяo����EDI�Ɍ��ʂ̒l������
004028C6  |. 83C4 08        ADD     ESP, 8

00402901  |. E8 612D0A00    CALL    TENSHOU.004A5667 NPC�̏ꍇ�@���̊֐����Ăяo����EDI�Ɍ��ʂ̒l������B
00402906  |. 83C4 04        ADD     ESP, 4


����̌��ʁAEDI�̒l�Ő��ۂ̃����N�����܂�B
EDI=0 ���̏�ő����ɏo�z
EDI=1 �����̎��t���ɐ���
EDI=2 ���s�����B����ɔے肳�ꂽ�B
EDI=3 �t�ɓ�������Ă��܂��A�o�z���Ă������B(�喼��R�c�����o�z����Ə�񂪋����̂Œ���)
EDI=4 ���s�����B�t�Ɏ�ɕs���͂Ȃ��̂��H �ƌ���ꂽ�B
*/


//----------------------------------------------------------------------------------------------
// �d�|���鑤�Ǝd�|�����鑤�̕����ԍ����E�����߁B

static int iActiveBushouOfJudgeResultOfNPCNaiouInMain = -1;
static int iPassiveBushouOfJudgeResultOfNPCNaiouInMain = -1;

void OnTenshouExeJudgeResultOfNPCNaiouInMain_BeforeExecute() {
}

/*
0040288F  |. E8 8D1B0500    CALL    TENSHOU.00454421
				�������� JMP TSMod.OnTSExeJudgeResultOfNPCNaiouInMain_Before �Ə��������Ă����ۂ̏���
00402894  |. 83C4 04        ADD     ESP, 4
*/
int pTenshouExeJumpFromToOnTenshouExeJudgeResultOfNPCNaiouInMain_Before	    =0x40288F; // �֐��͂��̃A�h���X����AOnTenshouExeJudgeResultOfNPCNaiouInMain_Before�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeJudgeResultOfNPCNaiouInMain_Before =0x454421; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeJudgeResultOfNPCNaiouInMain_Before  =0x402894; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeJudgeResultOfNPCNaiouInMain_Before() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {

		// ���XTENSHOU.EXE���ɂ��������̂�������
		call pTenshouExeJumpCallFromToOnTenshouExeJudgeResultOfNPCNaiouInMain_Before

		mov iActiveBushouOfJudgeResultOfNPCNaiouInMain, ESI // �d�|���鑤
		mov iPassiveBushouOfJudgeResultOfNPCNaiouInMain, EBX // �d�|�����鑤

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}

	OnTenshouExeJudgeResultOfNPCNaiouInMain_BeforeExecute();

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

		jmp pTenshouExeReturnLblFromOnTenshouExeJudgeResultOfNPCNaiouInMain_Before
	}
}



char cmdOnTenshouExeJumpFromJudgeResultOfNPCNaiouInMain_Before[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeJudgeResultOfNPCNaiouInMain_Before() {

	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeJudgeResultOfNPCNaiouInMain_Before;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeJudgeResultOfNPCNaiouInMain_Before + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeJudgeResultOfNPCNaiouInMain_Before  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromJudgeResultOfNPCNaiouInMain_Before+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeJudgeResultOfNPCNaiouInMain_Before), cmdOnTenshouExeJumpFromJudgeResultOfNPCNaiouInMain_Before, 5, NULL); //5�o�C�g�̂ݏ�������
}







//----------------------------------------------------------------------------------------------




static int iResultOfJudgeResultOfNPCNaiouInMain = 1; // ����

extern int GetJudgeResultOfNaiouInMain(int iActiveBushou, int iPassiveBushou, int iResult);

void OnTenshouExeJudgeResultOfNPCNaiouInMainExecute() {
	int iActiveBushou  = iActiveBushouOfJudgeResultOfNPCNaiouInMain - 1;   // �d�|������
	int iPassiveBushou = iPassiveBushouOfJudgeResultOfNPCNaiouInMain - 1;  // �d�|����ꂽ��

	int iOverRide = GetJudgeResultOfNaiouInMain(iActiveBushou, iPassiveBushou, iResultOfJudgeResultOfNPCNaiouInMain);
	if (iOverRide != -1) {
		iResultOfJudgeResultOfNPCNaiouInMain = iOverRide;
	}
}


/*
00402901  |. E8 612D0A00    CALL    TENSHOU.004A5667
				�������� JMP TSMod.OnTSExeJudgeResultOfNPCNaiouInMain �Ə��������Ă����ۂ̏���
00402906  |. 83C4 04        ADD     ESP, 4
*/
int pTenshouExeJumpFromToOnTenshouExeJudgeResultOfNPCNaiouInMain	 =0x402901; // �֐��͂��̃A�h���X����AOnTenshouExeJudgeResultOfNPCNaiouInMain�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeJudgeResultOfNPCNaiouInMain =0x4A5667; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeJudgeResultOfNPCNaiouInMain  =0x402906; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeJudgeResultOfNPCNaiouInMain() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {

		// ���XTENSHOU.EXE���ɂ��������̂�������
		call pTenshouExeJumpCallFromToOnTenshouExeJudgeResultOfNPCNaiouInMain

		mov iResultOfJudgeResultOfNPCNaiouInMain, EDI
		/*
		EDI=0 ���̏�ő����ɏo�z
		EDI=1 �����̎��t���ɐ���
		EDI=2 ���s�����B����ɔے肳�ꂽ�B
		EDI=3 �t�ɓ�������Ă��܂��A�o�z���Ă������B(�喼��R�c�����o�z����Ə�񂪋����̂Œ���)
		EDI=4 ���s�����B�t�Ɏ�ɕs���͂Ȃ��̂��H �ƌ���ꂽ�B
		*/

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}

	OnTenshouExeJudgeResultOfNPCNaiouInMainExecute();

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

		mov EDI, iResultOfJudgeResultOfNPCNaiouInMain // ���ʂ̏㏑��

		jmp pTenshouExeReturnLblFromOnTenshouExeJudgeResultOfNPCNaiouInMain
	}
}



char cmdOnTenshouExeJumpFromJudgeResultOfNPCNaiouInMain[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeJudgeResultOfNPCNaiouInMain() {

	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeJudgeResultOfNPCNaiouInMain;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeJudgeResultOfNPCNaiouInMain + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeJudgeResultOfNPCNaiouInMain  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromJudgeResultOfNPCNaiouInMain+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeJudgeResultOfNPCNaiouInMain), cmdOnTenshouExeJumpFromJudgeResultOfNPCNaiouInMain, 5, NULL); //5�o�C�g�̂ݏ�������
}


