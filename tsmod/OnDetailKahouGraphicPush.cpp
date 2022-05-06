
#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"



/*
�ƕ�ڍ׉�ʂ̃��C�A�E�g
004949E8  |. 6A 30          PUSH    30
004949EA  |. E8 16680100    CALL    TENSHOU.004AB205
004949EF  |. 50             PUSH    EAX                              ; /Arg5
004949F0  |. 8BCB           MOV     ECX, EBX                         ; |
004949F2  |. 6A 40          PUSH    40                               ; |Arg4 = 00000040   �� 64�s�N�Z��
004949F4  |. 6A 40          PUSH    40                               ; |Arg3 = 00000040   �� 64�s�N�Z��
004949F6  |. 6A 24          PUSH    24                               ; |Arg2 = 00000024   �� x���W
004949F8  |. 6A 10          PUSH    10                               ; |Arg1 = 00000010   �� y���W
004949FA  |. E8 EA960100    CALL    TENSHOU.004AE0E9                 ; \TENSHOU.004AE0E9  �� �������ƕ�A�C�R���v�b�V��
*/

/*
�܂��A�ƕ�̔ԍ��ƃA�C�R�����v�b�V�������B
���̎��A���A���摜�ƕ󂪂���΁A40,40��0,0�ɂ���̂��ǂ�
004949EF  |. 50             PUSH    EAX                              ; /Arg5
004949F0  |. 8BCB           MOV     ECX, EBX                         ; |
004949F2  |. 6A 40          PUSH    40                               ; |Arg4 = 00000040
004949F4  |. 6A 40          PUSH    40                               ; |Arg3 = 00000040
004949F6  |. 6A 24          PUSH    24                               ; |Arg2 = 00000024
004949F8  |. 6A 10          PUSH    10                               ; |Arg1 = 00000010
004949FA  |. E8 EA960100    CALL    TENSHOU.004AE0E9                 ; \TENSHOU.004AE0E9


���ɁA���Ȃ炸�A�ǂ��ɕ`�������̂��̏��ƂȂ�B����ɂ��A�_�C�A���O�̈ʒu���킩��B
00449906  |. 68 50FD4D00    PUSH    TENSHOU.004DFD50                 ; /pRect2 = 004DFD50 {208.,136.,528.,336.} �� �������͂悭�킩���B�`��͈͂��H
0044990B  |. 894D F4        MOV     DWORD PTR SS:[EBP-C], ECX        ; |
0044990E  |. 034D 14        ADD     ECX, DWORD PTR SS:[EBP+14]       ; |
00449911  |. BB 08FF4D00    MOV     EBX, TENSHOU.004DFF08            ; |
00449916  |. 8945 F8        MOV     DWORD PTR SS:[EBP-8], EAX        ; |
00449919  |. 894D FC        MOV     DWORD PTR SS:[EBP-4], ECX        ; |
0044991C  |. 8D45 F0        LEA     EAX, DWORD PTR SS:[EBP-10]       ; |
0044991F  |. 50             PUSH    EAX                              ; |pRect1 = 0018FAA8 {224.,172.,288.,236.} �� �����炪�ƕ�̕`�悷�ׂ����W
00449920  |. 53             PUSH    EBX                              ; |pRectDst => TENSHOU.004DFF08
00449921  |. FF15 F4754E00  CALL    DWORD PTR DS:[<&USER32.Intersect>; \IntersectRect
*/

extern char* getExistMovieKahouFilePath( int iKahouID );
extern char* getExistPictureKahouFilePath( int iKahouID );
extern void getAviPixelSize( char *szKahouName, int *width, int *height );
extern void getBmpPixelSize( char *szKahouName, int *width, int *height );


/*
 * �ƕ�̃O���t�B�b�N���v�b�V�����鎞
 */

int iCurKahouIDOnGraphicPush = 0L;
BOOL iCurKahouIs24bitGraphic = FALSE;
void OnTenshouExeDetailKahouGraphicPushExecute() {

	iCurKahouIs24bitGraphic = FALSE;

	// esi�̒l����A�ƕ󂪓����Ă���ꏊ��˂��~�߁A���̒l��iCurKahouIDOnGraphicPush�ɉ��߂ē����B
	// MOV     EBP, DWORD PTR DS:[ESI+70] �Ɠ������Ƃ���낤�Ƃ����̂��B
	char *p = (char *)iCurKahouIDOnGraphicPush + 0x70;
	iCurKahouIDOnGraphicPush = *p;
	iCurKahouIDOnGraphicPush--; // �ԍ����h�c��

	if ( 0 <= iCurKahouIDOnGraphicPush && iCurKahouIDOnGraphicPush < GAMEDATASTRUCT_KAHOU_NUM ) {
		// �ΏۂƂȂ�ƕ󂪃��[�r�[���ǂ����`�F�b�N
		char *p = getExistMovieKahouFilePath( iCurKahouIDOnGraphicPush );
		// ����̓��[�r�[���B
		if ( p ) {
			iCurKahouIs24bitGraphic = TRUE;
		}

		p = getExistPictureKahouFilePath( iCurKahouIDOnGraphicPush );
		// ����͉摜���B
		if ( p ) {
			iCurKahouIs24bitGraphic = TRUE;
		}
	}
}


/*
004949EA  |. E8 16680100    CALL    TENSHOU.004AB205
				�������� JMP TSMod.OnTSExeDetailKahouGraphicPush �Ə��������Ă����ۂ̏���
004949EF  |. 50             PUSH    EAX                              ; /Arg5
004949F0  |. 8BCB           MOV     ECX, EBX                         ; |
004949F2  |. 6A 40          PUSH    40                               ; |Arg4 = 00000040   �� 64�s�N�Z��
004949F4  |. 6A 40          PUSH    40                               ; |Arg3 = 00000040   �� 64�s�N�Z��
004949F6  |. 6A 24          PUSH    24                               ; |Arg2 = 00000024   �� x���W
*/
int pTenshouExeJumpFromToOnTenshouExeDetailKahouGraphicPush		=0x4949EA; // �֐��͂��̃A�h���X����AOnTenshouExeDetailKahouGraphicPush�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeDetailKahouGraphicPush =0x4AB205; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeDetailKahouGraphicPush	=0x4949F6; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeDetailKahouGraphicPush() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {
		mov iCurKahouIDOnGraphicPush, esi // �ƕ�h�c�֘A�A�h���X���R�s�[

		// ���XTENSHOU.EXE���ɂ��������̂�������
		call pTenshouExeJumpCallFromToOnTenshouExeDetailKahouGraphicPush

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
	OnTenshouExeDetailKahouGraphicPushExecute();

	if ( iCurKahouIs24bitGraphic ) {

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

			// tenshou.exe�ɂ����������B24bit�ƕ��\������ۂ́A���̉摜��height��0�Ƃ���B�����ケ��Ō����Ȃ�
			push eax
			mov ecx, ebx
			push 0x0				// height ��0�ɂ���B
			push 0x40				// width ��64px�̂܂�

			jmp pTenshouExeReturnLblFromOnTenshouExeDetailKahouGraphicPush
		}
	}

	else {
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

			// tenshou.exe�ɂ����������B
			push eax
			mov ecx, ebx
			push 0x40				// height ��0�ɂ���B
			push 0x40				// width ��64px�̂܂�

			jmp pTenshouExeReturnLblFromOnTenshouExeDetailKahouGraphicPush
		}
	}
}

/*
004538DA  |. FF75 08        PUSH    DWORD PTR SS:[EBP+8]
004538DD  |. E8 DC110400    CALL    TENSHOU.00494ABE
004538E2  |. E8 F07F0200    CALL    TENSHOU.0047B8D7
				�������� JMP TSMod.OnTSExeDetailKahouGraphicPush �Ə��������Ă����ۂ̏���
004538E7  |. 8D8D 7CFFFFFF  LEA     ECX, DWORD PTR SS:[EBP-84]
004538ED  |. 8935 80154D00  MOV     DWORD PTR DS:[4D1580], ESI
*/
char cmdOnTenshouExeJumpFromDetailKahouGraphicPush[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeDetailKahouGraphicPush() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeDetailKahouGraphicPush;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeDetailKahouGraphicPush + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeDetailKahouGraphicPush  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromDetailKahouGraphicPush+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeDetailKahouGraphicPush), cmdOnTenshouExeJumpFromDetailKahouGraphicPush, 5, NULL); //5�o�C�g�̂ݏ�������
}




