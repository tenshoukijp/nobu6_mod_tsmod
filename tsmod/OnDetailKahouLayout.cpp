
#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"



/*
�ƕ�ڍ׉�ʂ̃��C�A�E�g

004948FC  /$ 53             PUSH    EBX
004948FD  |. 56             PUSH    ESI
004948FE  |. 57             PUSH    EDI
004948FF  |. 8BF1           MOV     ESI, ECX
00494901  |. 55             PUSH    EBP
00494902  |. FF76 70        PUSH    DWORD PTR DS:[ESI+70]
00494905  |. E8 8CB1FBFF    CALL    TENSHOU.0044FA96
0049490A  |. 83C4 04        ADD     ESP, 4
0049490D  |. 8BF8           MOV     EDI, EAX
0049490F  |. FF76 70        PUSH    DWORD PTR DS:[ESI+70]
00494912  |. E8 F5B1FBFF    CALL    TENSHOU.0044FB0C
00494917  |. 83C4 04        ADD     ESP, 4
0049491A  |. 8BE8           MOV     EBP, EAX
0049491C  |. BB F8674E00    MOV     EBX, TENSHOU.004E67F8            ;  ASCII "�VK"
00494921  |. 6A 04          PUSH    4
00494923  |. 6A 0F          PUSH    0F
00494925  |. 8BCB           MOV     ECX, EBX
00494927  |. FF76 6C        PUSH    DWORD PTR DS:[ESI+6C]
0049492A  |. E8 ED990100    CALL    TENSHOU.004AE31C
0049492F  |. 6A 2C          PUSH    2C
00494931  |. 8BCB           MOV     ECX, EBX
00494933  |. 68 E0000000    PUSH    0E0
00494938  |. E8 4BCBF6FF    CALL    TENSHOU.00401488
0049493D  |. FF76 70        PUSH    DWORD PTR DS:[ESI+70]
00494940  |. E8 3DB1FBFF    CALL    TENSHOU.0044FA82
00494945  |. 83C4 04        ADD     ESP, 4
00494948  |. 50             PUSH    EAX
00494949  |. 68 E0C04C00    PUSH    TENSHOU.004CC0E0                 ;  ASCII "%10s"
0049494E  |. 53             PUSH    EBX
0049494F  |. E8 5C9C0100    CALL    TENSHOU.004AE5B0
00494954  |. 83C4 0C        ADD     ESP, 0C
00494957  |. 8BCB           MOV     ECX, EBX
00494959  |. 6A 50          PUSH    50
0049495B  |. 68 88000000    PUSH    88
00494960  |. E8 23CBF6FF    CALL    TENSHOU.00401488
00494965  |. FF76 70        PUSH    DWORD PTR DS:[ESI+70]
00494968  |. E8 FFB1FBFF    CALL    TENSHOU.0044FB6C
0049496D  |. 83C4 04        ADD     ESP, 4
00494970  |. 50             PUSH    EAX
00494971  |. 68 482F4D00    PUSH    TENSHOU.004D2F48                 ;  ASCII "%2d����"
00494976  |. 53             PUSH    EBX
00494977  |. E8 349C0100    CALL    TENSHOU.004AE5B0
0049497C  |. 83C4 0C        ADD     ESP, 0C
0049497F  |. 8BCB           MOV     ECX, EBX
00494981  |. 6A 50          PUSH    50
00494983  |. 68 C8000000    PUSH    0C8
00494988  |. E8 FBCAF6FF    CALL    TENSHOU.00401488
0049498D  |. 8D47 FD        LEA     EAX, DWORD PTR DS:[EDI-3]
00494990  |. 83F8 01        CMP     EAX, 1
00494993  |. 1BC0           SBB     EAX, EAX
00494995  |. 83C0 02        ADD     EAX, 2
00494998  |. 0FAFC5         IMUL    EAX, EBP
0049499B  |. 50             PUSH    EAX
0049499C  |. FF34BD 80144D0>PUSH    DWORD PTR DS:[EDI*4+4D1480]
004949A3  |. 68 3C2F4D00    PUSH    TENSHOU.004D2F3C                 ;  ASCII "%-4s +%2d"
004949A8  |. 53             PUSH    EBX
004949A9  |. E8 029C0100    CALL    TENSHOU.004AE5B0
004949AE  |. 83C4 10        ADD     ESP, 10
004949B1  |. 6A 00          PUSH    0
004949B3  |. 6A 00          PUSH    0
004949B5  |. 6A 00          PUSH    0
004949B7  |. 8BCB           MOV     ECX, EBX
004949B9  |. BF 38C84D00    MOV     EDI, TENSHOU.004DC838
004949BE  |. E8 6714F7FF    CALL    TENSHOU.00405E2A
004949C3  |. 8BCF           MOV     ECX, EDI
004949C5  |. E8 5707F7FF    CALL    TENSHOU.00405121
004949CA  |. 50             PUSH    EAX
004949CB  |. FF76 70        PUSH    DWORD PTR DS:[ESI+70]
004949CE  |. E8 01B1FBFF    CALL    TENSHOU.0044FAD4
004949D3  |. 83C4 04        ADD     ESP, 4
004949D6  |. 40             INC     EAX
004949D7  |. B9 30BB4D00    MOV     ECX, TENSHOU.004DBB30
004949DC  |. 50             PUSH    EAX
004949DD  |. E8 C22FFEFF    CALL    TENSHOU.004779A4
004949E2  |. 6A 04          PUSH    4
004949E4  |. 8BCF           MOV     ECX, EDI
004949E6  |. 6A 00          PUSH    0
004949E8  |. 6A 30          PUSH    30
004949EA  |. E8 16680100    CALL    TENSHOU.004AB205
004949EF  |. 50             PUSH    EAX                              ; /Arg5
004949F0  |. 8BCB           MOV     ECX, EBX                         ; |
004949F2  |. 6A 40          PUSH    40                               ; |Arg4 = 00000040   �� 64�s�N�Z��
004949F4  |. 6A 40          PUSH    40                               ; |Arg3 = 00000040   �� 64�s�N�Z��
004949F6  |. 6A 24          PUSH    24                               ; |Arg2 = 00000024   �� x���W
004949F8  |. 6A 10          PUSH    10                               ; |Arg1 = 00000010   �� y���W
004949FA  |. E8 EA960100    CALL    TENSHOU.004AE0E9                 ; \TENSHOU.004AE0E9  �� �������ƕ�A�C�R���v�b�V��
004949FF  |. 8B6E 70        MOV     EBP, DWORD PTR DS:[ESI+70]
00494A02  |. 2BD2           SUB     EDX, EDX
00494A04  |. 4D             DEC     EBP
00494A05  |. B9 32000000    MOV     ECX, 32
00494A0A  |. 8BC5           MOV     EAX, EBP
00494A0C  |. F7F1           DIV     ECX
00494A0E  |. 2BD2           SUB     EDX, EDX
00494A10  |. 8D78 0F        LEA     EDI, DWORD PTR DS:[EAX+F]
00494A13  |. 8BC5           MOV     EAX, EBP
00494A15  |. 6A 14          PUSH    14
00494A17  |. 69FF F4010000  IMUL    EDI, EDI, 1F4
00494A1D  |. F7F1           DIV     ECX
00494A1F  |. 03FA           ADD     EDI, EDX
00494A21  |. 8BCB           MOV     ECX, EBX
00494A23  |. 03FF           ADD     EDI, EDI
00494A25  |. E8 970AF8FF    CALL    TENSHOU.004154C1
00494A2A  |. 6A 04          PUSH    4
00494A2C  |. 8BCB           MOV     ECX, EBX
00494A2E  |. 6A 0F          PUSH    0F
00494A30  |. FF76 6C        PUSH    DWORD PTR DS:[ESI+6C]
00494A33  |. E8 E4980100    CALL    TENSHOU.004AE31C
00494A38  |. 6A 24          PUSH    24
00494A3A  |. 8BCB           MOV     ECX, EBX
00494A3C  |. 6A 58          PUSH    58								�� �������ƕ󖼂̃��C�A�E�g
00494A3E  |. E8 45CAF6FF    CALL    TENSHOU.00401488
00494A43  |. 81FD FFFF0000  CMP     EBP, 0FFFF
00494A49  |. 74 19          JE      SHORT TENSHOU.00494A64
00494A4B  |. 6A 00          PUSH    0                                ; /Arg8 = 00000000
00494A4D  |. 6A 00          PUSH    0                                ; |Arg7 = 00000000
00494A4F  |. 6A 00          PUSH    0                                ; |Arg6 = 00000000
00494A51  |. 6A 00          PUSH    0                                ; |Arg5 = 00000000
00494A53  |. 6A 00          PUSH    0                                ; |Arg4 = 00000000
00494A55  |. 6A 00          PUSH    0                                ; |Arg3 = 00000000
00494A57  |. 6A 00          PUSH    0                                ; |Arg2 = 00000000
00494A59  |. 57             PUSH    EDI                              ; |Arg1
00494A5A  |. E8 0176FDFF    CALL    TENSHOU.0046C060                 ; \TENSHOU.0046C060
00494A5F  |. 83C4 20        ADD     ESP, 20
00494A62  |. EB 0B          JMP     SHORT TENSHOU.00494A6F
00494A64  |> FF76 70        PUSH    DWORD PTR DS:[ESI+70]
00494A67  |. E8 B8AFFBFF    CALL    TENSHOU.0044FA24
00494A6C  |. 83C4 04        ADD     ESP, 4
00494A6F  |> 50             PUSH    EAX
00494A70  |. BE F8674E00    MOV     ESI, TENSHOU.004E67F8            ;  ASCII "�VK"
00494A75  |. 56             PUSH    ESI
00494A76  |. E8 359B0100    CALL    TENSHOU.004AE5B0
00494A7B  |. 83C4 08        ADD     ESP, 8
00494A7E  |. 8BCE           MOV     ECX, ESI
00494A80  |. 6A 6C          PUSH    6C
00494A82  |. 6A 10          PUSH    10
00494A84  |. E8 FFC9F6FF    CALL    TENSHOU.00401488
00494A89  |. 81FD FFFF0000  CMP     EBP, 0FFFF
00494A8F  |. 74 15          JE      SHORT TENSHOU.00494AA6
00494A91  |. 33C0           XOR     EAX, EAX
00494A93  |. 50             PUSH    EAX                              ; /Arg8 => 00000000
00494A94  |. 50             PUSH    EAX                              ; |Arg7 => 00000000
00494A95  |. 50             PUSH    EAX                              ; |Arg6 => 00000000
00494A96  |. 47             INC     EDI                              ; |
00494A97  |. 50             PUSH    EAX                              ; |Arg5 => 00000000
00494A98  |. 50             PUSH    EAX                              ; |Arg4 => 00000000
00494A99  |. 50             PUSH    EAX                              ; |Arg3 => 00000000
00494A9A  |. 50             PUSH    EAX                              ; |Arg2 => 00000000
00494A9B  |. 57             PUSH    EDI                              ; |Arg1
00494A9C  |. E8 BF75FDFF    CALL    TENSHOU.0046C060                 ; \TENSHOU.0046C060
00494AA1  |. 83C4 20        ADD     ESP, 20
00494AA4  |. EB 05          JMP     SHORT TENSHOU.00494AAB
00494AA6  |> A1 74154D00    MOV     EAX, DWORD PTR DS:[4D1574]
00494AAB  |> 50             PUSH    EAX
00494AAC  |. 68 F8674E00    PUSH    TENSHOU.004E67F8                 ;  ASCII "�VK"
00494AB1  |. E8 FA9A0100    CALL    TENSHOU.004AE5B0
00494AB6  |. 83C4 08        ADD     ESP, 8
00494AB9  |. 5D             POP     EBP
00494ABA  |. 5F             POP     EDI
00494ABB  |. 5E             POP     ESI
00494ABC  |. 5B             POP     EBX
00494ABD  \. C3             RETN

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

�����Ă��̌�A�����Ɉȉ���ʉ߂���B
������58���A�ƕ�摜�̑傫���ɏ]���āA�ő�7F�܂ő傫������Ηǂ��B
00494A38  |. 6A 24          PUSH    24
00494A3A  |. 8BCB           MOV     ECX, EBX
00494A3C  |. 6A 58          PUSH    58
00494A3E  |. E8 45CAF6FF    CALL    TENSHOU.00401488
00494A43  |. 81FD FFFF0000  CMP     EBP, 0FFFF


*/
extern char* getExistMovieKahouFilePath( int iKahouID );
extern char* getExistPictureKahouFilePath( int iKahouID );
extern void getAviPixelSize( char *szKahouName, int *width, int *height );
extern void getBmpPixelSize( char *szKahouName, int *width, int *height );


/*
 * �ƕ�ڍ׏���\�����̃��C�A�E�g����
 */

char iCurKahouMeiXPos = 0x58;
int iCurKahouIDOnLayout = 0;
void OnTenshouExeDetailKahouLayoutExecute() {

	// �K���f�t�H���g�ŏ�����
	iCurKahouMeiXPos = 0x58;

	if ( 0 <= iCurKahouIDOnLayout && iCurKahouIDOnLayout < GAMEDATASTRUCT_KAHOU_NUM ) {
		// �ΏۂƂȂ�ƕ󂪃��[�r�[���ǂ����`�F�b�N
		char *p = getExistMovieKahouFilePath( iCurKahouIDOnLayout );
		// ����̓��[�r�[���B
		if ( p ) {
			// ����̏c���𓾂�B
			int width = 64;
			int height = 64;
			getAviPixelSize(p, &width, &height);

			// ����̕���64�s�N�Z�����傫����΁A0x58���A���̑傫���������B�����A0x7F���z���Ȃ��悤�ɂ���B
			int iTmpCurKahouMeiXPos = iCurKahouMeiXPos + width-64;
			if ( iTmpCurKahouMeiXPos > 0x7F ) {
				iTmpCurKahouMeiXPos = 0x7F;
			}
			iCurKahouMeiXPos = iTmpCurKahouMeiXPos;

			return;
		}

		p = getExistPictureKahouFilePath( iCurKahouIDOnLayout );
		// ����͉摜���B
		if ( p ) {
			// �摜�̏c���𓾂�B
			int width = 64;
			int height = 64;
			getBmpPixelSize(p, &width, &height);

			// ����̕���64�s�N�Z�����傫����΁A0x58���A���̑傫���������B�����A0x7F���z���Ȃ��悤�ɂ���B
			int iTmpCurKahouMeiXPos = iCurKahouMeiXPos + width-64;
			if ( iTmpCurKahouMeiXPos > 0x7F ) {
				iTmpCurKahouMeiXPos = 0x7F;
			}
			iCurKahouMeiXPos = iTmpCurKahouMeiXPos;

			return;
		}
	}

}

#pragma warning(push)
#pragma warning(disable:4409)

/*
00494A33  |. E8 E4980100    CALL    TENSHOU.004AE31C
				�������� JMP TSMod.OnTSExeDetailKahouLayout �Ə��������Ă����ۂ̏���
00494A38  |. 6A 24          PUSH    24
00494A3A  |. 8BCB           MOV     ECX, EBX
00494A3C  |. 6A 58          PUSH    58								�� �������ƕ󖼂̃��C�A�E�g
00494A3E  |. E8 45CAF6FF    CALL    TENSHOU.00401488
*/
int pTenshouExeJumpFromToOnTenshouExeDetailKahouLayout		=0x494A33; // �֐��͂��̃A�h���X����AOnTenshouExeDetailKahouLayout�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeDetailKahouLayout  =0x4AE31C; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeDetailKahouLayout	=0x494A3E; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeDetailKahouLayout() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {
		mov iCurKahouIDOnLayout, ebp // �ƕ�̂h�c���R�s�[

		// ���XTENSHOU.EXE���ɂ��������̂�������
		call pTenshouExeJumpCallFromToOnTenshouExeDetailKahouLayout

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
	OnTenshouExeDetailKahouLayoutExecute();

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

		// TENSHOU.EXE�̌��X�̏���������
		push    0x24  // �� �������ƕ󖼂�y�ʒu�̃��C�A�E�g  0x24 �Œ�
		MOV     ECX, EBX

		PUSH    iCurKahouMeiXPos  // �� �������ƕ󖼂�x�ʒu�̃��C�A�E�g  �ő�ł�0x7F�܂łƂ���B

		jmp pTenshouExeReturnLblFromOnTenshouExeDetailKahouLayout
	}
}
// �Ώۂ̃R�[�h
#pragma warning(pop)

/*
004538DA  |. FF75 08        PUSH    DWORD PTR SS:[EBP+8]
004538DD  |. E8 DC110400    CALL    TENSHOU.00494ABE
004538E2  |. E8 F07F0200    CALL    TENSHOU.0047B8D7
				�������� JMP TSMod.OnTSExeDetailKahouLayout �Ə��������Ă����ۂ̏���
004538E7  |. 8D8D 7CFFFFFF  LEA     ECX, DWORD PTR SS:[EBP-84]
004538ED  |. 8935 80154D00  MOV     DWORD PTR DS:[4D1580], ESI
*/
char cmdOnTenshouExeJumpFromDetailKahouLayout[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeDetailKahouLayout() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeDetailKahouLayout;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeDetailKahouLayout + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeDetailKahouLayout  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromDetailKahouLayout+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeDetailKahouLayout), cmdOnTenshouExeJumpFromDetailKahouLayout, 5, NULL); //5�o�C�g�̂ݏ�������
}




