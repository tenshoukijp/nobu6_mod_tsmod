#define _CRT_SECURE_NO_WARNINGS
#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "GameDataStruct.h"
#include "ScenarioMod.h"
#include "MessageN6PPatch.h"
#include "FuncFukidashiDialog.h"
/*
//--------------------------���� EAX�͑��������ԍ�
0048B089   . 50             PUSH    EAX								�����ԍ��H
0048B08A   . 57             PUSH    EDI                              ; /Arg8
0048B08B   . 57             PUSH    EDI                              ; |Arg7
0048B08C   . BD 907C4D00    MOV     EBP, TENSHOU.004D7C90            ; |
0048B091   . 57             PUSH    EDI                              ; |Arg6
0048B092   . 57             PUSH    EDI                              ; |Arg5
0048B093   . 57             PUSH    EDI                              ; |Arg4
0048B094   . 57             PUSH    EDI                              ; |Arg3
0048B095   . 55             PUSH    EBP                              ; |Arg2 => 004D7C90
0048B096   . 68 E8360000    PUSH    36E8                             ; |Arg1 = 000036E8
0048B09B   . E8 C00FFEFF    CALL    TENSHOU.0046C060                 ; \TENSHOU.0046C060
0048B0A0   . 83C4 20        ADD     ESP, 20
0048B0A3   . 50             PUSH    EAX								���������A�h���X�H
0048B0A4   . E8 FE6E0100    CALL    TENSHOU.004A1FA7
0048B0A9   . 83C4 0C        ADD     ESP, 0C

//--------------------------�E�� EAX�͑��������ԍ�
0048B13D   . 50             PUSH    EAX
0048B13E   . 57             PUSH    EDI                              ; /Arg8
0048B13F   . 57             PUSH    EDI                              ; |Arg7
0048B140   . 57             PUSH    EDI                              ; |Arg6
0048B141   . 57             PUSH    EDI                              ; |Arg5
0048B142   . 57             PUSH    EDI                              ; |Arg4
0048B143   . 57             PUSH    EDI                              ; |Arg3
0048B144   . 55             PUSH    EBP                              ; |Arg2
0048B145   . 68 EB360000    PUSH    36EB                             ; |Arg1 = 000036EB		�� �V�i���IID
0048B14A   . E8 110FFEFF    CALL    TENSHOU.0046C060                 ; \TENSHOU.0046C060
0048B14F   . 83C4 20        ADD     ESP, 20



// �܂�����
0048B1E9   . 57             PUSH    EDI
0048B1EA   . 50             PUSH    EAX
0048B1EB   . 57             PUSH    EDI                              ; /Arg8
0048B1EC   . 57             PUSH    EDI                              ; |Arg7
0048B1ED   . 57             PUSH    EDI                              ; |Arg6
0048B1EE   . 57             PUSH    EDI                              ; |Arg5
0048B1EF   . 57             PUSH    EDI                              ; |Arg4
0048B1F0   . 57             PUSH    EDI                              ; |Arg3
0048B1F1   . 57             PUSH    EDI                              ; |Arg2
0048B1F2   . 68 F0360000    PUSH    36F0                             ; |Arg1 = 000036F0
0048B1F7   . E8 640EFEFF    CALL    TENSHOU.0046C060                 ; \TENSHOU.0046C060
0048B1FC   . 83C4 20        ADD     ESP, 20
0048B1FF   . 50             PUSH    EAX
0048B200   . E8 A26D0100    CALL    TENSHOU.004A1FA7
0048B205   . 83C4 0C        ADD     ESP, 0C

//----����̃��b�Z�[�W�����ŕύX
0048B208   . 57             PUSH    EDI
0048B209   . 55             PUSH    EBP
0048B20A   . 68 F1360000    PUSH    36F1
0048B20F   . E8 380AFAFF    CALL    TENSHOU.0042BC4C
0048B214   . 83C4 0C        ADD     ESP, 0C

//----����̃��b�Z�[�W�����ŕύX
0048B2AD   . 57             PUSH    EDI
0048B2AE   . 55             PUSH    EBP
0048B2AF   . 68 F5360000    PUSH    36F5
0048B2B4   . E8 9309FAFF    CALL    TENSHOU.0042BC4C
0048B2B9   . 83C4 0C        ADD     ESP, 0C

//----����̃��b�Z�[�W�����ŕύX
0048B2BC   . 57             PUSH    EDI
0048B2BD   . 55             PUSH    EBP
0048B2BE   . 68 F6360000    PUSH    36F6
0048B2C3   . E8 8409FAFF    CALL    TENSHOU.0042BC4C
0048B2C8   . 83C4 0C        ADD     ESP, 0C


// -- �E���̃E�B���h�E�������H
0048B412   . B9 F0994D00    MOV     ECX, TENSHOU.004D99F0            ;  ASCII "�VK"
0048B417   . E8 1C20FFFF    CALL    TENSHOU.0047D438
0048B41C   . 8BCB           MOV     ECX, EBX
0048B41E   . E8 1520FFFF    CALL    TENSHOU.0047D438

// -- ����̂̃E�B���h�E�������H
MOV     ECX, TENSHOU.004D99F0
0048B0CF   . 8BCB           MOV     ECX, EBX
0048B0D1   . E8 6223FFFF    CALL    TENSHOU.0047D438
0048B0D6   . E8 1382F9FF    CALL    TENSHOU.004232EE
*/

/*
0048B174   . 83C4 20        ADD     ESP, 20

0048B177   . 50             PUSH    EAX							�� EAX�͕�����ւ̃|�C���^
0048B178   . E8 2A6E0100    CALL    TENSHOU.004A1FA7
0048B17D   . 83C4 0C        ADD     ESP, 0C

*/


//---------------------����Ɉ����o�����o��-------------------------
int iBushouIDOfNormalFukidashiMessage = -1;
char pszMessageOfNormalFukidashiMessage[512] = "";
int iTargetMemoryAddressOfOfNormalFukidashiMessage = NULL;

NB6FUKIDASHIRELATEPERSONS nb6fukidashirelatepersons_for_push; // �蓮�Ńv�b�V������p��Person�֌W

static void ReWriteNormalFukidashiMessage() {
	// MessageN6PPatch��ʉ߂��邽�߂̂��V����

	// �O�����b�Z�[�W�u��������ScenarioMod�ɂčs��
	if ( p_snOnRewriteFukidashiMessage ) {
		char szModMsgBuf[512] = "";

		nb6fukidashirelatepersons_for_push.i1stPersonID = iBushouIDOfNormalFukidashiMessage+1;
		nb6fukidashirelatepersons_for_push.i2ndPersonID = 0xFFFF;
		nb6fukidashirelatepersons_for_push.i3rdPersonID = 0xFFFF;

		// �����Ŗ��O�����o���B
		char name[256] = "";

		if ( nb6fukidashirelatepersons_for_push.i1stPersonID <= 532 ) {
			strcat( name, nb6bushouname[nb6fukidashirelatepersons_for_push.i1stPersonID-1].familyname);
			strcat( name, nb6bushouname[nb6fukidashirelatepersons_for_push.i1stPersonID-1].fastname);
		}

		// ������, ���݂̏��������ΏۂƂȂ��Ă��郁�b�Z�[�W, �֘A�l�����
		((PFNSNONREWRITEFUKIDASHIMESSAGE) p_snOnRewriteFukidashiMessage)(szModMsgBuf, name, pszMessageOfNormalFukidashiMessage, &nb6fukidashirelatepersons_for_push);

		if ( strlen(szModMsgBuf) > 1 ) {
			strcpy( pszMessageOfNormalFukidashiMessage, szModMsgBuf );
		}

	}
	// �����œn���ė������̂��R�s�[
	strcpy( (char *)iTargetMemoryAddressOfOfNormalFukidashiMessage, pszMessageOfNormalFukidashiMessage);

}

int pCallFromFuncFukidashiMessage1 = 0x46C060; // ���XTENSHOU.EXE���ɂ�����CALL��
int pCallFromFuncFukidashiMessage2 = 0x4A1FA7; // ���XTENSHOU.EXE���ɂ�����CALL��
int iBushouIDOfLeftNormalFukidashiMessage = -1;
void FuncLeftNormalFukidashiMessageAsm() {

	__asm {
		mov eax, iBushouIDOfLeftNormalFukidashiMessage
		mov edi, 0
		mov ebp, 0x4d7c90
		push edi
		push eax
		push edi
		push edi
		push edi
		push edi
		push edi
		push edi
		push ebp
		push 0x36e8
		call pCallFromFuncFukidashiMessage1                 ; \TENSHOU.0046C060
		add     esp, 0x20
	}

	__asm {
		mov iTargetMemoryAddressOfOfNormalFukidashiMessage, eax // eax������������̃A�h���X
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

	ReWriteNormalFukidashiMessage();

	__asm {
		pop edi
		pop esi
		pop esp
		pop edx
		pop ecx
		pop ebx
		pop eax
	}

	__asm {
		push eax   // eax �̓��b�Z�[�W�̊i�[��̃������A�h���X�B���CMP.getLocation�قɂ���Ɠ����ꏊ�ŁA���4d7c90�̂��ƁB

		call pCallFromFuncFukidashiMessage2

		add   esp, 0x0C
	}
}


// 
void FuncLeftNormalFukidashiMessage(int iBushouID, char *szMessage) {
	iBushouIDOfLeftNormalFukidashiMessage = iBushouID+1;
	iBushouIDOfNormalFukidashiMessage = iBushouIDOfLeftNormalFukidashiMessage;
	strcpy( pszMessageOfNormalFukidashiMessage, szMessage);

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
	FuncLeftNormalFukidashiMessageAsm();

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



//---------------------�E���Ɉ����o�����o��-------------------------
int pCallFromFuncFukidashiMessage3 = 0x4A1FFD; // ���XTENSHOU.EXE���ɂ�����CALL��
int iBushouIDOfRightNormalFukidashiMessage = -1;
void FuncRightNormalFukidashiMessageAsm() {

	__asm {
		mov eax, iBushouIDOfRightNormalFukidashiMessage
		mov edi, 0
		mov ebp, 0x4d7c90
		push edi
		push eax
		push edi
		push edi
		push edi
		push edi
		push edi
		push edi
		push ebp
		push 0x36eb
		call pCallFromFuncFukidashiMessage1                 ; \TENSHOU.0046C060
		add     esp, 0x20
	}

	__asm {
		mov iTargetMemoryAddressOfOfNormalFukidashiMessage, eax // eax������������̃A�h���X
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

	ReWriteNormalFukidashiMessage();

	__asm {
		pop edi
		pop esi
		pop esp
		pop edx
		pop ecx
		pop ebx
		pop eax
	}

	__asm {
		push eax   // eax �̓��b�Z�[�W�̊i�[��̃������A�h���X�B���CMP.getLocation�قɂ���Ɠ����ꏊ�ŁA���4d7c90�̂��ƁB

		call pCallFromFuncFukidashiMessage3

		add   esp, 0x0C
	}

}


// 
void FuncRightNormalFukidashiMessage(int iBushouID, char *szMessage) {
	iBushouIDOfRightNormalFukidashiMessage = iBushouID+1;
	iBushouIDOfNormalFukidashiMessage = iBushouIDOfRightNormalFukidashiMessage;
	strcpy( pszMessageOfNormalFukidashiMessage, szMessage);
	
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
	FuncRightNormalFukidashiMessageAsm();

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
//---------------------�^�񒆂ɐ����o�����o��-------------------------
int pCallFromFuncFukidashiMessage4 = 0x4A2052; // ���XTENSHOU.EXE���ɂ�����CALL��
int iBushouIDOfCenterNormalFukidashiMessage = -1;
void FuncCenterNormalFukidashiMessageAsm() {

	__asm {
		mov eax, iBushouIDOfCenterNormalFukidashiMessage
		mov esi, 0
		mov edi, 0xFFFF
		mov ebp, 0x4d7c90
		push    esi         
		push    esi         
		push	esi // push    ebx         
		push    esi         
		push    esi         
		push    esi         
		push    ebp    
		push    0x13C8        
		call    pCallFromFuncFukidashiMessage1
		add     esp, 0x20
	}

	__asm {
		mov iTargetMemoryAddressOfOfNormalFukidashiMessage, eax // eax������������̃A�h���X
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

	ReWriteNormalFukidashiMessage();

	__asm {
		pop edi
		pop esi
		pop esp
		pop edx
		pop ecx
		pop ebx
		pop eax
	}

	__asm {
		mov esi, 0
		mov edi, 0xFFFF
		push edi
		push edi
		push edi
		push 2
		push esi
		push esi
		push eax   // eax �̓��b�Z�[�W�̊i�[��̃������A�h���X�B���CMP.getLocation�قɂ���Ɠ����ꏊ�ŁA���4d7c90�̂��ƁB

		push iBushouIDOfCenterNormalFukidashiMessage
		call pCallFromFuncFukidashiMessage4

		add  esp, 0x20

	}

}


// 
void FuncCenterNormalFukidashiMessage(int iBushouID, char *szMessage) {
	iBushouIDOfCenterNormalFukidashiMessage = iBushouID+1;
	strcpy( pszMessageOfNormalFukidashiMessage, szMessage);
	
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
	FuncCenterNormalFukidashiMessageAsm();

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
*/



/*
0048B208   . 57             PUSH    EDI							�� 0 �Œ�
0048B209   . 55             PUSH    EBP							�� 4d7c90 �Œ�
0048B20A   . 68 F1360000    PUSH    36F1
0048B20F   . E8 380AFAFF    CALL    TENSHOU.0042BC4C			�� �Ăяo���֐�
0048B214   . 83C4 0C        ADD     ESP, 0C
*/

int pCallFromFuncLeftFukidashiUpdate1 = 0x42BC4C; // ���XTENSHOU.EXE���ɂ�����CALL��
int iBushouIDOfLeftNormalFukidashiUpdate = -1;
int isOnLeftNormalFukidashiUpdating = FALSE; // ���b�Z�[�W�g�́uUpdate�v�֐��ɂ����āA����O�����當�����ύX���邱�Ƃ̃V�O�i���B
void FuncLeftNormalFukidashiUpdateAsm() {

	__asm {
		mov eax, iBushouIDOfLeftNormalFukidashiUpdate
		mov edi, 0
		mov ebp, 0x4d7c90
		push edi
		push ebp
		push 0x36F1
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

	ReWriteNormalFukidashiMessage();

	__asm {
		pop edi
		pop esi
		pop esp
		pop edx
		pop ecx
		pop ebx
		pop eax
	}

	__asm {
		call pCallFromFuncLeftFukidashiUpdate1                 ; \TENSHOU.0042BC4C
		add     esp, 0x0C
	}
}


// 
void FuncLeftNormalFukidashiUpdate(int iBushouID, char *szMessage) {
	iBushouIDOfLeftNormalFukidashiUpdate = iBushouID+1;
	iBushouIDOfNormalFukidashiMessage = iBushouIDOfLeftNormalFukidashiUpdate;
	strcpy( pszMessageOfNormalFukidashiMessage, szMessage);

	isOnLeftNormalFukidashiUpdating = TRUE;

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
	FuncLeftNormalFukidashiUpdateAsm();

	__asm {
		pop edi
		pop esi
		pop esp
		pop edx
		pop ecx
		pop ebx
		pop eax
	}

	isOnLeftNormalFukidashiUpdating = FALSE;
}




/*
00411B3E  |. 53             PUSH    EBX
00411B3F  |. 57             PUSH    EDI
00411B40  |. 68 31140000    PUSH    1431
00411B45  |. E8 34A10100    CALL    TENSHOU.0042BC7E
00411B4A  |. 83C4 0C        ADD     ESP, 0C

*/

int pCallFromFuncRightFukidashiUpdate1 = 0x42BC7E; // ���XTENSHOU.EXE���ɂ�����CALL��
int iBushouIDOfRightNormalFukidashiUpdate = -1;
int isOnRightNormalFukidashiUpdating = FALSE;
void FuncRightNormalFukidashiUpdateAsm() {

	__asm {
		mov eax, iBushouIDOfRightNormalFukidashiUpdate
		mov edi, 0
		mov ebp, 0x4d7c90
		push edi
		push ebp
		push 0x1431
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

	ReWriteNormalFukidashiMessage();

	__asm {
		pop edi
		pop esi
		pop esp
		pop edx
		pop ecx
		pop ebx
		pop eax
	}

	__asm {
		call pCallFromFuncRightFukidashiUpdate1                 ; \TENSHOU.0042BC7E
		add     esp, 0x0C
	}
}


// 
void FuncRightNormalFukidashiUpdate(int iBushouID, char *szMessage) {
	iBushouIDOfRightNormalFukidashiUpdate = iBushouID+1;
	iBushouIDOfNormalFukidashiMessage = iBushouIDOfRightNormalFukidashiUpdate;
	strcpy( pszMessageOfNormalFukidashiMessage, szMessage);

	isOnRightNormalFukidashiUpdating = TRUE;

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
	FuncRightNormalFukidashiUpdateAsm();

	__asm {
		pop edi
		pop esi
		pop esp
		pop edx
		pop ecx
		pop ebx
		pop eax
	}

	isOnRightNormalFukidashiUpdating = FALSE;
}





//---------------------�����o���X�V-------------------------

// �����o���̍X�V
/*
void FuncRightNormalFukidashiUpdate(int iBushouID, char *szMessage) {
	FuncRightNormalFukidashiDelete();
	FuncRightNormalFukidashiMessage(iBushouID, szMessage);
}
*/
/*
// �����o���̍X�V
void FuncLeftNormalFukidashiUpdate(int iBushouID, char *szMessage) {
	FuncLeftNormalFukidashiDelete();
	FuncLeftNormalFukidashiMessage(iBushouID, szMessage);
}
*/





//---------------------����Ɉ����o��������-------------------------
int pCallFromFuncFukidashiDelete = 0x47D438; // ���XTENSHOU.EXE���ɂ�����CALL��


void FuncLeftNormalFukidashiDeleteAsm() {

	__asm {
		mov ecx, 0x4D9AA8
		call pCallFromFuncFukidashiDelete
	}

}
void FuncRightNormalFukidashiDeleteAsm() {

	__asm {
		mov ecx, 0x4D99F0
		call pCallFromFuncFukidashiDelete
	}

}


// 
void FuncLeftNormalFukidashiDelete() {
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
	FuncLeftNormalFukidashiDeleteAsm();

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

void FuncRightNormalFukidashiDelete() {
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
	FuncRightNormalFukidashiDeleteAsm();

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




// �O���Q�Ɨp
void WINAPI Extern_FuncLeftNormalFukidashiMessage(int iBushouID, char *szMessage) {
	FuncLeftNormalFukidashiMessage(iBushouID, szMessage);
}
void WINAPI Extern_FuncRightNormalFukidashiMessage(int iBushouID, char *szMessage) {
	FuncRightNormalFukidashiMessage(iBushouID, szMessage);
}
void WINAPI Extern_FuncLeftNormalFukidashiUpdate(int iBushouID, char *szMessage) {
	FuncLeftNormalFukidashiUpdate(iBushouID, szMessage);
}
void WINAPI Extern_FuncRightNormalFukidashiUpdate(int iBushouID, char *szMessage) {
	FuncRightNormalFukidashiUpdate(iBushouID, szMessage);
}

void WINAPI Extern_FuncLeftNormalFukidashiDelete() {
	FuncLeftNormalFukidashiDelete();
}
void WINAPI Extern_FuncRightNormalFukidashiDelete() {
	FuncRightNormalFukidashiDelete();
}





/*
 * ���o���̍���̃��b�Z�[�W���X�V���ꂽ�ۂ̃A�h���X
 */
int pointerOfFukidashiLeftUpdateMsgPointerExecute = NULL;
void OnTenshouExeFukidashiLeftUpdateMsgPointerExecute() {
	
	if (isOnLeftNormalFukidashiUpdating) {
		// �����œn���ė������̂��R�s�[
		strcpy( (char *)pointerOfFukidashiLeftUpdateMsgPointerExecute, pszMessageOfNormalFukidashiMessage);

	}
}


/*
0042BC5C  |. FF7424 24      PUSH    DWORD PTR SS:[ESP+24]            ; |Arg1
0042BC60  |. E8 FB030400    CALL    TENSHOU.0046C060                 ; \TENSHOU.0046C060
				�������� JMP TSMod.OnTSExeFukidashiLeftUpdateMsgPointer �Ə��������Ă����ۂ̏���
0042BC65  |. 83C4 20        ADD     ESP, 20

*/
int pTenshouExeJumpFromToOnTenshouExeFukidashiLeftUpdateMsgPointer	 =0x42BC60; // �֐��͂��̃A�h���X����AOnTenshouExeFukidashiLeftUpdateMsgPointer�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeFukidashiLeftUpdateMsgPointer =0x46C060; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeFukidashiLeftUpdateMsgPointer  =0x42BC65; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeFukidashiLeftUpdateMsgPointer() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {
		// ���XTENSHOU.EXE���ɂ��������̂�������
		call pTenshouExeJumpCallFromToOnTenshouExeFukidashiLeftUpdateMsgPointer

		// ���̎��AEAX�̒l���A���㕬�o�����b�Z�[�W�ōX�V�\������ۂ̃��b�Z�[�W������ւ̃|�C���^�̏ꏊ
		mov pointerOfFukidashiLeftUpdateMsgPointerExecute, EAX

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}
	
	OnTenshouExeFukidashiLeftUpdateMsgPointerExecute();

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

		jmp pTenshouExeReturnLblFromOnTenshouExeFukidashiLeftUpdateMsgPointer
	}
}

/*
*/
char cmdOnTenshouExeJumpFromFukidashiLeftUpdateMsgPointer[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeFukidashiLeftUpdateMsgPointer() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeFukidashiLeftUpdateMsgPointer;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeFukidashiLeftUpdateMsgPointer + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeFukidashiLeftUpdateMsgPointer  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromFukidashiLeftUpdateMsgPointer+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeFukidashiLeftUpdateMsgPointer), cmdOnTenshouExeJumpFromFukidashiLeftUpdateMsgPointer, 5, NULL); //5�o�C�g�̂ݏ�������
}








//-----------------------------------------------------------------------------------------------------
/*
 * ���o���̉E���̃��b�Z�[�W���X�V���ꂽ�ۂ̃A�h���X
 */
int pointerOfFukidashiRightUpdateMsgPointerExecute = NULL;
void OnTenshouExeFukidashiRightUpdateMsgPointerExecute() {
	if (isOnRightNormalFukidashiUpdating) {
		// �����œn���ė������̂��R�s�[
		strcpy( (char *)pointerOfFukidashiRightUpdateMsgPointerExecute, pszMessageOfNormalFukidashiMessage);
	}
}


/*
0042BC8E  |. FF7424 24      PUSH    DWORD PTR SS:[ESP+24]            ; |Arg1
0042BC92  |. E8 C9030400    CALL    TENSHOU.0046C060                 ; \TENSHOU.0046C060
				�������� JMP TSMod.OnTSExeFukidashiRightUpdateMsgPointer �Ə��������Ă����ۂ̏���
0042BC97  |. 83C4 20        ADD     ESP, 20

*/
int pTenshouExeJumpFromToOnTenshouExeFukidashiRightUpdateMsgPointer	   = 0x42BC92; // �֐��͂��̃A�h���X����AOnTenshouExeFukidashiRightUpdateMsgPointer�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeFukidashiRightUpdateMsgPointer =0x46C060; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeFukidashiRightUpdateMsgPointer  =0x42BC97; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeFukidashiRightUpdateMsgPointer() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {
		// ���XTENSHOU.EXE���ɂ��������̂�������
		call pTenshouExeJumpCallFromToOnTenshouExeFukidashiRightUpdateMsgPointer

		// ���̎��AEAX�̒l���A�E�����o�����b�Z�[�W�ōX�V�\������ۂ̃��b�Z�[�W������ւ̃|�C���^�̏ꏊ
		mov pointerOfFukidashiRightUpdateMsgPointerExecute, EAX

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}
	
	OnTenshouExeFukidashiRightUpdateMsgPointerExecute();

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

		jmp pTenshouExeReturnLblFromOnTenshouExeFukidashiRightUpdateMsgPointer
	}
}

/*
*/
char cmdOnTenshouExeJumpFromFukidashiRightUpdateMsgPointer[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeFukidashiRightUpdateMsgPointer() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeFukidashiRightUpdateMsgPointer;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeFukidashiRightUpdateMsgPointer + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeFukidashiRightUpdateMsgPointer  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromFukidashiRightUpdateMsgPointer+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeFukidashiRightUpdateMsgPointer), cmdOnTenshouExeJumpFromFukidashiRightUpdateMsgPointer, 5, NULL); //5�o�C�g�̂ݏ�������
}

