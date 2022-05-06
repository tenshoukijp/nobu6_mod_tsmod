#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"

#include "ScenarioMod.h"

// �o�b��p�̐��̓o�p�����̉�

/*
004298AA   /$ B8 229A4200    MOV     EAX, TENSHOU.00429A22
004298AF   |. E8 C86B0800    CALL    TENSHOU.004B047C
004298B4   |. 81EC A8000000  SUB     ESP, 0A8
004298BA   |. 8D8D 4CFFFFFF  LEA     ECX, DWORD PTR SS:[EBP-B4]
004298C0   |. 53             PUSH    EBX
004298C1   |. 56             PUSH    ESI
004298C2   |. 57             PUSH    EDI
004298C3   |. 33F6           XOR     ESI, ESI
004298C5   |. E8 6D010000    CALL    TENSHOU.00429A37
004298CA   |. FF75 0C        PUSH    DWORD PTR SS:[EBP+C]
004298CD   |. C745 F0 010000>MOV     DWORD PTR SS:[EBP-10], 1
004298D4   |. 8975 FC        MOV     DWORD PTR SS:[EBP-4], ESI
004298D7   |. E8 030EFEFF    CALL    TENSHOU.0040A6DF
004298DC   |. 83C4 04        ADD     ESP, 4
004298DF   |. 85C0           TEST    EAX, EAX
004298E1   |. 75 10          JNZ     SHORT TENSHOU.004298F3
004298E3   |. FF75 0C        PUSH    DWORD PTR SS:[EBP+C]
004298E6   |. FF75 08        PUSH    DWORD PTR SS:[EBP+8]
004298E9   |. E8 EAFEFFFF    CALL    TENSHOU.004297D8			�� ����EAX�̌��ʂ�1�Ȃ�o�p����
004298EE   |. 83C4 08        ADD     ESP, 8
004298F1   |. 8BF0           MOV     ESI, EAX					�� EAX��ESI��
004298F3   |> 6A 01          PUSH    1
004298F5   |. 6A 0A          PUSH    0A
004298F7   |. E8 ED3A0600    CALL    TENSHOU.0048D3E9
004298FC   |. 83C4 08        ADD     ESP, 8
004298FF   |. A1 18EF4B00    MOV     EAX, DWORD PTR DS:[4BEF18]
00429904   |. 8D8D 4CFFFFFF  LEA     ECX, DWORD PTR SS:[EBP-B4]
0042990A   |. 33FF           XOR     EDI, EDI
0042990C   |. FF75 08        PUSH    DWORD PTR SS:[EBP+8]             ; /Arg4
0042990F   |. FF75 0C        PUSH    DWORD PTR SS:[EBP+C]             ; |Arg3
00429912   |. 50             PUSH    EAX                              ; |Arg2 => 0000FFFF
00429913   |. FF35 14EF4B00  PUSH    DWORD PTR DS:[4BEF14]            ; |Arg1 = 0000FFFF
00429919   |. E8 9DB00700    CALL    TENSHOU.004A49BB                 ; \TENSHOU.004A49BB
0042991E   |. BB 01000000    MOV     EBX, 1
00429923   |> 8B04BD 20EF4B0>MOV     EAX, DWORD PTR DS:[EDI*4+4BEF20]
0042992A   |. 68 FFFF0000    PUSH    0FFFF
0042992F   |. FF75 08        PUSH    DWORD PTR SS:[EBP+8]
00429932   |. 8945 EC        MOV     DWORD PTR SS:[EBP-14], EAX
00429935   |. E8 A0B60700    CALL    TENSHOU.004A4FDA
0042993A   |. 83C4 04        ADD     ESP, 4
0042993D   |. 50             PUSH    EAX                              ; /Arg3
0042993E   |. FF75 0C        PUSH    DWORD PTR SS:[EBP+C]             ; |Arg2
00429941   |. FF75 EC        PUSH    DWORD PTR SS:[EBP-14]            ; |Arg1
00429944   |. E8 362B0400    CALL    TENSHOU.0046C47F                 ; \TENSHOU.0046C47F
00429949   |. 83C4 0C        ADD     ESP, 0C
0042994C   |. 8D8D 4CFFFFFF  LEA     ECX, DWORD PTR SS:[EBP-B4]       ; |
00429952   |. 50             PUSH    EAX                              ; |Arg1
00429953   |. E8 32B30700    CALL    TENSHOU.004A4C8A                 ; \TENSHOU.004A4C8A
00429958   |. E8 7A1F0500    CALL    TENSHOU.0047B8D7
0042995D   |. 8945 EC        MOV     DWORD PTR SS:[EBP-14], EAX
00429960   |. 85C0           TEST    EAX, EAX						�����̒i�K��ESI���P�Ȃ�o�p����(EAX�ł͂Ȃ�ESI)
00429962   |. 74 0B          JE      SHORT TENSHOU.0042996F
00429964   |. 3BC3           CMP     EAX, EBX
00429966   |. 74 1A          JE      SHORT TENSHOU.00429982
00429968   |. 83F8 02        CMP     EAX, 2
0042996B   |. 74 28          JE      SHORT TENSHOU.00429995
0042996D   |. EB 4B          JMP     SHORT TENSHOU.004299BA
0042996F   |> C745 E8 DD0B00>MOV     DWORD PTR SS:[EBP-18], 0BDD
00429976   |. C745 E4 FFFF00>MOV     DWORD PTR SS:[EBP-1C], 0FFFF
0042997D   |. 895D F0        MOV     DWORD PTR SS:[EBP-10], EBX
00429980   |. EB 38          JMP     SHORT TENSHOU.004299BA
00429982   |> C745 E8 DF0B00>MOV     DWORD PTR SS:[EBP-18], 0BDF
00429989   |. C745 E4 070000>MOV     DWORD PTR SS:[EBP-1C], 7
00429990   |. 895D F0        MOV     DWORD PTR SS:[EBP-10], EBX
00429993   |. EB 25          JMP     SHORT TENSHOU.004299BA
00429995   |> 85F6           TEST    ESI, ESI						�����̒i�K��ESI���P�Ȃ琬��(���ESI�ɐݒ肵�Ă����΁A���̂܂܂̒l������)
00429997   |. 74 0C          JE      SHORT TENSHOU.004299A5
00429999   |. C745 E8 E10B00>MOV     DWORD PTR SS:[EBP-18], 0BE1
004299A0   |. 895D E4        MOV     DWORD PTR SS:[EBP-1C], EBX
004299A3   |. EB 15          JMP     SHORT TENSHOU.004299BA
004299A5   |> C745 F0 000000>MOV     DWORD PTR SS:[EBP-10], 0
004299AC   |. 6A 02          PUSH    2
004299AE   |. 53             PUSH    EBX
004299AF   |. 57             PUSH    EDI
004299B0   |. E8 B7340800    CALL    TENSHOU.004ACE6C
004299B5   |. 83C4 0C        ADD     ESP, 0C
004299B8   |. 8BF8           MOV     EDI, EAX
004299BA   |> 837D F0 00     CMP     DWORD PTR SS:[EBP-10], 0
004299BE   |.^0F84 5FFFFFFF  JE      TENSHOU.00429923
004299C4   |. FF75 E4        PUSH    DWORD PTR SS:[EBP-1C]
004299C7   |. FF75 08        PUSH    DWORD PTR SS:[EBP+8]
004299CA   |. E8 0BB60700    CALL    TENSHOU.004A4FDA
004299CF   |. 83C4 04        ADD     ESP, 4
004299D2   |. 50             PUSH    EAX                              ; /Arg3
004299D3   |. FF75 0C        PUSH    DWORD PTR SS:[EBP+C]             ; |Arg2
004299D6   |. FF75 E8        PUSH    DWORD PTR SS:[EBP-18]            ; |Arg1
004299D9   |. E8 A12A0400    CALL    TENSHOU.0046C47F                 ; \TENSHOU.0046C47F
004299DE   |. 83C4 0C        ADD     ESP, 0C
004299E1   |. 8D8D 4CFFFFFF  LEA     ECX, DWORD PTR SS:[EBP-B4]       ; |
004299E7   |. 50             PUSH    EAX                              ; |Arg1
004299E8   |. E8 9DB20700    CALL    TENSHOU.004A4C8A                 ; \TENSHOU.004A4C8A
004299ED   |. 6A 02          PUSH    2
004299EF   |. E8 3697FFFF    CALL    TENSHOU.0042312A
004299F4   |. 83C4 04        ADD     ESP, 4
004299F7   |. 8D8D 4CFFFFFF  LEA     ECX, DWORD PTR SS:[EBP-B4]
004299FD   |. E8 363A0500    CALL    TENSHOU.0047D438
00429A02   |. C745 FC FFFFFF>MOV     DWORD PTR SS:[EBP-4], -1
00429A09   |. E8 1E000000    CALL    TENSHOU.00429A2C
00429A0E   |. 8B45 EC        MOV     EAX, DWORD PTR SS:[EBP-14]
00429A11   |. 8B4D F4        MOV     ECX, DWORD PTR SS:[EBP-C]
00429A14   |. 64:890D 000000>MOV     DWORD PTR FS:[0], ECX
00429A1B   |. 5F             POP     EDI
00429A1C   |. 5E             POP     ESI
00429A1D   |. 5B             POP     EBX
00429A1E   |. 8BE5           MOV     ESP, EBP
00429A20   |. 5D             POP     EBP
00429A21   \. C3             RETN
*/


/* �m�o�b�@�o�b�@����
004298A3   |. 5D             POP     EBP
004298A4   |. F7D8           NEG     EAX
004298A6   |. 5F             POP     EDI
004298A7   |. 5E             POP     ESI
004298A8   |. 5B             POP     EBX
004298A9   \. C3             RETN			�� ���̒i�K��EAX��1�Ȃ�o�p�����A0�Ȃ�o�p���s
*/

/*
004297D8   /$ 53             PUSH    EBX
004297D9   |. 56             PUSH    ESI
004297DA   |. 57             PUSH    EDI
004297DB   |. 55             PUSH    EBP
004297DC   |. FF7424 14      PUSH    DWORD PTR SS:[ESP+14]
004297E0   |. E8 F5B70700    CALL    TENSHOU.004A4FDA			�� ���̒l�̂`�w�����f���鑤�̑喼�̕����ԍ��Ǝv����B
004297E5   |. 66:8B7C24 1C   MOV     DI, WORD PTR SS:[ESP+1C] 
004297EA   |. 83C4 04        ADD     ESP, 4
004297ED   |. 66:8BD8        MOV     BX, AX							�i�����ł`�w���a�w�ɃR�s�[���Ă���̂Œ��Ӂj
*/

int iDaimyoBushouIDOfPostBattleTreatmentPeriod = -1; // �ߗ��������鑤�̑喼�̕����ԍ�(+1������)





//-------------------------------------------���f���鑤�̑喼�̔ԍ��̕ێ�--------------------------------


/*
004297DC   |. FF7424 14      PUSH    DWORD PTR SS:[ESP+14]
004297E0   |. E8 F5B70700    CALL    TENSHOU.004A4FDA			�� ���̒l�̂`�w�����f���鑤�̑喼�̕����ԍ��Ǝv����B
				�������� JMP TSMod.OnTSExeDaimyoBushouIDOfPostBattleTreatment �Ə��������Ă����ۂ̏���
004297E5   |. 66:8B7C24 1C   MOV     DI, WORD PTR SS:[ESP+1C] 
004297EA   |. 83C4 04        ADD     ESP, 4
*/
int pTenshouExeJumpFromToOnTenshouExeDaimyoBushouIDOfPostBattleTreatment	 =0x4297E0; // �֐��͂��̃A�h���X����AOnTenshouExeDaimyoBushouIDOfPostBattleTreatment�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeDaimyoBushouIDOfPostBattleTreatment =0x4A4FDA; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeDaimyoBushouIDOfPostBattleTreatment  =0x4297E5; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeDaimyoBushouIDOfPostBattleTreatment() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {
		// ���XTENSHOU.EXE���ɂ��������̂�������
		call pTenshouExeJumpCallFromToOnTenshouExeDaimyoBushouIDOfPostBattleTreatment

		mov iDaimyoBushouIDOfPostBattleTreatmentPeriod, EAX	// ���̒l�̂`�w�̕������������f���鑤�̑喼�̕����ԍ��B�����ۑ����Ă����B

		jmp pTenshouExeReturnLblFromOnTenshouExeDaimyoBushouIDOfPostBattleTreatment
	}
}

char cmdOnTenshouExeJumpFromDaimyoBushouIDOfPostBattleTreatment[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeDaimyoBushouIDOfPostBattleTreatment() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeDaimyoBushouIDOfPostBattleTreatment;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeDaimyoBushouIDOfPostBattleTreatment + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeDaimyoBushouIDOfPostBattleTreatment  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromDaimyoBushouIDOfPostBattleTreatment+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeDaimyoBushouIDOfPostBattleTreatment), cmdOnTenshouExeJumpFromDaimyoBushouIDOfPostBattleTreatment, 5, NULL); //5�o�C�g�̂ݏ�������
}






//-------------------------------------------���f���̓o�p�̐���--------------------------------
extern BOOL PostBattleTreatmentPeriod; // �O���[�o���t���O�B�ߗ������̃^�C�~���O

int iFlagOfSuccessOrFailOfPostBattleTreatment = -1;	// �o�p�̐��ہB 1�������B0�����s�B
int iTreatmentBushouOffSuccessOrFailOfPostBattleTreatment = -1; // ���f�����̕����ԍ�(1������)
void OnTenshouExeSuccessOrFailOfPostBattleTreatmentExecute() {

	// ���f����鑤�̕���ID
	int iTreatmenBushouID = (iTreatmentBushouOffSuccessOrFailOfPostBattleTreatment & 0xFFFF) - 1;

	// ���f���鑤�̑叫�̕���ID(�喼����Ȃ�)
	int iDaimyouBushouID = (iDaimyoBushouIDOfPostBattleTreatmentPeriod & 0xFFFF) - 1;

	// �푈��̏�����
	if ( PostBattleTreatmentPeriod ) {
		// �������L���͈͂ɓ����Ă���B
		if ( 0 <= iTreatmenBushouID && iTreatmenBushouID < GAMEDATASTRUCT_BUSHOU_NUM && 
			 0 <= iDaimyouBushouID && iDaimyouBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

			// ScenarioMod�̃J�X�^�������ŕߔ������o�p�ۂ�ݒ肷��
			if ( p_snOnCustomCondition ) {															                        //����������鑤    ��������(�ʏ�喼)		�� ���ݗ\�肳��Ă��鐬�ی���(1�������B0�����s)
				int isTouyouSuccessOrFail = ((PFNSNONCUSTOMCONDITION) p_snOnCustomCondition)("AS����::�푈::�ߗ������o�p��", iTreatmenBushouID+1, iDaimyouBushouID+1, iFlagOfSuccessOrFailOfPostBattleTreatment, "", "", 0.0, 0.0);
				// �ߔ������o�p�ی��ʂ��ύX�ƂȂ��Ă���B
				if ( isTouyouSuccessOrFail != CUSTOM_CONDITION_NOMODIFY ) {
					iFlagOfSuccessOrFailOfPostBattleTreatment = isTouyouSuccessOrFail;
				}
			}
		}
	}
}


/*
004298A3   |. 5D             POP     EBP
004298A4   |. F7D8           NEG     EAX
				�������� JMP TSMod.OnTSExeSuccessOrFailOfPostBattleTreatment �Ə��������Ă����ۂ̏���
004298A6   |. 5F             POP     EDI
004298A7   |. 5E             POP     ESI
004298A8   |. 5B             POP     EBX
004298A9   \. C3             RETN			�� ���̒i�K��EAX��1�Ȃ�o�p�����A0�Ȃ�o�p���s
*/
int pTenshouExeJumpFromToOnTenshouExeSuccessOrFailOfPostBattleTreatment	    =0x04298A4; // �֐��͂��̃A�h���X����AOnTenshouExeSuccessOrFailOfPostBattleTreatment�ւƃW�����v���Ă���B
int pTenshouExeReturnLblFromOnTenshouExeSuccessOrFailOfPostBattleTreatment  =0x4298A9; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeSuccessOrFailOfPostBattleTreatment() {
	// �X�^�b�N�ɂ��߂Ă���
	// �X�^�b�N�ɂ��߂Ă���
	__asm {
		// �������炪���XTenshou.EXE�ɂ���������
		NEG     EAX
		POP     EDI
		POP     ESI
		POP     EBX

		mov iFlagOfSuccessOrFailOfPostBattleTreatment, EAX // ���ۂ̌��݂̌��ʂ����Ă����B1���o�p�����B0���o�p���s
		mov iTreatmentBushouOffSuccessOrFailOfPostBattleTreatment, EDI // ���f����鑤�̕����ԍ��B

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
	OnTenshouExeSuccessOrFailOfPostBattleTreatmentExecute();

	__asm {

		pop edi
		pop esi
		pop ebp
		pop esp
		pop edx
		pop ecx
		pop ebx
		pop eax

		mov EAX, iFlagOfSuccessOrFailOfPostBattleTreatment // ���f�̐��ۂ��㏑������B

		jmp pTenshouExeReturnLblFromOnTenshouExeSuccessOrFailOfPostBattleTreatment
	}
}




char cmdOnTenshouExeJumpFromSuccessOrFailOfPostBattleTreatment[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeSuccessOrFailOfPostBattleTreatment() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeSuccessOrFailOfPostBattleTreatment;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeSuccessOrFailOfPostBattleTreatment + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeSuccessOrFailOfPostBattleTreatment  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromSuccessOrFailOfPostBattleTreatment+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeSuccessOrFailOfPostBattleTreatment), cmdOnTenshouExeJumpFromSuccessOrFailOfPostBattleTreatment, 5, NULL); //5�o�C�g�̂ݏ�������
}




