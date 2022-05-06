#include "WinTarget.h"

#include "TSModCommand.h"
#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"
#include "ScenarioMod.h"

// �G���f�B���O�e�X�g�p�̃t���O
int iEndingConditionTest = -1;
// �G���f�B���O���}�����喼�h�c
int iEndingConditionDaimyoID = -1;

// �G���f�B���O���}�����喼�h�c
char szEndingConditionRewriteMsg[100] = "";			// �_�C�A���O�̒��g
char szEndingConditionRewriteYakushokuMsg[20] = "�j�I��E�A�C";
char szEndingConditionRewriteDoumeiAllMsg[20] = "�j�I��������";
char szEndingConditionRewriteScenarioModAllMsg[20] = "";
int isMustRewriteEndingConditionRewriteMsg = -1;  // 1�̎��́A��E�A�C�B2�̎��́A��������

void OnTenshouExeEndingConditionYakushokuMsgRewrite() {
	// �G���f�B���O�u�������p������ɗL���Ȓl�������Ă���B
	if ( isMustRewriteEndingConditionRewriteMsg == 1) {
		// �S������̃��b�Z�[�W�ł���΁c
		if ( strstr ( (char *)CPM.getMemoryAddress(LOC1), "�͑S���̓����B������" ) ) {
			// �Y���̃��b�Z�[�W�Œu��������
			WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(CPM.getMemoryAddress(LOC1)), szEndingConditionRewriteMsg, strlen(szEndingConditionRewriteMsg)+1, NULL);
		}
	}
}

void OnTenshouExeEndingConditionScenarioModMsgRewrite() {
	// ScenarioMod�p
	if ( isMustRewriteEndingConditionRewriteMsg == 3) {
		// �S������̃��b�Z�[�W�ł���΁c
		if ( strstr ( (char *)CPM.getMemoryAddress(LOC1), "�͑S���̓����B������" ) ) {
			// �Y���̃��b�Z�[�W�Œu��������
			WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(CPM.getMemoryAddress(LOC1)), szEndingConditionRewriteMsg, strlen(szEndingConditionRewriteMsg)+1, NULL);
		}
	}
}

#pragma warning( push )
#pragma warning( disable: 4996 )


// ScenarioMod�Ƌ��L�̃G���f�B���O�_�C�A���O�̌^
struct TEndingDialogInfo {
	int iDaimyoNo; // +1�̕�
	char szEndingTitle[20];
	char szEndingConditionRewriteMsg[100];
	int isNeverEnding;
	char szEndingMovieName[50];
};

extern void TenshouExeEndingTitlePushAddressClear();
void OnTenshouExeEndingConditionTestExecute() {

	// �����G���f�B���O�˓��Ȃ牽�����Ȃ�
	if ( CommonGlobalHandle::isEndingMode ) {
		return;
	}

	// �G���f�B���O�̃p�����^�N���A��A�g�b�v���j���[�ɖ߂�O�ɂ����ɂ܂������B
	if (CommonGlobalHandle::isEndingModeAfter) {
		TenshouExeEndingTitlePushAddressClear();
		return;
	}

	isMustRewriteEndingConditionRewriteMsg = 0;

	if ( p_snOnEndingCondition ) {
		// �G���f�B���O�̊Y���҂����Ȃ��ꍇ�̂݁A���̓��ʔ���B

		TEndingDialogInfo pEndingDialogInfo;
		// ����ȃG���f�B���O�̐���
		BOOL isReWrite = ((PFNSNONENDINGCONDITION) p_snOnEndingCondition)(&pEndingDialogInfo, nb6turn.daimyo, -1);
		if ( isReWrite ) {

			// �G���f�B���O�����t���O�������Ă����
			if (pEndingDialogInfo.isNeverEnding ) {
				// �R���f�B�V�����͖����������ƂɂȂ�B(1�Ȃ�Γ���҂͋��Ȃ�)
				iEndingConditionTest = 1;

				return;
			}

			// �G���f�B���O�̊Y���҂����Ȃ��ꍇ�̂݁A���̓��ʔ��菈�����s���B
			// �����I�ɂȂ�炩�̃G���f�B���O�ɂ���Ƃ����ӎv
			int iDaimyoID = nb6turn.daimyo-1;

			if ( 0 <= iDaimyoID && iDaimyoID < GAMEDATASTRUCT_DAIMYO_NUM ) {

				iEndingConditionTest = 0; // �N��������(�Ƃ������G���f�B���O)�ւƌ�����

				// ���ꑊ���喼�Ƃ��Ăh�c������
				iEndingConditionDaimyoID = iDaimyoID + 1;

				// �G���f�B���O�̃^�C�g�������ƂŎg���̈�ɃR�s�[���Ă���
				strcpy( szEndingConditionRewriteScenarioModAllMsg, pEndingDialogInfo.szEndingTitle );

				// �G���f�B���O�̃��b�Z�[�W���R�s�[����B
				strcpy( szEndingConditionRewriteMsg, pEndingDialogInfo.szEndingConditionRewriteMsg );

				// ScenarioMod�p�ɒu�������K�v������Ƃ����t���O��ON�Ƃ���B
				isMustRewriteEndingConditionRewriteMsg = 3;

				return;
			}
		}


		// ���������Ă��Ȃ��A�������́A�L���łȂ��Ȃ�΁ACUSTOM_CONDITION_NOMODIFY �Ȃ瑼�̃G���f�B���O�����ւƗ���Ă���
	}

	if ( TSModCommand::MyselfDaimyo::cmdGameEndConditionIsGettingPosition == 1) { 
		// �G���f�B���O�̊Y���҂����Ȃ��ꍇ�̂݁A���̒n������������s���B
		if ( iEndingConditionDaimyoID == 0xFFFF && iEndingConditionTest == 1 ) {
			for ( int iDaimyoID=0; iDaimyoID < GAMEDATASTRUCT_DAIMYO_NUM; iDaimyoID++ ) {
				// �Y���̑喼���v���C���[�S���̑喼�ł���΁c
				if ( isPlayerBushou( nb6daimyo[iDaimyoID].attach-1 )) {
					int iYakushokuID = getNormalizedDaimyoPosition( iDaimyoID );
					// ���̑喼����E�������Ă���΁c
					if ( iYakushokuID > 0 ) {
						// ���ꑊ���喼�Ƃ��Ăh�c������
						iEndingConditionDaimyoID = iDaimyoID+1;
						// �N�������ꂵ�����ƂƂ���B�A
						iEndingConditionTest = 0;

						// �G���f�B���O�̃��b�Z�[�W���쐬����B
						strcpy( szEndingConditionRewriteMsg, "");
						char szWsprintfEndingConditionRewriteMsg[100] = "";
						wsprintf( szWsprintfEndingConditionRewriteMsg, "%s%s�͖��{�̖�E�A%s�ɏA�C�����I\x0",
						nb6bushouname[nb6daimyo[iDaimyoID].attach-1].familyname,
						nb6bushouname[nb6daimyo[iDaimyoID].attach-1].fastname,
						getPositionTypeName(iYakushokuID));
						strcpy( szEndingConditionRewriteMsg, szWsprintfEndingConditionRewriteMsg );

						// �u�������K�v������Ƃ����t���O��ON�Ƃ���B
						isMustRewriteEndingConditionRewriteMsg = 1;

					}
				}
			}
		}
	}
	// �퍑���オ�G���h���X�Ƃ����ݒ肪�Ȃ���Ă���Ȃ�΁c
	else if ( TSModCommand::AllDaimyo::cmdTheSengokouPeriodIsEndless ) {
		// �R���f�B�V�����͖����������ƂɂȂ�B(1�Ȃ�Γ���҂͋��Ȃ�)
		iEndingConditionTest = 1;

	// �����^�G���f�B���O�̎���
	}
	else {
		// �G���f�B���O�̊Y���҂����Ȃ��ꍇ�̂݁A���̓����^�G���f�B���O����������s���B
		if ( iEndingConditionDaimyoID == 0xFFFF && iEndingConditionTest == 1 ) {

			for ( int iDaimyoID=0; iDaimyoID < GAMEDATASTRUCT_DAIMYO_NUM; iDaimyoID++ ) {

				// �Y��ID�̑喼�����Ȃ���΁A���̐l
				if (nb6daimyo[iDaimyoID].attach == 0xFFFF) {
					continue;
				}

				int iMyBushouID = nb6daimyo[iDaimyoID].attach-1;

				// �Y���̑喼���v���C���[�S���̑喼�ł���΁c
				if ( isPlayerBushou( iMyBushouID )) {

					if ( iDaimyoID == GetMaximumInfluenceDaimyo() ) {

						// �S�Ă̐����喼�Ɠ�����Ԃł��邩�ǂ������`�F�b�N
						bool isAllDoumeiStatus = true;

						for ( int iTargetDaimyoID=0; iTargetDaimyoID < GAMEDATASTRUCT_DAIMYO_NUM; iTargetDaimyoID++ ) {

							// �Y��ID�̑喼�������Ȃ�A���̑喼
							if (iDaimyoID == iTargetDaimyoID) {
								continue;
							}

							// �Y��ID�̑喼�����݂��Ȃ���΁A���̑喼
							if (nb6daimyo[iTargetDaimyoID].attach == 0xFFFF) {
								continue;
							}

							if ( !isDoumeiKankei(iDaimyoID, iTargetDaimyoID) ) {
								isAllDoumeiStatus = false;
							}
						}

						if ( isAllDoumeiStatus ) {
							// �N�������ꂵ�����ƂƂ���B�A
							iEndingConditionTest = 0;

							// �G���f�B���O���}�����喼�͊Y���喼
							iEndingConditionDaimyoID = iDaimyoID+1;

							// �u�������K�v������Ƃ����t���O��ON�Ƃ���B
							isMustRewriteEndingConditionRewriteMsg = 2;


						}
					}
				}
			}
		}
	}
}

#pragma warning( pop )


/*
����B������̃t���O�͈ȉ���EAX�Ɉˑ�����B
EAX��0�Ȃ�Γ���B
1�Ȃ�΂܂����ꂵ�Ă��Ȃ��B

���̎���ESI�����ꂵ���喼�ԍ�(�P�n�܂�)�B���ꂵ���喼�����Ȃ����ɂ�0xFFFF�ƂȂ��Ă���B

0047EC3F  |. E8 4A700200    CALL    TENSHOU.004A5C8E
				�������� JMP TSMod.OnTSExeEndingConditionTest �Ə��������Ă����ۂ̏���
0047EC44  |. 83C4 04        ADD     ESP, 4
0047EC47  |. 85C0           TEST    EAX, EAX
0047EC49  |. 75 34          JNZ     SHORT TENSHOU.0047EC7F
0047EC4B  |. 68 00400000    PUSH    4000
0047EC50  |. E8 5E83FEFF    CALL    TENSHOU.00466FB3
 */
int pTenshouExeJumpFromToOnTenshouExeEndingConditionTest    =0x47EC3F; // �֐��͂��̃A�h���X����AOnTenshouExeEndingConditionTest�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeEndingConditionTest=0x4A5C8E; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeEndingConditionTest =0x47EC44; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w

// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeEndingConditionTest() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {
		// ���XTENSHOU.EXE���ɂ��������̂�������
		call pTenshouExeJumpCallFromToOnTenshouExeEndingConditionTest

		// �G���f�B���O���}����������Ȃ�(0=����҂�����B1�Ȃ�Γ���҂͂��Ȃ�)
		mov iEndingConditionTest, EAX

		// �G���f�B���O���}�����喼�̂h�c(0xFFFF�Ȃ�ΊY���҂͋��Ȃ�)
		mov iEndingConditionDaimyoID, ESI

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}
	
	// �G���f�B���O�R���f�B�V������
	OnTenshouExeEndingConditionTestExecute();

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

		// ���ꂵ��������邩�ǂ������㏑��
		mov eax, iEndingConditionTest
		// ���ꂵ���喼�����㏑��
		mov esi, iEndingConditionDaimyoID

		jmp pTenshouExeReturnLblFromOnTenshouExeEndingConditionTest
	}
}

/*
00444769  |. 50             PUSH    EAX
0044476A  |. E8 3A2C0200    CALL    TENSHOU.004673A9
				�������� JMP TSMod.OnTSExeEndingConditionTest �Ə��������Ă����ۂ̏���
0044476F  |. 83C4 04        ADD     ESP, 4
*/
char cmdOnTenshouExeJumpFromEndingConditionTest[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeEndingConditionTest() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeEndingConditionTest;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeEndingConditionTest + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeEndingConditionTest  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromEndingConditionTest+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeEndingConditionTest), cmdOnTenshouExeJumpFromEndingConditionTest, 5, NULL); //5�o�C�g�̂ݏ�������
}






//---------------------------------------------------------------------------------------------






//-------------------------------------------------------------------


int EndingTitlePushAddress = -1;

// �Q�[�����I������̂ŁAEnding����݂̕�����㏑�����ŕύX�������̂����ƂւƖ߂�
void TenshouExeEndingTitlePushAddressClear() {

	EndingTitlePushAddress = 0x4D1978; // �f�t�H���g�̏�ԂɈꉞ�߂��Ă���

	iEndingConditionTest = -1;
	// �G���f�B���O�e�X�g�p�̃t���O
	iEndingConditionDaimyoID = -1;
	// �G���f�B���O���}�����喼�h�c

	ZeroMemory(szEndingConditionRewriteMsg, sizeof(szEndingConditionRewriteMsg)); // �_�C�A���O�̒��g�N���A�B����͕������ꗗ�Ȃǂƃ������A�h���X�����L�Ȃ̂ŃN���A���Ă���
	ZeroMemory(szEndingConditionRewriteScenarioModAllMsg, sizeof(szEndingConditionRewriteScenarioModAllMsg)); // ScenarioMod����̎󂯌��B

	WriteProcessMemory(CommonGlobalHandle::curProcess, (LPVOID)(CPM.getMemoryAddress(LOC1)), szEndingConditionRewriteMsg, sizeof(szEndingConditionRewriteMsg), NULL);

	// �v���C���Ă���l���Ȃ������Ƃŋ����I�ɂQ�d�ڂ̃��[�v�������ł���̂ł͂Ȃ����B
	if (CommonGlobalHandle::isEndingModeAfter) {
		for (int iPG = 0; iPG < GAMEDATASTRUCT_PLAYERS8_NUM; iPG++) {
			// �v���C���[���S�����Ă���R�c�̂h�c���m��
			nb6players8[iPG].gundan = 0xFFFF;
		}
	}
}

void OnTenshouExeEndingTitlePushExecute() {

	// �ꉞ�O�̂��߃f�t�H���g�i�j�I�S������̕�����ւ̃A�h���X�j
	EndingTitlePushAddress = 0x4D1978;

	if ( TSModCommand::MyselfDaimyo::cmdGameEndConditionIsGettingPosition ) { 

		// �j�I�S������ �̕������i�j�I��E�A�C�j�ɕύX���邩�ǂ���
		if ( isMustRewriteEndingConditionRewriteMsg == 1) {
			EndingTitlePushAddress = (int)szEndingConditionRewriteYakushokuMsg;

		} else {
			// ���ꂪ�f�t�H���g�B
			EndingTitlePushAddress = 0x4D1978;
		}

	// �j�I�S������ �̕������i�j�I��������j�ɕύX���邩�ǂ���
	} else if (isMustRewriteEndingConditionRewriteMsg == 2 ) {
		EndingTitlePushAddress = (int)szEndingConditionRewriteDoumeiAllMsg;
	}

	// ScenarioMod����̏�������
	if (isMustRewriteEndingConditionRewriteMsg == 3 ) {

		EndingTitlePushAddress = (int)szEndingConditionRewriteScenarioModAllMsg;
	}
}


/*
0047EC04  |. 50             PUSH    EAX                              ; /Arg8 => 0000FFFF
0047EC05  |. 50             PUSH    EAX                              ; |Arg7 => 0000FFFF
0047EC06  |. 50             PUSH    EAX                              ; |Arg6 => 0000FFFF
0047EC07  |. 6A 0F          PUSH    0F                               ; |Arg5 = 0000000F
0047EC09  |. 6A 00          PUSH    0                                ; |Arg4 = 00000000
0047EC0B  |. 6A 01          PUSH    1                                ; |Arg3 = 00000001
0047EC0D  |. 68 78194D00    PUSH    TENSHOU.004D1978                 ; |Arg2 = 004D1978
				�������� JMP TSMod.OnTSExeEndingTitlePush �Ə��������Ă����ۂ̏���
0047EC12  |. 56             PUSH    ESI                              ; |Arg1
0047EC13  |. BE 38914D00    MOV     ESI, TENSHOU.004D9138            ; |ASCII "XWK"
0047EC18  |. 8BCE           MOV     ECX, ESI                         ; |
0047EC1A  |. E8 4A230200    CALL    TENSHOU.004A0F69                 ; \TENSHOU.004A0F69
 */
int pTenshouExeJumpFromToOnTenshouExeEndingTitlePush    =0x47EC0D; // �֐��͂��̃A�h���X����AOnTenshouExeEndingTitlePush�ւƃW�����v���Ă���B
int pTenshouExeReturnLblFromOnTenshouExeEndingTitlePush =0x47EC12; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeEndingTitlePush() {
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
	OnTenshouExeEndingTitlePushExecute();

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

		// �����������^�C�g�����v�b�V������B
		push EndingTitlePushAddress

		jmp pTenshouExeReturnLblFromOnTenshouExeEndingTitlePush
	}
}

/*
00444769  |. 50             PUSH    EAX
0044476A  |. E8 3A2C0200    CALL    TENSHOU.004673A9
				�������� JMP TSMod.OnTSExeEndingTitlePush �Ə��������Ă����ۂ̏���
0044476F  |. 83C4 04        ADD     ESP, 4
*/
char cmdOnTenshouExeJumpFromEndingTitlePush[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeEndingTitlePush() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeEndingTitlePush;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeEndingTitlePush + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeEndingTitlePush  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromEndingTitlePush+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeEndingTitlePush), cmdOnTenshouExeJumpFromEndingTitlePush, 5, NULL); //5�o�C�g�̂ݏ�������
}
