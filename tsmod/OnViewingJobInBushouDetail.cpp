#pragma pack(1)
#define _CRT_SECURE_NO_WARNINGS

#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"
#include "ScenarioMod.h"
#include "VariableNameRegistAndJudge.h"
#include "EXBushou_All.h"




/*
			���ҋ@			�����S     		���E��		�����l
004D13E8  [AC FE 4C] 00 �sA4 FE 4C�t 00 [9C FE 4C] 00 [AC B8 4C] 00  
			������			���m��		�ꊅ
004D13F8  [94 FE 4C] 00 [8C FE 4C] 00 [84 B4 4B] 00 64 B4 4B 00  
004D1408  DC BB 4B 00 5C B4 4B 00 D4 BB 4B                 

*/

/*
00493D5D  |> 56             PUSH    ESI
00493D5E  |. E8 7F86F7FF    CALL    TENSHOU.0040C3E2					�������̊֐��̒��ŁAJob�ԍ�(0x10=�E�ҁ`0x40=�m��)��1,2,3,4�ɕϊ����Ă���B
00493D63  |. 83C4 04        ADD     ESP, 4
00493D66  |. 8BF8           MOV     EDI, EAX
00493D68  |. 85FF           TEST    EDI, EDI
00493D6A  |. 74 2B          JE      SHORT TENSHOU.00493D97
00493D6C  |. 68 A0000000    PUSH    0A0
00493D71  |. BB F8674E00    MOV     EBX, TENSHOU.004E67F8            ;  ASCII "�VK"
00493D76  |. 68 A8000000    PUSH    0A8
00493D7B  |. 8BCB           MOV     ECX, EBX
00493D7D  |. E8 06D7F6FF    CALL    TENSHOU.00401488
00493D82  |. FF34BD EC134D0>PUSH    DWORD PTR DS:[EDI*4+4D13EC]�@�@�@�@�@�� ������4D13EC���A�u�E�ҁv�u���l�v�u�����v�u�m���v �E��=1 ���l=2 ����=3 �m��=4
00493D89  |. 68 582E4D00    PUSH    TENSHOU.004D2E58                 ;  ASCII "%4s"
00493D8E  |. 53             PUSH    EBX
00493D8F  |. E8 1CA80100    CALL    TENSHOU.004AE5B0
*/

#define GAMEDATASTRUCT_JOB_EX_COUNT 15

// �E�ƕ�����̃��X�g
char szJobEXList[GAMEDATASTRUCT_JOB_EX_COUNT][9] = {
	"�@",
	"�E��",
	"���l",
	"����",
	"�m��",
	// ----����ȍ~��TSMOD�K�{----
	"�C��",
	"����",
	"���l",
	"����",
	" �؎x�O ",
	// ----����ȍ~��.jp�̒��̐l�p----
	"  �R�m  ",
	" ���p�m ",
	"  ����  ",
	"�|",
	"�|",
};

// �E�ƕ�����ւ̃|�C���^�̃��X�g
char *pJobExList[GAMEDATASTRUCT_JOB_EX_COUNT] = {
	szJobEXList[0],
	(char *)0x4CFE9C, // �E�� ��TENSHOU.EXE���A�h���X  szJobEXList[1],
	(char *)0x4CB8AC, // ���l ��TENSHOU.EXE���A�h���X  szJobEXList[2],
	(char *)0x4CFE94, // ���� ��TENSHOU.EXE���A�h���X  szJobEXList[3],
	(char *)0x4CFE8C, // �m�� ��TENSHOU.EXE���A�h���X  szJobEXList[4],
	szJobEXList[5],
	szJobEXList[6],
	szJobEXList[7],
	szJobEXList[8],
	szJobEXList[9],
	szJobEXList[10],
	szJobEXList[11],
	szJobEXList[12],
	szJobEXList[13],
	szJobEXList[14],
};

char szJobNameAnkokuKishiDancho[11]  = "����������"; // �� �Í��R�m�c��
char szJobNameAnkokuKishi[9]  = "�Í��R�m";
char szJobNameYamiSouryo[9] = " ���m�� ";
char szJobNameKemonoTsukai[9] = " �b�g�� ";
char szJobNameShirokishi[9] = " ���R�m ";
char szJobNameSenseiJutsushi[9] = "�萯�p�t";
char szJobNameYuuyokujin[9] = " �L���l ";
char szJobNameMajo[9] = "  ����  ";
char szJobNameKensei[9] = "  ����  ";
char szJobNameMadoushi[9] = " �d�p�m ";
char szJobNameIkusaMico[9] = "  �ޏ�  ";

char szJobNameCustomJobName[32] = ""; // �J�X�^���i�n�a


// �ΏۃW���u�̑Ώۂ̕���ID(�z���ł͂Ȃ��A�����̒l�ł���(�z����+1������))
int iViewingJobInBushouID = 0;
int iViewingJobInJobID = 0;
int iViewingJobInXPosition = 0xA8;


void OnTenshouExeResetAllJobName() {
	pJobExList[0] =szJobEXList[0];

	pJobExList[1]=(char *)0x4CFE9C; // �E�҂����ɖ߂�
	pJobExList[2]=(char *)0x4CB8AC; // ���l�����ɖ߂�
	pJobExList[3]=(char *)0x4CFE94; // ���������ɖ߂�
	pJobExList[4]=(char *)0x4CFE8C; // �m�������ɖ߂�

	pJobExList[5] =szJobEXList[5];
	pJobExList[6] =szJobEXList[6];
	pJobExList[7] =szJobEXList[7];
	pJobExList[8] =szJobEXList[8];
	pJobExList[9] =szJobEXList[9];
	pJobExList[10]=szJobEXList[10];
	pJobExList[11]=szJobEXList[11];
	pJobExList[12]=szJobEXList[12];
	pJobExList[13]=szJobEXList[13];
	pJobExList[14]=szJobEXList[14];
}

// ���ƁE���l �ȂǂƂ������u�������悤�̕�����1300�l���̃��X�g
extern char *pOriginalBushouJobExList[GAMEDATASTRUCT_REGISTED_BUSHOU_NUM];


void OnTenshouExeViewingJobInBushouDetailExecute() {

	// ���ʂS�o�C�g������ID�Ȃ̂ŁA�}�X�N
	iViewingJobInBushouID = iViewingJobInBushouID & 0xFFFF;

	// �͈͊O�Ȃ�_��
	if ( !(0 <= iViewingJobInBushouID-1 && iViewingJobInBushouID-1 < GAMEDATASTRUCT_BUSHOU_NUM )) {
		return;
	}

	// �^���^���X�ł���΁c�����ł͂Ȃ��Í��R�m�Ƃ���
	if (isLancelotTartare(iViewingJobInBushouID - 1)) {

		// �w�����x���w���|�C���^���A�w�Í��R�m�x���w���|�C���^�ɒ����B�w�����x�ł���Ȃ���w�Í��R�m�x�ƂȂ�̂��A�B
		pJobExList[3] = (char *)szJobNameAnkokuKishi;

		// �E�Ƃ͎���2�������O��ƂȂ��Ă���\���ʒu�ŌŒ肳��Ă���B�����ł�4�����z�u����̂ŁA�ʒu��ύX����B
		// �Í��R�m�ƕ\�����邽�߂ɁAX���W�𒲐�
		//iViewingJobInXPosition = 0x90; // �T�����O��̔z�u�ɂ��邽�߁A�Q�����ɑ΂��Ă�⍶�ɔz�u�B���ʂ͂T�����Ȃ�0x90�����A�O���ŃM�b�`�M�`�Ȃ̂ł���ɍ���-1
		iViewingJobInXPosition = 0x98; // �S�����O��̔z�u�ɂ��邽�߁A�Q�����ɑ΂��Ă�⍶�ɔz�u

	// ��ɂł���΁c�ޏ��Ƃ���
	} else if (isShuiHidechiyo(iViewingJobInBushouID - 1)) {

			// �w�����x���w���|�C���^���A�w�Í��R�m�x���w���|�C���^�ɒ����B�w�����x�ł���Ȃ���w�Í��R�m�x�ƂȂ�̂��A�B
			pJobExList[3] = (char *)szJobNameIkusaMico;

			// �E�Ƃ͎���2�������O��ƂȂ��Ă���\���ʒu�ŌŒ肳��Ă���B�����ł�4�����z�u����̂ŁA�ʒu��ύX����B
			// �Í��R�m�ƕ\�����邽�߂ɁAX���W�𒲐�
			//iViewingJobInXPosition = 0x90; // �T�����O��̔z�u�ɂ��邽�߁A�Q�����ɑ΂��Ă�⍶�ɔz�u�B���ʂ͂T�����Ȃ�0x90�����A�O���ŃM�b�`�M�`�Ȃ̂ł���ɍ���-1
			iViewingJobInXPosition = 0x98; // �S�����O��̔z�u�ɂ��邽�߁A�Q�����ɑ΂��Ă�⍶�ɔz�u

	// �Í��R�m�ł���΁c�R�m�ł͂Ȃ��Í��R�m�Ƃ���
	} else if ( IsLoslorien(iViewingJobInBushouID-1) ) {

		// �n�{�������R�m�Ȃ�A�����Ƃ���B
		if (  strcmp(nb6bushouname[iViewingJobInBushouID-1].familyname, "�I�J�K")==0 && strcmp(nb6bushouname[iViewingJobInBushouID-1].fastname, "�d�e�f")==0 && nb6bushouref[iViewingJobInBushouID-1].job == 0xA0) {
			pJobExList[10]=(char *)szJobNameKensei;

		// �J�`���A���L���V�^���Ȃ�΁c�؎x�O�ł͂Ȃ��őm���Ƃ���
		} else if ( strcmp(nb6bushouname[iViewingJobInBushouID-1].familyname, "�^�_�`")==0 && strcmp(nb6bushouname[iViewingJobInBushouID-1].fastname, "�a�b�c")==0 && nb6bushouref[iViewingJobInBushouID-1].job == 0x90) {
			// �w�؎x�O�x���w���|�C���^���A�w�őm���x���w���|�C���^�ɒ����B�w�؎x�O�x�ł���Ȃ���w�őm���x�ƂȂ�̂��A�B
			pJobExList[9]=(char *)szJobNameYamiSouryo;

			// �n�{�����ȊO�͈Í��R�m
		} else if ( nb6bushouref[iViewingJobInBushouID-1].job == 0xA0 ) {
			// �w�R�m�x���w���|�C���^���A�w�Í��R�m�x���w���|�C���^�ɒ����B�w�R�m�x�ł���Ȃ���w�Í��R�m�x�ƂȂ�̂��A�B
			pJobExList[10]=(char *)szJobNameAnkokuKishi;
		}

		iViewingJobInXPosition = 0x98; // �S�����O��̔z�u�ɂ��邽�߁A�Q�����ɑ΂��Ă�⍶�ɔz�u


	// �E�H�[�����Ȃ�΁c���p�t�ł͂Ȃ��萯�p�t�Ƃ���
	} else if ( strcmp(nb6bushouname[iViewingJobInBushouID-1].familyname, "�q�r")==0 && strcmp(nb6bushouname[iViewingJobInBushouID-1].fastname, "�n�o�p")==0 ) {

		// �w���p�t�x���w���|�C���^���A�w�萯�p�t�x���w���|�C���^�ɒ����B�w���p�t�x�ł���Ȃ���w�萯�p�t�x�ƂȂ�̂��A�B
		pJobExList[11]=(char *)szJobNameSenseiJutsushi;

		// �E�Ƃ͎���2�������O��ƂȂ��Ă���\���ʒu�ŌŒ肳��Ă���B�����ł�4�����z�u����̂ŁA�ʒu��ύX����B
		// " �b�g�� "�ƕ\�����邽�߂ɁAX���W�𒲐�
		iViewingJobInXPosition = 0x98; // �S�����O��̔z�u�ɂ��邽�߁A�Q�����ɑ΂��Ă�⍶�ɔz�u

	// �J�m�[�v�X�Ȃ�΁c���l�ł͂Ȃ��L���l�Ƃ���
	} else if ( strcmp(nb6bushouname[iViewingJobInBushouID-1].familyname, "�O�u")==0 && strcmp(nb6bushouname[iViewingJobInBushouID-1].fastname, "�[�\�]")==0 ) {

		// �w�����x���w���|�C���^���A�w�L���l�x���w���|�C���^�ɒ����B
		pJobExList[12]=(char *)szJobNameYuuyokujin;

		// �E�Ƃ͎���2�������O��ƂȂ��Ă���\���ʒu�ŌŒ肳��Ă���B�����ł�4�����z�u����̂ŁA�ʒu��ύX����B
		// " �b�g�� "�ƕ\�����邽�߂ɁAX���W�𒲐�
		iViewingJobInXPosition = 0x98; // �S�����O��̔z�u�ɂ��邽�߁A�Q�����ɑ΂��Ă�⍶�ɔz�u

	// �M���o���h�ł���΁c�R�m�ł͂Ȃ��b�g���Ƃ���
	} else if ( strcmp(nb6bushouname[iViewingJobInBushouID-1].familyname, "�g�h�i")==0 && strcmp(nb6bushouname[iViewingJobInBushouID-1].fastname, "���")==0 ) {

		// �w�R�m�x���w���|�C���^���A�w�b�g���x���w���|�C���^�ɒ����B�w�R�m�x�ł���Ȃ���w�b�g���x�ƂȂ�̂��A�B
		pJobExList[10]=(char *)szJobNameKemonoTsukai;

		// �E�Ƃ͎���2�������O��ƂȂ��Ă���\���ʒu�ŌŒ肳��Ă���B�����ł�4�����z�u����̂ŁA�ʒu��ύX����B
		// " �b�g�� "�ƕ\�����邽�߂ɁAX���W�𒲐�
		iViewingJobInXPosition = 0x98; // �S�����O��̔z�u�ɂ��邽�߁A�Q�����ɑ΂��Ă�⍶�ɔz�u

	// �~���f�B�����A�M���_�X�Ȃ�΁c�R�m�ł͂Ȃ����R�m�Ƃ���
	} else if ( (strcmp(nb6bushouname[iViewingJobInBushouID-1].familyname, "�O�P�Q")==0 && strcmp(nb6bushouname[iViewingJobInBushouID-1].fastname, "�R�S�T")==0) || 
				(strcmp(nb6bushouname[iViewingJobInBushouID-1].familyname, "�G�H�F")==0 && strcmp(nb6bushouname[iViewingJobInBushouID-1].fastname, "������")==0)
		) {

		// �w�R�m�x���w���|�C���^���A�w���R�m�x���w���|�C���^�ɒ����B�w�R�m�x�ł���Ȃ���w���R�m�x�ƂȂ�̂��A�B
		pJobExList[10]=(char *)szJobNameShirokishi;

		// �E�Ƃ͎���2�������O��ƂȂ��Ă���\���ʒu�ŌŒ肳��Ă���B�����ł�4�����z�u����̂ŁA�ʒu��ύX����B
		// " ���R�m "�ƕ\�����邽�߂ɁAX���W�𒲐�
		iViewingJobInXPosition = 0x98; // �S�����O��̔z�u�ɂ��邽�߁A�Q�����ɑ΂��Ă�⍶�ɔz�u

	// �f�l�u�Ȃ�΁c���p�t�ł͂Ȃ������Ƃ���
	} else if ( strcmp(nb6bushouname[iViewingJobInBushouID-1].familyname, "�j�k")==0 && strcmp(nb6bushouname[iViewingJobInBushouID-1].fastname, "�l�m")==0 ) {

		// �w���p�t�x���w���|�C���^���A�w�����x���w���|�C���^�ɒ����B
		pJobExList[11]=(char *)szJobNameMajo;

		// �E�Ƃ͎���2�������O��ƂȂ��Ă���\���ʒu�ŌŒ肳��Ă���B�����ł�4�����z�u����̂ŁA�ʒu��ύX����B
		// "  ����  "�ƕ\�����邽�߂ɁAX���W�𒲐�
		iViewingJobInXPosition = 0x98; // �S�����O��̔z�u�ɂ��邽�߁A�Q�����ɑ΂��Ă�⍶�ɔz�u


	// ���h�����Ȃ�΁c���p�t�ł͂Ȃ������m�Ƃ���
	} else if ( strcmp(nb6bushouname[iViewingJobInBushouID-1].familyname, "�y�z�{")==0 && strcmp(nb6bushouname[iViewingJobInBushouID-1].fastname, "�|�}�~")==0 ) {

		// �w���p�t�x���w���|�C���^���A�w�����m�x���w���|�C���^�ɒ����B
		pJobExList[11]=(char *)szJobNameMadoushi;

		// �E�Ƃ͎���2�������O��ƂȂ��Ă���\���ʒu�ŌŒ肳��Ă���B�����ł�4�����z�u����̂ŁA�ʒu��ύX����B
		// " �����m "�ƕ\�����邽�߂ɁAX���W�𒲐�
		iViewingJobInXPosition = 0x98; // �S�����O��̔z�u�ɂ��邽�߁A�Q�����ɑ΂��Ă�⍶�ɔz�u


	// ����ȊO�́A���Ƃ��Ƃ̐E�Ɩ��ɖ߂�
	} else {
		// �S�Ă̐E�Ƃ̖��O�����ւƖ߂��B
		OnTenshouExeResetAllJobName();

		 // JobID��0xA0(=iViewingJobInJobID=9=�؎x�O)�𒴂��Ă���Ȃ�΁AX���W�𒲐�
		if ( iViewingJobInJobID >= 9 ) {
			 iViewingJobInXPosition = 0x98; // �S�����O��̔z�u�ɂ��邽�߁A�Q�����ɑ΂��Ă�⍶�ɔz�u
			// ����ȊO�́A�f�t�H���g
		} else {
			 iViewingJobInXPosition = 0xA8;	// �f�t�H���g��2�����O��̔z�u�ɖ߂�
		}

		// �V�ċL�Ɍ��X���镐���ł���Ȃ�΁c
		if ( IsOriginalRegistedBushou( iViewingJobInBushouID-1 ) ) {

			if ( 0 <= iViewingJobInBushouID-1 && iViewingJobInBushouID-1 < GAMEDATASTRUCT_BUSHOU_NUM ) {

				int iFaceID = nb6bushouname[iViewingJobInBushouID-1].number;
				// ���S�����đ傫�ڂ�
				char szTempName[20] = "";
				strcpy( szTempName, pOriginalBushouJobExList[iFaceID]);

				// �L���Ȓu�������������͂����Ă���Ȃ�΁c
				if ( strlen( szTempName ) > 1 ) {

					// �Y��������JOB�̂Ƃ��������������B
					int iJobID = (nb6bushouref[iViewingJobInBushouID-1].job / 0x10); // JOB�̒l��16(=0x10)�Ŋ���Ɣz��̂h�c�ƂȂ�B

					// �Y���̐E�Ɩ����Q�������Z���ꍇ�A������𒷂����Ă��K�v������B
					if ( strlen(szTempName) < 4 ) {
						string s = string("  ")+szTempName+"  ";
						strcpy( szJobNameCustomJobName, s.c_str() );
					} else {
						strcpy( szJobNameCustomJobName, szTempName); // �S�p�łS�������l�`�w
					}
					pJobExList[iJobID] = szJobNameCustomJobName;
					iViewingJobInXPosition = 0xA8 - (strlen(szJobNameCustomJobName)-4)*4;
				}
			}
		}

		// ScenarioMod���L���ł���΁A
		// ScenarioMod�ŃJ�X�^���i�n�a��ݒ肵�Ă���ꍇ�A�����ݒ肷��B
		if ( p_snOnViewingJobName ) {
			if ( 0 <= iViewingJobInBushouID-1 && iViewingJobInBushouID-1 < GAMEDATASTRUCT_BUSHOU_NUM ) {
				// ���S�����đ傫�ڂ�
				char szTempName[20] = "";
				bool isReWrite = ((PFNSNONVIEWINGJOBNAME)p_snOnViewingJobName)( szTempName, iViewingJobInBushouID );
				if ( isReWrite ) {
					// �Y��������JOB�̂Ƃ��������������B
					int iJobID = (nb6bushouref[iViewingJobInBushouID-1].job / 0x10); // JOB�̒l��16(=0x10)�Ŋ���Ɣz��̂h�c�ƂȂ�B

					// �Y���̐E�Ɩ����Q�������Z���ꍇ�A������𒷂����Ă��K�v������B
					if ( strlen(szTempName) < 4 ) {
						string s = string("  ")+szTempName+"  ";
						strcpy( szJobNameCustomJobName, s.c_str() );
					} else {
						strcpy( szJobNameCustomJobName, szTempName); // �S�p�łS�������l�`�w
					}
					pJobExList[iJobID] = szJobNameCustomJobName;
					iViewingJobInXPosition = 0xA8 - (strlen(szJobNameCustomJobName)-4)*4;
				}
			}
		}
	}
}


/*
00493D71  |. BB F8674E00    MOV     EBX, TENSHOU.004E67F8
00493D76  |. 68 A8000000    PUSH    0A8
				�������� JMP TSMod.OnTSExeViewingJobInBushouDetail �Ə��������Ă����ۂ̏���
00493D7B  |. 8BCB           MOV     ECX, EBX
00493D7D  |. E8 06D7F6FF    CALL    TENSHOU.00401488
00493D82  |. FF34BD EC134D0>PUSH    DWORD PTR DS:[EDI*4+4D13EC]
00493D89  |. 68 582E4D00    PUSH    TENSHOU.004D2E58                 ;  ASCII "%4s"
00493D8E  |. 53             PUSH    EBX
00493D8F  |. E8 1CA80100    CALL    TENSHOU.004AE5B0
00493D94  |. 83C4 0C        ADD     ESP, 0C
00493D89  |. 68 582E4D00    PUSH    TENSHOU.004D2E58                 ;  ASCII "%4s"
*/
int pTenshouExeJumpFromToOnTenshouExeViewingJobInBushouDetail		=0x493D76; // �֐��͂��̃A�h���X����AOnTenshouExeViewingJobInBushouDetail�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeViewingJobInBushouDetail   =0x401488; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeViewingJobInBushouDetail	=0x493D89; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeViewingJobInBushouDetail() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {
		mov iViewingJobInBushouID, esi	// ����ID���R�s�[�������Aesi�̉���4�o�C�g������ID�Ȃ̂ŁA�}�X�N����K�v����B
		mov iViewingJobInJobID, eax		// �E��ID���R�s�[

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}
	
	// ���̊֐����ň��S�Ƀf�[�^�����H����B
	OnTenshouExeViewingJobInBushouDetailExecute();

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
		
		// ���XTENSHOU.EXE���ɂ����������W�𒲐���������
		push iViewingJobInXPosition

		// ���XTENSHOU.EXE���ɂ��������̂�������
		mov ecx, ebx

		call pTenshouExeJumpCallFromToOnTenshouExeViewingJobInBushouDetail

		push DWORD PTR DS:[edi*4+pJobExList]

		jmp pTenshouExeReturnLblFromOnTenshouExeViewingJobInBushouDetail
	}
}

/*
00493D71  |. BB F8674E00    MOV     EBX, TENSHOU.004E67F8
00493D76  |. 68 A8000000    PUSH    0A8
				�������� JMP TSMod.OnTSExeViewingJobInBushouDetail �Ə��������Ă����ۂ̏���
00493D7B  |. 8BCB           MOV     ECX, EBX
00493D7D  |. E8 06D7F6FF    CALL    TENSHOU.00401488
00493D82  |. FF34BD EC134D0>PUSH    DWORD PTR DS:[EDI*4+4D13EC]
00493D89  |. 68 582E4D00    PUSH    TENSHOU.004D2E58                 ;  ASCII "%4s"
00493D8E  |. 53             PUSH    EBX
00493D8F  |. E8 1CA80100    CALL    TENSHOU.004AE5B0
00493D94  |. 83C4 0C        ADD     ESP, 0C
00493D89  |. 68 582E4D00    PUSH    TENSHOU.004D2E58                 ;  ASCII "%4s"
*/
char cmdOnTenshouExeJumpFromJobInBushouDetail[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeViewingJobInBushouDetail() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeViewingJobInBushouDetail;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeViewingJobInBushouDetail + 5 );
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeViewingJobInBushouDetail  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromJobInBushouDetail+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeViewingJobInBushouDetail), cmdOnTenshouExeJumpFromJobInBushouDetail, 5, NULL); // 6�o�C�g�̂ݏ������� ���̖��߂�6�o�C�g�A�V�K�̖��߂��T�o�C�g�Ȃ̂ŁANOP���t��������Ă���B
}

/*
 * �ȉ��A1300�l���̕����̏o�����̃��X�g�B
 */
char *pOriginalBushouJobExList[GAMEDATASTRUCT_REGISTED_BUSHOU_NUM] = {
"����",
"",
"�����l",
"",
"",
"��������l",
"��������l",
"���l����l",
"",
"�����l",
"��쥒��l",
"�d�b",
"",
"���",
"���",
"",
"",
"",
"",
"���",
"���",
"",
"",
"���l",
"���l",
"���l",
"�m�����",
"",
"",
"�m������",
"���ƥ�|��",
"",
"",
"���������",
"�ؒO����",
"����",
"���l����l",
"",
"",
"",
"���l��ؒO",
"�d�b",
"����",
"",
"",
"�ؒO����",
"������",
"���",
"���",
"���������",
"",
"���������",
"���������",
"",
"",
"",
"",
"",
"",
"���",
"",
"���l",
"���l",
"",
"",
"���l��ؒO",
"�������l",
"",
"",
"",
"",
"",
"",
"",
"���l��̐l",
"",
"���",
"",
"",
"",
"���",
"",
"������",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"�B��",
"",
"",
"���l����l",
"",
"",
"���l��B��",
"",
"���l��ؒO",
"",
"",
"",
"�d�b",
"����@",
"",
"���",
"���",
"",
"",
"���",
"�d�b",
"�d�b",
"",
"",
"",
"",
"",
"",
"",
"���l",
"",
"",
"�������",
"���l��ؒO",
"",
"",
"",
"�m������l",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"���l",
"",
"�ؒO����",
"���",
"���",
"",
"�ؒO��B��",
"���l��m��",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"����",
"������",
"",
"",
"����",
"����",
"����",
"����",
"",
"",
"",
"",
"",
"",
"",
"",
"���l",
"���l",
"",
"",
"",
"",
"",
"",
"",
"",
"���",
"",
"",
"",
"",
"",
"",
"",
"",
"����@",
"",
"",
"�d�b",
"���",
"���l",
"���l",
"���l",
"���l",
"",
"",
"",
"�d�b",
"���l��B��",
"",
"",
"�d�b",
"���",
"���������",
"",
"",
"",
"",
"������",
"",
"",
"�d�b",
"�d�b",
"",
"",
"���",
"",
"",
"���l",
"",
"",
"���l",
"���l",
"���ƥ�|��",
"",
"",
"",
"",
"",
"",
"",
"���",
"������",
"���l",
"������",
"",
"����",
"���l",
"���",
"���l",
"",
"����",
"�m������",
"�m������",
"�m������",
"",
"",
"�d�b",
"",
"",
"",
"���l",
"����",
"",
"���l",
"���",
"�d�b",
"����",
"",
"����",
"����",
"",
"��������l",
"",
"�������l",
"",
"���l",
"",
"",
"",
"",
"",
"",
"����",
"����",
"",
"",
"�d�b",
"",
"",
"",
"",
"",
"���l�����",
"����",
"",
"",
"���l",
"",
"",
"���l",
"���ƥ�ؒO",
"���",
"",
"",
"",
"",
"",
"",
"",
"",
"�d�b",
"������",
"",
"",
"��쥋���",
"���",
"���l",
"������",
"�d�b",
"�d�b",
"",
"",
"���l",
"��������l",
"",
"",
"",
"��쥋���",
"���l",
"�m������",
"����",
"����",
"",
"",
"",
"���",
"",
"���",
"���ƥ���",
"����",
"",
"���",
"���",
"",
"",
"����",
"",
"",
"���",
"",
"",
"���l�����",
"",
"�����l",
"",
"���l",
"�B�������",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"���l",
"�d�b",
"�d�b",
"",
"�d�b",
"",
"",
"",
"",
"",
"",
"",
"���l",
"���",
"",
"",
"",
"",
"���l",
"",
"",
"���",
"���l",
"",
"���",
"",
"���",
"",
"",
"�d�b",
"",
"",
"",
"",
"",
"���l",
"",
"",
"",
"",
"",
"���",
"���l",
"���l",
"���ƥ���l",
"",
"",
"",
"",
"",
"",
"",
"",
"���l",
"���l",
"",
"",
"���l",
"",
"",
"���l",
"���l",
"���l",
"",
"",
"���l",
"������",
"������",
"������",
"",
"���l",
"�d�b",
"",
"",
"�d�b",
"",
"���",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"���",
"���",
"���",
"",
"",
"�d�b",
"���l",
"",
"",
"",
"",
"",
"",
"",
"",
"���l",
"���l",
"",
"",
"�d�b",
"",
"",
"���",
"���",
"",
"",
"",
"",
"",
"",
"",
"���������",
"���l",
"",
"���l",
"",
"���l",
"���l",
"������",
"������",
"",
"",
"������",
"���l",
"���l",
"���l",
"�d�b",
"���l",
"���l",
"",
"",
"",
"",
"�d�b",
"���l",
"",
"",
"���l�����",
"���l",
"���l",
"",
"",
"",
"�d�b",
"",
"",
"",
"",
"�d�b",
"���l",
"���l",
"���l",
"",
"",
"",
"",
"",
"",
"",
"�C������l",
"",
"",
"",
"",
"",
"���l",
"",
"",
"",
"���l",
"",
"�d�b",
"",
"",
"",
"",
"������",
"����@",
"",
"",
"",
"",
"",
"",
"",
"���l",
"��쥐ؒO",
"",
"����",
"����",
"����",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"����",
"���l",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"���l",
"",
"",
"",
"",
"",
"���l",
"���l",
"���l",
"",
"",
"�m������",
"���l",
"���l",
"���l",
"���l",
"���l",
"���l",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"���",
"",
"",
"",
"",
"���l",
"���l",
"������",
"",
"���l",
"",
"���l",
"���l����",
"",
"",
"���",
"���l",
"���l",
"������",
"���l",
"���l",
"",
"",
"",
"������",
"",
"",
"����",
"",
"",
"",
"",
"",
"���l",
"",
"",
"���l",
"���l",
"",
"���l",
"���l",
"���l",
"���l",
"����",
"����@",
"",
"",
"���",
"�d�b",
"���",
"������",
"�d�b",
"�m������",
"�������l",
"",
"",
"",
"",
"",
"",
"",
"",
"���l",
"����",
"���ƥ����",
"���ƥ���",
"",
"",
"",
"",
"",
"",
"",
"",
"������",
"���l",
"���l",
"",
"",
"���",
"���l",
"",
"",
"",
"",
"",
"",
"",
"���l",
"���l",
"���l",
"",
"���l",
"���l�����",
"���㥕�",
"",
"����",
"���㥕�",
"",
"",
"�m������",
"���l",
"���l",
"���l",
"���l",
"���l",
"",
"",
"",
"",
"",
"",
"",
"���l",
"",
"",
"",
"������",
"������",
"",
"�m����ؒO",
"",
"",
"",
"�������l",
"��쥍���",
"��쥍���",
"��쥍���",
"���l",
"���l",
"���l",
"���l",
"",
"",
"",
"",
"���l",
"",
"",
"���l",
"���l",
"����",
"",
"",
"����",
"",
"",
"",
"",
"",
"",
"",
"",
"���l",
"���l",
"",
"���l",
"���l",
"����",
"����",
"����",
"",
"�d�b����l",
"",
"",
"",
"",
"",
"���l",
"���l",
"���l",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"������",
"�|���l",
"������",
"������",
"",
"",
"",
"���l�����",
"���l�����",
"",
"���l",
"���l",
"",
"���l",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"���l",
"���l",
"",
"���l",
"",
"",
"���l",
"",
"",
"",
"�d�b�����",
"�d�b�����",
"",
"���l�����",
"",
"����",
"",
"",
"����",
"",
"",
"���l",
"���l",
"���l",
"���l",
"",
"",
"���",
"",
"",
"���l",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"������",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"�|���l",
"������",
"������",
"",
"",
"",
"",
"",
"",
"�ؒO����",
"�ؒO����",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"���l",
"���l",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"���l",
"��������l",
"",
"",
"",
"���l",
"",
"�ؒO����",
"",
"",
"",
"",
"",
"",
"",
"������",
"",
"���l",
"",
"",
"",
"���l",
"",
"",
"",
"",
"",
"���l",
"���l",
"",
"���l",
"���l",
"���l",
"",
"���",
"",
"",
"",
"",
"���",
"���",
"�d�b",
"",
"",
"",
"",
"",
"",
"",
"���l",
"���l",
"",
"�����l",
"",
"",
"",
"",
"",
"",
"",
"��������l",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"���l",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"����@",
"",
"",
"",
"���l",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"���l����",
"���l",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"�d�b",
"",
"���l",
"�|���l",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"���l",
"���l",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"���l",
"",
"",
"",
"",
"",
"",
"",
"������",
"������",
"������",
"",
"",
"",
"���l",
"",
"���l",
"���l",
"",
"���l",
"",
"",
"",
"",
"���l",
"���l",
"",
"",
"",
"",
"",
"",
"���l",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"������",
"",
"",
"",
"",
"",
"",
"",
"���l",
"",
"",
"",
"",
"",
"",
"���l����l",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"���l",
"",
"",
"",
"",
"���l",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"�̐l",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"�B��",
"",
"",
"�ؒO�����",
"",
"",
"���l",
"",
"",
"���l",
"",
"",
"",
"",
"",
"",
"���l",
"���������",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"���l",
"������",
"�d�b�����",
"",
"",
"",
"���l",
"�ؒO����l",
"",
"",
"",
"�̐l",
"",
"",
"",
"",
"",
"���l",
"���l",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"���l",
"",
"",
"",
"",
"",
"���l",
"���l",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"�ؒO����l",
"���l",
"",
"",
"",
"",
"",
"",
"",
"",
"",
""
};
