#include "WinTarget.h"
#include <fstream>

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"
#include "MessageN6PPatch.h"

using namespace std;



/*
 * ��b���ɢ����̖��O���ID����i�[����K�v������^�C�~���O(Wsprintf�ւ̕ϐ��i�[)�ŌĂяo�����
 */

// ����̕����̖��O���i�[����Ă��镶����ւ̃A�h���X
int iStrAddressOppositeBushouName = -1;

// ����̕�����ID
int iOppositeBushouID = -1;

void OnTenshouExeWsprintfOppositeIDAndStrExecute() {
	// ���ɉ����Ȃ��BWsprintf�ւ̃p�����^���߂𒼐ڈӎ�����K�v���o��΍l����B
}


/*
0046B8F2  |. 50             PUSH    EAX				���̒i�K�ŁAESI�͑����ID, EAX������̖��O				
0046B8F3  |. FF75 0C        PUSH    DWORD PTR SS:[EBP+C]								
0046B8F6  |. E8 F2180400    CALL    TENSHOU.004AD1ED								
				�������� JMP TSMod.OnTSExeWsprintfOppositeIDAndStr �Ə��������Ă����ۂ̏���
0046B8FB  |. 83C4 08        ADD     ESP, 8								
0046B8FE  |> 33C0           XOR     EAX, EAX								
0046B900  |. EB 0E          JMP     SHORT TENSHOU.0046B910								
 */
int pTenshouExeJumpFromToOnTenshouExeWsprintfOppositeIDAndStr    =0x46B8F6; // �֐��͂��̃A�h���X����AOnTenshouExeWsprintfOppositeIDAndStr�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeWsprintfOppositeIDAndStr=0x4AD1ED; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeWsprintfOppositeIDAndStr =0x46B8FB; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeWsprintfOppositeIDAndStr() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {
		// ���XTENSHOU.EXE���ɂ��������̂�������
		call pTenshouExeJumpCallFromToOnTenshouExeWsprintfOppositeIDAndStr

		// ����̕�����ID���R�s�[
		mov iOppositeBushouID, esi;

		// ����̕����̖��O���i�[����Ă���A�h���X���R�s�[
		mov iStrAddressOppositeBushouName, eax;

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
	OnTenshouExeWsprintfOppositeIDAndStrExecute();

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

		jmp pTenshouExeReturnLblFromOnTenshouExeWsprintfOppositeIDAndStr
	}
}

/*
00444769  |. 50             PUSH    EAX
0044476A  |. E8 3A2C0200    CALL    TENSHOU.004673A9
				�������� JMP TSMod.OnTSExeWsprintfOppositeIDAndStr �Ə��������Ă����ۂ̏���
0044476F  |. 83C4 04        ADD     ESP, 4
*/
char cmdOnTenshouExeJumpFromWsprintfOppositeIDAndStr[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeWsprintfOppositeIDAndStr() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeWsprintfOppositeIDAndStr;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeWsprintfOppositeIDAndStr + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeWsprintfOppositeIDAndStr  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromWsprintfOppositeIDAndStr+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeWsprintfOppositeIDAndStr), cmdOnTenshouExeJumpFromWsprintfOppositeIDAndStr, 5, NULL); //5�o�C�g�̂ݏ�������
}




