#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "ScenarioMod.h"
#include <map>
using namespace std;

/*
 �U�̃V�i���I�^�C�g�����\������鎞
 */

/*
0049D05B  |. 57                   PUSH    EDI
0049D05C  |. BE 08374D00          MOV     ESI, TENSHOU.004D3708
0049D061  |. BF 28374D00          MOV     EDI, TENSHOU.004D3728
*/



// �֐���char **�̃Z�b�g���o���Ă���
extern map<FARPROC, char **> psnGetProcAdressAndPointerMap;

void OnTenshouExeViewingScenarioTitleListExecute() {

	// ScenarioMod�̊֐����Ăяo��
	if ( p_snOnViewingScenarioTitleList ) {
		 p_snOnViewingScenarioTitleList();
	}

	// �����ɗ��邽�тɁA�V�i���I�ڍא������̊O���֐��ƃ|�C���^�Ƃ̃}�b�s���O�̓N���A����B
	psnGetProcAdressAndPointerMap.clear();
}


/*
0049D05B  |. 57                   PUSH    EDI
0049D05C  |. BE 08374D00          MOV     ESI, TENSHOU.004D3708
				�������� JMP TSMod.OnTSExeViewingScenarioTitleList �Ə��������Ă����ۂ̏���
0049D061  |. BF 28374D00          MOV     EDI, TENSHOU.004D3728
*/
int pTenshouExeJumpFromToOnTenshouExeViewingScenarioTitleList	   =0x49D05C; // �֐��͂��̃A�h���X����AOnTenshouExeViewingScenarioTitleList�ւƃW�����v���Ă���B
int pTenshouExeReturnLblFromOnTenshouExeViewingScenarioTitleList   =0x49D061; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeViewingScenarioTitleList() {
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
	
	// �Q�[���f�[�^���v���C���[�^�[�����ς�����i�K�œǂݍ��ݍX�V���Ă���
	OnTenshouExeViewingScenarioTitleListExecute();

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

		// ���Xtenshou.exe�ɂ�����������������
		MOV     ESI, 0x4D3708

		jmp pTenshouExeReturnLblFromOnTenshouExeViewingScenarioTitleList
	}
}

/*
*/
char cmdOnTenshouExeJumpFromViewingScenarioTitleList[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeViewingScenarioTitleList() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeViewingScenarioTitleList;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeViewingScenarioTitleList + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeViewingScenarioTitleList  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromViewingScenarioTitleList+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeViewingScenarioTitleList), cmdOnTenshouExeJumpFromViewingScenarioTitleList, 5, NULL); //5�o�C�g�̂ݏ�������
}




