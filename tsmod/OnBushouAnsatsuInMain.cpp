#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"
#include "ScenarioMod.h"
#include "FuncPlayBGM.h"



/*
00403DBB   . 8B4424 04      MOV     EAX, DWORD PTR SS:[ESP+4]                      ;  TENSHOU.004DDF38  �� �����ÎE�R�}���h�̃X�^�[�g�ʒu
00403DBF   . 53             PUSH    EBX
00403DC0   . 66:8B58 0C     MOV     BX, WORD PTR DS:[EAX+C]
00403DC4   . 56             PUSH    ESI
00403DC5   . 66:8B70 04     MOV     SI, WORD PTR DS:[EAX+4]
00403DC9   . 57             PUSH    EDI
00403DCA   . 55             PUSH    EBP
00403DCB   . 53             PUSH    EBX
00403DCC   . E8 FC5C0000    CALL    TENSHOU.00409ACD

00403DE7   . 56             PUSH    ESI
00403DE8   . E8 9DFDFFFF    CALL    TENSHOU.00403B8A
00403DED   . 83C4 08        ADD     ESP, 8
00403DF0   . 85C0           TEST    EAX, EAX									�� ������EAX���ÎE�����������ۂ�
																			       ���̏u�� ESI �����s�����̕����ԍ��AEBX���ÎE�Ώە����̕����ԍ�
00403DF2   . 74 13          JE      SHORT TENSHOU.00403E07
*/



int iAnsatuJikkouBushouInMain = -1; // ���s����
int iAnsatuSareruBushouInMain = -1; // �ÎE�Ώە���
int iAnsatuResultBushouInMain = -1; // �ÎE����

// ���C����ʂ̈ÎE���ɑ喼�����ƁA���y���r�؂�Ă��܂�������������
int iAnsatuBGMOfPatchInAnsatsuBushouInMain = -1;

void OnTenshouExeAnsatsuBushouInMainExecute() {
	int iJikkouBushouID = iAnsatuJikkouBushouInMain - 1;

	if ( 0 <= iJikkouBushouID && iJikkouBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
		// ���s�����������X���b�g�^���^���X�ł���΁A
		if (isLancelotTartare(iJikkouBushouID)) {
			// �ǂ�قǕ��������Ƃ��A1/2�̈ÎE�������͖񑩂���Ă���B
			if (rand() % 2 == 0) {
				iAnsatuResultBushouInMain = 1; // �ÎE�̌��ʂ͐���
			}

		// ���s�������G���ł���΁A
		} else if (isShuiHidechiyo(iJikkouBushouID)) {
			
			// �ǂ�قǕ��������Ƃ��A1/2�̈ÎE�������͖񑩂���Ă���B
			if (rand() % 2 == 0) {
				iAnsatuResultBushouInMain = 1; // �ÎE�̌��ʂ͐���
			}

		// �����ł���΁A�ÎE����肢
		} else if ( nb6bushouref[iJikkouBushouID].job == 0xC0 ) { // ����
			// �ǂ�قǕ��������Ƃ��A1/2�̈ÎE�������͖񑩂���Ă���B
			if ( rand() % 2 == 0 ) {
				iAnsatuResultBushouInMain = 1; // �ÎE�̌��ʂ͐���
			}
		}
	}


	int iSareruBushouID = iAnsatuSareruBushouInMain - 1;

	if ( 0 <= iSareruBushouID && iSareruBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
		// �ÎE�����Ώۂ������X���b�g�^���^���X�ł���΁A
		if (isLancelotTartare(iSareruBushouID)) {
			iAnsatuResultBushouInMain = 0; // ���݂͐�΂Ɏ��s�ł���B

		// �ÎE�����Ώۂ���ɂł���΁A
		} else if ( isShuiHidechiyo(iSareruBushouID) ) {
			iAnsatuResultBushouInMain = 0; // ���݂͐�΂Ɏ��s�ł���B

		} else if ( nb6bushouref[iSareruBushouID].job >= 0xA0 ) { // �R�m�ȏ�̏㋉�E
			// 75%�͐�΂ɖh�䂵�Ă��܂��B
			if ( rand() % 4 != 0 ) {
				iAnsatuResultBushouInMain = 0; // ���݂͂قƂ�ǎ��s�ł���B
			}
		}
	}

	if ( 0 <= iJikkouBushouID && iJikkouBushouID < GAMEDATASTRUCT_BUSHOU_NUM &&
	     0 <= iSareruBushouID && iSareruBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// ScenarioMod�̃J�X�^�������ŌR�t��ݒ肷��
		if ( p_snOnCustomCondition ) {
			// ���C����ʂł̈ÎE�̐���																									�����s��	�@���󓮎�			�����ݗ\�肳��Ă��錋��
			int isScenarioModDeathResult = ((PFNSNONCUSTOMCONDITION) p_snOnCustomCondition)("AS����::���C��::�ÎE�󓮎Ҏ��S��", iJikkouBushouID+1, iSareruBushouID+1, iAnsatuResultBushouInMain, "", "", 0.0, 0.0);
			if ( isScenarioModDeathResult != CUSTOM_CONDITION_NOMODIFY ) {
				iAnsatuResultBushouInMain = isScenarioModDeathResult;
			}
		}
	}

	if (iAnsatuResultBushouInMain) {
		iAnsatuBGMOfPatchInAnsatsuBushouInMain = getPlayingBGM();
	}

}


/*
00403DE8   . E8 9DFDFFFF    CALL    TENSHOU.00403B8A
00403DED   . 83C4 08        ADD     ESP, 8
00403DF0   . 85C0           TEST    EAX, EAX									�� ������EAX���ÎE�����������ۂ�
																			       ���̏u�� ESI �����s�����̕����ԍ��AEBX���ÎE�Ώە����̕����ԍ�
*/

int pTenshouExeJumpFromToOnTenshouExeAnsatsuBushouInMain    =0x403DE8; // �֐��͂��̃A�h���X����AOnTenshouExeAnsatsuBushouInMain�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeAnsatsuBushouInMain=0x403B8A; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeAnsatsuBushouInMain =0x403DED; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeAnsatsuBushouInMain() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {

		call pTenshouExeJumpCallFromToOnTenshouExeAnsatsuBushouInMain

		mov iAnsatuJikkouBushouInMain, esi // ecx �����s�����̂h�c
		mov iAnsatuSareruBushouInMain, ebx // ebx ���ÎE�Ώە����̂h�c

		mov iAnsatuResultBushouInMain, eax // eax ���ÎE�������A���s��

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
	OnTenshouExeAnsatsuBushouInMainExecute();

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

		mov eax, iAnsatuResultBushouInMain	// eax �ɈÎE���ʂ��t���f

		jmp pTenshouExeReturnLblFromOnTenshouExeAnsatsuBushouInMain
	}
}

/*
00403DE8   . E8 9DFDFFFF    CALL    TENSHOU.00403B8A
00403DED   . 83C4 08        ADD     ESP, 8
00403DF0   . 85C0           TEST    EAX, EAX									�� ������EAX���ÎE�����������ۂ�
																			       ���̏u�� ESI �����s�����̕����ԍ��AEBX���ÎE�Ώە����̕����ԍ�
*/
char cmdOnTenshouExeJumpFromAnsatsuBushouInMain[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeAnsatsuBushouInMain() {

	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeAnsatsuBushouInMain;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeAnsatsuBushouInMain + 5 );
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeAnsatsuBushouInMain  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromAnsatsuBushouInMain+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeAnsatsuBushouInMain), cmdOnTenshouExeJumpFromAnsatsuBushouInMain, 5, NULL); //5�o�C�g�̂ݏ�������
}




