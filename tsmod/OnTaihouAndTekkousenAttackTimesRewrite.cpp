#include "WinTarget.h"
#include <stdlib.h>
#include <string.h>

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"



// ��C�������͓S�b�D�����Ƃ��Ƃ��Ă��镐����ID
int iTaihouAndTekkousenAttackTimesRewriteBushouID = -1;

// ��C�������͓S�b�D�̔��ˉ�
int iTaihouAndTekkousenAttackTimesRewriteCanonFireCnt = 1;

void OnTenshouExeTaihouAndTekkousenAttackTimesRewriteExecute() {

	// iTaihouAndTekkousenAttackTimesRewriteCanonFireCnt == 0 �̎��́A���X�����ɂ���͂��ł͂Ȃ������B�����ď������Ă͂����Ȃ�!!
	if ( iTaihouAndTekkousenAttackTimesRewriteCanonFireCnt == 0 ) {
		return;
	}

	// ���������͈̔͂ł��邱��
	if (0 <= iTaihouAndTekkousenAttackTimesRewriteBushouID-1 && iTaihouAndTekkousenAttackTimesRewriteBushouID-1 < GAMEDATASTRUCT_BUSHOU_NUM) {

		// �^���^���X�ł���΁A��C�A�������́A�S�b�D�̔��ˉ񐔂��A�A�C�e���@���ɂ�����炸�A�Q�񑝂₷
		if ( isLancelotTartare(iTaihouAndTekkousenAttackTimesRewriteBushouID-1) ) {
			//
			iTaihouAndTekkousenAttackTimesRewriteCanonFireCnt += 2;
		}

		// ��ɂł���΁A��C�A�������́A�S�b�D�̔��ˉ񐔂��A�A�C�e���@���ɂ�����炸�A1�񑝂₷
		else if (isShuiHidechiyo(iTaihouAndTekkousenAttackTimesRewriteBushouID - 1)) {
			//
			iTaihouAndTekkousenAttackTimesRewriteCanonFireCnt += 1;
		}

	}
}


/*
00432B40  |> 85C0           TEST    EAX, EAX
00432B42  |. 0F84 4D010000  JE      TENSHOU.00432C95
00432B48  |. 8945 E0        MOV     DWORD PTR SS:[EBP-20], EAX	       ��C�������͓S�b�D�̉񐔂����Bebp�A�h���X�����肪�A���̏u�ԗ��p���Ă���푈���̊Y�������̈ꎞ���炵���B����ebp-20�����ˉ񐔂̌��݂̎c��񐔂̏ꏊ�B		
				�������� JMP TSMod.OnTSExeTaihouAndTekkousenAttackTimesRewrite �Ə��������Ă����ۂ̏���
00432B4B  |> 8D45 C4        /LEA     EAX, DWORD PTR SS:[EBP-3C]		�@ �������̓��[�v�̓��ŕʂ̍s����W�����v�Q�Ƃ��Ă�Ƃ��낪����̂ŁA�֐����ɂƂ肱��ł͂����Ȃ��B								
 */
int pTenshouExeJumpFromToOnTenshouExeTaihouAndTekkousenAttackTimesRewrite    =0x432B40; // �֐��͂��̃A�h���X����AOnTenshouExeTaihouAndTekkousenAttackTimesRewrite�ւƃW�����v���Ă���B
int pTenshouExeReturnLblFromOnTenshouExeTaihouAndTekkousenAttackTimesRewrite =0x432B48; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
int pTenshouExeJumpLblFromOnTenshouExeTaihouAndTekkousenAttackTimesNoNeedRewrite =0x432C95; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeTaihouAndTekkousenAttackTimesRewrite() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {
		// ��C�������͓S�b�D�̑ł�
		mov iTaihouAndTekkousenAttackTimesRewriteCanonFireCnt, eax;

		// ��C�������͓S�b�D�����Ƃ��Ƃ��Ă��镐����ID
		mov iTaihouAndTekkousenAttackTimesRewriteBushouID, esi;

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}
	
	// 
	OnTenshouExeTaihouAndTekkousenAttackTimesRewriteExecute();

	// ���������ɗ���ׂ��ł͂Ȃ������ꍇ
	if ( iTaihouAndTekkousenAttackTimesRewriteCanonFireCnt == 0 ) {

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

			// ���X test eax, eax �̌��ʂ�0�ł������ꍇ�ɔ�Ԃׂ��������͂��̏ꏊ�ւƋA��B
			jmp pTenshouExeJumpLblFromOnTenshouExeTaihouAndTekkousenAttackTimesNoNeedRewrite
		}

	}

	if ( iTaihouAndTekkousenAttackTimesRewriteCanonFireCnt != 0 ) {

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
			// ���ˉ񐔂��v�Z�ɂ����̂ŏ㏑���B
			mov eax, iTaihouAndTekkousenAttackTimesRewriteCanonFireCnt

			jmp pTenshouExeReturnLblFromOnTenshouExeTaihouAndTekkousenAttackTimesRewrite
		}
	}
}

/*
00432B48  |. 8945 E0        MOV     DWORD PTR SS:[EBP-20], EAX									��C�������͓S�b�D�̉񐔂����B		
				�������� JMP TSMod.OnTSExeTaihouAndTekkousenAttackTimesRewrite �Ə��������Ă����ۂ̏���
00432B4B  |> 8D45 C4        /LEA     EAX, DWORD PTR SS:[EBP-3C]											
*/
char cmdOnTenshouExeJumpFromTaihouAndTekkousenAttackTimesRewrite[9]="\xE9\x90\x90\x90\x90\x90\x90\x90";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeTaihouAndTekkousenAttackTimesRewrite() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeTaihouAndTekkousenAttackTimesRewrite;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeTaihouAndTekkousenAttackTimesRewrite + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeTaihouAndTekkousenAttackTimesRewrite  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromTaihouAndTekkousenAttackTimesRewrite+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeTaihouAndTekkousenAttackTimesRewrite), cmdOnTenshouExeJumpFromTaihouAndTekkousenAttackTimesRewrite, 8, NULL); //11�o�C�g�̂ݏ�������
}




