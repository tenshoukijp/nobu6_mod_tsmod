#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "GameDataStruct.h"
// #include "SDKXAudio.h"


/*
004A20A6  |. 50             PUSH    EAX                              ; /Arg4 => 00000000
004A20A7  |. 50             PUSH    EAX                              ; |Arg3 => 00000000
004A20A8  |. 50             PUSH    EAX                              ; |Arg2 => 00000000
004A20A9  |. FF75 24        PUSH    DWORD PTR SS:[EBP+24]            ; |Arg1
004A20AC  |. E8 C4B1FEFF    CALL    TENSHOU.0048D275                 ; \TENSHOU.0048D275
004A20B1  |. 83C4 10        ADD     ESP, 10
*/

/*
0048D275  /$ 55             PUSH    EBP
0048D276  |. 8BEC           MOV     EBP, ESP
0048D278  |. 83EC 04        SUB     ESP, 4
0048D27B  |. 833D F0294D00 >CMP     DWORD PTR DS:[4D29F0], 0
0048D282  |. 56             PUSH    ESI
0048D283  |. 57             PUSH    EDI
0048D284  |. 74 57          JE      SHORT TENSHOU.0048D2DD
0048D286  |. 6A 01          PUSH    1
0048D288  |. E8 149DFDFF    CALL    TENSHOU.00466FA1
0048D28D  |. 83C4 04        ADD     ESP, 4
0048D290  |. 85C0           TEST    EAX, EAX
0048D292  |. 74 49          JE      SHORT TENSHOU.0048D2DD
0048D294  |. 6A 06          PUSH    6
0048D296  |. BF 00000000    MOV     EDI, 0
0048D29B  |. E8 43000000    CALL    TENSHOU.0048D2E3
0048D2A0  |. 83C4 04        ADD     ESP, 4
0048D2A3  |. 8B75 0C        MOV     ESI, DWORD PTR SS:[EBP+C]
0048D2A6  |. 85F6           TEST    ESI, ESI
0048D2A8  |. 74 03          JE      SHORT TENSHOU.0048D2AD
0048D2AA  |. 8B7D 10        MOV     EDI, DWORD PTR SS:[EBP+10]
0048D2AD  |> 8B45 08        MOV     EAX, DWORD PTR SS:[EBP+8]
0048D2B0  |. 83F8 21        CMP     EAX, 21
0048D2B3  |. 73 28          JNB     SHORT TENSHOU.0048D2DD
0048D2B5  |. 8D4D FC        LEA     ECX, DWORD PTR SS:[EBP-4]
0048D2B8  |. 51             PUSH    ECX
0048D2B9  |. FF3485 F8294D0>PUSH    DWORD PTR DS:[EAX*4+4D29F8]
0048D2C0  |. E8 912D0200    CALL    <JMP.&N6PWAV.LoadWaveResource>
0048D2C5  |. 83C4 08        ADD     ESP, 8
0048D2C8  |. 85C0           TEST    EAX, EAX
0048D2CA  |. 74 11          JE      SHORT TENSHOU.0048D2DD
0048D2CC  |. FF75 14        PUSH    DWORD PTR SS:[EBP+14]
0048D2CF  |. 57             PUSH    EDI
0048D2D0  |. FF75 FC        PUSH    DWORD PTR SS:[EBP-4]
0048D2D3  |. 50             PUSH    EAX
0048D2D4  |. 56             PUSH    ESI
0048D2D5  |. E8 282D0200    CALL    <JMP.&N6PAudio.dsndPlayStart>
0048D2DA  |. 83C4 14        ADD     ESP, 14
0048D2DD  |> 5F             POP     EDI
0048D2DE  |. 5E             POP     ESI
0048D2DF  |. 8BE5           MOV     ESP, EBP
0048D2E1  |. 5D             POP     EBP
0048D2E2  \. C3             RETN
*/


int iResIDInFuncPlayResourceWav = 1;
short iResIDInFuncPlayResourceDummy = 0;

//---------------------���̍Đ�-------------------------
int pCallFromFuncPlayResourceWavAsm = 0x48D275; // ���XTENSHOU.EXE���ɂ�����CALL��
void FuncPlayResourceWavAsm() {
	__asm {
		// ����1�`3 2�o�C�g����
		push iResIDInFuncPlayResourceDummy;
		push iResIDInFuncPlayResourceDummy;
		push iResIDInFuncPlayResourceDummy;
		// ����4 ���̔ԍ�(�z����INDEX�ƌ������ق����ǂ�)(INT=4�o�C�g)
		push iResIDInFuncPlayResourceWav;
		call pCallFromFuncPlayResourceWavAsm

		// �X�^�b�N�����ꍞ�񂾕�
		add  esp, 0x10
	}

}

// n6pwav.dll���ɂ��郊�\�[�X���Đ�����B
// iResID�̓��\�[�X���̂h�c�ł͂Ȃ��A���Ԗڂ̔z��ɓ����Ă��邩�H�@�ł���B
// ���݁A���s�X�e�b�v�Ƃ��Ă͐����������Ă���悤�Ɏv���邪�A0��(=601)����������Ȃ��B�������Ⴂ���Ă��邩�ȁH
void FuncPlayResourceWav(int iResID) {
	iResIDInFuncPlayResourceWav = iResID;

	__asm {
		push eax
		push ebx
		push ecx
		push edx
		push esp
		push esi
		push edi
	}

	// TENSHOU.EXE���̊֐��Ăяo��
	FuncPlayResourceWavAsm();

	__asm {
		pop edi
		pop esi
		pop esp
		pop edx
		pop ecx
		pop ebx
		pop eax
	}
}



/*
�v���O����ID(=iResID)	���\�[�X��	�g��ꏊ
0	601	�^�[���J�n 
1	602	���� 
2	603	���s 
3	604	�L�� 
4	605	���� 
5	606	�䕗 
6	607	�n�k 
7	608	�a�� 
8	609	�ʏ�퓬�A���C����ʂł̐푈���A���Z��I�̑���
9	610	�ˌ����A���x����B708�Ɠ����ɂȂ�B
11	611	��C����
12	612	��C�s���[��
13	613	����E���e 
10	614	�S�C 
15	615	��@���A�����Ԃ̃C�x���g���ɂ����p 
16	616	�J�� 
14	617	�ǉz���i���������j 
17	618	�ꊅ 
18	619	�����E���� 
19	620	�ŖS�E�߂����C�x���g 
20	701	�푈�˓� 
21	702	�y���i�O���j 
22	703	���� 
23	704	��
24	705	�H
25	706	�~
26	707	�t
27	708	�ˌ����̌p���� 610�Ɠ����ɖ�B 
28	709	����騁i�{�ې苒�j 
29	710	��C���e�E�S�C�ł��ꎞ 
30	711	�J 
31	712	���i�䕗���j 
32	713	�� 
*/
// ���ʂ�API�ŉ��̍Đ�
BOOL FuncPlaySoundWav( int iResID , BOOL isLoop ) {
	// �����̐ݒ肪�L���ł���΁c  �����ăt�H�A�O�����h�E�B���h�E�Ȃ�΁c
	if ( !isSEValid() ) {
		return FALSE;
	}
	
	// �t�H�A�O�����h�ł͂Ȃ���A���A���b�Z�[�W�X�s�[�h����ԍ�(�������j�Ȃ�΁A�T�E���h�͖炳�Ȃ�
	if ( GetForegroundWindow()!=CommonGlobalHandle::tenshouWND && nb6settings.messagespd == 0) {
		return FALSE;
	}
	if ( isLoop ) {
		isLoop = SND_LOOP;
	}
	// ����炷�B																		�����͔񓯊�  �����\�[�X���
	return PlaySound( MAKEINTRESOURCE(iResID), CommonGlobalHandle::hResourceWavMod, SND_ASYNC | SND_RESOURCE | isLoop );
}

BOOL FuncPlaySoundSyncWav( int iResID ) {
	// �����̐ݒ肪�L���ł���΁c  �����ăt�H�A�O�����h�E�B���h�E�Ȃ�΁c
	if ( !isSEValid() ) {
		return FALSE;
	}
	
	// �t�H�A�O�����h�ł͂Ȃ���A���A���b�Z�[�W�X�s�[�h����ԍ�(�������j�Ȃ�΁A�T�E���h�͖炳�Ȃ�
	if ( GetForegroundWindow()!=CommonGlobalHandle::tenshouWND && nb6settings.messagespd == 0) {
		return FALSE;
	}

	// ����炷�B																		�����͔񓯊�  �����\�[�X���
	return PlaySound( MAKEINTRESOURCE(iResID), CommonGlobalHandle::hResourceWavMod, SND_SYNC | SND_RESOURCE );
}


BOOL FuncStopSoundWav( int iResID ) {
	// �����~�߂�B													  �����\�[�X���
	return PlaySound( NULL, CommonGlobalHandle::hResourceWavMod, SND_ASYNC | SND_RESOURCE );
}



