#include "TSModCommand.h"
#include "CommonGlobalHandle.h"
#include "GameDataStruct.h"
#include "EXBushou_All.h"

/*
0040E2C7  /$ 55             PUSH    EBP�@�@�@�@�@�@�@�@�� ���̕ӂ�ECX���v���̕���ID
0040E2C8  |. 8BEC           MOV     EBP, ESP
0040E2CA  |. 53             PUSH    EBX
0040E2CB  |. 56             PUSH    ESI
0040E2CC  |. 33DB           XOR     EBX, EBX
0040E2CE  |. 57             PUSH    EDI
0040E2CF  |. 8B75 0C        MOV     ESI, DWORD PTR SS:[EBP+C]
0040E2D2  |. 66:BF F501     MOV     DI, 1F5						�� 501�Ԃ���X�^�[�g
0040E2D6  |> 57             /PUSH    EDI
0040E2D7  |. E8 A1BCFFFF    |CALL    TENSHOU.00409F7D
0040E2DC  |. 83C4 04        |ADD     ESP, 4
0040E2DF  |. 83F8 0C        |CMP     EAX, 0C
0040E2E2  |. 72 51          |JB      SHORT TENSHOU.0040E335
0040E2E4  |. F6C4 80        |TEST    AH, 80
0040E2E7  |. 75 4C          |JNZ     SHORT TENSHOU.0040E335
0040E2E9  |. 57             |PUSH    EDI
0040E2EA  |. E8 EEC4FFFF    |CALL    TENSHOU.0040A7DD
0040E2EF  |. 83C4 04        |ADD     ESP, 4
0040E2F2  |. 85C0           |TEST    EAX, EAX
0040E2F4  |. 75 3F          |JNZ     SHORT TENSHOU.0040E335
0040E2F6  |. 57             |PUSH    EDI
0040E2F7  |. E8 B3C4FFFF    |CALL    TENSHOU.0040A7AF
0040E2FC  |. 83C4 04        |ADD     ESP, 4
0040E2FF  |. 85C0           |TEST    EAX, EAX						����EAX��TRUE�Ȃ�P�Ƃ݂Ȃ��Œ�v���̂P��
0040E301  |. 74 32          |JE      SHORT TENSHOU.0040E335
0040E303  |. FF75 14        |PUSH    DWORD PTR SS:[EBP+14]           ; /Arg4
0040E306  |. FF75 10        |PUSH    DWORD PTR SS:[EBP+10]           ; |Arg3
0040E309  |. FF75 08        |PUSH    DWORD PTR SS:[EBP+8]            ; |Arg2
0040E30C  |. 57             |PUSH    EDI                             ; |Arg1
0040E30D  |. E8 3C000000    |CALL    TENSHOU.0040E34E                ; \TENSHOU.0040E34E
0040E312  |. 83C4 10        |ADD     ESP, 10
0040E315  |. 85C0           |TEST    EAX, EAX						����EAX��TRUE�Ȃ�P�Ƃ݂Ȃ��Œ�v���̂P�B
0040E317  |. 74 1C          |JE      SHORT TENSHOU.0040E335
0040E319  |. 837D 18 00     |CMP     DWORD PTR SS:[EBP+18], 0
0040E31D  |. 74 0B          |JE      SHORT TENSHOU.0040E32A
0040E31F  |. 57             |PUSH    EDI
0040E320  |. FF55 18        |CALL    DWORD PTR SS:[EBP+18]
0040E323  |. 83C4 04        |ADD     ESP, 4
0040E326  |. 85C0           |TEST    EAX, EAX
0040E328  |. 74 0B          |JE      SHORT TENSHOU.0040E335
0040E32A  |> 85F6           |TEST    ESI, ESI
0040E32C  |. 74 06          |JE      SHORT TENSHOU.0040E334
0040E32E  |. 66:893E        |MOV     WORD PTR DS:[ESI], DI
0040E331  |. 83C6 02        |ADD     ESI, 2
0040E334  |> 43             |INC     EBX
0040E335  |> 66:47          |INC     DI
0040E337  |. 66:81FF 1402   |CMP     DI, 214						532�l�܂ŁB	
0040E33C  |.^76 98          \JBE     SHORT TENSHOU.0040E2D6
0040E33E  |. 85F6           TEST    ESI, ESI
0040E340  |. 74 05          JE      SHORT TENSHOU.0040E347
0040E342  |. 66:C706 FFFF   MOV     WORD PTR DS:[ESI], 0FFFF
0040E347  |> 8BC3           MOV     EAX, EBX
0040E349  |. 5F             POP     EDI
0040E34A  |. 5E             POP     ESI
0040E34B  |. 5B             POP     EBX
0040E34C  |. 5D             POP     EBP
0040E34D  \. C3             RETN

*/


// 0040E2D2  |. 66:BF F501     MOV     DI, 1F5						�� 501�Ԃ���X�^�[�g
#define HIME_MIN_RANGE_ADDRESS	0x40E2D4


// �P�Ɣ��肷��̂�501�l�ڈȍ~�ł͂Ȃ��A1�l�ڈȍ~�Ƃ���B
void SetHideRangePatch() {
	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	short sValue = 1;
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(HIME_MIN_RANGE_ADDRESS), &sValue, 2, NULL); //1�o�C�g�̂ݏ�������
}





//---------------------------------------------------------------------------------------------------

bool isHimeTartareAndVersalia(int iBushouID, int iHimeBushouID) {
	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM) {
		if ( 0 <= iHimeBushouID && iHimeBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
			if ( isLancelotTartare( iBushouID ) ) {
				// �x���Z���A�I���F���X
				if ( strcmp(nb6bushouname[iHimeBushouID].familyname, "�^�_�`")==0 && strcmp(nb6bushouname[iHimeBushouID].fastname, "�a�b�c")==0 ) {
					return true;
				}
			}
		}
	}
	return false;
}

bool isHimeBaalzephonAndOzma(int iBushouID, int iHimeBushouID) {
	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM) {
		if ( 0 <= iHimeBushouID && iHimeBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
			// �I�Y�}�E���[�E�O���V���X�ƃo�[���[�t�H���EV�E���[���Y �Ȃ�u����ҁv�Ƃ���B
			if ( strcmp( nb6bushouname[iHimeBushouID].familyname, "�v�w�x")==0 && strcmp( nb6bushouname[iHimeBushouID].fastname, "�y�z�{")==0
				&& strcmp( nb6bushouname[iBushouID].familyname, "�I�J�K")==0 && strcmp( nb6bushouname[iBushouID].fastname, "�L�M�N")==0 ) {
				// �����
				return true;
			}
		}
	}
	return false;
}


/*
 * �P���ۂ��̔��莞�A���̇A�B���ۂɕP�Ȃ̂��ǂ����̑O��
 */

int iOttoBushouIDOnJudgeHimeHanteiFromCallBusyouDetailGamen = -1; // �v���̕����ԍ��������Ă���A�h���X

int iResultOnJudgeHimeHanteiPrev = 0;  // EAX�p�ɂ������ʒl�B�P�����Ɣ��f���邽�߂ɂ�1�ɂ���K�v������B
int iHimeBushouIDOnJudgeHimeHanteiPrev = -1; // �P���̕����ԍ��̒l�BEDI�̉�������DI���������g��

/*
bool isHimeBaalzephonAndOzmaTmpMarryFlag = false; // �o�[���[�t�H���ƃI�Y�}�p
int isHimeOzmaOriginalState = -1; // �I�Y�}�p
*/
void OnTenshouExeJudgeHimeHanteiPrevExecute() {

	iOttoBushouIDOnJudgeHimeHanteiFromCallBusyouDetailGamen = iOttoBushouIDOnJudgeHimeHanteiFromCallBusyouDetailGamen + 0x10;
	int iBushouID = *(WORD *)(iOttoBushouIDOnJudgeHimeHanteiFromCallBusyouDetailGamen) - 1;

	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		int iHimeBushouID = (iHimeBushouIDOnJudgeHimeHanteiPrev & 0xFFFF) - 1; // �P������ID

		// �����X���b�g�ƃx���T���A
		// �o�[���[�t�H���ƃI�Y�}
		if ( isHimeTartareAndVersalia( iBushouID, iHimeBushouID) ) {

			iResultOnJudgeHimeHanteiPrev = 1; // ���ʂ��㏑�����āA�P���X�g�ɓo�ꂳ����B
		}

		else if ( isHimeBaalzephonAndOzma(iBushouID, iHimeBushouID ) ) {

			// iResultOnJudgeHimeHanteiPrev = 1; // ���ʂ��㏑�����āA�P���X�g�ɓo�ꂳ����B

			/*
			isHimeBaalzephonAndOzmaTmpMarryFlag = false; // �o�[���[�t�H���ƃI�Y�}�p

			// 2�l�Ƃ��������Ă��Ȃ���΁c
			if ( nb6bushouref[iBushouID].spouse == 0xFF && nb6bushouref[iHimeBushouID].spouse == 0xFF ) {
				// �ꎞ�I��2�l������������B

				// �usetHimeMarriage�v�𗘗p����Ɩ������܂ŕω�����̂Ŏg��Ȃ��B
				// �����܂ō����

				// �I�Y�}��P�ɂ���B
				isHimeOzmaOriginalState = nb6bushouname[iHimeBushouID].State; // ���̃X�e�[�g��ێ�
				nb6bushouname[iHimeBushouID].State = 5; // �ꎞ�P�ɂ��āc

				// ���҂̍Ȓl���I�Y�}�̂��̂ɂ��Č����𑕂�
				nb6bushouref[iBushouID].spouse = iHimeBushouID-500;
				nb6bushou[iBushouID].spouse = iHimeBushouID-500;

				nb6bushouref[iHimeBushouID].spouse = iHimeBushouID-500;
				nb6bushou[iHimeBushouID].spouse = iHimeBushouID-500;
				
				// ��Ō��ւƖ߂��̂ŁA�ꎞ�I�Ȍ����t���O��ON�ɂ���
				isHimeBaalzephonAndOzmaTmpMarryFlag = true;
			}
			*/
		}

	}
}


/*
0040E2F7  |. E8 B3C4FFFF    |CALL    TENSHOU.0040A7AF
				�������� JMP TSMod.OnTSExeJudgeHimeHanteiPrev �Ə��������Ă����ۂ̏���
0040E2FC  |. 83C4 04        |ADD     ESP, 4
0040E2FF  |. 85C0           |TEST    EAX, EAX						����EAX��TRUE�Ȃ�P�Ƃ݂Ȃ��Œ�v���̂P��
0040E301  |. 74 32          |JE      SHORT TENSHOU.0040E335
*/
int pTenshouExeJumpFromToOnTenshouExeJudgeHimeHanteiPrev	 =0x40E2F7; // �֐��͂��̃A�h���X����AOnTenshouExeJudgeHimeHanteiPrev�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeJudgeHimeHanteiPrev =0x40A7AF; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeJudgeHimeHanteiPrev	 =0x40E2FC; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeJudgeHimeHanteiPrev() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {

		mov iOttoBushouIDOnJudgeHimeHanteiFromCallBusyouDetailGamen, ebp; // �����ԍ��������Ă���A�h���X���R�s�[

		mov iHimeBushouIDOnJudgeHimeHanteiPrev, edi // �����������R�s�[

		call pTenshouExeJumpCallFromToOnTenshouExeJudgeHimeHanteiPrev // ���X��TENSHOU.EXE�̏���

		mov iResultOnJudgeHimeHanteiPrev, eax // ���ʂ��R�s�[���Ă���

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
	OnTenshouExeJudgeHimeHanteiPrevExecute();

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

		mov eax, iResultOnJudgeHimeHanteiPrev // ���ʂ��㏑������B

		jmp pTenshouExeReturnLblFromOnTenshouExeJudgeHimeHanteiPrev
	}
}

/*
00419DED   . 50             PUSH    EAX                              ; |Arg1
00419DEE   . E8 B843FFFF    CALL    TENSHOU.0040E1AB                 ; \TENSHOU.0040E1AB  ������Call�Ŕ�I�őI�𕐏����X�g�ꗗ�ɂȂ肦�镐�����X�g���A4DBD40�ɍ���Ă���
				�������� JMP TSMod.OnTSExeJudgeHimeHanteiPrev �Ə��������Ă����ۂ̏���
00419DF3   . 83C4 18        ADD     ESP, 18
*/
char cmdOnTenshouExeJumpFromJudgeHimeHanteiPrev[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeJudgeHimeHanteiPrev() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeJudgeHimeHanteiPrev;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeJudgeHimeHanteiPrev + 5 );
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeJudgeHimeHanteiPrev  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromJudgeHimeHanteiPrev+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeJudgeHimeHanteiPrev), cmdOnTenshouExeJumpFromJudgeHimeHanteiPrev, 5, NULL); //5�o�C�g�̂ݏ�������
}




//----------------------------------------------------------------------------------






/*
 * �P���ۂ��̔��莞�A���̇A�B���ۂɕP�Ȃ̂��ǂ����̑O��
 */

int iOttoModeIDOnJudgeHimeHanteiFromCallBusyouDetailGamen = -1;

int iResultOnJudgeHimeHanteiPost = 0;
int iHimeBushouIDOnJudgeHimeHanteiPost = -1;
int iResultOnJudgeHimeHentaiPostNeedButton = 0; // �u�P�v�{�^���K�v��
void OnTenshouExeJudgeHimeHanteiPostExecute() {

	// ���[�hID�̎擾(0x7��0xB���A�Ȃ�)
	iOttoModeIDOnJudgeHimeHanteiFromCallBusyouDetailGamen = iOttoBushouIDOnJudgeHimeHanteiFromCallBusyouDetailGamen + 0x8;
	int iModeID = *(WORD *)(iOttoModeIDOnJudgeHimeHanteiFromCallBusyouDetailGamen);

	// �v�̕���ID�����߂Ď擾
	iOttoBushouIDOnJudgeHimeHanteiFromCallBusyouDetailGamen = iOttoBushouIDOnJudgeHimeHanteiFromCallBusyouDetailGamen + 0x10;
	int iBushouID = *(WORD *)(iOttoBushouIDOnJudgeHimeHanteiFromCallBusyouDetailGamen) - 1;

	int iHimeBushouID = (iHimeBushouIDOnJudgeHimeHanteiPost & 0xFFFF) - 1; // �P������ID

	if ( isHimeTartareAndVersalia( iBushouID, iHimeBushouID) ) {

		if ( iModeID == 0x7 || iModeID==0xF ) { // �����悭�킩��Ȃ����A���X�g�\�����̓��[�h��0x7��0xB��2����s�����B�܂��A�P�{�^�����̂��̂��L�����ۂ��̍ۂɂ͂�0xF�̃��[�h�ƂȂ�B��ʂ̕������X�g�ł�0xF�Ƃ��Ď��s�����B
			iResultOnJudgeHimeHanteiPost = 1; // ���ʂ��㏑�����āA�P���X�g�ɓo�ꂳ����B
		}

	}
	else if ( isHimeBaalzephonAndOzma( iBushouID, iHimeBushouID) ) {

		if ( iModeID == 0x7 || iModeID==0xF ) { // �����悭�킩��Ȃ����A���X�g�\�����̓��[�h��0x7��0xB��2����s�����B�܂��A�P�{�^�����̂��̂��L�����ۂ��̍ۂɂ͂�0xF�̃��[�h�ƂȂ�B��ʂ̕������X�g�ł�0xF�Ƃ��Ď��s�����B
			// iResultOnJudgeHimeHanteiPost = 1; // ���ʂ��㏑�����āA�P���X�g�ɓo�ꂳ����B
		}

	}

	/*
	else if (  isHimeBaalzephonAndOzmaTmpMarryFlag ) {
		if ( iModeID==0xF || iModeID==0xF  ) { // �����悭�킩��Ȃ����A���X�g�\�����̓��[�h��0x7��0xB��2����s�����B�܂��A�P�{�^�����̂��̂��L�����ۂ��̍ۂɂ͂�0xF�̃��[�h�ƂȂ�B��ʂ̕������X�g�ł�0xF�Ƃ��Ď��s�����B
			iResultOnJudgeHimeHanteiPost = 1; // ���ʂ��㏑�����āA�P���X�g�ɓo�ꂳ����B
		}
		// �I�Y�}�����ɖ߂��B
		nb6bushouname[iHimeBushouID].State = isHimeOzmaOriginalState; // ���̃X�e�[�g��ێ�

		// ���҂̍Ȓl���I�Y�}�̂��̂ɂ��Č����𑕂�
		nb6bushouref[iBushouID].spouse = 0xFF;
		nb6bushou[iBushouID].spouse = 0xFF;

		nb6bushouref[iHimeBushouID].spouse = 0xFF;
		nb6bushou[iHimeBushouID].spouse = 0xFF;
		
		// �ꎞ�����t���OOFF
		isHimeBaalzephonAndOzmaTmpMarryFlag = false;
	}
	*/
}


/*
0040E30D  |. E8 3C000000    |CALL    TENSHOU.0040E34E                ; \TENSHOU.0040E34E
				�������� JMP TSMod.OnTSExeJudgeHimeHanteiPost �Ə��������Ă����ۂ̏���
0040E312  |. 83C4 10        |ADD     ESP, 10
0040E315  |. 85C0           |TEST    EAX, EAX						����EAX��TRUE�Ȃ�P�Ƃ݂Ȃ��Œ�v���̂P�B
0040E317  |. 74 1C          |JE      SHORT TENSHOU.0040E335
*/
int pTenshouExeJumpFromToOnTenshouExeJudgeHimeHanteiPost	 =0x40E30D; // �֐��͂��̃A�h���X����AOnTenshouExeJudgeHimeHanteiPost�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeJudgeHimeHanteiPost =0x40E34E; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeJudgeHimeHanteiPost	 =0x40E312; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeJudgeHimeHanteiPost() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {

		mov iOttoBushouIDOnJudgeHimeHanteiFromCallBusyouDetailGamen, ebp; // ���[�h�������Ă���A�h���X���R�s�[

		mov iHimeBushouIDOnJudgeHimeHanteiPost, edi // �����������R�s�[

		call pTenshouExeJumpCallFromToOnTenshouExeJudgeHimeHanteiPost // ���X��TENSHOU.EXE�̏���

		mov iResultOnJudgeHimeHanteiPost, eax // ���ʂ��R�s�[���Ă���

		mov iResultOnJudgeHimeHentaiPostNeedButton, ebx // ebx�ɍŏI�I�ȕP�J�E���g

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
	OnTenshouExeJudgeHimeHanteiPostExecute();

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

		mov eax, iResultOnJudgeHimeHanteiPost // ���ʂ��㏑������B

		jmp pTenshouExeReturnLblFromOnTenshouExeJudgeHimeHanteiPost
	}
}

/*
00419DED   . 50             PUSH    EAX                              ; |Arg1
00419DEE   . E8 B843FFFF    CALL    TENSHOU.0040E1AB                 ; \TENSHOU.0040E1AB  ������Call�Ŕ�I�őI�𕐏����X�g�ꗗ�ɂȂ肦�镐�����X�g���A4DBD40�ɍ���Ă���
				�������� JMP TSMod.OnTSExeJudgeHimeHanteiPost �Ə��������Ă����ۂ̏���
00419DF3   . 83C4 18        ADD     ESP, 18
*/
char cmdOnTenshouExeJumpFromJudgeHimeHanteiPost[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeJudgeHimeHanteiPost() {

	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeJudgeHimeHanteiPost;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeJudgeHimeHanteiPost + 5 );
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeJudgeHimeHanteiPost  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromJudgeHimeHanteiPost+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeJudgeHimeHanteiPost), cmdOnTenshouExeJumpFromJudgeHimeHanteiPost, 5, NULL); //5�o�C�g�̂ݏ�������
}


