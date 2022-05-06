#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"
#include "ScenarioMod.h"
#include "PluginMod.h"

/*
0044476F  |. 83C4 04        ADD     ESP, 4
00444772  |. E8 03230200    CALL    TENSHOU.00466A7A
00444777  |. 8BF0           MOV     ESI, EAX
00444779  |.-E9 3C47EBFF    JMP     TSMod.JmpOnTSExeSobaChange  �đ���
0044477E  |. B9 03000000    MOV     ECX, 3
00444783  |. 03C6           ADD     EAX, ESI
00444785  |. 2BD2           SUB     EDX, EDX
00444787  |. F7F1           DIV     ECX
00444789  |. 8BF2           MOV     ESI, EDX
0044478B  |. E8 472C0200    CALL    TENSHOU.004673D7		��������ǂݍ���
00444790  |. 50             PUSH    EAX                              ; /Arg4
00444791     6A 00          PUSH    0
00444793     6A 00          PUSH    0
00444795  |. 56             PUSH    ESI                              ; |Arg1
00444796  |. E8 3B010000    CALL    TENSHOU.004448D6                 ; \TENSHOU.004448D6
0044479B  |. 83C4 10        ADD     ESP, 10
0044479E  |. 50             PUSH    EAX
0044479F  |. E8 3B2C0200    CALL    TENSHOU.004673DF�@�@�@�@�������ꏑ������
004447A4  |. 83C4 04        ADD     ESP, 4
004447A7  |. E8 192C0200    CALL    TENSHOU.004673C5�@�@�@�@���n����ǂݍ���
004447AC  |. 50             PUSH    EAX                              ; /Arg4
004447AD     6A 00          PUSH    0
004447AF     6A 00          PUSH    0
004447B1  |. 56             PUSH    ESI                              ; |Arg1
004447B2  |. E8 1F010000    CALL    TENSHOU.004448D6                 ; \TENSHOU.004448D6
004447B7  |. 83C4 10        ADD     ESP, 10
004447BA  |. 50             PUSH    EAX
004447BB  |. E8 0D2C0200    CALL    TENSHOU.004673CD		���n���ꏑ������
004447C0  |. 83C4 04        ADD     ESP, 4
004447C3  |. E8 EB2B0200    CALL    TENSHOU.004673B3		���S�C����ǂݍ���
004447C8  |. 50             PUSH    EAX                              ; /Arg4
004447C9     6A 00          PUSH    0
004447CB     6A 00          PUSH    0
004447CD  |. 56             PUSH    ESI                              ; |Arg1
004447CE  |. E8 03010000    CALL    TENSHOU.004448D6                 ; \TENSHOU.004448D6
004447D3  |. 83C4 10        ADD     ESP, 10
004447D6  |. 50             PUSH    EAX
004447D7  |. E8 DF2B0200    CALL    TENSHOU.004673BB		���S�C���ꏑ������
004447DC  |. 83C4 04        ADD     ESP, 4

00444803  |. 83C4 0C        ADD     ESP, 0C					���{���̕����ꏑ������
00444806  |. 50             PUSH    EAX
00444807  |. E8 D32B0200    CALL    TENSHOU.004673DF



0044482A  |. 50             PUSH    EAX						���{���̔n���ꏑ������
0044482B  |. E8 9D2B0200    CALL    TENSHOU.004673CD
00444830  |. 83C4 04        ADD     ESP, 4


00444846  |. E8 E0000000    CALL    TENSHOU.0044492B                 ; \TENSHOU.0044492B
0044484B  |. 83C4 0C        ADD     ESP, 0C
0044484E  |. 50             PUSH    EAX
0044484F  |. E8 672B0200    CALL    TENSHOU.004673BB        ���{���̓S�C���ꏑ������
00444854  |. 83C4 04        ADD     ESP, 4


*/


// ���ꂪ�Ӑ}�I�ɏC�����ꂽ�B
NB6SOBA modify_soba;

void OnTenshouExeSobaChangeExecute() {

	// ���G�A�e���ꂪ�ύX���ꂽ���`�F�b�N�B�ύX���ꂽ�t���O�����Z�b�g
	modify_soba.rise    = 0;
	modify_soba.gun     = 0;
	modify_soba.horse   = 0;
	modify_soba.soldier = 0;

	// ����ύX�ύX���ɍs���T�[�r�X
	EXSuperService_OnSobaChange();

	// PluginMod�n�̂O�ȏ�i�����\�j�Ȃc�k�k�̃��C���Q�[���J�n���̃��\�b�h���Ăяo���B
	// ���s�ΏۂƂȂ��Ă���DLL�����ɑ{�����āc
	for ( vector<HMODULE>::iterator it = listOfTargetDLL.begin(); it != listOfTargetDLL.end(); it++ ) {

		// �Ώۂ�DLL��MainGameStart�֐������݂��邩���`�F�b�N�B
		PFNTARGETMODCHANGESOBA p_ChangeSoba = (PFNTARGETMODCHANGESOBA)GetProcAddress(*it, "OnSobaChange");

		if ( !p_ChangeSoba ) {
			continue;
		}

		((PFNTARGETMODCHANGESOBA) p_ChangeSoba)();
	}

	// SenarioMod�ő���������㏑������B
	if ( p_snOnChangeSoba ) {
		((PFNSNONCHANGESOBA) p_snOnChangeSoba)(&modify_soba);
	}

	// �Ă̑���������ύX���Ă���Ȃ�΁A����Ɋ�Â��ď㏑���B
	if ( modify_soba.rise ) {
		nb6soba.rise = modify_soba.rise;
	}
}


/*
0044476F  |. 83C4 04        ADD     ESP, 4
00444772  |. E8 03230200    CALL    TENSHOU.00466A7A
00444777  |. 8BF0           MOV     ESI, EAX
00444779  |. E8 6B2C0200    CALL    TENSHOU.004673E9
				�������� JMP TSMod.OnTSExeSobaChange �Ə��������Ă����ۂ̏���
0044477E  |. B9 03000000    MOV     ECX, 3

 */
int pTenshouExeJumpFromToOnTenshouExeSobaChange    =0x444779; // �֐��͂��̃A�h���X����AOnTenshouExeSobaChange�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeSobaChange=0x4673E9; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeSobaChange =0x44477E; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeSobaChange() {
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
	OnTenshouExeSobaChangeExecute();

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
		call pTenshouExeJumpCallFromToOnTenshouExeSobaChange

		jmp pTenshouExeReturnLblFromOnTenshouExeSobaChange
	}
}

/*
00444769  |. 50             PUSH    EAX
0044476A  |. E8 3A2C0200    CALL    TENSHOU.004673A9
				�������� JMP TSMod.OnTSExeSobaChange �Ə��������Ă����ۂ̏���
0044476F  |. 83C4 04        ADD     ESP, 4
*/
char cmdOnTenshouExeJumpFromSobaChange[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeSobaChange() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeSobaChange;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeSobaChange + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeSobaChange  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromSobaChange+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeSobaChange), cmdOnTenshouExeJumpFromSobaChange, 5, NULL); //5�o�C�g�̂ݏ�������
}




//------------------------------------------------------------------------------------------


int SobaSoldierChange = 0;
void OnTenshouExeSobaSoldierChangeExecute() {
	// ������㏑�����Ă���΁c
	if ( modify_soba.soldier ) {
		SobaSoldierChange = modify_soba.soldier;
		nb6soba.soldier = modify_soba.soldier;
	}
}


/*
00444803  |. 83C4 0C        ADD     ESP, 0C					���{���̕����ꏑ������
00444806  |. 50             PUSH    EAX
00444807  |. E8 D32B0200    CALL    TENSHOU.004673DF
�������� JMP TSMod.OnTSExeSobaSoldierChange �Ə��������Ă����ۂ̏���
0044480C  |. 83C4 04        ADD     ESP, 4

 */
int pTenshouExeJumpFromToOnTenshouExeSobaSoldierChange    =0x444807; // �֐��͂��̃A�h���X����AOnTenshouExeSobaSoldierChange�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeSobaSoldierChange=0x4673DF; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeSobaSoldierChange =0x44480C; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeSobaSoldierChange() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {
		// ���m�̑����ۑ�
		mov SobaSoldierChange, eax;

		// �v�b�V�������̂͂Ȃ��������Ƃɂ���
		pop eax;

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}
	
	OnTenshouExeSobaSoldierChangeExecute();

	// ���m�̑��ꂪ�v���O�����ŕύX���ꂽ�B
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

		// �l���㏑�����Ă���̂𔽉f
		mov eax, SobaSoldierChange

		push eax; // ���XTENSHOU.EXE�ɂ�����push����

		// ���XTENSHOU.EXE���ɂ��������̂�������
		call pTenshouExeJumpCallFromToOnTenshouExeSobaSoldierChange

		jmp pTenshouExeReturnLblFromOnTenshouExeSobaSoldierChange
	}
}

/*
0044479B  |. 83C4 10        ADD     ESP, 10
0044479E  |. 50             PUSH    EAX
0044479F  |. E8 3B2C0200    CALL    TENSHOU.004673DF
				�������� JMP TSMod.OnTSExeSobaSoldierChange �Ə��������Ă����ۂ̏���
004447A4  |. 83C4 04        ADD     ESP, 4

 */
char cmdOnTenshouExeJumpFromSobaSoldierChange[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeSobaSoldierChange() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeSobaSoldierChange;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeSobaSoldierChange + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeSobaSoldierChange  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromSobaSoldierChange+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeSobaSoldierChange), cmdOnTenshouExeJumpFromSobaSoldierChange, 5, NULL); //5�o�C�g�̂ݏ�������
}






//------------------------------------------------------------------------------------------




int SobaHorseChange = 0;
void OnTenshouExeSobaHorseChangeExecute() {
	// ������㏑�����Ă���΁c
	if ( modify_soba.horse ) {
		SobaHorseChange = modify_soba.horse;
		nb6soba.horse = modify_soba.horse;
	}
}


/*
0044482A  |. 50             PUSH    EAX						���{���̔n���ꏑ������
0044482B  |. E8 9D2B0200    CALL    TENSHOU.004673CD
				�������� JMP TSMod.OnTSExeSobaHorseChange �Ə��������Ă����ۂ̏���
00444830  |. 83C4 04        ADD     ESP, 4

 */
int pTenshouExeJumpFromToOnTenshouExeSobaHorseChange    =0x44482B; // �֐��͂��̃A�h���X����AOnTenshouExeSobaHorseChange�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeSobaHorseChange=0x4673CD; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeSobaHorseChange =0x444830; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeSobaHorseChange() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {
		// ���m�̑����ۑ�
		mov SobaHorseChange, eax;

		// �v�b�V�������̂͂Ȃ��������Ƃɂ���
		pop eax;

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}
	
	OnTenshouExeSobaHorseChangeExecute();

	// ���m�̑��ꂪ�v���O�����ŕύX���ꂽ�B
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

		// �l���㏑�����Ă���̂𔽉f
		mov eax, SobaHorseChange

		push eax; // ���XTENSHOU.EXE�ɂ�����push����

		// ���XTENSHOU.EXE���ɂ��������̂�������
		call pTenshouExeJumpCallFromToOnTenshouExeSobaHorseChange

		jmp pTenshouExeReturnLblFromOnTenshouExeSobaHorseChange
	}
}

/*
0044479B  |. 83C4 10        ADD     ESP, 10
0044479E  |. 50             PUSH    EAX
0044479F  |. E8 3B2C0200    CALL    TENSHOU.004673DF
				�������� JMP TSMod.OnTSExeSobaHorseChange �Ə��������Ă����ۂ̏���
004447A4  |. 83C4 04        ADD     ESP, 4

 */
char cmdOnTenshouExeJumpFromSobaHorseChange[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeSobaHorseChange() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeSobaHorseChange;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeSobaHorseChange + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeSobaHorseChange  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromSobaHorseChange+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeSobaHorseChange), cmdOnTenshouExeJumpFromSobaHorseChange, 5, NULL); //5�o�C�g�̂ݏ�������
}



//------------------------------------------------------------------------------------------

int SobaGunChange = 0;
void OnTenshouExeSobaGunChangeExecute() {
	// ������㏑�����Ă���΁c
	if ( modify_soba.gun ) {
		SobaGunChange = modify_soba.gun;
		nb6soba.gun = modify_soba.gun;
	}
}


/*
0044484E  |. 50             PUSH    EAX
0044484F  |. E8 672B0200    CALL    TENSHOU.004673BB        ���{���̓S�C���ꏑ������
				�������� JMP TSMod.OnTSExeSobaGunChange �Ə��������Ă����ۂ̏���
00444854  |. 83C4 04        ADD     ESP, 4

 */
int pTenshouExeJumpFromToOnTenshouExeSobaGunChange    =0x44484F; // �֐��͂��̃A�h���X����AOnTenshouExeSobaGunChange�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeSobaGunChange=0x4673BB; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeSobaGunChange =0x444854; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeSobaGunChange() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {
		// ���m�̑����ۑ�
		mov SobaGunChange, eax;

		// �v�b�V�������̂͂Ȃ��������Ƃɂ���
		pop eax;

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}
	
	OnTenshouExeSobaGunChangeExecute();

	// ���m�̑��ꂪ�v���O�����ŕύX���ꂽ�B
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

		// �l���㏑�����Ă���̂𔽉f
		mov eax, SobaGunChange

		push eax; // ���XTENSHOU.EXE�ɂ�����push����

		// ���XTENSHOU.EXE���ɂ��������̂�������
		call pTenshouExeJumpCallFromToOnTenshouExeSobaGunChange

		jmp pTenshouExeReturnLblFromOnTenshouExeSobaGunChange
	}
}

/*
0044479B  |. 83C4 10        ADD     ESP, 10
0044479E  |. 50             PUSH    EAX
0044479F  |. E8 3B2C0200    CALL    TENSHOU.004673DF
				�������� JMP TSMod.OnTSExeSobaGunChange �Ə��������Ă����ۂ̏���
004447A4  |. 83C4 04        ADD     ESP, 4

 */
char cmdOnTenshouExeJumpFromSobaGunChange[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeSobaGunChange() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeSobaGunChange;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeSobaGunChange + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeSobaGunChange  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromSobaGunChange+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeSobaGunChange), cmdOnTenshouExeJumpFromSobaGunChange, 5, NULL); //5�o�C�g�̂ݏ�������
}


