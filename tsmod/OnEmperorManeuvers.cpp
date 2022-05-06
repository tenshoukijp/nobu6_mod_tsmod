#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"



/*
00427648   . E8 89070600    CALL    TENSHOU.00487DD6												
0042764D   . 66:A1 F4E54B00 MOV     AX, WORD PTR DS:[4BE5F4]										������4BE5F4�ɂQ�o�C�g�^�ŕ�����񂪓����Ă���
00427653   . 83C4 04        ADD     ESP, 4												
00427656   . 56             PUSH    ESI                              ; /Arg4												
00427657   . 6A 00          PUSH    0                                ; |Arg3 = 00000000												
00427659   . 6A 1C          PUSH    1C                               ; |Arg2 = 0000001C												
0042765B   . 50             PUSH    EAX                              ; |Arg1												
0042765C   . E8 66B20200    CALL    TENSHOU.004528C7                 ; \TENSHOU.004528C7												
00427661   . 83C4 10        ADD     ESP, 10												
00427664   . E8 AAF8FFFF    CALL    TENSHOU.00426F13												�����̒i�K�ŁA4CB618��4�o�C�g�^�ł����������Ă���B
00427669   . B9 FFFF0000    MOV     ECX, 0FFFF												
0042766E   . 8D50 FC        LEA     EDX, DWORD PTR DS:[EAX-4]												
*/



// ����H��̍ۂ̂���
int iMoneyOnEmperorManeuvers = 0;
WORD iEmperorManeuversBushouID = 0;


#define BUSHOU_ONEMPEROR_MANEUVERS_ADRESSS	0x4BE5F4
#define MONEY_ONEMPEROR_MANEUVERS_ADRESSS	0x4CB618

void OnTenshouExeEmperorManeuversExecute() {

	
	// ������ID�𓾂�
	ReadProcessMemory( 
		CommonGlobalHandle::curProcess,
		(LPCVOID)(BUSHOU_ONEMPEROR_MANEUVERS_ADRESSS), 
		&iEmperorManeuversBushouID, 
		2,
		NULL);

	// �����̋��z�𓾂�
	ReadProcessMemory( 
		CommonGlobalHandle::curProcess,
		(LPCVOID)(MONEY_ONEMPEROR_MANEUVERS_ADRESSS), 
		&iMoneyOnEmperorManeuvers, 
		4,
		NULL);

	if (0 <= iEmperorManeuversBushouID-1 && iEmperorManeuversBushouID-1 < GAMEDATASTRUCT_BUSHOU_NUM ) { 

		// ���Ƃł���Ȃ�΁c
		if (nb6bushouref[iEmperorManeuversBushouID-1].job == 0x60) {

			// ���ʂ�{�ɂ���
			iMoneyOnEmperorManeuvers = iMoneyOnEmperorManeuvers * 3; // 3�{�̌���6x3=18���v���������ƂɂȂ��Ă��I�[�o�[�t���[�͐S�z���Ȃ��Ă��悢
		}

		// �����ł���Ȃ�΁c
		if (nb6bushouref[iEmperorManeuversBushouID-1].job == 0x80) {

			// ��1�̎��ɁA0�ɂ��Ă��܂�Ȃ��悤��(��y�Y�̗����Ȃ��Ȃ��Ă��܂����Ƃ̉��)
			if ( iMoneyOnEmperorManeuvers >= 3 ) {
				// ���ʂ𔼕��ɂ���
				iMoneyOnEmperorManeuvers = iMoneyOnEmperorManeuvers / 3;
			}
		}
	}

}


/*
00426FDC  |. 0305 18B64C00  ADD     EAX, DWORD PTR DS:[4CB618]
				�������� JMP TSMod.OnTSExeEmperorManeuvers �Ə��������Ă����ۂ̏���
00426FE2  |. 2BD2           SUB     EDX, EDX
00426FE4  |. F7F1           DIV     ECX
*/
int pTenshouExeJumpFromToOnTenshouExeEmperorManeuvers		=0x426FDC; // �֐��͂��̃A�h���X����AOnTenshouExeEmperorManeuvers�ւƃW�����v���Ă���B
int pTenshouExeReturnLblFromOnTenshouExeEmperorManeuvers	=0x426FE2; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeEmperorManeuvers() {
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
	OnTenshouExeEmperorManeuversExecute();

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
		add eax, iMoneyOnEmperorManeuvers

		jmp pTenshouExeReturnLblFromOnTenshouExeEmperorManeuvers
	}
}

/*
00426FDC  |. 0305 18B64C00  ADD     EAX, DWORD PTR DS:[4CB618]
				�������� JMP TSMod.OnTSExeEmperorManeuvers �Ə��������Ă����ۂ̏���
00426FE2  |. 2BD2           SUB     EDX, EDX
00426FE4  |. F7F1           DIV     ECX
*/
char cmdOnTenshouExeJumpFromEmperorManeuvers[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeEmperorManeuvers() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeEmperorManeuvers;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeEmperorManeuvers + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeEmperorManeuvers  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromEmperorManeuvers+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeEmperorManeuvers), cmdOnTenshouExeJumpFromEmperorManeuvers, 5, NULL); //5�o�C�g�̂ݏ�������
}



