#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "SetBushouAvgParam.h"
#include "ScenarioMod.h"
/*
 �ҋ@�������o�ꂷ��
 */
/*
00441B19   > E8 46030000    CALL    TENSHOU.00441E64		
00441B1E   . E8 1F220000    CALL    TENSHOU.00443D42		�� �ҋ@�������o�ꂷ��B
00441B23   . E8 0EB00000    CALL    TENSHOU.0044CB36
*/

/*
00443D5B  /$ B8 2B3E4400    MOV     EAX, TENSHOU.00443E2B
00443D60  |. E8 17C70600    CALL    TENSHOU.004B047C
00443D65  |. 83EC 20        SUB     ESP, 20
00443D68  |. 8D4D D4        LEA     ECX, DWORD PTR SS:[EBP-2C]
00443D6B  |. 56             PUSH    ESI
00443D6C  |. E8 8B18FCFF    CALL    TENSHOU.004055FC
00443D71  |. 33C0           XOR     EAX, EAX
00443D73  |. BE C08C4C00    MOV     ESI, TENSHOU.004C8CC0            ;  ASCII "B:TAIKI.N6P"
00443D78  |. 50             PUSH    EAX
00443D79  |. 8D4D D4        LEA     ECX, DWORD PTR SS:[EBP-2C]
00443D7C  |. 56             PUSH    ESI
00443D7D  |. 8945 FC        MOV     DWORD PTR SS:[EBP-4], EAX
00443D80  |. E8 643D0300    CALL    TENSHOU.00477AE9
00443D85  |. 85C0           TEST    EAX, EAX
00443D87  |. 75 09          JNZ     SHORT TENSHOU.00443D92
00443D89  |. 56             PUSH    ESI
*/

extern void EXSuperService_IfAbilityIsUnderdevelopedOnAlwaysMainGame(int iBushouID);
extern void EXSuperService_IfFixExperienceMaxOnAlwaysMainGame(int iBushouID);
extern void GeneralBushouBornExecute(); // �ėp�������a�������B

// �P�̃_���Ȋ����������
extern void ModifyWrongFaceDamegao(int iBushouID);

/*
 * 
 */
int faceListOnTaikiAppearance[GAMEDATASTRUCT_BUSHOU_NUM] = {-1};

// �ҋ@�������o�ꂷ��O�ɁA��ꗗ����[�ۑ�
void OnTenshouExeTaikiAppearanceExecuteBefore() {
	for (int iBushouID=0; iBushouID < GAMEDATASTRUCT_BUSHOU_NUM; iBushouID++ ) {

		// �_���狸��������B
		ModifyWrongFaceDamegao(iBushouID);

		faceListOnTaikiAppearance[iBushouID] = nb6bushouname[iBushouID].number;
	}
}


extern vector<int> registerSkippedOfIncrementTaikiBushouCount;
extern vector<int> registerModifiedOfIncrementTaikiBushouCount;


// �ҋ@�������o�ꂵ����A��ꗗ�Ɣ�r���āA�H������Ă��镐�������낢�돈��
void OnTenshouExeTaikiAppearanceExecuteAfter() {
	// TSMod.txt�̐ݒ�ɂ���ẮA���S�g�ɒǉ��ŁA�ėp������a��������B
	if ( nb6year.season == 0 ) { // �t����
		GeneralBushouBornExecute(); // �ėp�������a�������B
	}

	for (int iBushouID=0; iBushouID < GAMEDATASTRUCT_BUSHOU_NUM; iBushouID++ ) { // �Ō�̂Q�l�̓`�F�b�N����ׂ��ł͂Ȃ�
		// �ԍ����H������Ă���Ƃ������Ƃ́A�����ŕ������V�K�o�ꂵ���B
		if ( faceListOnTaikiAppearance[iBushouID] != nb6bushouname[iBushouID].number ) {

			auto ret1 = std::find(registerSkippedOfIncrementTaikiBushouCount.begin(), registerSkippedOfIncrementTaikiBushouCount.end(), iBushouID);

			// �X�L�b�v�Ȃ牽�����Ȃ�
			if (ret1 != registerSkippedOfIncrementTaikiBushouCount.end()) {
				// �V���Ȕԍ����X���b�g�ɓ����B
				faceListOnTaikiAppearance[iBushouID] = nb6bushouname[iBushouID].number;

				continue;
			}

			auto ret2 = std::find(registerModifiedOfIncrementTaikiBushouCount.begin(), registerModifiedOfIncrementTaikiBushouCount.end(), iBushouID);

			// �����ς݃��X�g�Ɍ��������牽�����Ȃ�
			if (ret2 != registerModifiedOfIncrementTaikiBushouCount.end()) {

				// �V���Ȕԍ����X���b�g�ɓ����B
				faceListOnTaikiAppearance[iBushouID] = nb6bushouname[iBushouID].number;

				continue;


			} else {

				// �_���狸��������B
				ModifyWrongFaceDamegao(iBushouID);

				// �V���Ȕԍ����X���b�g�ɓ����B
				faceListOnTaikiAppearance[iBushouID] = nb6bushouname[iBushouID].number;

				// �Z�\�����J���ȏꍇ�̏���
				EXSuperService_IfAbilityIsUnderdevelopedOnAlwaysMainGame(iBushouID);

				// �����̏n���x���l�`�w�Œ�ł������ꍇ�̏���
				EXSuperService_IfFixExperienceMaxOnAlwaysMainGame(iBushouID);

				// ���V���[�Y�̕����̔\�͂𔽉f����
				SetBushouAvgParamAppearanceBushou(iBushouID);

				// �Ō�̂Q�l�̓`�F�b�N����ׂ��ł͂Ȃ��̂ōi��
				if (iBushouID < GAMEDATASTRUCT_BUSHOU_NUM - 2) {

					// ScenarioMod�ւƉ��z�����̒a����ʒm
					if (p_snOnBornGeneralBushou) {
						p_snOnBornGeneralBushou(iBushouID);
					}
				}
			}
		}
	}

	// ���̃^�C�~���O�ŁA���GScenarioMod�ŃX�L�b�v���ĂƂ������������̃f�[�^�̓N���A
	registerSkippedOfIncrementTaikiBushouCount.clear();
	// ���̃^�C�~���O�ŁA���G�o�ꂵ���������X�g�̓N���A
	registerModifiedOfIncrementTaikiBushouCount.clear();

}


/*
00441B19   > E8 46030000    CALL    TENSHOU.00441E64		
00441B1E   . E8 1F220000    CALL    TENSHOU.00443D42		�� �ҋ@�������o�ꂷ��B
							�������� JMP TSMod.OnTSExeTaikiAppearance �Ə��������Ă����ۂ̏���
00441B23   . E8 0EB00000    CALL    TENSHOU.0044CB36

*/
int pTenshouExeJumpFromToOnTenshouExeTaikiAppearance	=0x441B1E; // �֐��͂��̃A�h���X����AOnTenshouExeTaikiAppearance�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeTaikiAppearance=0x443D42; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeTaikiAppearance	=0x441B23; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeTaikiAppearance() {
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
	
	OnTenshouExeTaikiAppearanceExecuteBefore();

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
		call pTenshouExeJumpCallFromToOnTenshouExeTaikiAppearance

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}

	OnTenshouExeTaikiAppearanceExecuteAfter();

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

		jmp pTenshouExeReturnLblFromOnTenshouExeTaikiAppearance
	}
}





char cmdOnTenshouExeJumpFromTaikiAppearance[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeTaikiAppearance() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeTaikiAppearance;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeTaikiAppearance + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeTaikiAppearance  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromTaikiAppearance+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeTaikiAppearance), cmdOnTenshouExeJumpFromTaikiAppearance, 5, NULL); //5�o�C�g�̂ݏ�������
}


