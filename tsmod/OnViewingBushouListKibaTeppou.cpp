#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"


/*
00439237   . E8 1B10FDFF    CALL TENSHOU.0040A257
0043923C   . 83C4 04        ADD ESP,4
0043923F   . 50             PUSH EAX
00439240   . E8 6948FDFF    CALL TENSHOU.0040DAAE �����̌��ʂŕ�����ւ̃|�C���^��EAX�ɓ���B����40DAAE���͔̂ėp�̊֐��Ȃ̂Œ��g������Ȃ����ƁB
													EAX 004D67EC ASCII "  ��"
00439245   . 83C4 08        ADD     ESP, 8



0040DAAE  /$ 56             PUSH ESI                                                 ;  TENSHOU.004E66E8
0040DAAF  |. E8 C5F6FFFF    CALL TENSHOU.0040D179
0040DAB4  |. 8BF0           MOV ESI,EAX
0040DAB6  |. 8B4424 08      MOV EAX,DWORD PTR SS:[ESP+8]
0040DABA  |. 83F8 02        CMP EAX,2
0040DABD  |. 72 07          JB SHORT TENSHOU.0040DAC6
0040DABF  |. B8 4CB94B00    MOV EAX,TENSHOU.004BB94C                                 ;  ASCII "�|"
0040DAC4  |. EB 07          JMP SHORT TENSHOU.0040DACD
0040DAC6  |> 8B0485 A8134D0>MOV EAX,DWORD PTR DS:[EAX*4+4D13A8]
0040DACD  |> 50             PUSH EAX
0040DACE  |. FF7424 10      PUSH DWORD PTR SS:[ESP+10]
0040DAD2  |. 68 48B94B00    PUSH TENSHOU.004BB948                                    ;  ASCII "%*s"
0040DAD7  |. 56             PUSH ESI
0040DAD8  |. E8 4B000A00    CALL TENSHOU.004ADB28
0040DADD  |. 83C4 10        ADD ESP,10
0040DAE0  |. 8BC6           MOV EAX,ESI
0040DAE2  |. 5E             POP ESI
0040DAE3  \. C3             RETN
*/

// ���ꕔ�����𓾂�
extern char* getSpecialHeitaiStringOfBushou(int iBushouID);

int iPointerOfViewingBushouListKibaTeppou = NULL; // ���ꕔ��������ւ̃|�C���^
int iBushouIDOfViewingBushouListKibaTeppou = -1; // �ΏۂƂȂ镐���̂h�c�BECX�̂�����������CX�����g��

void OnTenshouExeViewingBushouListKibaTeppouExecute() {
	iBushouIDOfViewingBushouListKibaTeppou = (iBushouIDOfViewingBushouListKibaTeppou & 0xFFFF);
	int iBushouID = iBushouIDOfViewingBushouListKibaTeppou-1;

	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
		// ����ȕ��������擾
		char *p = getSpecialHeitaiStringOfBushou(iBushouID);

		iPointerOfViewingBushouListKibaTeppou = (int)p; // ���̃A�h���X��n���B
	}
}


/*
00439237   . E8 1B10FDFF    CALL TENSHOU.0040A257
0043923C   . 83C4 04        ADD ESP,4
0043923F   . 50             PUSH EAX
00439240   . E8 6948FDFF    CALL TENSHOU.0040DAAE �����̌��ʂŕ�����ւ̃|�C���^��EAX�ɓ���B����40DAAE���͔̂ėp�̊֐��Ȃ̂Œ��g������Ȃ����ƁB
													EAX 004D67EC ASCII "  ��"
				�������� JMP TSMod.OnTSExeViewingBushouListKibaTeppou �Ə��������Ă����ۂ̏���
00439245   . 83C4 08        ADD     ESP, 8

*/
int pTenshouExeJumpFromToOnTenshouExeViewingBushouListKibaTeppou	 =0x439240; // �֐��͂��̃A�h���X����AOnTenshouExeViewingBushouListKibaTeppou�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeViewingBushouListKibaTeppou =0x40DAAE; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeViewingBushouListKibaTeppou	 =0x439245; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeViewingBushouListKibaTeppou() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {
		// �����h�c���m��
		mov iBushouIDOfViewingBushouListKibaTeppou, ecx

			// ���XTENSHOU.EXE���ɂ��������̂�������
		call pTenshouExeJumpCallFromToOnTenshouExeViewingBushouListKibaTeppou

		// ���X�̂��̂�ۑ����Ă����B
		mov iPointerOfViewingBushouListKibaTeppou, eax

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
	OnTenshouExeViewingBushouListKibaTeppouExecute();

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

		// �R�n�S�C�́A���Ƃ��~���A�g�����ꂽ������ŏ㏑������B
		mov eax, iPointerOfViewingBushouListKibaTeppou

		jmp pTenshouExeReturnLblFromOnTenshouExeViewingBushouListKibaTeppou
	}
}

/*
0049902C  |. 8BCE           MOV     ECX, ESI
0049902E  |. 68 B8334D00    PUSH    TENSHOU.004D33B8                 ;  ASCII "��ʃT�C�Y��I�����Ă�������"
00499033  |. E8 16550100    CALL    TENSHOU.004AE54E
				�������� JMP TSMod.OnTSExeViewingBushouListKibaTeppou �Ə��������Ă����ۂ̏���
00499038  |. 5E             POP     ESI
*/
char cmdOnTenshouExeJumpFromViewingBushouListKibaTeppou[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeViewingBushouListKibaTeppou() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeViewingBushouListKibaTeppou;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeViewingBushouListKibaTeppou + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeViewingBushouListKibaTeppou  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromViewingBushouListKibaTeppou+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeViewingBushouListKibaTeppou), cmdOnTenshouExeJumpFromViewingBushouListKibaTeppou, 5, NULL); //5�o�C�g�̂ݏ�������
}






