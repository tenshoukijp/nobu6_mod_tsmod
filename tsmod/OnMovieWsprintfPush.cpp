#include "WinTarget.h"

#include <sys/types.h>
#include <sys/stat.h>

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"

/*
�g�p���@�́A
004B4F73   . 6A 01          PUSH    1
004B4F75   . 6A 21          PUSH    21
004B4F77   . E8 E87DF8FF    CALL    TENSHOU.0043CD64

�݂����Ȋ����̂悤���B


���[�r�[�̍Đ��֐��́A�ȉ����X�^�[�g�Ǝv����B
0043CD64  /$ 55             PUSH    EBP
0043CD65  |. 33C0           XOR     EAX, EAX
0043CD67  |. 8BEC           MOV     EBP, ESP
0043CD69  |. 81EC 18010000  SUB     ESP, 118
0043CD6F  |. 53             PUSH    EBX
0043CD70  |. 8945 EC        MOV     DWORD PTR SS:[EBP-14], EAX
0043CD73  |. 56             PUSH    ESI
0043CD74  |. 8945 F0        MOV     DWORD PTR SS:[EBP-10], EAX
0043CD77  |. C745 F4 400100>MOV     DWORD PTR SS:[EBP-C], 140
0043CD7E  |. C745 F8 F00000>MOV     DWORD PTR SS:[EBP-8], 0F0
0043CD85  |. 57             PUSH    EDI
0043CD86  |. 8B7D 08        MOV     EDI, DWORD PTR SS:[EBP+8]
0043CD89  |. 3BF8           CMP     EDI, EAX
0043CD8B  |. 0F8E A1010000  JLE     TENSHOU.0043CF32
0043CD91  |. 83FF 24        CMP     EDI, 24
0043CD94  |. 0F8D 98010000  JGE     TENSHOU.0043CF32
0043CD9A  |. 83FF 01        CMP     EDI, 1
0043CD9D  |. 7C 05          JL      SHORT TENSHOU.0043CDA4
0043CD9F  |. 83FF 03        CMP     EDI, 3
0043CDA2  |. 7E 21          JLE     SHORT TENSHOU.0043CDC5
0043CDA4  |> 833D 38C24B00 >CMP     DWORD PTR DS:[4BC238], 0
0043CDAB  |. 0F84 81010000  JE      TENSHOU.0043CF32
0043CDB1  |. E8 41A20200    CALL    TENSHOU.00466FF7
0043CDB6  |. 85C0           TEST    EAX, EAX
0043CDB8  |. 0F85 74010000  JNZ     TENSHOU.0043CF32
0043CDBE  |. BE 01000000    MOV     ESI, 1
0043CDC3  |. EB 02          JMP     SHORT TENSHOU.0043CDC7
0043CDC5  |> 33F6           XOR     ESI, ESI
0043CDC7  |> BB 80954C00    MOV     EBX, TENSHOU.004C9580            ;  ASCII "T:\"
0043CDCC  |> 6A 01          /PUSH    1                               ; /ErrorMode = SEM_FAILCRITICALERRORS
0043CDCE  |. FF15 18754E00  |CALL    DWORD PTR DS:[<&KERNEL32.SetErr>; \SetErrorMode
0043CDD4  |. 53             |PUSH    EBX                             ; /Arg1
0043CDD5  |. 8945 FC        |MOV     DWORD PTR SS:[EBP-4], EAX       ; |
0043CDD8  |. E8 1CCD0200    |CALL    TENSHOU.00469AF9                ; \TENSHOU.00469AF9
0043CDDD  |. 83C4 04        |ADD     ESP, 4
0043CDE0  |. 85C0           |TEST    EAX, EAX
0043CDE2  |. 74 30          |JE      SHORT TENSHOU.0043CE14
0043CDE4  |. FF34BD 88954C0>|PUSH    DWORD PTR DS:[EDI*4+4C9588]     ; /<%s>
0043CDEB  |. 53             |PUSH    EBX                             ; |<%s>
0043CDEC  |. 68 17F4C001    |PUSH    1C0F417                         ; |Format = 01C0F417 ???




*/

char szExistDirName[5]= ".\\";

// �^�[�Q�b�g���[�r�[�̃|�C���^�������Ă���x�[�X�̏ꏊ(0x4C9588����̍���)
int iTargetMoviePointerIX = 0;
// �^�[�Q�b�g���[�r�[�ւ̃|�C���^�̒l�������Ă���A�h���X
int *pTargetMovieAddress = NULL;
char *pszTargetMovieName = NULL;

char *pszOriginalDirName = NULL;

void OnTenshouExeMovieWsprintfPushExecute() {

	struct stat  st;

	// 0043CDE4  |. FF34BD 88954C00     |PUSH    DWORD PTR DS:[EDI*4+4C9588]     ; /<%s>
	// �ΏۂƂȂ郀�[�r�[�̖��O�ւ̃|�C���^�������Ă���A�h���X���v�Z���ē���B
	// ��̃A�Z���u���̂����AEDI*4+4C9588�@�Ƃ���[ ] �𔲂�����Ԃ̒l
	pTargetMovieAddress = (int *)(iTargetMoviePointerIX*4 + 0x4C9588);

	// ���̃A�h���X�ɂ��钆�g��char *�Ƃ��ĕ]���������̂��A�Y���̃��[�r�[�̏ꏊ�ł���B
	// ���Ȃ킿[EDI*4+4C9588]�Ƃ���[ ]��t�������̂Ɠ���������B
	pszTargetMovieName = (char *)(*pTargetMovieAddress);

	// �f�B���N�g���t���̑��΂̃t���l�[����
	char szMovieName[256] = "";
	wsprintf(szMovieName, "MOVIE\\%s.avi", pszTargetMovieName);

	// ���̃��[�r�[�̓n�[�h�f�B�X�N�ɑ��݂��邩
	if (stat(szMovieName, &st) == 0) {
		// ���݂���
		// ���݂����ꍇ�́ACD����ł͂Ȃ��A���΂���ɕύX����B
		pszOriginalDirName = szExistDirName;

		// endlogo������B
		if ( stat("MOVIE\\ENDLOGO.avi", &st) == 0 ) {
			AddModuleItem( (HMODULE)OnTenshouExeMovieWsprintfPushExecute+5, "movie && logo"); //OnTenshouExeMovieWsprintfPushExecute+5�Ƃ������l�ɈӖ��͂Ȃ��B���j�[�N�ɂ��邽�߂����B
		// endlogo�͂Ȃ��B
		} else {
			AddModuleItem( (HMODULE)OnTenshouExeMovieWsprintfPushExecute+5, "movie"); //OnTenshouExeMovieWsprintfPushExecute+5�Ƃ������l�ɈӖ��͂Ȃ��B���j�[�N�ɂ��邽�߂����B
		}
	} else {
		// ���݂��Ȃ�
	}
}

/*
0043CDE4  |. FF34BD 88954C00     |PUSH    DWORD PTR DS:[EDI*4+4C9588]     ; /<%s>
0043CDEB     53                  PUSH    EBX
0043CDEC  |. 68 18964C00         |PUSH    TENSHOU.004C9618                ; |Format = ".\Movie\%s.avi"
				�������� JMP TSMod.OnTSExeMovieWsprintfPush �Ə��������Ă����ۂ̏���
0043CDF1  |. 8D8D E8FEFFFF       |LEA     ECX, DWORD PTR SS:[EBP-118]     ; |
0043CDF7  |. 51                  |PUSH    ECX                             ; |s
0043CDF8  |. FF15 F0754E00       |CALL    DWORD PTR DS:[<&USER32.wsprintf>; \wsprintfA
*/
int pTenshouExeJumpFromToOnTenshouExeMovieWsprintfPush		=0x43CDEC; // �֐��͂��̃A�h���X����AOnTenshouExeMovieWsprintfPush�ւƃW�����v���Ă���B
int pTenshouExeJumpPushFromToOnTenshouExeMovieWsprintfPush  =0x4C9618; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeMovieWsprintfPush	=0x43CDF1; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeMovieWsprintfPush() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {

		mov iTargetMoviePointerIX, edi

		// �f�B���N�g���̖��O�̕�����ւ̃|�C���^
		mov pszOriginalDirName, ebx

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
	OnTenshouExeMovieWsprintfPushExecute();

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

		// �P��TENSHOU.EXE����Push�������͂�߂�
		pop ebx

		// �ւ��ɏ󋵂��݂Čv�Z�����f�B���N�g����������(���̂܂܂Ȃ̂��A�.\��ɒu��������̂�)
		push pszOriginalDirName

		// ���XTENSHOU.EXE���ɂ��������̂�������
		push pTenshouExeJumpPushFromToOnTenshouExeMovieWsprintfPush

		jmp pTenshouExeReturnLblFromOnTenshouExeMovieWsprintfPush
	}
}

/*
0043CDE4  |. FF34BD 88954C00     |PUSH    DWORD PTR DS:[EDI*4+4C9588]     ; /<%s>
0043CDEB     53                  PUSH    EBX
0043CDEC  |. 68 18964C00         |PUSH    TENSHOU.004C9618                ; |Format = ".\Movie\%s.avi"
				�������� JMP TSMod.OnTSExeMovieWsprintfPush �Ə��������Ă����ۂ̏���
0043CDF1  |. 8D8D E8FEFFFF       |LEA     ECX, DWORD PTR SS:[EBP-118]     ; |
0043CDF7  |. 51                  |PUSH    ECX                             ; |s
0043CDF8  |. FF15 F0754E00       |CALL    DWORD PTR DS:[<&USER32.wsprintf>; \wsprintfA
*/
char cmdOnTenshouExeJumpFromMovieWsprintfPush[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeMovieWsprintfPush() {

	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeMovieWsprintfPush;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeMovieWsprintfPush + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeMovieWsprintfPush  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromMovieWsprintfPush+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeMovieWsprintfPush), cmdOnTenshouExeJumpFromMovieWsprintfPush, 5, NULL); //5�o�C�g�̂ݏ�������
}




