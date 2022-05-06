#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"

// ������P�p�̍�����O���ҏW���ɁA�����������

/*
�ȉ��A����������肵����


00407CB4  |. E8 1E3C0700    CALL    TENSHOU.0047B8D7
00407CB9  |. 8945 F0        MOV     DWORD PTR SS:[EBP-10], EAX   �� ����EAX�Ɋ�ԍ��������Ă��邪�A
																	�{���͈ȉ��̂悤�ɐ擪��0b11�Ƃ����`�ŃZ�[�u�f�[�^�ɓ����Ă��邪�A���̒i�K�ł͍ŏ���0b10�Ƃ����悤�ɂP�t���O�������Ȃ��B

													���Z�[�u�f�[�^�⃁�����̍�����ԍ�

															  ����B���オ0x0�@�E����0xF 1�͍���̂P���B
�@�@�@�@�@													  ��
													0b1100000[1111][000][00] 
�@�@�@�@�@													  ���@�@ �� �� �s�̒��łS�̂ǂ�Ȃ̂�
�@�@�@�@�@													  ��16�� ���ҏ��E�m���Ȃǂ̍s


														  
													�����̒i�K��EAX�̊�ԍ�(�ŏ���0b10�ƂP�r�b�g�������ĂȂ�)

															  ����B���オ0x0�@�E����0xF 1�͍���̂P���B
�@�@�@�@�@													  ��
													0b1000000[1111][000][00] 
�@�@�@�@�@													  ���@�@ �� �� �s�̒��łS�̂ǂ�Ȃ̂�
�@�@�@�@�@													  ��16�� ���ҏ��E�m���Ȃǂ̍s

*/


// �������������Ɗ炪�o�Ȃ��ԍ��B����Ă����̔ԍ��͎g���Ă͂Ȃ�Ȃ�
extern DDRAW_HIMEKAO_BUG_MAPPING nb6ddraw_himekao_bug[10];


int iFaceIdOffGeneralKaoFaceExecute = -1;
int iExterniFaceIdOffGeneralKaoFaceExecuteNormalizedFaceID = -1;
void OnTenshouExeDecideOfGeneralKaoFaceExecute() {
	int iNormalizedFaceID = iFaceIdOffGeneralKaoFaceExecute | 0x4000; // 0b[010][00][00][0000][00000] �Ƃ�or

	// �ʖڊ炩�ǂ������`�F�b�N
	for ( int i=0; i < sizeof(nb6ddraw_himekao_bug)/sizeof(nb6ddraw_himekao_bug[0]); i++ ) {
		// �����_����ƈ�v�����Ȃ�A�����Ƃ��߂�������ɒ���
		if ( iNormalizedFaceID == nb6ddraw_himekao_bug[i].wrong ) {
			iNormalizedFaceID = nb6ddraw_himekao_bug[i].right;
			iFaceIdOffGeneralKaoFaceExecute = iNormalizedFaceID & 0xbfff; // 0b[101][11][11][1111][11111] �Ƃ�and 
			break;
		}
	}

	iExterniFaceIdOffGeneralKaoFaceExecuteNormalizedFaceID = iNormalizedFaceID;
	// OutputDebugStream("%d", iNormalizedFaceID);
}


/*
00407CB4  |. E8 1E3C0700    CALL    TENSHOU.0047B8D7
				�������� JMP TSMod.OnTSExeDecideOfGeneralKaoFace �Ə��������Ă����ۂ̏���
00407CB9  |. 8945 F0        MOV     DWORD PTR SS:[EBP-10], EAX   �� ����EAX�Ɋ�ԍ��������Ă��邪�A
*/
int pTenshouExeJumpFromToOnTenshouExeDecideOfGeneralKaoFace		=0x407CB4; // �֐��͂��̃A�h���X����AOnTenshouExeDecideOfGeneralKaoFace�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeDecideOfGeneralKaoFace =0x47B8D7; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeDecideOfGeneralKaoFace	=0x407CB9; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeDecideOfGeneralKaoFace() {

	// �X�^�b�N�ɂ��߂Ă���
	__asm {

		// ���XTENSHOU.EXE���ɂ��������̂�������
		call pTenshouExeJumpCallFromToOnTenshouExeDecideOfGeneralKaoFace

		// �I��������ԍ���ۑ�
		mov iFaceIdOffGeneralKaoFaceExecute, eax

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
	OnTenshouExeDecideOfGeneralKaoFaceExecute();

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

		// �ʖڊ����������ꍇ������̂Ō��ʂ��㏑��
		mov eax, iFaceIdOffGeneralKaoFaceExecute

		jmp pTenshouExeReturnLblFromOnTenshouExeDecideOfGeneralKaoFace
	}
}

/*
0049D7AA  |. 6A 03          |PUSH    3                               ; |Arg3 = 00000003
0049D7AC  |. 56             |PUSH    ESI                             ; |Arg2
0049D7AD  |. 56             |PUSH    ESI                             ; |Arg1
0049D7AE  |. E8 D3540000    |CALL    TENSHOU.004A2C86                ; \TENSHOU.004A2C86
				�������� JMP TSMod.OnTSExeDecideOfGeneralKaoFace �Ə��������Ă����ۂ̏���
0049D7B3  |. 83C4 1C        |ADD     ESP, 1C
0049D7B6  |. 85C0           |TEST    EAX, EAX
*/
char cmdOnTenshouExeJumpFromDecideOfGeneralKaoFace[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeDecideOfGeneralKaoFace() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeDecideOfGeneralKaoFace;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeDecideOfGeneralKaoFace + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeDecideOfGeneralKaoFace  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromDecideOfGeneralKaoFace+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeDecideOfGeneralKaoFace), cmdOnTenshouExeJumpFromDecideOfGeneralKaoFace, 5, NULL); //5�o�C�g�̂ݏ�������
}




