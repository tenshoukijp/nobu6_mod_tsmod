#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"
#include "TSModCommand.h"



// �퓬���̏o�w���X�g�I�����ɁA�o�w�ő吔�����肳���^�C�~���O

int iSubstituteMaxShutujinNum = -1;

void OnTenshouExeSubstituteMaxShutujinNumExecute() {
	// �o�w�����w�肵�Ă���ꍇ�B�ő吔������ɕς���
	if ( TSModCommand::AllGundan::cmdTheNumberOfTheMaxGoToWarFix > 0 ) {
	}
}


/*
004826A9  |. E8 B48FFAFF    |CALL    TENSHOU.0042B662										�@�@�@�@���c��o�w���Z�o�֐�		
				�������� JMP TSMod.OnTSExeSubstituteMaxShutujinNum �Ə��������Ă����ۂ̏���
004826AE  |. 83C4 04        |ADD     ESP, 4												
004826B1  |. 50             |PUSH    EAX                             ; |Arg2						���c��o�w�l��
 */
int pTenshouExeJumpFromToOnTenshouExeSubstituteMaxShutujinNum    =0x4826A9; // �֐��͂��̃A�h���X����AOnTenshouExeSubstituteMaxShutujinNum�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeSubstituteMaxShutujinNum=0x42B662; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeSubstituteMaxShutujinNum =0x4826AE; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeSubstituteMaxShutujinNum() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {
		// ���XTENSHOU.EXE���ɂ��������̂�������
		call pTenshouExeJumpCallFromToOnTenshouExeSubstituteMaxShutujinNum

		mov iSubstituteMaxShutujinNum, eax

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
	OnTenshouExeSubstituteMaxShutujinNumExecute();

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

		mov eax, iSubstituteMaxShutujinNum

		jmp pTenshouExeReturnLblFromOnTenshouExeSubstituteMaxShutujinNum
	}
}

/*
004826A9  |. E8 B48FFAFF    |CALL    TENSHOU.0042B662										�@�@�@�@���c��o�w���Z�o�֐�		
				�������� JMP TSMod.OnTSExeSubstituteMaxShutujinNum �Ə��������Ă����ۂ̏���
004826AE  |. 83C4 04        |ADD     ESP, 4												
004826B1  |. 50             |PUSH    EAX                             ; |Arg2						���c��o�w�l��
*/
char cmdOnTenshouExeJumpFromSubstituteMaxShutujinNum[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeSubstituteMaxShutujinNum() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeSubstituteMaxShutujinNum;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeSubstituteMaxShutujinNum + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeSubstituteMaxShutujinNum  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromSubstituteMaxShutujinNum+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeSubstituteMaxShutujinNum), cmdOnTenshouExeJumpFromSubstituteMaxShutujinNum, 5, NULL); //5�o�C�g�̂ݏ�������
}







//------------------------------------------------------------------------------------------------







// �퓬���̏o�w���X�g�I�����ɁA(���Ɓ������o�w�ł��܂��Ƃ�����)���b�Z�[�W���̏o�w�ő吔�̂����肳���^�C�~���O

int iSubstituteMaxShutujinMsg = -1;

void OnTenshouExeSubstituteMaxShutujinMsgExecute() {
	// �o�w�����w�肵�Ă���ꍇ�B�ő吔������ɕς���
	if ( TSModCommand::AllGundan::cmdTheNumberOfTheMaxGoToWarFix > 0 ) {
		iSubstituteMaxShutujinMsg = 8;
	}
}


/*
0048273F  |> 50             PUSH    EAX
00482740  |. FF75 FE        PUSH    DWORD PTR SS:[EBP-2]
00482743  |. E8 14AFF8FF    CALL    TENSHOU.0040D65C
				�������� JMP TSMod.OnTSExeSubstituteMaxShutujinMsg �Ə��������Ă����ۂ̏���
00482748  |. 83C4 04        ADD     ESP, 4
0048274B  |. 50             PUSH    EAX
 */
int pTenshouExeJumpFromToOnTenshouExeSubstituteMaxShutujinMsg    =0x482743; // �֐��͂��̃A�h���X����AOnTenshouExeSubstituteMaxShutujinMsg�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeSubstituteMaxShutujinMsg=0x40D65C; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeSubstituteMaxShutujinMsg =0x482748; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeSubstituteMaxShutujinMsg() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {
		// push�������̓i�V
		pop iSubstituteMaxShutujinMsg // �󂯎M�ϐ����Ȃ��̂œK����
		pop iSubstituteMaxShutujinMsg // �󂯎M�ϐ����Ȃ��̂œK����

		// eax��iSubstituteMaxShutujinMsg�ɃR�s�[
		mov iSubstituteMaxShutujinMsg, eax

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
	OnTenshouExeSubstituteMaxShutujinMsgExecute();

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

		// �㏑������iSubstituteMaxShutujinMsg��eax�ɃR�s�[
		mov eax, iSubstituteMaxShutujinMsg

		// ���Xpush���ׂ����̂�������
		PUSH    EAX
		PUSH    DWORD PTR SS:[EBP-0x2]

		// ���XTENSHOU.EXE���ɂ��������̂�������
		call pTenshouExeJumpCallFromToOnTenshouExeSubstituteMaxShutujinMsg

		jmp pTenshouExeReturnLblFromOnTenshouExeSubstituteMaxShutujinMsg
	}
}

/*
0048273F  |> 50             PUSH    EAX
00482740  |. FF75 FE        PUSH    DWORD PTR SS:[EBP-2]
00482743  |. E8 14AFF8FF    CALL    TENSHOU.0040D65C
				�������� JMP TSMod.OnTSExeSubstituteMaxShutujinMsg �Ə��������Ă����ۂ̏���
00482748  |. 83C4 04        ADD     ESP, 4
0048274B  |. 50             PUSH    EAX
*/
char cmdOnTenshouExeJumpFromSubstituteMaxShutujinMsg[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeSubstituteMaxShutujinMsg() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeSubstituteMaxShutujinMsg;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeSubstituteMaxShutujinMsg + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeSubstituteMaxShutujinMsg  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromSubstituteMaxShutujinMsg+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeSubstituteMaxShutujinMsg), cmdOnTenshouExeJumpFromSubstituteMaxShutujinMsg, 5, NULL); //5�o�C�g�̂ݏ�������
}




//------------------------------------------------------------------------------------------------



// �퓬���̏o�w���X�g�I�����ɁA�o�w�c�萔�����肳���^�C�~���O

int iSubstituteMaxShutujinRemain1 = -1;

void OnTenshouExeSubstituteMaxShutujinRemain1Execute() {

	// �o�w�����w�肵�Ă���ꍇ�B�ő吔������ɕς���
	if ( TSModCommand::AllGundan::cmdTheNumberOfTheMaxGoToWarFix > 0 ) {
		iSubstituteMaxShutujinRemain1 = 8;
	}
}


/*
0042B6B5  |. E8 A6FF0100    CALL    TENSHOU.0044B660
				�������� JMP TSMod.OnTSExeSubstituteMaxShutujinRemain1 �Ə��������Ă����ۂ̏���
0042B6BA  |. 83C4 04        ADD     ESP, 4
0042B6BD  |. 2BC7           SUB     EAX, EDI
 */
int pTenshouExeJumpFromToOnTenshouExeSubstituteMaxShutujinRemain1    =0x42B6B5; // �֐��͂��̃A�h���X����AOnTenshouExeSubstituteMaxShutujinRemain1�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeSubstituteMaxShutujinRemain1=0x44B660; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeSubstituteMaxShutujinRemain1 =0x42B6BA; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeSubstituteMaxShutujinRemain1() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {
		// ���XTENSHOU.EXE���ɂ��������̂�������
		call pTenshouExeJumpCallFromToOnTenshouExeSubstituteMaxShutujinRemain1

		mov iSubstituteMaxShutujinRemain1, eax

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
	OnTenshouExeSubstituteMaxShutujinRemain1Execute();

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

		mov eax, iSubstituteMaxShutujinRemain1

		jmp pTenshouExeReturnLblFromOnTenshouExeSubstituteMaxShutujinRemain1
	}
}

/*
004826A9  |. E8 B48FFAFF    |CALL    TENSHOU.0042B662										�@�@�@�@���c��o�w���Z�o�֐�		
				�������� JMP TSMod.OnTSExeSubstituteMaxShutujinRemain1 �Ə��������Ă����ۂ̏���
004826AE  |. 83C4 04        |ADD     ESP, 4												
004826B1  |. 50             |PUSH    EAX                             ; |Arg2						���c��o�w�l��
*/
char cmdOnTenshouExeJumpFromSubstituteMaxShutujinRemain1[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeSubstituteMaxShutujinRemain1() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeSubstituteMaxShutujinRemain1;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeSubstituteMaxShutujinRemain1 + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeSubstituteMaxShutujinRemain1  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromSubstituteMaxShutujinRemain1+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeSubstituteMaxShutujinRemain1), cmdOnTenshouExeJumpFromSubstituteMaxShutujinRemain1, 5, NULL); //5�o�C�g�̂ݏ�������
}






//---------------------------------------------------------------------




// �퓬���̏o�w���X�g�I�����ɁA�o�w�c�萔�����肳���^�C�~���O

int iSubstituteMaxShutujinRemain2 = -1;


void OnTenshouExeSubstituteMaxShutujinRemain2Execute() {

	// �o�w�����w�肵�Ă���ꍇ�B�ő吔������ɕς���
	if ( TSModCommand::AllGundan::cmdTheNumberOfTheMaxGoToWarFix > 0 ) {
		iSubstituteMaxShutujinRemain2 = 8;
	}
}


/*
0042B6C6  |. E8 95FF0100    CALL    TENSHOU.0044B660
				�������� JMP TSMod.OnTSExeSubstituteMaxShutujinRemain2 �Ə��������Ă����ۂ̏���
0042B6CB  |. 83C4 04        ADD     ESP, 4
0042B6CE  |. 2BC7           SUB     EAX, EDI


 */
int pTenshouExeJumpFromToOnTenshouExeSubstituteMaxShutujinRemain2    =0x42B6C6; // �֐��͂��̃A�h���X����AOnTenshouExeSubstituteMaxShutujinRemain2�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeSubstituteMaxShutujinRemain2=0x44B660; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeSubstituteMaxShutujinRemain2 =0x42B6CB; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeSubstituteMaxShutujinRemain2() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {
		// ���XTENSHOU.EXE���ɂ��������̂�������
		call pTenshouExeJumpCallFromToOnTenshouExeSubstituteMaxShutujinRemain2

		mov iSubstituteMaxShutujinRemain2, eax

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
	OnTenshouExeSubstituteMaxShutujinRemain2Execute();

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

		mov eax, iSubstituteMaxShutujinRemain2

		jmp pTenshouExeReturnLblFromOnTenshouExeSubstituteMaxShutujinRemain2
	}
}

/*
004826A9  |. E8 B48FFAFF    |CALL    TENSHOU.0042B662										�@�@�@�@���c��o�w���Z�o�֐�		
				�������� JMP TSMod.OnTSExeSubstituteMaxShutujinRemain2 �Ə��������Ă����ۂ̏���
004826AE  |. 83C4 04        |ADD     ESP, 4												
004826B1  |. 50             |PUSH    EAX                             ; |Arg2						���c��o�w�l��
*/
char cmdOnTenshouExeJumpFromSubstituteMaxShutujinRemain2[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeSubstituteMaxShutujinRemain2() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeSubstituteMaxShutujinRemain2;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeSubstituteMaxShutujinRemain2 + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeSubstituteMaxShutujinRemain2  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromSubstituteMaxShutujinRemain2+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeSubstituteMaxShutujinRemain2), cmdOnTenshouExeJumpFromSubstituteMaxShutujinRemain2, 5, NULL); //5�o�C�g�̂ݏ�������
}






//---------------------------------------------------------------------

bool bRewrite8WakuShutsuJined = false; // ����łW���������Ȃ�o�w���Ă��܂����ꍇ�Ƀj�[���j�b�N���s���Ȃ��悤�ɁB

// �j�[���j�b�N���������p
// 004826E7     2BC6           SUB     EAX, ESI
// 004826E9     74 05          JE      SHORT TENSHOU.004826F0   ��������NopNop�ɂ���B
// 004826EB     83F8 08        CMP     EAX, 8
void WriteAsmJumperOnTenshouExeSubstituteMaxShutujinCaseAll() {

	// �o�w�����w�肵�Ă���ꍇ�B�ő吔������ɕς���
	if ( TSModCommand::AllGundan::cmdTheNumberOfTheMaxGoToWarFix > 0 ) {
		if ( !bRewrite8WakuShutsuJined ) {


			char szNopNop[] = "\x90\x90";

			// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
			WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(0x4826E9), szNopNop, 2, NULL); //2�o�C�g�̂ݏ�������

			bRewrite8WakuShutsuJined = true;
		}
	}
}