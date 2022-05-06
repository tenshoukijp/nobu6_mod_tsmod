#define _CRT_SECURE_NO_WARNINGS

#include "WinTarget.h"
#include <stdlib.h>

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXBushou_All.h"
#include "ScenarioMod.h"



//--------------------------------�ȉ��A�R�n�S�C�������ǂ�������--------------------------------
/*
 * �����ڍׂ̋R�n�S�C���\������鎞
 */


extern int iViewingJobInBushouID; // �����ڍ׏��\���ŁA�\�����悤�Ƃ��Ă��镐���h�c�������Ă���

/*
 * �R�n�S�C������\������K�v�����邩�ǂ����̔���B
 */
void OnTenshouExeBushouDetailKibaTeppouJudgeExecute() {
}


/*
00493D98  |. E8 BA64F7FF    CALL    TENSHOU.0040A257
							�������� JMP TSMod.OnTSExeBushouDetailKibaTeppouJudge �Ə��������Ă����ۂ̏���
00446A81  |. 83C4 04        ADD     ESP, 4
00446A84  |. 85C0           TEST    EAX, EAX					������EAX��1�Ȃ�R�n�S�C�����Ƃ������ƁB
00493DA2  |. 74 24          JE      SHORT TENSHOU.00493DC8

*/
int pTenshouExeJumpFromToOnTenshouExeBushouDetailKibaTeppouJudge	=0x493D98; // �֐��͂��̃A�h���X����AOnTenshouExeBushouDetailKibaTeppouJudge�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeBushouDetailKibaTeppouJudge=0x40A257; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeBushouDetailKibaTeppouJudge	=0x493DA2; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeBushouDetailKibaTeppouJudge() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {
		// ���XTENSHOU.EXE���ɂ��������̂�������
		call pTenshouExeJumpCallFromToOnTenshouExeBushouDetailKibaTeppouJudge

		add esp, 4

		// �R�n�S�C�����͕K���\������K�v������Ƃ������Ƃɂ��Ă��܂��B
		mov eax, 1

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
	OnTenshouExeBushouDetailKibaTeppouJudgeExecute();

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

		// �R�n�S�C������\������K�v�����邩�ǂ����̔���
		test eax, eax 

		jmp pTenshouExeReturnLblFromOnTenshouExeBushouDetailKibaTeppouJudge
	}
}





char cmdOnTenshouExeJumpFromBushouDetailKibaTeppouJudge[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeBushouDetailKibaTeppouJudge() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeBushouDetailKibaTeppouJudge;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeBushouDetailKibaTeppouJudge + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeBushouDetailKibaTeppouJudge  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromBushouDetailKibaTeppouJudge+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeBushouDetailKibaTeppouJudge), cmdOnTenshouExeJumpFromBushouDetailKibaTeppouJudge, 5, NULL); //5�o�C�g�̂ݏ�������
}







//--------------------------------�ȉ��A���ۂɕ\������--------------------------------

char szSpecialButaiNon[11] = "";
char szSpecialButaiAnkokukishidan[11] = "��������"; // �����X���[���A��
char szSpecialButaiYumi[11] = "�|";
char szSpecialButaiYari[11] = "��";
char szSpecialButaiOno[11] = "��";
char szSpecialButaiMuchi[11] = "��";
char szSpecialButaiTuchi[11] = "��";
char szSpecialButaiTue[11] = "��";
char szSpecialButaiKenKitetsu[11] = "����R�S";
char szSpecialButaiYumiKitetsu[11] = "�|��R�S";
char szSpecialButaiYariKitetsu[11] = "����R�S";
char szSpecialButaiMuchiKitetsu[11] = "�ڥ�R�S";
char szSpecialButaiOnoKitetsu[11] = "����R�S";
char szSpecialButaiTueKitetsu[11] = "��R�S";
char szSpecialButaiYumiYari[11] = "�|���";
char szSpecialButaiYariOno[11] = "�����";
char szSpecialButaiTuchiOno[11] = "�ƥ��";
char szSpecialButaiTueYumi[11] = "��|";
char szSpecialButaiMuchiYumi[11] = "�ڥ�|";
char szSpecialButaiYumiYariKitetsu[11] = "�|�����R�S";
char szSpecialButaiYariOnoKitetsu[11] = "�������R�S";
char szSpecialButaiTuchiOnoKitetsu[11] = "�ƥ����R�S";
char szSpecialButaiTueYumiKitetsu[11] = "��|��R�S";
char szSpecialButaiMuchiYumiKitetsu[11] = "�ڥ�|��R�S";

int pAddressSpecialButai = NULL;

char szDisplayString[15] = "";

int iViewingSpecialButaiInXPosition = 0x98;

extern int isTenShougiModeDialog; // �V�����̃_�C�A���O�\�����ӂ̎������A���ꕺ�Ȃ̑���Ɂu�i�ʁv�ƂȂ�B
extern char *getSpecialDaniStringOfBushou(int iBushouID); // �w���

char* getSpecialHeitaiStringOfBushou(int iBushouID) {

	// �����̏��������I�ԏu�Ԃ����́u�i�ʁv�̕\���ւƐ؂�ւ���B
	if ( isTenShougiModeDialog ) {
		return getSpecialDaniStringOfBushou(iBushouID);
	}


	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
		int is_horsegun = nb6bushouref[iBushouID].horsegun;			// �R�n�S�C����
		int value_yumi = nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi;	// �|����
		int value_yari = nb6savedata_ex.bushou_ex[iBushouID].aptitudeyari;	// ������												
		int value_ono = nb6savedata_ex.bushou_ex[iBushouID].aptitudeono;	// ������												
		int value_muchi = nb6savedata_ex.bushou_ex[iBushouID].aptitudemuchi;	// �ڑ���												
		int value_tuchi = nb6savedata_ex.bushou_ex[iBushouID].aptitudetuchi;	// �Ƒ���												
		int value_tue = nb6savedata_ex.bushou_ex[iBushouID].aptitudetue;	// �񑮐�												
		int value_ken = nb6savedata_ex.bushou_ex[iBushouID].aptitudeken;	// ������												

		char *szTempName = szSpecialButaiNon;

		// �|�����R�n�S�C �S���g����
		if (  value_yumi >= 3  && value_yari >= 3&& is_horsegun ) {
			szTempName = szSpecialButaiYumiYariKitetsu;

		// ��|��R�n�S�C���g����
		} else if ( value_tue >= 3 && value_yumi >= 3 && is_horsegun ) {
			szTempName = szSpecialButaiTueYumiKitetsu;

		// �ڥ�|��R�n�S�C���g����
		} else if ( value_muchi >= 3 && value_yumi >= 3 && is_horsegun ) {
			szTempName = szSpecialButaiMuchiYumiKitetsu;

		// �������R�n�S�C���g����
		} else if ( value_yari >= 3 && value_ono >= 3 && is_horsegun ) {
			szTempName = szSpecialButaiYariOnoKitetsu;

		// �ƥ����R�n�S�C���g����
		} else if ( value_tuchi >= 3 && value_ono >= 3 && is_horsegun ) {
			szTempName = szSpecialButaiTuchiOnoKitetsu;

		// ����R�n�S�C���g����(������^���^���X��p)
		} else if ( value_ken >= 5 && is_horsegun ) {
			szTempName = szSpecialButaiKenKitetsu;

		// ����R�n�S�C���g����
		} else if ( value_yari >= 3 && is_horsegun ) {
			szTempName = szSpecialButaiYariKitetsu;

		// �ڥ�R�n�S�C���g����
		} else if ( value_muchi >= 3 && is_horsegun ) {
			szTempName = szSpecialButaiMuchiKitetsu;

		// �|��R�n�S�C���g����
		} else if ( value_yumi >= 3 && is_horsegun ) {
			szTempName = szSpecialButaiYumiKitetsu;

		// �ڥ�R�n�S�C���g����
		} else if ( value_muchi >= 3 && is_horsegun ) {
			szTempName = szSpecialButaiMuchiKitetsu;

		// ����R�n�S�C���g����
		} else if ( value_ono >= 3 && is_horsegun ) {
			szTempName = szSpecialButaiOnoKitetsu;

		// ��R�n�S�C���g����
		} else if ( value_tue >= 3 && is_horsegun ) {
			szTempName = szSpecialButaiTueKitetsu;

		// �|������g����
		} else if ( value_yari >= 3 && value_yumi >= 3 ) {
			szTempName = szSpecialButaiYumiYari;

		// �ƥ�����g����
		} else if ( value_tuchi >= 3 && value_ono >= 3 ) {
			szTempName = szSpecialButaiTuchiOno;

		// ��������g����
		} else if ( value_yari >= 3 && value_ono >= 3 ) {
			szTempName = szSpecialButaiYariOno;

		// ��|���g����
		} else if ( value_tue >= 3 && value_yumi >= 3 ) {
			szTempName = szSpecialButaiTueYumi;

		// �ڥ�|���g����
		} else if ( value_muchi >= 3 && value_yumi >= 3 ) {
			szTempName = szSpecialButaiMuchiYumi;

		// �|�̂�
		} else if ( value_yumi >= 3 ) {
			szTempName = szSpecialButaiYumi;

		// ���̂�
		} else if ( value_yari >= 3 ) {
			szTempName = szSpecialButaiYari;

		// ���̂�
		} else if ( value_ono >= 3 ) {
			szTempName = szSpecialButaiOno;

		// �ڂ̂�
		} else if ( value_muchi >= 3 ) {
			szTempName = szSpecialButaiMuchi;

		// ��̂�
		} else if ( value_tue >= 3 ) {
			szTempName = szSpecialButaiTue;

		// �R�n�S�C�̂ݎg����
		} else if ( is_horsegun ) {
			szTempName = (char *)0x4CFF0C; // �R�n�S�C���̕�����ւ̃|�C���^

		// �����g���Ȃ�
		} else {
			szTempName = szSpecialButaiNon;
		}

		// ScenarioMod���L���ł���΁A
		// ScenarioMod�ŃJ�X�^���ȓ��ꕺ�Ԗ���ݒ肵�Ă���ꍇ�A�����ݒ肷��B
		char szTempSpecialName[20] = "";
		if ( p_snOnViewingSpecialFormName ) {
			// ���S�����đ傫�ڂ�
			bool isReWrite = ((PFNSNONVIEWINGSPECIALFORMNAME)p_snOnViewingSpecialFormName)( szTempSpecialName, iBushouID+1 );
			if ( isReWrite ) {
				szTempName = szTempSpecialName;
			}
		}
		
		return szTempName;
	}

	return NULL;
}


void OnTenshouExeBushouDetailKibaTeppouExecute() {

	iViewingSpecialButaiInXPosition = 0x98; // �f�t�H���g�l�ŕK��������

	// �R�n�S�C�̏��
	int iBushouID = iViewingJobInBushouID-1;

	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		char *szTempName = getSpecialHeitaiStringOfBushou(iBushouID);

		// �Y���̓��ꕔ�������Q�������Z���ꍇ�A������𒷂����Ă��K�v������B
		if ( strlen(szTempName) < 4 ) {
			string s = string("  ")+szTempName+"  ";
			strcpy( szDisplayString, s.c_str() );
		} else {
			strcpy( szDisplayString, szTempName); // �S�p�łS�������l�`�w
		}
		iViewingSpecialButaiInXPosition = 0xA8 - (strlen(szDisplayString)-4)*4;

		pAddressSpecialButai = (int)szDisplayString;

	// �������Ȃ����͈͂ɓ����Ă��Ȃ��ꍇ��(���肦�Ȃ���)�A�Ƃ肠�����A�󔒏�Ԃ�n���Ă���
	} else {
		// 
		pAddressSpecialButai = (int)szSpecialButaiNon;
	}
}


/*
00493DAE     68 98000000    PUSH    98
				�������� JMP TSMod.OnTSExeBushouDetailKibaTeppou �Ə��������Ă����ۂ̏���
00493DB3  |. 8BCF           MOV     ECX, EDI
00493DB5  |. E8 CED6F6FF    CALL    TENSHOU.00401488
00493DBA  |. 68 0CFF4C00    PUSH    TENSHOU.004CFF0C                 ;  ASCII "�R�n�S�C"   �� �����͋R�n�S�C�̃A�h���X�Btenshou.exe���Ō��ߑł����ߍ��݂���Ă���B
00493DBF  |. 57             PUSH    EDI
*/
int pTenshouExeJumpFromToOnTenshouExeBushouDetailKibaTeppou		=0x493DAE; // �֐��͂��̃A�h���X����AOnTenshouExeBushouDetailKibaTeppou�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeBushouDetailKibaTeppou =0x401488; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeBushouDetailKibaTeppou	=0x493DBF; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeBushouDetailKibaTeppou() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {

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
	OnTenshouExeBushouDetailKibaTeppouExecute();

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

		push iViewingSpecialButaiInXPosition

		// ���XTENSHOU.EXE���ɂ��������̂�������
		mov ecx, edi

		call pTenshouExeJumpCallFromToOnTenshouExeBushouDetailKibaTeppou

		// �R�n�S�C���̗��ɕ\�����ׂ�������ւ̃|�C���^��
		push pAddressSpecialButai 

		jmp pTenshouExeReturnLblFromOnTenshouExeBushouDetailKibaTeppou
	}
}

/*
00446A7B  |. 57             PUSH    EDI
00446A7C  |. E8 3159FCFF    CALL    TENSHOU.0040C3B2
				�������� JMP TSMod.OnTSExeBushouDetailKibaTeppou �Ə��������Ă����ۂ̏���
00446A81  |. 83C4 04        ADD     ESP, 4
00446A84  |. 85C0           TEST    EAX, EAX
00446A86  |. 74 45          JE      SHORT TENSHOU.00446ACD
*/
char cmdOnTenshouExeJumpFromBushouDetailKibaTeppou[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeBushouDetailKibaTeppou() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeBushouDetailKibaTeppou;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeBushouDetailKibaTeppou + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeBushouDetailKibaTeppou  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromBushouDetailKibaTeppou+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeBushouDetailKibaTeppou), cmdOnTenshouExeJumpFromBushouDetailKibaTeppou, 5, NULL); //5�o�C�g�̂ݏ�������
}




