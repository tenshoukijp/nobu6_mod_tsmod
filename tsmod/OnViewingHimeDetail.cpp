#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "VariableNameRegistAndJudge.h"


/*
 �P�̏ڍ׃y�[�W��\�������ۂ̏���
 */


/*
�Y�����^�[�Q�b�g�̕P�̔ԍ�

00494480  |. 66:8B5E 70     MOV     BX, WORD PTR DS:[ESI+70] // ������BX���P���̕����ԍ�
00494484  |. 53             PUSH    EBX
00494485  |. E8 F35AF7FF    CALL    TENSHOU.00409F7D
0049448A  |. 83C4 04        ADD     ESP, 4



���������̎��AEBP��BP��
00494530  |. 68 102F4D00    PUSH    TENSHOU.004D2F10                 ;  ASCII "%s�̖�"
00494535  |. 68 F8674E00    PUSH    TENSHOU.004E67F8                 ;  ASCII "�VK"
0049453A  |. E8 71A00100    CALL    TENSHOU.004AE5B0
0049453F  |. 83C4 0C        ADD     ESP, 0C

�������Ă��Ȃ��Ƃ�������������B
*/

int iHimeBushouIDOnViewingHimeDtail = -1;
void OnTenshouExeViewingHimeDetailExecute() {
	iHimeBushouIDOnViewingHimeDtail = iHimeBushouIDOnViewingHimeDtail & 0xFFFF; // BX���W�X�^�����ɂ���B
}


/*
00494485  |. E8 F35AF7FF    CALL    TENSHOU.00409F7D
				�������� JMP TSMod.OnTSExeViewingHimeDetail �Ə��������Ă����ۂ̏���
0049448A  |. 83C4 04        ADD     ESP, 4
*/
int pTenshouExeJumpFromToOnTenshouExeViewingHimeDetail		=0x494485; // �֐��͂��̃A�h���X����AOnTenshouExeViewingHimeDetail�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeViewingHimeDetail  =0x409F7D; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeViewingHimeDetail	=0x49448A; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeViewingHimeDetail() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {
		mov iHimeBushouIDOnViewingHimeDtail, ebx; // �ΏۂƂȂ�P�̕����ԍ��̊m��

		// ���XTENSHOU.EXE���ɂ��������̂�������
		call pTenshouExeJumpCallFromToOnTenshouExeViewingHimeDetail

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
	OnTenshouExeViewingHimeDetailExecute();

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

		jmp pTenshouExeReturnLblFromOnTenshouExeViewingHimeDetail
	}
}

char cmdOnTenshouExeJumpFromViewingHimeDetail[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeViewingHimeDetail() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeViewingHimeDetail;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeViewingHimeDetail + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeViewingHimeDetail  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromViewingHimeDetail+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeViewingHimeDetail), cmdOnTenshouExeJumpFromViewingHimeDetail, 5, NULL); //5�o�C�g�̂ݏ�������
}


// -----------------------------------------------------------------------------------------------


// ���̎��ɁA���̕������u��������

extern bool isHimeTartareAndVersalia(int iBushouID, int iHimeBushouID);


int iOyaBushouIDOnViewingHimeDetailMusume = -1;
int iPushAddressOnViewingHimeDetailMusume = 0x4D2F10; // ;  ASCII "%s�̖�"�̃A�h���X
char szPushAdressOnViewingHimeDetailYoujo[] = "%s�̗{��";
char szPushAdressOnViewingHimeDetailMekake[] = "%s�̈���";

void OnTenshouExeViewingHimeDetailMusumeExecute() {
	iOyaBushouIDOnViewingHimeDetailMusume = iOyaBushouIDOnViewingHimeDetailMusume & 0xFFFF; // BX���W�X�^�����ɂ���B

	int iBushouID = iOyaBushouIDOnViewingHimeDetailMusume-1;
	int iHimeBushouID = iHimeBushouIDOnViewingHimeDtail-1;

	// �܂��f�t�H���g�����Ă���
	iPushAddressOnViewingHimeDetailMusume = 0x4D2F10;

	if ( ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) && ( 0 <= iHimeBushouID && iHimeBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) ) {

		// �����X���b�g�ƃx���T���A
		if ( isHimeTartareAndVersalia(iBushouID, iHimeBushouID) ) {
			iPushAddressOnViewingHimeDetailMusume = int(szPushAdressOnViewingHimeDetailMekake);
		}

		// �ȉ��{���̗v���𖞂�������
		// ���X �� �����ȊO
		else if ( nb6bushouname[iHimeBushouID].number == 366 && nb6bushouname[iBushouID].number != 31 &&
			IsOriginalRegistedBushou(iHimeBushouID) && IsOriginalRegistedBushou(iBushouID) ) { 
			iPushAddressOnViewingHimeDetailMusume = int(szPushAdressOnViewingHimeDetailYoujo);
		// �� �� �����ȊO
		}
		else if ( nb6bushouname[iHimeBushouID].number == 367 && nb6bushouname[iBushouID].number != 31 &&
			IsOriginalRegistedBushou(iHimeBushouID) && IsOriginalRegistedBushou(iBushouID) ) { 
			iPushAddressOnViewingHimeDetailMusume = int(szPushAdressOnViewingHimeDetailYoujo);
		}
		// ���X �� �����ȊO
		else if ( nb6bushouname[iHimeBushouID].number == 368 && nb6bushouname[iBushouID].number != 31 &&
			IsOriginalRegistedBushou(iHimeBushouID) && IsOriginalRegistedBushou(iBushouID) ) { 
			iPushAddressOnViewingHimeDetailMusume = int(szPushAdressOnViewingHimeDetailYoujo);
		}
		// �O�c�� �� ���ƈȊO
		else if ( nb6bushouname[iHimeBushouID].number == 378 && nb6bushouname[iBushouID].number != 2 &&
			IsOriginalRegistedBushou(iHimeBushouID) && IsOriginalRegistedBushou(iBushouID) ) { 
			iPushAddressOnViewingHimeDetailMusume = int(szPushAdressOnViewingHimeDetailYoujo);
		}
		// ���؂��قȂ�
		else if ( nb6bushouref[iHimeBushouID].blood != nb6bushouref[iBushouID].blood ) {
			iPushAddressOnViewingHimeDetailMusume = int(szPushAdressOnViewingHimeDetailYoujo);
		}
	}
	// �悭�킩���z�́u���v�̂܂�
}


/*
EBP��BP���e�̔ԍ�
00494530  |. 68 102F4D00    PUSH    TENSHOU.004D2F10                 ;  ASCII "%s�̖�"
00494535  |. 68 F8674E00    PUSH    TENSHOU.004E67F8                 ;  ASCII "�VK"
				�������� JMP TSMod.OnTSExeViewingHimeDetailMusume �Ə��������Ă����ۂ̏���
*/
int pTenshouExeJumpFromToOnTenshouExeViewingHimeDetailMusume	=0x494530; // �֐��͂��̃A�h���X����AOnTenshouExeViewingHimeDetailMusume�ւƃW�����v���Ă���B
int pTenshouExeReturnLblFromOnTenshouExeViewingHimeDetailMusume	=0x494535; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeViewingHimeDetailMusume() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {
		mov iOyaBushouIDOnViewingHimeDetailMusume, EBP // �e�̕����ԍ��̊m��

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
	OnTenshouExeViewingHimeDetailMusumeExecute();

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

		push iPushAddressOnViewingHimeDetailMusume; // TENSHOU.EXE�Ɍ��X���݂�������

		jmp pTenshouExeReturnLblFromOnTenshouExeViewingHimeDetailMusume
	}
}

char cmdOnTenshouExeJumpFromViewingHimeDetailMusume[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeViewingHimeDetailMusume() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeViewingHimeDetailMusume;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeViewingHimeDetailMusume + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeViewingHimeDetailMusume  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromViewingHimeDetailMusume+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeViewingHimeDetailMusume), cmdOnTenshouExeJumpFromViewingHimeDetailMusume, 5, NULL); //5�o�C�g�̂ݏ�������
}


//---------------------------------------------------------------------------



int iOttoBushouIDOnViewingHimeDetailTsuma = -1;
int iPushAddressOnViewingHimeDetailTsuma = 0x4D2EF8; // ;  ASCII "%s�̍�"�̃A�h���X
char szPushAdressOnViewingHimeDetailSeisitsu[] = "%s�̐���";
char szPushAdressOnViewingHimeDetailSokusitsu[] = "%s�̑���";
char szPushAdressOnViewingHimeDetailKonyakusya[] = "%s�̍����";
void OnTenshouExeViewingHimeDetailTsumaExecute() {
	iOttoBushouIDOnViewingHimeDetailTsuma = iOttoBushouIDOnViewingHimeDetailTsuma & 0xFFFF; // DI���W�X�^�����ɂ���B

	int iBushouID = iOttoBushouIDOnViewingHimeDetailTsuma-1;
	int iHimeBushouID = iHimeBushouIDOnViewingHimeDtail-1;

	// �܂��f�t�H���g�����Ă����B
	iPushAddressOnViewingHimeDetailTsuma = 0x4D2EF8;

	if ( ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) && ( 0 <= iHimeBushouID && iHimeBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) ) {
		// �I�Y�}�E���[�E�O���V���X�ƃo�[���[�t�H���EV�E���[���Y �Ȃ�u����ҁv�Ƃ���B
		if ( strcmp( nb6bushouname[iHimeBushouID].familyname, "�v�w�x") == 0 && strcmp( nb6bushouname[iHimeBushouID].fastname, "�y�z�{") == 0 
			&& strcmp( nb6bushouname[iBushouID].familyname, "�I�J�K") == 0 && strcmp( nb6bushouname[iBushouID].fastname, "�L�M�N") == 0 ) { 
			// �����
			iPushAddressOnViewingHimeDetailTsuma = int(szPushAdressOnViewingHimeDetailKonyakusya);

		// �����̏����𖞂������̂��v�������񂾂�A�����ɂ��̏������o��
		// �z�K �Έ� �� ���c�M��
		} else if ( nb6bushouname[iHimeBushouID].number == 372 && nb6bushouname[iBushouID].number == 19 &&
			IsOriginalRegistedBushou(iHimeBushouID) && IsOriginalRegistedBushou(iBushouID) ) { 
			iPushAddressOnViewingHimeDetailTsuma = int(szPushAdressOnViewingHimeDetailSokusitsu);

		// ���X �� �G�g
		} else if ( nb6bushouname[iHimeBushouID].number == 366 && nb6bushouname[iBushouID].number == 5 &&
			IsOriginalRegistedBushou(iHimeBushouID) && IsOriginalRegistedBushou(iBushouID) ) { 
			iPushAddressOnViewingHimeDetailTsuma = int(szPushAdressOnViewingHimeDetailSokusitsu);

		// �� �� �G�g
		} else if ( nb6bushouname[iHimeBushouID].number == 367 && nb6bushouname[iBushouID].number == 5 &&
			IsOriginalRegistedBushou(iHimeBushouID) && IsOriginalRegistedBushou(iBushouID) ) { 
			iPushAddressOnViewingHimeDetailTsuma = int(szPushAdressOnViewingHimeDetailSokusitsu);

		// �B�q(�]) �� �G�g
		} else if ( nb6bushouname[iHimeBushouID].number == 368 && nb6bushouname[iBushouID].number == 5 &&
			IsOriginalRegistedBushou(iHimeBushouID) && IsOriginalRegistedBushou(iBushouID) ) { 
			iPushAddressOnViewingHimeDetailTsuma = int(szPushAdressOnViewingHimeDetailSokusitsu);

		// ���̑��͐����Ƃ���
		} else {
			iPushAddressOnViewingHimeDetailTsuma = int(szPushAdressOnViewingHimeDetailSeisitsu);
		}
	}
	// �悭�킩���z�́u�ȁv�̂܂�
}


/*
EDI��DI���e�̔ԍ�
0049466E  |. 68 F82E4D00    PUSH    TENSHOU.004D2EF8                 ;  ASCII "%s�̍�"
00494673  |. 68 F8674E00    PUSH    TENSHOU.004E67F8                 ;  ASCII "�VK"
				�������� JMP TSMod.OnTSExeViewingHimeDetailTsuma �Ə��������Ă����ۂ̏���
*/
int pTenshouExeJumpFromToOnTenshouExeViewingHimeDetailTsuma		=0x49466E; // �֐��͂��̃A�h���X����AOnTenshouExeViewingHimeDetailTsuma�ւƃW�����v���Ă���B
int pTenshouExeReturnLblFromOnTenshouExeViewingHimeDetailTsuma	=0x494673; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeViewingHimeDetailTsuma() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {
		mov iOttoBushouIDOnViewingHimeDetailTsuma, EDI // �v�̕����ԍ��̊m��

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
	OnTenshouExeViewingHimeDetailTsumaExecute();

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

		push iPushAddressOnViewingHimeDetailTsuma; // TENSHOU.EXE�Ɍ��X���݂�������

		jmp pTenshouExeReturnLblFromOnTenshouExeViewingHimeDetailTsuma
	}
}

char cmdOnTenshouExeJumpFromViewingHimeDetailTsuma[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeViewingHimeDetailTsuma() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeViewingHimeDetailTsuma;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeViewingHimeDetailTsuma + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeViewingHimeDetailTsuma  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromViewingHimeDetailTsuma+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeViewingHimeDetailTsuma), cmdOnTenshouExeJumpFromViewingHimeDetailTsuma, 5, NULL); //5�o�C�g�̂ݏ�������
}





//--------------------------------------------------------------------------------



//---------------------------------------------------------------------------




//---------------------------------------------------------------------------
/*
004945C4  |. 3BD8           CMP     EBX, EAX
004945C6  |. B8 0C2F4D00    MOV     EAX, TENSHOU.004D2F0C                                     ;  ASCII "�o"
004945CB  |. 77 05          JA      SHORT TENSHOU.004945D2
004945CD  |. B8 082F4D00    MOV     EAX, TENSHOU.004D2F08                                     ;  ASCII "��"
004945D2  |> 50             PUSH    EAX
*/


int iShiteiBushouIDOnViewingHimeDetailShimai = -1;
int iDefaultAddressOnViewingHimeDetailAne = 0x4D2F0C; // ;  ASCII "�o"�̃A�h���X
int iDefaultAddressOnViewingHimeDetailImouto = 0x4D2F08; // ;  ASCII "��"�̃A�h���X
int iMovAddressOnViewingHimeDetailShimai = 0x4D2F0C; // ;  �o�����I�[�o�[���C�h�����A�h���X
char szPushAdressOnViewingHimeDetailGiShi[] = "%s�̋`�o";
char szPushAdressOnViewingHimeDetailGimai[] = "%s�̋`��";
void OnTenshouExeViewingHimeDetailShimaiExecute() {
	iShiteiBushouIDOnViewingHimeDetailShimai = iShiteiBushouIDOnViewingHimeDetailShimai & 0xFFFF; // BX���W�X�^�����ɂ���B

	int iBushouID = iShiteiBushouIDOnViewingHimeDetailShimai-1;
	int iHimeBushouID = iHimeBushouIDOnViewingHimeDtail-1;

	// �o�̃A�h���X���f�t�H���g�ɓ���Ă���
	iMovAddressOnViewingHimeDetailShimai = int(iDefaultAddressOnViewingHimeDetailAne);

	if ( ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) && ( 0 <= iHimeBushouID && iHimeBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) ) {
		// �P���o�ł���΁c
		if ( getYearsOld(iHimeBushouID) > getYearsOld(iBushouID) ) { // �����̏����𖞂������̂��v�������񂾂�A�����ɂ��̏������o��

			// �o�̏ꍇ�̃f�t�H���g
			iMovAddressOnViewingHimeDetailShimai = int(iDefaultAddressOnViewingHimeDetailAne);

			if ( 0 ) {
				iMovAddressOnViewingHimeDetailShimai = int(szPushAdressOnViewingHimeDetailGiShi);
			}

		// ���ł���΁c
		} else {

			// ���̏ꍇ�̃f�t�H���g
			iMovAddressOnViewingHimeDetailShimai = int(iDefaultAddressOnViewingHimeDetailImouto);

			if ( 0 ) {
				iMovAddressOnViewingHimeDetailShimai = int(szPushAdressOnViewingHimeDetailGimai);
			}
		}
	}

}


/*
EDI��DI���e�̔ԍ�
004945C4  |. 3BD8           CMP     EBX, EAX
				�������� JMP TSMod.OnTSExeViewingHimeDetailShimai �Ə��������Ă����ۂ̏���
004945C6  |. B8 0C2F4D00    MOV     EAX, TENSHOU.004D2F0C                                     ;  ASCII "�o"
004945CB  |. 77 05          JA      SHORT TENSHOU.004945D2
004945CD  |. B8 082F4D00    MOV     EAX, TENSHOU.004D2F08                                     ;  ASCII "��"
004945D2  |> 50             PUSH    EAX
*/
int pTenshouExeJumpFromToOnTenshouExeViewingHimeDetailShimai	=0x4945C4; // �֐��͂��̃A�h���X����AOnTenshouExeViewingHimeDetailShimai�ւƃW�����v���Ă���B
int pTenshouExeReturnLblFromOnTenshouExeViewingHimeDetailShimai	=0x4945D2; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeViewingHimeDetailShimai() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {
		mov iShiteiBushouIDOnViewingHimeDetailShimai, EDI // �v�̕����ԍ��̊m��

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
	OnTenshouExeViewingHimeDetailShimaiExecute();

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

		mov eax, iMovAddressOnViewingHimeDetailShimai; // TENSHOU.EXE�Ɍ��X���݂�������

		jmp pTenshouExeReturnLblFromOnTenshouExeViewingHimeDetailShimai
	}
}

char cmdOnTenshouExeJumpFromViewingHimeDetailShimai[8]="\xE9\x90\x90\x90\x90\x90\x90";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeViewingHimeDetailShimai() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeViewingHimeDetailShimai;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeViewingHimeDetailShimai + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeViewingHimeDetailShimai  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromViewingHimeDetailShimai+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeViewingHimeDetailShimai), cmdOnTenshouExeJumpFromViewingHimeDetailShimai, 7, NULL); //7�o�C�g�̂ݏ�������
}

