#define _CRT_SECURE_NO_WARNINGS

#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "FuncSelectMenuDialog.h"
#include "OutputDebugStream.h"


/*
00419AF4  |. 33DB           XOR     EBX, EBX
00419AF6  |. BF FFFF0000    MOV     EDI, 0FFFF
00419AFB  |> 53             /PUSH    EBX                             ; /Arg7				�� 0 �Œ�
00419AFC  |. 6A 70          |PUSH    70                              ; |Arg6 = 00000070		�H
00419AFE  |. 53             |PUSH    EBX                             ; |Arg5
00419AFF  |. 68 A0B64C00    |PUSH    TENSHOU.004CB6A0                ; |Arg4 = 004CB6A0		������|�C���^�̔z��̃A�h���X
00419B04  |. 6A 03          |PUSH    3                               ; |Arg3 = 00000003		�I�����̐�
00419B06     68 A0000000    PUSH    0A0														�x���W
00419B0B     68 18020000    PUSH    118														�w���W
00419B10  |. E8 6C920800    |CALL    TENSHOU.004A2D81                ; \TENSHOU.004A2D81
00419B15  |. 83C4 1C        |ADD     ESP, 1C


RETURN�ł���EAX�͑I��������0, 1, 2,...
�I�����Ȃ������ꍇ(�E�N���b�N�Ȃ�) 0xFFFF ��EAX�ɓ���B
*/

// ���j���[�̐��̍ő�
#define SELECT_MENU_NUM_MAX  36

// �P�̃��j���[�̒��̕�����̍ő�͑S�p�R�P�����B���p63
#define SELECT_MENU_STRING_LENGTH_MAX	64

char *pSelectMenuStringArrayPointer[SELECT_MENU_NUM_MAX] = {NULL}; // ���j���[�͍ő�łR�U�B������͊i���j���[������ւ̃|�C���^�z��
// �ƕ��ނ̃J�X�^��������

// �P�̃Z���N�g���j���[�^
struct SELECT_MENU {
	char szKahouTypeString[SELECT_MENU_STRING_LENGTH_MAX]; // �S�p�R�P�����B
};
SELECT_MENU select_menu_string[SELECT_MENU_NUM_MAX] = {""}; // �e�X�̕����񃁃j���[


//---------------------��ґI���_�C�A���O-------------------------
int pCallFromFuncSelectMenuDialogAsm = 0x4A2D81; // ���XTENSHOU.EXE���ɂ�����CALL��
int iResultOfFuncSelectMenuDialog = 0;	// �I������
int iXPosOfSelectMenuDialog = 0xA0;		// �w���W 
int iYPosOfSelectMenuDialog = 0x118;	// �x���W
int iMenuNumOfSelectMenuDialog = 3;		// ���j���[�̌�
int iMenuMinWidthOfSelectMenuDialog = 0x70; // ���j���̍Œᕝ

int iSelectMenuStringArrayPointerOfSelectMenuDialog = 0x4CB6A0; // �f�t�H���g�͔�I�̔z��

void FuncSelectMenuDialogAsm() {
	__asm {
		push 0xF
		push iMenuMinWidthOfSelectMenuDialog // �����炭���j���[�̍Œᕝ
		push 0
		push iSelectMenuStringArrayPointerOfSelectMenuDialog
		push iMenuNumOfSelectMenuDialog
		push iYPosOfSelectMenuDialog
		push iXPosOfSelectMenuDialog

		call pCallFromFuncSelectMenuDialogAsm

		// �I�����ʂ̔ԍ���eax�ɓ���
		mov iResultOfFuncSelectMenuDialog, eax

		// push���������Ȃ��ɂ���
		pop eax
		pop eax
		pop eax
		pop eax
		pop eax
		pop eax
		pop eax

		mov eax, iResultOfFuncSelectMenuDialog;
	}
}


bool isInitSelectMenuPointer = false; // ���������t���O
void InitSelectMenuPointer() {
	if ( !isInitSelectMenuPointer ) {
		// �ŏ��Ɋe�X�̕�����̃|�C���^�͎w���Ă���
		for ( int i=0; i<SELECT_MENU_NUM_MAX; i++ ) {
			pSelectMenuStringArrayPointer[i] = select_menu_string[i].szKahouTypeString;
		}

		// �|�C���^�̔z��̐擪�̃A�h���X��int�^�Ƃ��čT���Ă����B���j���[�_�C�A���O�ɂ͂��̒l��n���K�v������B
		iSelectMenuStringArrayPointerOfSelectMenuDialog = (int)pSelectMenuStringArrayPointer;

		isInitSelectMenuPointer = true;
	}
}

int SelectMenuDialog(vector<string> menu_list, int posX, int posY, int MinMenuWidth) {
	// ���j���[�̌����ő吔�𒴂��Ă���΁A
	if ( menu_list.size() > SELECT_MENU_NUM_MAX ) {
		return 0xFFFF; // �������������̂Ɠ������ƂƂ���B
	}
	// ���j���[���̂P�����݂��Ȃ��B
	if ( menu_list.size() == 0 ) {
		return 0xFFFF; // �������������̂Ɠ������ƂƂ���B
	}

	InitSelectMenuPointer();

	iMenuMinWidthOfSelectMenuDialog = MinMenuWidth;

	// ���j���[�̌����T���Ă���
	iMenuNumOfSelectMenuDialog = menu_list.size();
	// ���j���[�����A�ő吔���I�[�o�[���Ă�����A�ő吔�ɒ���
	if ( iMenuNumOfSelectMenuDialog > SELECT_MENU_NUM_MAX ) { iMenuNumOfSelectMenuDialog = SELECT_MENU_NUM_MAX; }

	// ��Ԓ���������T���B
	int max_string_length = iMenuMinWidthOfSelectMenuDialog/8; // �_�C�A���O�̍Œᕝ�𔼊p�����̕��Ŋ������l

	// �e���j���[�̕�������A�O���[�o���ϐ��ɃR�s�[���Ă���
	for ( int i=0; i < iMenuNumOfSelectMenuDialog ; i++ ) {

		strncpy( select_menu_string[i].szKahouTypeString, menu_list[i].c_str(), SELECT_MENU_STRING_LENGTH_MAX);

		// ��ԍŌ��NULL�ɂ��Ĉ��S��
		select_menu_string[i].szKahouTypeString[SELECT_MENU_STRING_LENGTH_MAX-1] = NULL;

		// ��Ԓ��������X�V�`�F�b�N
		int cur_len = strlen( menu_list[i].c_str() );
		if ( max_string_length < cur_len ) {
			max_string_length = cur_len;
		}

	}

	RECT rc; // �N���C�A���g�̈�̃T�C�Y
	::GetClientRect(CommonGlobalHandle::tenshouWND, &rc);

	int width = rc.right-rc.left;
	int height = rc.bottom-rc.top;

	// �w�肳��Ă��Ȃ� �� ������
	if ( posX == -1 ) {
		int left = width/2 - 4*(max_string_length); // �����ɂȂ�悤�ɍ����v�Z�B�i�ő�̕����񐔁j���W�^�Q�̃s�N�Z���� �^�񒆂�荶�ɔz�u 
		iXPosOfSelectMenuDialog = left;
	} else {
		iXPosOfSelectMenuDialog = posX;
	}

	// �w�肳��Ă��Ȃ� �� ������
	if ( posY == -1 ) {
		int top = height/2 - 8*(iMenuNumOfSelectMenuDialog+1); // �����ɂȂ�悤�Ƀg�b�v���v�Z�B�i���j���[���{�P�j���P�U�^�Q�̃s�N�Z���� �^�񒆂���ɔz�u 
		iYPosOfSelectMenuDialog = top;
	} else {
		iYPosOfSelectMenuDialog = posY;
	}

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
	FuncSelectMenuDialogAsm();

	__asm {
		pop edi
		pop esi
		pop esp
		pop edx
		pop ecx
		pop ebx
		pop eax
	}

	// ���ʂ�Ԃ��B
	return iResultOfFuncSelectMenuDialog;
}


