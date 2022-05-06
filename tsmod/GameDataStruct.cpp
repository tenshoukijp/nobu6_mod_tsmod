#define _CRT_SECURE_NO_WARNINGS

#include "WinTarget.h"
#include <fstream>
#include <math.h>
#include <algorithm>


#include "TSModCommand.h"
#include "CommonGlobalHandle.h"
#include "GameDataStruct.h"
#include "TMath.h"
#include "OnChangeWeather.h"
#include "ScenarioMod.h"
#include "FuncBattleUnitStateRepaint.h"

using namespace std;

#pragma pack(1)

NB6YEAR				*const _nb6year = (NB6YEAR *)GAMEDATASTRUCT_YEAR_ADDRESSS;
NB6TURN				*const _nb6turn = (NB6TURN *)GAMEDATASTRUCT_TURN_ADDRESSS;
NB6SOBA				*const _nb6soba = (NB6SOBA *)GAMEDATASTRUCT_SOBA_ADDRESSS;
NB6PLAYERS8			*const nb6players8 = (NB6PLAYERS8 *)GAMEDATASTRUCT_PLAYERS8_ADDRESSS;
NB6BUSHOUCOPY		nb6bushoucopy[GAMEDATASTRUCT_BUSHOU_NUM]		= {};
NB6BUSHOU			*const nb6bushouref = (NB6BUSHOU *)GAMEDATASTRUCT_BUSHOU_ADDRESSS;

NB6BUSHOUNAME		*const nb6bushouname = (NB6BUSHOUNAME *)GAMEDATASTRUCT_BUSHOUNAME_ADDRESSS;
NB6BUSHOURETSUDEN   nb6bushouretsuden[GAMEDATASTRUCT_RETSUDEN_NUM] = {};
NB6KAHOURETSUDEN    nb6kahouretsuden[GAMEDATASTRUCT_KAHOU_NUM] = {};
NB6CASTLE			*const nb6castle = (NB6CASTLE *)GAMEDATASTRUCT_CASTLE_ADDRESSS;
NB6CASTLEMAX		*const nb6castlemax = (NB6CASTLEMAX *)GAMEDATASTRUCT_CASTLEMAX_ADDRESSS;
NB6PARAMMAX			*const _nb6parammax = (NB6PARAMMAX *)GAMEDATASTRUCT_PARAMMAX_ADDRESS;
NB6PARAMMAX_MAIN	*const _nb6parammax_main = (NB6PARAMMAX_MAIN *)GAMEDATASTRUCT_PARAMMAX_MAIN_ADDRESS;
NB6DAIMYO			*const nb6daimyo = (NB6DAIMYO *)GAMEDATASTRUCT_DAIMYO_ADDRESS;
NB6GUNDAN			*const nb6gundan = (NB6GUNDAN *)GAMEDATASTRUCT_GUNDAN_ADDRESS;
NB6CHIMEI			*const nb6chimei = (NB6CHIMEI *)GAMEDATASTRUCT_CHIMEI_ADDRESS;
NB6KANI				*const nb6kani   = (NB6KANI *)GAMEDATASTRUCT_KANI_ADDRESS;
NB6KAHOU			*const nb6kahou  = (NB6KAHOU *)GAMEDATASTRUCT_KAHOU_ADDRESSS;
NB6SOLDIERMAX		nb6soldiermax[GAMEDATASTRUCT_SOLDIERMAX_NUM]= {};
NB6REQUIREDMERITS   *const nb6requiredmerits = (NB6REQUIREDMERITS *)GAMEDATASTRUCT_REQUIREDMERITS_ADDRESS;
NB6SETTINGS			*const _nb6settings = (NB6SETTINGS *)GAMEDATASTRUCT_SETTINGS_ADDRESSS;
NB6SETTINGSEX		*const _nb6settingsex = (NB6SETTINGSEX *)GAMEDATASTRUCT_SETTINGSEX_ADDRESSS;
NB6FUKIDASHIRELATEPERSONS *const nb6fukidashirelatepersons = (NB6FUKIDASHIRELATEPERSONS *)GAMEDATASTRUCT_FUKIDASHI_RELATE_PERSONS;
NB6CASTLE_HEX_ROLE	*const _nb6castle_hex_role = (NB6CASTLE_HEX_ROLE *)GAMEDATASTRUCT_CASTLE_HEX_ROLE_ADDRESS;
NB6CASTLE_HEX_HIGH	*const _nb6castle_hex_high = (NB6CASTLE_HEX_HIGH *)GAMEDATASTRUCT_CASTLE_HEX_HIGH_ADDRESS;

NB6FIELD_HEX_ROLE  *const _nb6field_hex_role = (NB6FIELD_HEX_ROLE *)GAMEDATASTRUCT_FIELD_HEX_ROLE_ADDRESS;

NB6CASTLE_HEX_ROLE *const _nb6fire_hex_role = (NB6CASTLE_HEX_ROLE *)GAMEDATASTRUCT_CASTLE_HEX_FIRE_ROLE_ADDRESS;


NB6CASTLE_HEX_UNIT_POS *const _nb6castle_hex_unit_pos = (NB6CASTLE_HEX_UNIT_POS *)GAMEDATASTRUCT_CASTLE_HEX_UNIT_POS_ADDRESS;
NB6FIELD_HEX_UNIT_POS  *const _nb6field_hex_unit_pos  = (NB6FIELD_HEX_UNIT_POS *)GAMEDATASTRUCT_FIELD_HEX_UNIT_POS_ADDRESS;


NB6BUSHOU_HEX_UNIT_INFO *const nb6bushou_hex_unit_info = (NB6BUSHOU_HEX_UNIT_INFO *)GAMEDATASTRUCT_WAR_HEX_UNIT_INFO_ADDRESS; // ���ɂ�����e�����̏��
NB6GUNDAN_HEX_INFO *const _nb6gundan_hex_info = (NB6GUNDAN_HEX_INFO *)GAMEDATASTRUCT_WAR_HEX_TURN_ADDRESS; 

NB6WAR_JOINT_DAIMYO_HEX_GROUP_INFO *const nb6war_joint_daimyo_hex_group_info = (NB6WAR_JOINT_DAIMYO_HEX_GROUP_INFO *)GAMEDATASTRUCT_WAR_JOINT_DAIMYO_HEX_GROUP_INFO_ADDRESS; // ���ɎQ�������喼�⑍�叫�̏��

NB6FUKIDASHI_DIALOG_LAYOUT *const _nb6fukidashi_dialog_layout = (NB6FUKIDASHI_DIALOG_LAYOUT *)GAMEDATASTRUCT_FUKIDASHI_DIALOG_LAYOUT_ADDRESS; // 

// ���݃v���C���̃V�i���I���P�[�V�����𓾂�(0�`5, �s���ȏꍇ-1)

int getCurScenarioLocation() {
	if ( 0 <= iSelectingScenarioLocation && iSelectingScenarioLocation <= 5 ) {
		return iSelectingScenarioLocation;
	}
	return -1;
}

int WINAPI Extern_getCurScenarioLocation() {
	return getCurScenarioLocation();
}

/*
004D3728  [E4 36 4D 00] [C4 36 4D 00] [A4 36 4D 00] [84 36 4D 00]  �EM.�6M.�6M.�EM.
004D3738  [64 36 4D 00] [44 36 4D 00]                          d6M.D6M.
*/
char* getScenarioTitle(int iLocation) {
	if ( 0 <= iLocation && iLocation <= 5 ) {
		int *p = (int *)GAMEDATASTRUCT_SCENARIO_NAME_POINTER_ARRAY_ADDRESS;
		int iTargetAddress = *(p+iLocation);
		char *pTitle = (char *)iTargetAddress;
		return pTitle;
	} else {
		return NULL;
	}
}


// ���݂̃Q�[���i�s���̔N���𓾂�B
int getYearAD() {
	return nb6year.year + getStartYearAD();
}

// �Q�[���̊J�n�N�B�f�t�H���g��1454�N�ł��邪�A���������ꍇ�ɔ����ăA�h���X���瓾��悤�ɂ���B
int getStartYearAD() {
	return *((WORD *)GAMEDATASTRUCT_START_YEAR_ADDRESS);
}

// ���݂̃Q�[���i�s���̋G�߂𓾂�
char szSeasonName[5][3] = {"�t", "��", "�H", "�~", "��"};
char* getSeasonName() {
	char season = nb6year.season;

	if (season==0) {
		return szSeasonName[0];
	} else if (season==1) {
		return szSeasonName[1];
	} else if (season==2) {
		return szSeasonName[2];
	} else if (season==3) {
		return szSeasonName[3];
	}
	return szSeasonName[4];
}

// �����̔N��𓾂�
int getYearsOld( int iBushouID ) {
	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
		return nb6year.year - nb6bushouref[iBushouID].birth + 1;
	}
	return 0xFF;
}

/*
* �����̔N���ݒ肷��B
*/
bool setYearsOld(int iBushouID, int iYearsOld) {

	// �͈̓`�F�b�N
	if (0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM) {

		int check = nb6year.year + 1 - iYearsOld;
		if (check < 0) {
			OutputDebugStream("�w��N��͔͈͊O�B�a���N�����ʂƂ��āA�V�ċL��N(1454)��葁�����܂�ƂȂ��Ă��܂��Ă���B");
			check = 0;
		}
		if (check > 255) {
			OutputDebugStream("�w��N��͔͈͊O�B�a���N�����ʂƂ��āA�V�ċL�ŏI�N(1708)���x�����܂�ƂȂ��Ă��܂��Ă���B");
			check = 255;
		}
		nb6bushouref[iBushouID].birth = check;
		return true;
	}
	else {
		OutputDebugStream("iBushouID�͔͈͊O");
		return false;
	}
}




// �Ώۂ̌R�cID�������ꂩ�̃v���C���[���͉��̌R�c�ł���(�ő�W�l)
// iTargetGundanID�ɂ͔z���̔ԍ�(attach-1�̐�)��n������
bool isPlayerGundan( WORD iTargetGundanID ) {

	if (iTargetGundanID >= GAMEDATASTRUCT_GUNDAN_NUM) { 
		return false;
	}

	WORD iGundanID = 0;
	for ( int iPG=0; iPG<GAMEDATASTRUCT_PLAYERS8_NUM; iPG++) {
		// �v���C���[���S�����Ă���R�c�̂h�c���m��
		iGundanID = nb6players8[iPG].gundan;

		// 0xFFFF�Ȃ�΃v���C���[�ł͂Ȃ�
		if ( iGundanID == 0xFFFF ) {
			continue;
		}

		// �v���C���[�S���̌R�c�̑喼���A�^�[�Q�b�g�Ƃ��Ă���Ώۂ̌R�c�̑喼�ƈ�v����Ȃ�΁A
		// ����̓v���C���[�̐��͉��̌R�c�ł���B
		// �ǂ���nb6player8��gundan�̒l�́A�����Ǘ���gundan�̒l����+1����Ă���悤�ł���B
		// ����Ă����ł�-1����B
		if (iGundanID >= 1 && nb6gundan[iGundanID-1].attach == nb6gundan[iTargetGundanID].attach ) {
			return true;
		}
	}
	
	return false;
}

// �Ώۂ̕���ID�������ꂩ�̃v���C���[���͉��̕����ł���
bool isPlayerBushou( WORD iTargetBushouID ) {
	// ����ID�͈̔͂Ɏ��܂��Ă��邱��
	if ( 0 <= iTargetBushouID && iTargetBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
		// �^�[�Q�b�g�̕����̌R�cID���v���C���[�̌R�c�ł��邩��Ԃ�
		return isPlayerGundan(nb6bushouref[iTargetBushouID].gundan-1);
	} else {
		return false;
	}
}

// �Ώۂ̑喼ID�������ꂩ�̃v���C���[�S���̑喼�ł���
bool isPlayerDaimyo( WORD iTargetDaimyoID ) {
	// ����ID�͈̔͂Ɏ��܂��Ă��邱��
	if ( 0 <= iTargetDaimyoID && iTargetDaimyoID < GAMEDATASTRUCT_DAIMYO_NUM ) {
		// �^�[�Q�b�g�̕����̌R�cID���v���C���[�̌R�c�ł��邩��Ԃ�
		return isPlayerGundan(nb6daimyo[iTargetDaimyoID].gundan-1);
	} else {
		return false;
	}
}


// ���y�E�R�n�E�S�C�E���R�̓K���𐔒l�œ���B(0=E,1=D,2=C,3=B,4=A,5=S)
int getBushouAptitude( int iBushouID, APTITUDE eAptitude ) {
	if (eAptitude==APTITUDE_ASIGARU) {
		return nb6bushouref[iBushouID].aptitudeasi;
	} else if (eAptitude==APTITUDE_KIBA) {
		return nb6bushouref[iBushouID].aptitudekib;
	} else if (eAptitude==APTITUDE_TEPPOU) {
		// �S�C�͂Q�̃t�B�b�g�t�B�[���h�ɕ�����Ă��܂��Ă���̂ŁA�v�Z
		int ret = nb6bushouref[iBushouID]._aptitudegun_lo << 2;
		ret += nb6bushouref[iBushouID]._aptitudegun_hi;
		return ret;
	} else if (eAptitude==APTITUDE_SUIGUN) {
		return nb6bushouref[iBushouID].aptitudesui;

	} else if (eAptitude == APTITUDE_YARI) {
		return nb6savedata_ex.bushou_ex[iBushouID].aptitudeyari;
	}
	else if (eAptitude == APTITUDE_YUMI) {
		return nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi;
	}
	return -1;
}


extern int WINAPI Extern_setBushouExAptitude(int iBushouID, APTITUDE eAptitude, int iTekisei);

// ���y�E�R�n�E�S�C�E���R�̓K���𐔒l�Őݒ肷��B(0=E,1=D,2=C,3=B,4=A,5=S)
void setBushouAptitude( int iBushouID, APTITUDE eAptitude, int iTekisei ) {
	if (eAptitude==APTITUDE_ASIGARU) {
		nb6bushouref[iBushouID].aptitudeasi = iTekisei;
	} else if (eAptitude==APTITUDE_KIBA) {
		nb6bushouref[iBushouID].aptitudekib = iTekisei;
	} else if (eAptitude==APTITUDE_TEPPOU) {
		// �S�C�͂Q�̃t�B�b�g�t�B�[���h�ɕ�����Ă��܂��Ă���̂ŁA�v�Z
		nb6bushouref[iBushouID]._aptitudegun_hi = iTekisei & 0x0003;  // 0b000000HH
		nb6bushouref[iBushouID]._aptitudegun_lo = iTekisei >> 2;      // 0b00000L00
	} else if (eAptitude==APTITUDE_SUIGUN) {
		nb6bushouref[iBushouID].aptitudesui = iTekisei;

	}
	else {
		Extern_setBushouExAptitude(iBushouID, eAptitude, iTekisei);
	}
}

// �|�̓K���l��ݒ肷��(ScenarioMod������̐ݒ�) (0=E,1=D,2=C,3=B,4=A,5=S) �l�̎擾�����˂�B
int WINAPI Extern_setBushouExAptitude(int iBushouID, APTITUDE eAptitude, int iTekisei ) {
	if ( eAptitude==APTITUDE_YUMI) {
		// -1�Ȃ�ΐݒ�ł͂Ȃ��A�l�̎擾
		if ( iTekisei == -1 ) {
			return nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi;
		}
		// �L���Ȓl�Ȃ���
		else {
			// �����|���Ȃ��̂ł���΁A�O�ŏ㏑��
			if ( TSModCommand::World::cmdTheYumiNoExists ) {
				nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi = 0;
			} else {
				nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi = iTekisei;
			}
			return iTekisei;
		}

	} else if (eAptitude==APTITUDE_YARI) {
		// -1�Ȃ�ΐݒ�ł͂Ȃ��A�l�̎擾
		if ( iTekisei == -1 ) {
			return nb6savedata_ex.bushou_ex[iBushouID].aptitudeyari;
		}
		else {
			nb6savedata_ex.bushou_ex[iBushouID].aptitudeyari = iTekisei;
			return iTekisei;
		}
	}
	return -1;
}




// �����̓Ɨ��l�𓾂�
int getBushouIndependence( int iBushouID ) {
	// �S�C�͂Q�̃t�B�b�g�t�B�[���h�ɕ�����Ă��܂��Ă���̂ŁA�v�Z
	int ret = nb6bushouref[iBushouID]._independence_lo << 2;
	ret += nb6bushouref[iBushouID]._independence_hi;
	return ret;
}

// �����̓Ɨ��l��ݒ肷��
void setBushouIndependence( int iBushouID, int iIndependence ) {
	nb6bushouref[iBushouID]._independence_hi = iIndependence & 0x0003;  // 0b000000HH
	nb6bushouref[iBushouID]._independence_lo = iIndependence >> 2;      // 0b00000L00
}

// �����̐퓬�l
int getBushouBat(int iBushouID) {
	// ��ˁ~��m/2000��
	return int(nb6bushouref[iBushouID].maxbat * (float(nb6bushouref[iBushouID].expbat) / float(2000)));
}
// �����̒q�d�l
int getBushouInt(int iBushouID) {
	// ��ˁ~��m/2000��
	return int(nb6bushouref[iBushouID].maxint * (float(nb6bushouref[iBushouID].expint) / float(2000)));
}
// �����̐����l
int getBushouGov(int iBushouID) {
	// ��ˁ~��m/2000��
	return int(nb6bushouref[iBushouID].maxgov * (float(nb6bushouref[iBushouID].expgov) / float(2000)));
}

// �ƕ�Ȃǂ��܂߂Ȃ��f�̐����˔\
int getBushouPureGovMax(int iBushouID) {
	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
		int upparam = 0; // �㏸�l
		for (int iKahouID =0; iKahouID < GAMEDATASTRUCT_KAHOU_NUM; iKahouID++) {
			// ���������^�C�v�F0 ���q  1 ����  2 ����  3 ����  4 �ԓ�  
			if ( 0 <= nb6kahou[iKahouID].type && nb6kahou[iKahouID].type <= 4 ) {
				// ������Ǝw�蕐������v����
				if (nb6kahou[iKahouID].attach == iBushouID + 1) {
					// ���܂łōő�̏㏸�l
					if (nb6kahou[iKahouID].param > upparam ) {
						upparam = nb6kahou[iKahouID].param;
					}
				}
			}
		}

		int maxgov = nb6bushouref[iBushouID].maxgov;
		return (maxgov - upparam) > 0 ? (maxgov - upparam) : 1;
	} else {
		return 0;
	}
}

// �ƕ�Ȃǂ��܂߂Ȃ��f�̐퓬�˔\
int getBushouPureBatMax(int iBushouID) {
	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
		int upparam = 0; // �㏸�l
		for (int iKahouID =0; iKahouID < GAMEDATASTRUCT_KAHOU_NUM; iKahouID++) {
			// �퓬�����^�C�v�F5 ����  6 ��  7 �  8 �n  9 ��  A �w�H�D   
			if ( 5 <= nb6kahou[iKahouID].type && nb6kahou[iKahouID].type <= 0xA ) {
				// ������Ǝw�蕐������v����
				if (nb6kahou[iKahouID].attach == iBushouID + 1) {
					// ���܂łōő�̏㏸�n
					if (nb6kahou[iKahouID].param > upparam ) {
						upparam = nb6kahou[iKahouID].param;
					}
				}
			}
		}

		int maxbat = nb6bushouref[iBushouID].maxbat;
		return (maxbat - upparam) > 0 ? (maxbat - upparam) : 1;
	} else {
		return 0;
	}
}

// �ƕ�Ȃǂ��܂߂Ȃ��f�̒q�d�˔\
int getBushouPureIntMax(int iBushouID) {
	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
		int upparam = 0; // �㏸�l
		for (int iKahouID =0; iKahouID < GAMEDATASTRUCT_KAHOU_NUM; iKahouID++) {
			// �q�d�����^�C�v�FB ���q�{  C �����Ԗ{
			if ( 0xB <= nb6kahou[iKahouID].type && nb6kahou[iKahouID].type <= 0xC ) {
				// ������Ǝw�蕐������v����
				if (nb6kahou[iKahouID].attach == iBushouID + 1) {
					// ���܂łōő�̏㏸�n
					if (nb6kahou[iKahouID].param > upparam ) {
						upparam = nb6kahou[iKahouID].param;
					}
				}
			}
		}

		int maxint = nb6bushouref[iBushouID].maxint;
		return (maxint - upparam) > 0 ? (maxint - upparam) : 1;
	} else {
		return 0;
	}
}

// �ƕ�Ȃǂ��܂߂Ȃ��f�̖�]
int getBushouPureAmbition(int iBushouID) {
	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
		int upparam = 0; // �㏸�l
		for (int iKahouID =0; iKahouID < GAMEDATASTRUCT_KAHOU_NUM; iKahouID++) {
			// ��]�����^�C�v�FD ����  E �n���V  F ��؎��v  10 ���n��  11 �\����  12 ����  13 �ʎ蔠  14 �r�[�h���t  15 ���ዾ  16 ���\����  17 ����
			if ( 0xD <= nb6kahou[iKahouID].type && nb6kahou[iKahouID].type <= 0x17 ) {
				// ������Ǝw�蕐������v����
				if (nb6kahou[iKahouID].attach == iBushouID + 1) {
					// ���܂łōő�̏㏸�n
					if (nb6kahou[iKahouID].param > upparam ) {
						upparam = nb6kahou[iKahouID].param;
					}
				}
			}
		}

		int ambition = nb6bushouref[iBushouID].ambition;
		return (ambition - upparam) > 0 ? (ambition - upparam) : 1;
	} else {
		return 0;
	}
}

// ���ʂȂǂ��܂߂Ȃ��f�̖���
int getBushouPureCharm(int iBushouID) {
	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
		int upparam = 0; // �㏸�l
		for (int iKaniID =0; iKaniID < GAMEDATASTRUCT_KANI_NUM; iKaniID++) {
			// ������Ǝw�蕐������v����
			if (nb6kani[iKaniID].attach == iBushouID + 1) {
				// ���܂łōő�̏㏸�n
				if (nb6kani[iKaniID].param > upparam ) {
					upparam = nb6kani[iKaniID].param;
				}
			}
		}

		int charm = nb6bushouref[iBushouID].charm;
		return (charm - upparam) > 0 ? (charm - upparam) : 1;
	} else {
		return 0;
	}
}



// �ƕ�ɂ�鐭���̏グ��
int getBushouGovMaxUpByKahou(int iBushouID) {
	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
		int upparam = 0; // �㏸�l
		for (int iKahouID =0; iKahouID < GAMEDATASTRUCT_KAHOU_NUM; iKahouID++) {
			// ���������^�C�v�F0 ���q  1 ����  2 ����  3 ����  4 �ԓ�  
			if ( 0 <= nb6kahou[iKahouID].type && nb6kahou[iKahouID].type <= 4 ) {
				// ������Ǝw�蕐������v����
				if (nb6kahou[iKahouID].attach == iBushouID + 1) {
					// ���܂łōő�̏㏸�l
					if (nb6kahou[iKahouID].param > upparam ) {
						upparam = nb6kahou[iKahouID].param;
					}
				}
			}
		}
		return upparam;
	} else {
		return 0;
	}
}

// �ƕ�ɂ��퓬�̏グ��
int getBushouBatMaxUpByKahou(int iBushouID) {
	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
		int upparam = 0; // �㏸�l
		for (int iKahouID =0; iKahouID < GAMEDATASTRUCT_KAHOU_NUM; iKahouID++) {
			// �퓬�����^�C�v�F5 ����  6 ��  7 �  8 �n  9 ��  A �w�H�D   
			if ( 5 <= nb6kahou[iKahouID].type && nb6kahou[iKahouID].type <= 0xA ) {
				// ������Ǝw�蕐������v����
				if (nb6kahou[iKahouID].attach == iBushouID + 1) {
					// ���܂łōő�̏㏸�n
					if (nb6kahou[iKahouID].param > upparam ) {
						upparam = nb6kahou[iKahouID].param;
					}
				}
			}
		}
		return upparam;
	} else {
		return 0;
	}
}

// �ƕ�ɂ��q�d�̏グ��
int getBushouIntMaxUpByKahou(int iBushouID) {
	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
		int upparam = 0; // �㏸�l
		for (int iKahouID =0; iKahouID < GAMEDATASTRUCT_KAHOU_NUM; iKahouID++) {
			// �q�d�����^�C�v�FB ���q�{  C �����Ԗ{
			if ( 0xB <= nb6kahou[iKahouID].type && nb6kahou[iKahouID].type <= 0xC ) {
				// ������Ǝw�蕐������v����
				if (nb6kahou[iKahouID].attach == iBushouID + 1) {
					// ���܂łōő�̏㏸�n
					if (nb6kahou[iKahouID].param > upparam ) {
						upparam = nb6kahou[iKahouID].param;
					}
				}
			}
		}

		return upparam;
	} else {
		return 0;
	}
}

// �ƕ�ɂ���]�̏グ��
int getBushouAmbitionUpByKahou(int iBushouID) {
		if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
		int upparam = 0; // �㏸�l
		for (int iKahouID =0; iKahouID < GAMEDATASTRUCT_KAHOU_NUM; iKahouID++) {
			// ��]�����^�C�v�FD ����  E �n���V  F ��؎��v  10 ���n��  11 �\����  12 ����  13 �ʎ蔠  14 �r�[�h���t  15 ���ዾ  16 ���\����  17 ����
			if ( 0xD <= nb6kahou[iKahouID].type && nb6kahou[iKahouID].type <= 0x17 ) {
				// ������Ǝw�蕐������v����
				if (nb6kahou[iKahouID].attach == iBushouID + 1) {
					// ���܂łōő�̏㏸�n
					if (nb6kahou[iKahouID].param > upparam ) {
						upparam = nb6kahou[iKahouID].param;
					}
				}
			}
		}
		return upparam;
	} else {
		return 0;
	}
}


// ���ʂɂ�閣�͂̏グ��
int getBushouCharmUpByKani(int iBushouID) {
	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
		int upparam = 0; // �㏸�l
		for (int iKaniID =0; iKaniID < GAMEDATASTRUCT_KANI_NUM; iKaniID++) {
			// ������Ǝw�蕐������v����
			if (nb6kani[iKaniID].attach == iBushouID + 1) {
				// ���܂łōő�̏㏸�n
				if (nb6kani[iKaniID].param > upparam ) {
					upparam = nb6kani[iKaniID].param;
				}
			}
		}
		return upparam;
	} else {
		return 0;
	}
}


// ���ʂ𕐏��ɃA�^�b�`����B���X�ʕ����������Ă����ꍇ�́A���̕�������w�肵�������ւƃA�^�b�`���ς��B
// �\�͒l�̕ύX�𔺂��B����ɖ߂��ꍇ�́A0xFFFF���w�肷�邱�ƁB
bool ReAttachKani(int iKaniID, int iNewBushouID) {
	if ( !(0 <= iKaniID && iKaniID < GAMEDATASTRUCT_KANI_NUM) ) {
		return false;
	}

	int iOldBushouID = nb6kani[iKaniID].attach - 1;
	// �{�l���B��������K�v���Ȃ��B
	if ( iOldBushouID == iNewBushouID ) {
		return true;
	}
	// ���X���삪�����Ă��āA�V���Ȏw������삾�B��������K�v���Ȃ��B
	if ( iOldBushouID >= 0xFFFE && iNewBushouID >= 0xFFFE )  {
		return true;
	}


	// ���̕����̐����\�͂̑f�l
	int iOldBushouPureCharm = 0;
	// �V�����w��̕����͐��K�̕����ł���A����ł͂Ȃ��B
	if ( 0 <= iOldBushouID && iOldBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
		iOldBushouPureCharm = getBushouPureCharm(iOldBushouID);
	// �s���Ȕԍ��͒���Ƃ������Ƃɂ���B
	} else {
		iOldBushouID = 0xFFFF;
	}

	int iNewBushouPureCharm = 0;
	// �V�����w��̕����͐��K�̕����ł���A����ł͂Ȃ��B
	if ( 0 <= iNewBushouID && iNewBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
		iNewBushouPureCharm = getBushouPureCharm(iNewBushouID);

	// �s���Ȕԍ��͒���Ƃ������Ƃɂ���B
	} else {
		iNewBushouID = 0xFFFF;
	}

	// �Ώۂ̉ƕ�̏����҂�ύX����B
	nb6kani[iKaniID].attach = (iNewBushouID == 0xFFFF) ? 0xFFFF : (iNewBushouID + 1);

	// ���̕����͐��K�̕����ł���A����ł͂Ȃ��B
	if ( 0 <= iOldBushouID && iOldBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
		int iOldBushouCharmUpByKani = getBushouCharmUpByKani(iOldBushouID);
		nb6bushouref[iOldBushouID].charm = iOldBushouPureCharm + iOldBushouCharmUpByKani;
	}

	// �V�����w��̕����͐��K�̕����ł���A����ł͂Ȃ��B
	if ( 0 <= iNewBushouID && iNewBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
		int iNewBushouCharmUpByKani = getBushouCharmUpByKani(iNewBushouID);
		nb6bushouref[iNewBushouID].charm = iNewBushouPureCharm + iNewBushouCharmUpByKani;
	}

	return true;
}


// �ƕ�𕐏��ɃA�^�b�`����B���X�ʕ����������Ă����ꍇ�́A���̕�������w�肵�������ւƃA�^�b�`���ς��B
// �\�͒l�̕ύX�𔺂��B���l�ɖ߂��ꍇ�́A0xFFFF���w�肷�邱�ƁB
bool ReAttachKahou(int iKahouID, int iNewBushouID) {
	if ( !(0 <= iKahouID && iKahouID < GAMEDATASTRUCT_KAHOU_NUM) ) {
		return false;
	}

	// �����͍~�낷
	nb6kahou[iKahouID].bLost = false;
	// ���o��͍~�낷�B
	nb6kahou[iKahouID].bMitojo = false;

	int iOldBushouID = nb6kahou[iKahouID].attach - 1;
	// �{�l���B��������K�v���Ȃ��B
	if ( iOldBushouID == iNewBushouID ) {
		return true;
	}
	// ���X���l�������Ă��āA�V���Ȏw������l���B��������K�v���Ȃ��B
	if ( iOldBushouID >= 0xFFFE && iNewBushouID >= 0xFFFE )  {
		return true;
	}

	// ���������^�C�v�F0 ���q  1 ����  2 ����  3 ����  4 �ԓ�  
	if ( 0 <= nb6kahou[iKahouID].type && nb6kahou[iKahouID].type <= 4 ) {

		// ���̕����̐����\�͂̑f�l
		int iOldBushouPureGovMax = 0;
		// �V�����w��̕����͐��K�̕����ł���A���l�ł͂Ȃ��B
		if ( 0 <= iOldBushouID && iOldBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
			iOldBushouPureGovMax = getBushouPureGovMax(iOldBushouID);
		// �s���Ȕԍ��͏��l�Ƃ������Ƃɂ���B
		} else {
			iOldBushouID = 0xFFFF;
		}

		int iNewBushouPureGovMax = 0;
		// �V�����w��̕����͐��K�̕����ł���A���l�ł͂Ȃ��B
		if ( 0 <= iNewBushouID && iNewBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
			iNewBushouPureGovMax = getBushouPureGovMax(iNewBushouID);

		// �s���Ȕԍ��͏��l�Ƃ������Ƃɂ���B
		} else {
			iNewBushouID = 0xFFFF;
		}

		// �Ώۂ̉ƕ�̏����҂�ύX����B
		nb6kahou[iKahouID].attach = (iNewBushouID == 0xFFFF) ? 0xFFFF : (iNewBushouID + 1);

		// �V�����w��̕����͐��K�̕����ł���A���l�ł͂Ȃ��B
		if ( 0 <= iOldBushouID && iOldBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
			int iOldBushouGovMaxUpByKahou = getBushouGovMaxUpByKahou(iOldBushouID);
			nb6bushouref[iOldBushouID].maxgov = iOldBushouPureGovMax + iOldBushouGovMaxUpByKahou;
		}

		// �V�����w��̕����͐��K�̕����ł���A���l�ł͂Ȃ��B
		if ( 0 <= iNewBushouID && iNewBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
			int iNewBushouGovMaxUpByKahou = getBushouGovMaxUpByKahou(iNewBushouID);
			nb6bushouref[iNewBushouID].maxgov = iNewBushouPureGovMax + iNewBushouGovMaxUpByKahou;
		}

	// �퓬�����^�C�v�F5 ����  6 ��  7 �  8 �n  9 ��  A �w�H�D   
	} else if ( 5 <= nb6kahou[iKahouID].type && nb6kahou[iKahouID].type <= 0xA ) {

		// ���̕����̐����\�͂̑f�l
		int iOldBushouPureBatMax = 0;
		// �V�����w��̕����͐��K�̕����ł���A���l�ł͂Ȃ��B
		if ( 0 <= iOldBushouID && iOldBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
			iOldBushouPureBatMax = getBushouPureBatMax(iOldBushouID);
		// �s���Ȕԍ��͏��l�Ƃ������Ƃɂ���B
		} else {
			iOldBushouID = 0xFFFF;
		}

		int iNewBushouPureBatMax = 0;
		// �V�����w��̕����͐��K�̕����ł���A���l�ł͂Ȃ��B
		if ( 0 <= iNewBushouID && iNewBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
			iNewBushouPureBatMax = getBushouPureBatMax(iNewBushouID);

		// �s���Ȕԍ��͏��l�Ƃ������Ƃɂ���B
		} else {
			iNewBushouID = 0xFFFF;
		}

		// �Ώۂ̉ƕ�̏����҂�ύX����B
		nb6kahou[iKahouID].attach = (iNewBushouID == 0xFFFF) ? 0xFFFF : (iNewBushouID + 1);

		// �V�����w��̕����͐��K�̕����ł���A���l�ł͂Ȃ��B
		if ( 0 <= iOldBushouID && iOldBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
			int iOldBushouBatMaxUpByKahou = getBushouBatMaxUpByKahou(iOldBushouID);
			nb6bushouref[iOldBushouID].maxbat = iOldBushouPureBatMax + iOldBushouBatMaxUpByKahou;
		}

		// �V�����w��̕����͐��K�̕����ł���A���l�ł͂Ȃ��B
		if ( 0 <= iNewBushouID && iNewBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
			int iNewBushouBatMaxUpByKahou = getBushouBatMaxUpByKahou(iNewBushouID);
			nb6bushouref[iNewBushouID].maxbat = iNewBushouPureBatMax + iNewBushouBatMaxUpByKahou;
		}


	// �q�d�����^�C�v�FB ���q�{  C �����Ԗ{
	} else if ( 0xB <= nb6kahou[iKahouID].type && nb6kahou[iKahouID].type <= 0xC ) {

		// ���̕����̐����\�͂̑f�l
		int iOldBushouPureIntMax = 0;
		// �V�����w��̕����͐��K�̕����ł���A���l�ł͂Ȃ��B
		if ( 0 <= iOldBushouID && iOldBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
			iOldBushouPureIntMax = getBushouPureIntMax(iOldBushouID);
		// �s���Ȕԍ��͏��l�Ƃ������Ƃɂ���B
		} else {
			iOldBushouID = 0xFFFF;
		}

		int iNewBushouPureIntMax = 0;
		// �V�����w��̕����͐��K�̕����ł���A���l�ł͂Ȃ��B
		if ( 0 <= iNewBushouID && iNewBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
			iNewBushouPureIntMax = getBushouPureIntMax(iNewBushouID);

		// �s���Ȕԍ��͏��l�Ƃ������Ƃɂ���B
		} else {
			iNewBushouID = 0xFFFF;
		}

		// �Ώۂ̉ƕ�̏����҂�ύX����B
		nb6kahou[iKahouID].attach = (iNewBushouID == 0xFFFF) ? 0xFFFF : (iNewBushouID + 1);

		// �V�����w��̕����͐��K�̕����ł���A���l�ł͂Ȃ��B
		if ( 0 <= iOldBushouID && iOldBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
			int iOldBushouIntMaxUpByKahou = getBushouIntMaxUpByKahou(iOldBushouID);
			nb6bushouref[iOldBushouID].maxint = iOldBushouPureIntMax + iOldBushouIntMaxUpByKahou;
		}

		// �V�����w��̕����͐��K�̕����ł���A���l�ł͂Ȃ��B
		if ( 0 <= iNewBushouID && iNewBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
			int iNewBushouIntMaxUpByKahou = getBushouIntMaxUpByKahou(iNewBushouID);
			nb6bushouref[iNewBushouID].maxint = iNewBushouPureIntMax + iNewBushouIntMaxUpByKahou;
		}

	// ��]�����^�C�v�FD ����  E �n���V  F ��؎��v  10 ���n��  11 �\����  12 ����  13 �ʎ蔠  14 �r�[�h���t  15 ���ዾ  16 ���\����  17 ����
	} else if  ( 0xD <= nb6kahou[iKahouID].type && nb6kahou[iKahouID].type <= 0x17 ) {

		// ���̕����̐����\�͂̑f�l
		int iOldBushouPureAmbition = 0;
		// �V�����w��̕����͐��K�̕����ł���A���l�ł͂Ȃ��B
		if ( 0 <= iOldBushouID && iOldBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
			iOldBushouPureAmbition = getBushouPureAmbition(iOldBushouID);
		// �s���Ȕԍ��͏��l�Ƃ������Ƃɂ���B
		} else {
			iOldBushouID = 0xFFFF;
		}

		int iNewBushouPureAmbition = 0;
		// �V�����w��̕����͐��K�̕����ł���A���l�ł͂Ȃ��B
		if ( 0 <= iNewBushouID && iNewBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
			iNewBushouPureAmbition = getBushouPureAmbition(iNewBushouID);

		// �s���Ȕԍ��͏��l�Ƃ������Ƃɂ���B
		} else {
			iNewBushouID = 0xFFFF;
		}

		// �Ώۂ̉ƕ�̏����҂�ύX����B
		nb6kahou[iKahouID].attach = (iNewBushouID == 0xFFFF) ? 0xFFFF : (iNewBushouID + 1);

		// �V�����w��̕����͐��K�̕����ł���A���l�ł͂Ȃ��B
		if ( 0 <= iOldBushouID && iOldBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
			int iOldBushouAmbitionUpByKahou = getBushouAmbitionUpByKahou(iOldBushouID);
			nb6bushouref[iOldBushouID].ambition = iOldBushouPureAmbition + iOldBushouAmbitionUpByKahou;
		}

		// �V�����w��̕����͐��K�̕����ł���A���l�ł͂Ȃ��B
		if ( 0 <= iNewBushouID && iNewBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
			int iNewBushouAmbitionUpByKahou = getBushouAmbitionUpByKahou(iNewBushouID);
			nb6bushouref[iNewBushouID].ambition = iNewBushouPureAmbition + iNewBushouAmbitionUpByKahou;
		}

	}

	return true;
}




// ��ID���畐��ID�����߂�B���s����0xFFFF
int getBushouIDFromFaceID(int iFaceID) {
	for ( int iBushouID=0; iBushouID<GAMEDATASTRUCT_BUSHOU_NUM; iBushouID++ ) {
		if ( nb6bushouname[iBushouID].number == iFaceID ) {
			return iBushouID;
		}
	}
	return 0xFFFF;
}


// �w�蕐���̔�����iBushouID�𓾂�B���������Ȃ��ꍇ�ɂ́A0xFFFF���Ԃ��Ă���B
int getHanryoBushouID(int iBushouID) {

	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// �w��̕����̔������
		int iBushouIDSpouse = nb6bushouref[iBushouID].spouse; //

		// �������Ă��Ȃ�
		if ( iBushouIDSpouse == 0xFF ) {
			return 0xFFFF;
		}

		int iTsumaBushouID = iBushouIDSpouse+500;

		// �����̔ԍ����������g��������A�����͍ȑ��̗���
		if ( iTsumaBushouID == iBushouID ) {
			for ( int i=0; i<GAMEDATASTRUCT_BUSHOU_NUM; i++ ) {
				// �������g�ȊO�ŁA���������ԍ��������Ă�̂��A�����̕���
				if ( i != iBushouID &&  nb6bushouref[i].spouse == iBushouIDSpouse ) {
					return i;
				}
			}
			return 0xFFFF;

		// �����͕v���B�Ȃɂ������ԍ����ݒ肳��Ă��邩�m�F�B
		} else {
			if ( nb6bushouref[iTsumaBushouID].spouse == iBushouIDSpouse ) {
				return iTsumaBushouID;
			// �Ȃ����ݒ肳��Ă��Ȃ��c�B�������Ńf�[�^���Ă���B
			} else {
				return 0xFFFF;
			}
		}
	} else {
		return 0xFFFF;
	}
}

// �P(���̕����̍Ȋ܂�)���A�w��̕����ƌ���������A�����֌W���͈�ؑ��삳��Ȃ��B
bool setHimeMarriage( int iHimeID, int iOttoID, BOOL withKodokuDeath ) {

	// ����l���B�A�E�g�B
	if ( iHimeID == iOttoID ) { return false; }

	// �v�ƕP���K��͈̔͂ɓ����Ă��Ȃ���΃A�E�g
	if (   0 <= iOttoID && iOttoID < GAMEDATASTRUCT_BUSHOU_NUM &&
		 500 <= iHimeID && iHimeID < GAMEDATASTRUCT_BUSHOU_NUM) {

		// �����P���̂h�c�̕������A�P�łȂ��Ȃ�΁A�A�E�g
		 if ( nb6bushouname[iHimeID].State != 5 ) { // 
			return false;
		}

		int _new_spouse = iHimeID-500; // �Ȃɓ���ׂ��l�́A�P�����ԍ��y�z��p�z-500

		// ���łɗ��҂͕v�w�ł���A��������K�v�������B
		if ( nb6bushouref[iOttoID].spouse == _new_spouse &&  nb6bushouref[iHimeID].spouse == _new_spouse ) {
			return true;
		}

		// �v�������łɌ������Ă��邩�A�������Ă����痣��
		int _otto_old_spouse = nb6bushouref[iOttoID].spouse; //
		// ����
		setHimeDivorce( _otto_old_spouse+500, withKodokuDeath );

		nb6bushouname[iHimeID].State = 5; // �V�W�ǓƏ����Ŏ��S������������Ȃ��̂ŕP�ɖ߂��B

		int _tuma_old_spouse = nb6bushouref[iHimeID].spouse; //
		setHimeDivorce( _tuma_old_spouse+500, withKodokuDeath );

		nb6bushouname[iHimeID].State = 5; // �V�W�ǓƏ����Ŏ��S������������Ȃ��̂ŕP�ɖ߂��B

		// �g���Y��ɂȂ����̂ŉ��߂Č���
		nb6bushouref[iOttoID].spouse = _new_spouse;
		nb6bushouref[iHimeID].spouse = _new_spouse;

		// ���喼�Ƃ��o���Ă�����
		int OldDaimyoID = nb6bushouref[iHimeID].attach;

		// �v���̑喼�̕��ɐg���񂹂�
		nb6bushouref[iHimeID].attach = nb6bushouref[iOttoID].attach;

		// ������v�̂��̂ŏ㏑������B
		strcpy( nb6bushouname[iHimeID].familyname, nb6bushouname[iOttoID].familyname );

		// ���ʁA�喼�Ƃ��ω������̂ł���΁A�m���N�������Z�b�g
		if ( OldDaimyoID != nb6bushouref[iHimeID].attach) {
			nb6bushouref[iHimeID].work = 0;
		}

		return true;
	}

	return false;
}


// �w��̕P�𗣍�������B
bool setHimeDivorce( int iHimeID, BOOL withKodokuDeath ) {

	// �v�ƕP���K��͈̔͂ɓ����Ă��Ȃ���΃A�E�g
	if ( 500 <= iHimeID && iHimeID < GAMEDATASTRUCT_BUSHOU_NUM ) {


		// �����P���̂h�c�̕������A�P�łȂ��Ȃ�΁A�A�E�g
		if ( nb6bushouname[iHimeID].State != 5 ) {
			return false;
		}

		int _tuma_spouse = nb6bushouref[iHimeID].spouse; //

		// ���X�������Ă��Ȃ��B
		if ( _tuma_spouse == 0xFF ) {
			return false;
		}

		// ���҂̌��������N���A
		for ( int iBushouID=0; iBushouID < GAMEDATASTRUCT_BUSHOU_NUM; iBushouID++ ) {
			// �v�������͍Ȃł���
			if ( nb6bushouref[iBushouID].spouse == _tuma_spouse ) {
				// ����
				nb6bushouref[iBushouID].spouse = 0xFF;
				// �v�ƍ�2�l�����Ƃ��l��0xFF�ɂ���K�v������̂ŁAbreak���Ȃ����ƁB
				// ���łɃf�[�^�ɃS�~�������Ă���ꍇ�ł��N���A
			}
		}


		// ���喼�Ƃ��o���Ă�����
		int OldDaimyoID = nb6bushouref[iHimeID].attach;

		// �ȉ��̐g������l�n�̏����́A�v���O�������ŏo���邾������Ă������Ƃ�����|�B
		// ���ɂ����œK�؂ȑ喼�ɋA���]�ڂł��Ȃ������Ƃ��Ă��A�V�ċL�̃v���O�����{�̂����G�ߎ��ɂ́A�K�؂ȑ喼�ɋA�������鏈�����s���B

		// ���������̂ŏ����喼��ύX����K�v��������B�܂��A�e���喼�����Ă��邩�ǂ����B
		for ( int iBushouID=0; iBushouID < GAMEDATASTRUCT_BUSHOU_NUM; iBushouID++ ) {

			// �喼�̂����c
			if ( nb6bushouname[iBushouID].State == 0 ) { // �喼

				// �����̐e���喼����Ă�B
				if ( nb6bushouref[iHimeID].parent < 5000 ) {
					// ���̑喼�͕P�̐e
					if ( nb6bushouname[iBushouID].number == nb6bushouref[iHimeID].parent ) {

						// �����ɐg���񂹂�
						nb6bushouref[iHimeID].attach = nb6bushouref[iBushouID].attach;

						strcpy( nb6bushouname[iHimeID].familyname, nb6bushouname[iBushouID].familyname );
						break;
					}
				}
			}
		}


		// �e���喼������Ă��Ȃ��Ƃ��A�e�������l(�����Z��o��)���喼������Ă��邩�ǂ����B
		for ( int iBushouID=0; iBushouID < GAMEDATASTRUCT_BUSHOU_NUM; iBushouID++ ) {

			// �喼�̂����c
			if ( nb6bushouname[iBushouID].State == 0 ) { // �喼

				// �e���������B
				if ( nb6bushouref[iHimeID].parent < 5000 ) {
					if ( nb6bushouref[iBushouID].parent == nb6bushouref[iHimeID].parent ) {

						// �����ɐg���񂹂�
						nb6bushouref[iHimeID].attach = nb6bushouref[iBushouID].attach;

						strcpy( nb6bushouname[iHimeID].familyname, nb6bushouname[iBushouID].familyname );
						break;
					}
				}
			}
		}

		// �e�Z�킪�喼����Ă��Ȃ��Ă��A��傪�喼������Ă���̂ł͂Ȃ����B
		for ( int iBushouID=0; iBushouID < GAMEDATASTRUCT_BUSHOU_NUM; iBushouID++ ) {

			// �喼�̂����c
			if ( nb6bushouname[iBushouID].State == 0 ) { // �喼

				// �e���������B
				if ( nb6bushouref[iHimeID].blood != 0xFF ) {
					if ( nb6bushouref[iBushouID].blood == nb6bushouref[iHimeID].blood ) {

						// �����ɐg���񂹂�
						nb6bushouref[iHimeID].attach = nb6bushouref[iBushouID].attach;

						strcpy( nb6bushouname[iHimeID].familyname, nb6bushouname[iBushouID].familyname );
						break;
					}
				}
			}
		}

		bool isTengaiKodoku = true;
		// �喼�łȂ��Ƃ��A�e���ǂ����ɂ���̂ł͂Ȃ����H
		for ( int iBushouID=0; iBushouID < GAMEDATASTRUCT_BUSHOU_NUM; iBushouID++ ) {

			// �����̐e
			if ( nb6bushouref[iHimeID].parent < 5000 ) {
				if ( nb6bushouname[iBushouID].number == nb6bushouref[iHimeID].parent ) {

					// �����ɐg���񂹂�
					nb6bushouref[iHimeID].attach = nb6bushouref[iBushouID].attach;

					strcpy( nb6bushouname[iHimeID].familyname, nb6bushouname[iBushouID].familyname );

					// �g��肪�����̂œV�W�ǓƂł͂Ȃ�
					isTengaiKodoku = false;
					break;
				}
			}
		}

		// �喼�łȂ��Ƃ��A�e�������l(�����Z��o��)���ǂ����ɂ���̂ł͂Ȃ����H
		for ( int iBushouID=0; iBushouID < GAMEDATASTRUCT_BUSHOU_NUM; iBushouID++ ) {

			// �e���������B
			if ( nb6bushouref[iHimeID].parent < 5000 ) {
				if ( nb6bushouref[iBushouID].parent == nb6bushouref[iHimeID].parent ) {

					// �����ɐg���񂹂�
					nb6bushouref[iHimeID].attach = nb6bushouref[iBushouID].attach;

					strcpy( nb6bushouname[iHimeID].familyname, nb6bushouname[iBushouID].familyname );

					// �g��肪�����̂œV�W�ǓƂł͂Ȃ�
					isTengaiKodoku = false;
					break;
				}
			}
		}


		// ���ʁA�喼�Ƃ��ω������̂ł���΁A�m���N�������Z�b�g
		if ( OldDaimyoID != nb6bushouref[iHimeID].attach) {
			nb6bushouref[iHimeID].work = 0;
		}

		if ( withKodokuDeath ) {
			if ( isTengaiKodoku ) {
				nb6bushouname[iHimeID].State = 7; // �V�W�ǓƂɂȂ����玀�S�Ƃ���B
			}
		}

		return true;
	}

	return false;
}




// State ���� �� State �Q�l��
bool setGenekiToRounin(int iBushouID) {
	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		if ( nb6bushouname[iBushouID].State == 2 ) { // ���𕐏��̏ꍇ��������������B

			// �Y�������̋��邪�����Ƃ�����񂪊i�[����Ă���B(���[�U�[�������ɂ���ĉ��Ă��Ȃ�)
			int iCastleID = nb6bushouref[iBushouID].castle-1;
			if (0 <= iCastleID && iCastleID < GAMEDATASTRUCT_CASTLE_NUM ) {
				if ( nb6castle[iCastleID].master == 0xFFFF ) { return false; } // �����N���X�g�����Ă���̂���ɒN���������Ă��Ȃ��B�������Ȃ��Breturn;

				// ��������̒l�ύX
				{
					nb6bushouname[iBushouID].State = 4; // ��Ԃ�Q�l�Ƃ���B
						
					nb6bushouname[iBushouID].position = 6; // �g���𑫌y�Ƃ���B

					nb6bushouref[iBushouID].attach = 0xFFFF; // �喼�ɏ������Ȃ��B
					nb6bushouref[iBushouID].gundan = 0xFFFF; // �R�c�ɏ������Ȃ��B
					nb6bushouref[iBushouID].loyal = 50; // ������50�Ƃ���B
					nb6bushouref[iBushouID].work = 0; // �d���N����0�Ƃ���B

					nb6bushouref[iBushouID].rise = 0xFFFF; // ��������Ȃ��Ƃ���B
					nb6bushouref[iBushouID].bRise = 0; // �����Ȃ��B

					nb6bushouref[iBushouID].soldier = 0; // ������0�Ƃ���B
					nb6bushouref[iBushouID].training = 0; // �P����0�Ƃ���B
					nb6bushouref[iBushouID].solloyal = 0; // ������0�Ƃ���B
					nb6bushouref[iBushouID].form = 0; // ���y���Ƃ���B

					// �M�����Z�b�g���Ȃ�
					if ( TSModCommand::AllBushou::cmdNonMeritoriousReset ) {
						int merits = nb6bushouref[iBushouID].merits; // �M���l�𕜌��B0.7�{
						nb6bushouref[iBushouID].merits = (WORD)(0.7 * merits);
					} else {
						nb6bushouref[iBushouID].merits = 0; // �M����0�Ƃ���B
					}
				}

				/* �@����
				      ��
				   ��-��-��-��

				   �Ƃ����̂�

				   ��-��-��

				   �Ƃ���B
				*/


				// �Q�l�ɂ����������́A���܏�傾
				if ( nb6castle[iCastleID].master-1 == iBushouID ) {
					nb6castle[iCastleID].master =  nb6bushouref[iBushouID].next; // ������next�����ɂ���B
				}

				// ������prev�͗L���ȕ������B
				if ( 0 < nb6bushouref[iBushouID].prev && nb6bushouref[iBushouID].prev <= GAMEDATASTRUCT_BUSHOU_NUM ) {

					// ���������ł͂Ȃ������ŁA
					// �P�O�̐l�̎��̃����N�悪�������g�̏ꍇ
					if ( nb6bushouref[iBushouID].prev != 0xFFFF && nb6bushouref[(nb6bushouref[iBushouID].prev-1)].next == iBushouID+1 ) {
						// �����ł͂Ȃ��A���̐l�ւƌq��������B
						nb6bushouref[(nb6bushouref[iBushouID].prev-1)].next = nb6bushouref[iBushouID].next;
					}
				}

				// ������next�͗L���ȕ������B
				if ( 0 < nb6bushouref[iBushouID].next && nb6bushouref[iBushouID].next <= GAMEDATASTRUCT_BUSHOU_NUM ) {
					// �P��̐l�̑O�̃����N�悪�������g�̏ꍇ
					if ( nb6bushouref[iBushouID].next != 0xFFFF && nb6bushouref[(nb6bushouref[iBushouID].next-1)].prev == iBushouID+1 ) {
						// �����ł͂Ȃ��A�O�̐l�ւƌq��������B
						nb6bushouref[(nb6bushouref[iBushouID].next-1)].prev = nb6bushouref[iBushouID].prev;
					}
				}


				/* �@
				     
				   ��-��

				   �Ƃ����̂�

				   ��-��-��

				   �Ƃ���B
				*/

				// �Q�l�Ƃ��Ă݂Ȃ������A������next�́A���ݘQ�l���X�g�̐擪�̐l
				nb6bushouref[iBushouID].next = nb6castle[iCastleID].ronin; // ���݂̐擪�̐l�������̎��̐l��

				// next���L���ȕ����Ȃ�΁c
				if ( 0 < nb6castle[iCastleID].ronin && nb6castle[iCastleID].ronin <= GAMEDATASTRUCT_BUSHOU_NUM ) {
					// ���̐l��prev��������
					nb6bushouref[ nb6castle[iCastleID].ronin-1].prev = iBushouID+1;

				}
				nb6bushouref[iBushouID].prev = 0xFFFF; // prev�͋��Ȃ�

				nb6castle[iCastleID].ronin = iBushouID+1; // ��̘Q�l�̐擪�������ɂ���B

				return true;

			}
		}
	}

	return false;
}


// State �Q�l �� State ������
bool setRouninToGeneki(int iBushouID) {
	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
		if ( nb6bushouname[iBushouID].State == 4 ) { // �Q�l�����̏ꍇ��������������B
			// �Y�������̋��邪�����Ƃ�����񂪊i�[����Ă���B(���[�U�[�������ɂ���ĉ��Ă��Ȃ�)
			int iCastleID = nb6bushouref[iBushouID].castle-1;
			if (0 <= iCastleID && iCastleID < GAMEDATASTRUCT_CASTLE_NUM ) {

				if ( nb6castle[iCastleID].ronin == 0xFFFF ) { return false; } // �����N���X�g�����Ă���̂��Q�l���N�����Ȃ����ƂɂȂ�������Ă�B�������Ȃ��Breturn;
				if ( nb6castle[iCastleID].attach == 0xFFFF ) { return false; } // �Y���̏邪�ǂ��̌R�c�ɂ��������Ă��Ȃ��ꍇ�A�Q�l�������Ƃ͏o���Ȃ��B
																		       // �Ȃ��Ȃ�A����͑喼�����邱�Ƃ��Ӗ����邩��B
				int iAttachGundanID = nb6castle[iCastleID].attach-1;
				int iAttachDaimyoID = -1;
				if ( 0 <= iAttachGundanID && iAttachGundanID < GAMEDATASTRUCT_GUNDAN_NUM ) {
					iAttachDaimyoID = nb6gundan[iAttachGundanID].attach - 1;
				} else {
					return false;
				}
				if ( ! (0 <= iAttachDaimyoID && iAttachDaimyoID < GAMEDATASTRUCT_DAIMYO_NUM ) ) {
					return false;
				}

				// ���݂̏�̏�傩��A�����N���X�g��H���Ă���
				int iLinkEndBusyouID = nb6castle[iCastleID].master;
				int iSafetyCnt = 0;
				while ( true ) {
					// �Z�[�t�e�B�J�E���g���\�����Ă���B
					if ( iSafetyCnt > GAMEDATASTRUCT_BUSHOU_NUM ) { 
						return false;
					}

					// ��傪���Ȃ��Ȃ�΁A�I���
					if ( iLinkEndBusyouID == 0xFFFF ) {
						break;
					}

					// ���݂̕����́Anext���L���ł͂Ȃ���΁A���݂̕������Ō�̐l�B�I���B
					if ( nb6bushouref[iLinkEndBusyouID-1].next == 0xFFFF ) {
						break;

					// ���݂̐l���L���ł���B
					} else {
						// �����N���X�g�����̐l�Ƃ���B
						if ( 0 < iLinkEndBusyouID && iLinkEndBusyouID <= GAMEDATASTRUCT_BUSHOU_NUM ) {
							iLinkEndBusyouID = nb6bushouref[iLinkEndBusyouID-1].next;
						}
					}
				}

				// ��������̒l�ύX
				{
					nb6bushouname[iBushouID].State = 2; // ��Ԃ������Ƃ���B
						
					nb6bushouref[iBushouID].attach = iAttachDaimyoID+1; // �喼�ɏ�������B
					nb6bushouref[iBushouID].gundan = iAttachGundanID+1; // �R�c�ɏ�������B
					nb6bushouref[iBushouID].loyal = 50; // ������50�Ƃ���B

					nb6bushouref[iBushouID].rise = 0xFFFF; // ��������Ȃ��Ƃ���B
					nb6bushouref[iBushouID].bRise = 0; // �����Ȃ��B

					nb6bushouref[iBushouID].soldier = 0; // ������0�Ƃ���B
					nb6bushouref[iBushouID].training = 0; // �P����0�Ƃ���B
					nb6bushouref[iBushouID].solloyal = 0; // ������0�Ƃ���B
					nb6bushouref[iBushouID].form = 0; // ���y���Ƃ���B

					//------------------------------------------------------
					// �M�����Z�b�g���Ȃ����[�h���ƁA�c��M���ɍ��킹�ď��i
					if ( TSModCommand::AllBushou::cmdNonMeritoriousReset ) {

						if ( nb6bushouref[iBushouID].merits >= nb6requiredmerits[0].merits ) { // �h�V���M�������B
							nb6bushouname[iBushouID].position = 2; // �h�V��
						} else if ( nb6bushouref[iBushouID].merits >= nb6requiredmerits[1].merits ) { // �ƘV���M�������B
							nb6bushouname[iBushouID].position = 3; // �ƘV��
						} else if ( nb6bushouref[iBushouID].merits >= nb6requiredmerits[2].merits ) { // �������M�������B
							nb6bushouname[iBushouID].position = 4; // ������
						} else if ( nb6bushouref[iBushouID].merits >= nb6requiredmerits[3].merits ) { // ���叫���M�������B
							nb6bushouname[iBushouID].position = 5; // ���叫��
						}
					// �f�t�H���g���ƌM���̓��Z�b�g
					} else {
						nb6bushouname[iBushouID].position = 6; // �g���𑫌y�Ƃ���B
					}
					//------------------------------------------------------
				}

				/* �@����
				      ��
				   ��-��-��-��

				   �Ƃ����̂�

				   ��-��-��

				   �Ƃ���B
				*/


				// �����ɂ����������́A���ܐ擪��
				if ( nb6castle[iCastleID].ronin-1 == iBushouID ) {
					nb6castle[iCastleID].ronin =  nb6bushouref[iBushouID].next; // ������next�����ɂ���B
				}

				// ������prev�͗L���ȕ������B
				if ( 0 < nb6bushouref[iBushouID].prev && nb6bushouref[iBushouID].prev <= GAMEDATASTRUCT_BUSHOU_NUM ) {

					// �������擪�ł͂Ȃ������ŁA
					// �P�O�̐l�̎��̃����N�悪�������g�̏ꍇ
					if ( nb6bushouref[iBushouID].prev != 0xFFFF && nb6bushouref[(nb6bushouref[iBushouID].prev-1)].next == iBushouID+1 ) {
						// �����ł͂Ȃ��A���̐l�ւƌq��������B
						nb6bushouref[(nb6bushouref[iBushouID].prev-1)].next = nb6bushouref[iBushouID].next;
					}
				}

				// ������next�͗L���ȕ������B
				if ( 0 < nb6bushouref[iBushouID].next && nb6bushouref[iBushouID].next <= GAMEDATASTRUCT_BUSHOU_NUM ) {
					// �P��̐l�̑O�̃����N�悪�������g�̏ꍇ
					if ( nb6bushouref[iBushouID].next != 0xFFFF && nb6bushouref[(nb6bushouref[iBushouID].next-1)].prev == iBushouID+1 ) {
						// �����ł͂Ȃ��A�O�̐l�ւƌq��������B
						nb6bushouref[(nb6bushouref[iBushouID].next-1)].prev = nb6bushouref[iBushouID].prev;
					}
				}


				/* �@
				     
				   ��-��

				   �Ƃ����̂�

				   ��-��-��

				   �Ƃ���B
				*/


				// �����͌����Ƃ��Ĉ�ԍŌ�ɂ������B������next�͋��Ȃ�
				nb6bushouref[iBushouID].next = 0xFFFF;

				// ���������́A�����ɂȂ�ƂƂ��ɁA���ɂ��Ȃ�ׂ��Ȃ�΁c
				if ( iLinkEndBusyouID == 0xFFFF ) {
					nb6bushouref[iBushouID].prev = 0xFFFF; // �����̑O�̐l�͋��Ȃ�

					nb6castle[iCastleID].master = iBushouID+1; // ���͎���

				// ���������N���X�g�̈�ԍŌ�ɂԂ牺����ׂ��B
				} else {

					nb6bushouref[iLinkEndBusyouID-1].next = iBushouID+1; // �����N���X�g�̍Ō��next��������

					nb6bushouref[iBushouID].prev = iLinkEndBusyouID; // �����̑O�̐l�́A��̃����N���X�g�̍Ō�̐l��
				}
				return true;

			}
		}
	}

	return false;
}


bool _setBushouTransportRonin(int iBushouID, int iNewCastleID) {
	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
		if ( nb6bushouname[iBushouID].State == 4 ) { // �Q�l�����̏ꍇ��������������B
			// �Y�������̋��邪�����Ƃ�����񂪊i�[����Ă���B(���[�U�[�������ɂ���ĉ��Ă��Ȃ�)
			int iCastleID = nb6bushouref[iBushouID].castle-1;

			if ( iCastleID == iNewCastleID ) { return true; } // �����z���悪�����������牽������K�v�͂Ȃ����������Ƃ݂Ȃ��B

			if (0 <= iCastleID && iCastleID < GAMEDATASTRUCT_CASTLE_NUM ) {

				if ( nb6castle[iCastleID].ronin == 0xFFFF ) { return false; } // �����N���X�g�����Ă���̂��Q�l���N�����Ȃ����ƂɂȂ�������Ă�B�������Ȃ��Breturn;

				// ��������̒l�ύX
				{
					// ���������]����ɕύX����B
					nb6bushouref[iBushouID].castle = iNewCastleID + 1;				
				}

				/* �@����
				      ��
				   ��-��-��-��

				   �Ƃ����̂�

				   ��-��-��

				   �Ƃ���B
				*/


				// �]�������������́A���ܐ擪��
				if ( nb6castle[iCastleID].ronin-1 == iBushouID ) {
					nb6castle[iCastleID].ronin =  nb6bushouref[iBushouID].next; // ������next�����ɂ���B
				}

				// ������prev�͗L���ȕ������B
				if ( 0 < nb6bushouref[iBushouID].prev && nb6bushouref[iBushouID].prev <= GAMEDATASTRUCT_BUSHOU_NUM ) {

					// �������擪�ł͂Ȃ������ŁA
					// �P�O�̐l�̎��̃����N�悪�������g�̏ꍇ
					if ( nb6bushouref[iBushouID].prev != 0xFFFF && nb6bushouref[(nb6bushouref[iBushouID].prev-1)].next == iBushouID+1 ) {
						// �����ł͂Ȃ��A���̐l�ւƌq��������B
						nb6bushouref[(nb6bushouref[iBushouID].prev-1)].next = nb6bushouref[iBushouID].next;
					}
				}

				// ������next�͗L���ȕ������B
				if ( 0 < nb6bushouref[iBushouID].next && nb6bushouref[iBushouID].next <= GAMEDATASTRUCT_BUSHOU_NUM ) {
					// �P��̐l�̑O�̃����N�悪�������g�̏ꍇ
					if ( nb6bushouref[iBushouID].next != 0xFFFF && nb6bushouref[(nb6bushouref[iBushouID].next-1)].prev == iBushouID+1 ) {
						// �����ł͂Ȃ��A�O�̐l�ւƌq��������B
						nb6bushouref[(nb6bushouref[iBushouID].next-1)].prev = nb6bushouref[iBushouID].prev;
					}
				}

				/* �@
				     
				   ��-��

				   �Ƃ����̂�

				   ��-��-��

				   �Ƃ���B
				*/

				// �Q�l�Ƃ��Ă݂Ȃ������A������next�́A���ݘQ�l���X�g�̐擪�̐l
				nb6bushouref[iBushouID].next = nb6castle[iNewCastleID].ronin; // �]����̌��݂̐擪�̐l�������̎��̐l��

				// next���L���ȕ����Ȃ�΁c
				if ( 0 < nb6castle[iNewCastleID].ronin && nb6castle[iNewCastleID].ronin <= GAMEDATASTRUCT_BUSHOU_NUM ) {
					// ���̐l��prev��������
					nb6bushouref[ nb6castle[iNewCastleID].ronin-1].prev = iBushouID+1;
				}
				nb6bushouref[iBushouID].prev = 0xFFFF; // prev�͋��Ȃ�

				nb6castle[iNewCastleID].ronin = iBushouID+1; // ��̘Q�l�̐擪�������ɂ���B

				return true;
			}
		}
	}

	return false;
}


bool setBushouTransport(int iBushouID, int iNewCastleID) {

	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// ���̋���ƐV��������
		int iCastleID = nb6bushouref[iBushouID].castle-1;
		if ( iCastleID == iNewCastleID ) { return true; } // ������ꏊ�ƁA�]���悪�����Ȃ牽������K�v�͂Ȃ��B

		// �Q�l�̏ꍇ�͈ړ]���邾��
		if ( nb6bushouname[iBushouID].State == 4 ) { // �Q�l�����̏ꍇ��������������B
			return _setBushouTransportRonin(iBushouID, iNewCastleID);

		// �����̏ꍇ�́A�Q�l�����Ă���A�ړ]���āA�܂�(�\�ł����)�����ɖ߂��B
		// �������邾�ƘQ�l�̂܂܈ړ]���邱�ƂƂȂ�B
		} else if ( nb6bushouname[iBushouID].State == 2 ) { // ���𕐏��ł���
			int _merits = nb6bushouref[iBushouID].merits;
			int _work = nb6bushouref[iBushouID].work;
			int _position = nb6bushouname[iBushouID].position;
			int _loyal = nb6bushouref[iBushouID].loyal;
			int _soldier = nb6bushouref[iBushouID].soldier;
			int _training = nb6bushouref[iBushouID].training;
			int _solloyal = nb6bushouref[iBushouID].solloyal;
			int _form = nb6bushouref[iBushouID].form;
			int _attach = nb6bushouref[iBushouID].attach;
			int _rise = nb6bushouref[iBushouID].rise;
			int _bRise = nb6bushouref[iBushouID].bRise;

			// ��[�A�Q�l�ɂ���
			bool isRouninka = setGenekiToRounin(iBushouID);
			if (! isRouninka) { return false; }

			// �ʒu�ړ�
			bool isTranska = _setBushouTransportRonin(iBushouID, iNewCastleID);
			if (! isTranska) { return false; }

			bool isGenekika = setRouninToGeneki(iBushouID);
			// �����ɂł��Ȃ������炻�̂܂ܘQ�l�Ƃ��ďI���B(���x�z��Ƃ����ƁA���̂܂ܘQ�l�̂܂܂ƂȂ�B)
			if (! isGenekika) { return false; }

			// ���X�����Ȃ̂ŁA�Q�l������O�̒l�ŕ���������̂�߂��B
			nb6bushouref[iBushouID].merits = _merits; // �M��
			nb6bushouname[iBushouID].position = _position; // �g��
			nb6bushouref[iBushouID].loyal = _loyal; // ����
			nb6bushouref[iBushouID].soldier = _soldier; // ����
			nb6bushouref[iBushouID].training = _training; // �P��
			nb6bushouref[iBushouID].solloyal = _solloyal; // �m�C
			nb6bushouref[iBushouID].form = _form; // ����

			// �]���O�Ɠ]����ő喼���ω����Ă��Ȃ��ꍇ�̂݁A�m���N��������n�����̒l�𕜌�
			if (_attach == nb6bushouref[iBushouID].attach) {
				nb6bushouref[iBushouID].work = _work; // �d���N��
				nb6bushouref[iBushouID].rise = _rise; // ������
				nb6bushouref[iBushouID].bRise = _bRise; // �������
			} else {
				nb6bushouref[iBushouID].grudge = 0; // �喼���ς�����ꍇ�A���݂͏���
			}

		}
	}
	return false;
}


// ���������̏�̏��Ƃ���B
// ����́u�����v�̕������A���̏�̏��ɂ���B
// ���̏�傪�u�R�c���v��u�喼�v���ƁA���̐l���̗���ƂȂ�B
bool setGenekiToJyosyu(int iBushouID) {

	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		//0�喼,1�R��,2����
		if ( nb6bushouname[iBushouID].State == 2 ) { // ���𕐏��̏ꍇ��������������B

			// �Y�������̋��邪�����Ƃ�����񂪊i�[����Ă���B(���[�U�[�������ɂ���ĉ��Ă��Ȃ�)
			int iCastleID = nb6bushouref[iBushouID].castle-1;

			if (0 <= iCastleID && iCastleID < GAMEDATASTRUCT_CASTLE_NUM ) {

				if ( nb6castle[iCastleID].master == 0xFFFF ) { return false; } // �����N���X�g�����Ă���̂���ɒN���������Ă��Ȃ��B�������Ȃ��Breturn;

				// �����܂ŗ������傪����̂Łc
				int iJyosyuBushouID = nb6castle[iCastleID].master-1;

				// �w�蕐�������łɏ��Ȃ牽�����Ȃ��Ă����ł���B
				if ( iBushouID== iJyosyuBushouID ) { return true; }

				// ���͋��邪(��������)�������Ȕj����f�[�^�ɂȂ��Ă�����A�������Ȃ��B
				if ( ! (0 <= iJyosyuBushouID && iJyosyuBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) ) {
					return false;
				}

				//0�喼,1�R��,2����

				// ��傪�����ł��R�c���ł��Ȃ��Ȃ��(�喼)�A�������Ȃ��B����ւ���ƕςȂ��ƂɂȂ�B
				// �����Ȃ��
				if ( nb6bushouname[iJyosyuBushouID].State == 2 ) {	

					; // ���ɉ������Ȃ�
				// �R�c���Ȃ��
				} else if ( nb6bushouname[iJyosyuBushouID].State == 1 ) {
					// ���̌R�c���������Ƃ���
					nb6bushouname[iJyosyuBushouID].State = 2; // �����Ƃ���
					
					// �t�Ɏw�蕐�����R�c��������
					nb6bushouname[iBushouID].State = 1; // �R�c��
					int iGundanID = nb6bushouref[iJyosyuBushouID].gundan-1;
					// �����ėv��ʔ���̂͂��ł��邪�ꉞ
					if ( 0 <= iGundanID && iGundanID < GAMEDATASTRUCT_GUNDAN_NUM ) {
						// �R�c���̌R�c���̔ԍ����A�w��̕����ɂ���B
						nb6gundan[iGundanID].leader = iBushouID + 1;
					}
				// �喼�Ȃ�΁c
				} else if ( nb6bushouname[iJyosyuBushouID].State == 0 ) {
					// ���̑喼�������Ƃ���
					nb6bushouname[iJyosyuBushouID].State = 2;
					// �喼�������l�̐g�����h�V�Ƃ���B
					nb6bushouname[iJyosyuBushouID].position = 2; //(�B��=0, �喼=1, �h�V=2, �ƘV=3, ����=4, ���叫=5, ���y��=6)
					// �t�Ɏw�蕐����喼�Ƃ���
					nb6bushouname[iBushouID].State = 0;
					nb6bushouname[iBushouID].position = 1;

					int iGundanID = nb6bushouref[iJyosyuBushouID].gundan-1;
					int iDaimyoID = nb6bushouref[iJyosyuBushouID].attach-1;
					// �����ėv��ʔ���̂͂��ł��邪�ꉞ
					if ( 0 <= iGundanID && iGundanID < GAMEDATASTRUCT_GUNDAN_NUM ) {
						// �R�c���̌R�c���̔ԍ����A�w��̕����ɂ���B
						nb6gundan[iGundanID].leader = iBushouID + 1;
					}
					if ( 0 <= iDaimyoID && iDaimyoID < GAMEDATASTRUCT_DAIMYO_NUM ) {
						// nb6daimyo�̑喼�̔ԍ����A�w��̕����ɂ���B
						nb6daimyo[iDaimyoID].attach = iBushouID + 1;
					}

				} else {
					return false;
				}




				/*����̕������X�g

				�@ �@����
				   ��
				   ��-��-��-��

				   �Ƃ����̂�

				   ��-��-��

				   �Ƃ���B
				   */


				// �p�[�W�����������́A���܏�傾
				if ( nb6castle[iCastleID].master-1 == iBushouID ) {
					nb6castle[iCastleID].master =  nb6bushouref[iBushouID].next; // ������next�����ɂ���B
				}

				// ������prev�͗L���ȕ������B
				if ( 0 < nb6bushouref[iBushouID].prev && nb6bushouref[iBushouID].prev <= GAMEDATASTRUCT_BUSHOU_NUM ) {

					// ���������ł͂Ȃ������ŁA
					// �P�O�̐l�̎��̃����N�悪�������g�̏ꍇ
					if ( nb6bushouref[iBushouID].prev != 0xFFFF && nb6bushouref[(nb6bushouref[iBushouID].prev-1)].next == iBushouID+1 ) {
						// �����ł͂Ȃ��A���̐l�ւƌq��������B
						nb6bushouref[(nb6bushouref[iBushouID].prev-1)].next = nb6bushouref[iBushouID].next;
					}
				}

				// ������next�͗L���ȕ������B
				if ( 0 < nb6bushouref[iBushouID].next && nb6bushouref[iBushouID].next <= GAMEDATASTRUCT_BUSHOU_NUM ) {
					// �P��̐l�̑O�̃����N�悪�������g�̏ꍇ
					if ( nb6bushouref[iBushouID].next != 0xFFFF && nb6bushouref[(nb6bushouref[iBushouID].next-1)].prev == iBushouID+1 ) {
						// �����ł͂Ȃ��A�O�̐l�ւƌq��������B
						nb6bushouref[(nb6bushouref[iBushouID].next-1)].prev = nb6bushouref[iBushouID].prev;
					}
				}



				/* ����̏�僊�X�g�@

				��-��

				�Ƃ����̂�

				��-��-��

				�Ƃ���B
				*/

				// ���������ƂȂ�̂ŁA�����̎��̐l���A���̏��̐l�Ƃ���B
				int iJosyu = nb6castle[iCastleID].master;
				nb6bushouref[iBushouID].next = iJosyu; // ���݂̐擪�̐l�������̎��̐l��

				// next���L���ȕ����Ȃ�΁c
				if ( 0 < iJosyu && iJosyu <= GAMEDATASTRUCT_BUSHOU_NUM ) {
					// ���̐l��prev��������
					nb6bushouref[iJosyu-1].prev = iBushouID+1;
				}
				
				nb6bushouref[iBushouID].prev = 0xFFFF; // �{�l���擪�ɂȂ�̂ŁAprev�͋��Ȃ�

				// �����čŌ�Ɏ���������
				nb6castle[iCastleID].master = iBushouID+1; // ��̏��������ɂ���B

				return true;
			}
		}
	}

	return false;
}



// bfile.nb6�ɑ��݂����`�̐�
int iExistRetudenNumOfBFileN6P = -1;



// �Ώۂ̕���ID�����ݓS�b�D��Ԃ��ǂ���(�S�b�D�������Ă��邩�ǂ����ł͂Ȃ��A���ɂ����āA���S�b�D�̌`�󂩂ǂ���)
int iCurTekkousenForm[GAMEDATASTRUCT_BUSHOU_NUM] = {-1};


int getTouzyouBushouNum() {
	WORD *pTouzyouBushouNum = (WORD *)GAMEDATASTRUCT_TOUZYOU_BUSHOU_NUM_ADDRESS;
	return *pTouzyouBushouNum;
}

int getTaikiBushouDataNum() {
	WORD *pMitouzyouBushouNum = (WORD *)GAMEDATASTRUCT_MITOUZYOU_BUSHOU_NUM_ADDRESS;
	return *pMitouzyouBushouNum;
}



// ���[�h���Ȃ��ł���Ă�o�ߋG�߁B���G�߂̕ς��ڂŉ��Z����Ă䂭�B
int nb6progressseason = 1L;


void setCastleName(int iCastleID, string name) {
	if (0 <= iCastleID && iCastleID < GAMEDATASTRUCT_CASTLE_NUM) {

		strncpy_s(nb6castle[iCastleID].name, name.c_str(), 8);
		nb6castle[iCastleID].name[8] = NULL;
		nb6castle[iCastleID].nameEdited = true;
	}
}


// ����ɕ���������߂��������ɁATENSHOU.EXE���̃|�C���^�z�񂩂��������悤�ɂ��������A���҂̉����ւ̑ϋv�͂����������B
char nb6CastleTypeName[GAMEDATASTRUCT_CASTLETYPE_NUM][5] = {
	"��",
	"��V",
	"��",
	"�䏊",
	"��",
	"",
	"��"
};

// ��̃^�C�vID�����̓I�ȕ�����𓾂�
char *getCastleTypeName(int iCastleType) {
	if ( iCastleType == 0x00 ) {
		return nb6CastleTypeName[0];
	}
	else if (iCastleType == 0x01 ) {
		return nb6CastleTypeName[1];
	}
	else if (iCastleType == 0x02 ) {
		return nb6CastleTypeName[2];
	}
	else if (iCastleType == 0x04 ) {
		return nb6CastleTypeName[3];
	}
	else if (iCastleType == 0x08 ) {
		return nb6CastleTypeName[4];
	}
	else if (iCastleType == 0x10 ) {
		return nb6CastleTypeName[5];
	}
	else if (iCastleType == 0x20 ) {
		return nb6CastleTypeName[6];
	}
	return "";
}


// �w��̌R�c���������Ă����̐��B
int getGundanHasCastleNum(int iGundanID) {
	int iHasCastleCnt = 0;
	if ( 0 <= iGundanID && iGundanID < GAMEDATASTRUCT_GUNDAN_NUM ) {
		for ( int iCastleID=0; iCastleID < GAMEDATASTRUCT_CASTLE_NUM; iCastleID++ ) {
			// �R�c�ԍ�����v������̂��J�E���g
			if ( iGundanID == nb6castle[iCastleID].attach-1 ) {
				iHasCastleCnt++;
			}
		}
	}
	return iHasCastleCnt;
}

// �w��̑喼���������Ă����̐��B
int getDaimyoHasCastleNum(int iDaimyoID) {
	int iHasCastleCnt = 0;
	
	if ( 0 <= iDaimyoID && iDaimyoID < GAMEDATASTRUCT_DAIMYO_NUM ) {
		for ( int iCastleID=0; iCastleID < GAMEDATASTRUCT_CASTLE_NUM; iCastleID++ ) {
			int iGundanID = nb6castle[iCastleID].attach-1;

			if ( 0 <= iGundanID && iGundanID < GAMEDATASTRUCT_GUNDAN_NUM ) {
				// �喼�ԍ�����v������̂��J�E���g
				if ( iDaimyoID == nb6gundan[iGundanID].attach-1 ) {
					iHasCastleCnt++;
				}
			}
		}
	}
	return iHasCastleCnt;
}

// �w��̑喼���������Ă����̔ԍ��y�z��p�z�̃��X�g
vector<int> getDaimyoHasCastleList(int iDaimyoID) {
	vector<int> castlelist;

	if (0 <= iDaimyoID && iDaimyoID < GAMEDATASTRUCT_DAIMYO_NUM) {
		for (int iCastleID = 0; iCastleID < GAMEDATASTRUCT_CASTLE_NUM; iCastleID++) {
			int iGundanID = nb6castle[iCastleID].attach - 1;

			if (0 <= iGundanID && iGundanID < GAMEDATASTRUCT_GUNDAN_NUM) {
				// �喼�ԍ�����v������̂��J�E���g
				if (iDaimyoID == nb6gundan[iGundanID].attach - 1) {
					castlelist.push_back(iCastleID);
				}
			}
		}
	}
	return castlelist;
}



// ��펞�̃w�b�N�X�n�̍��W���A�z��Ƃ��Ă̎��l��index�ւƕύX����B
void ConvertFieldHexPosToFieldIndexPos(double x, double y, int *out_x, int *out_y) {

	// ���W�l�͑Ó��Ȕ͈͂��H
	if ( 0<=x && x< GAMEDATASTRUCT_FIELD_HEX_COLS && 0<=y && y< GAMEDATASTRUCT_FIELD_HEX_ROWS) {
		// y�����W���l�ɂ��āA������
		int _y = round_������(y);
		// ���̋����s�ڂ���s�ڂ��ŁA���g���炷�B�����s�Ȃ�A"��HEX"�����l��x�ɓ����Ă��邽�߁B
		if (_y % 2==1) {
			x = x - 0.5;
		}
		// x���ł��߂��l�̐�����
		int _x = round_������(x);
		// �O��菬�����Ȃ邱�Ƃ�����̂łO��
		if ( _x < 0 ) { _x = 0; }
		if ( _x >= GAMEDATASTRUCT_FIELD_HEX_COLS ) { _x = GAMEDATASTRUCT_FIELD_HEX_COLS-1; }
		if ( _y >= GAMEDATASTRUCT_FIELD_HEX_ROWS ) { _y = GAMEDATASTRUCT_FIELD_HEX_ROWS-1; }

		// �z��A�N�Z�X�l�Ƃ��Ă�x��y���o��
		*out_x = _x;
		*out_y = _y;
	} else {
		*out_x = 0xFFFF;
		*out_y = 0xFFFF;
	}
}


// �U��펞�̃w�b�N�X�n�̍��W���A�z��Ƃ��Ă̎��l��index�ւƕύX����B
void ConvertCastleHexPosToCastleIndexPos(double x, double y, int *out_x, int *out_y) {

	// ���W�l�͑Ó��Ȕ͈͂��H
	if ( 0<=x && x< GAMEDATASTRUCT_CASTLE_HEX_COLS && 0<=y && y< GAMEDATASTRUCT_CASTLE_HEX_ROWS) {
		// y�����W���l�ɂ��āA������
		int _y = round_������(y);
		// ���̋����s�ڂ���s�ڂ��ŁA���g���炷�B�����s�Ȃ�A"��HEX"�����l��x�ɓ����Ă��邽�߁B
		if (_y % 2==1) {
			x = x - 0.5;
		}
		// x���ł��߂��l�̐�����
		int _x = round_������(x);
		// �O��菬�����Ȃ邱�Ƃ�����̂łO��
		if ( _x < 0 ) { _x = 0; }
		if ( _x >= GAMEDATASTRUCT_CASTLE_HEX_COLS ) { _x = GAMEDATASTRUCT_CASTLE_HEX_COLS-1; }
		if ( _y >= GAMEDATASTRUCT_CASTLE_HEX_ROWS ) { _y = GAMEDATASTRUCT_CASTLE_HEX_ROWS-1; }

		// �z��A�N�Z�X�l�Ƃ��Ă�x��y���o��
		*out_x = _x;
		*out_y = _y;
	} else {
		*out_x = 0xFFFF;
		*out_y = 0xFFFF;
	}
}

// ����w�b�N�X�ꏊ�̃`�b�v�̖���(FIELD_HEX_ROLE)�𓾂�
int GetFieldHexRole(double x, double y) {
	// ���W�l�͑Ó��Ȕ͈͂��H
	if ( 0<=x && x<= GAMEDATASTRUCT_FIELD_HEX_COLS && 0<=y && y<=GAMEDATASTRUCT_FIELD_HEX_ROWS) {
		// y�����W���l�ɂ��āA������
		int _y = round_������(y);
		// ���̋����s�ڂ���s�ڂ��ŁA���g���炷�B�����s�Ȃ�A"��HEX"�����l��x�ɓ����Ă��邽�߁B
		if (_y % 2==1) {
			x = x - 0.5;
		}
		// x���ł��߂��l�̐�����
		int _x = round_������(x);
		// �O��菬�����Ȃ邱�Ƃ�����̂łO��
		if ( _x < 0 ) { _x = 0; }
		if ( _x >= GAMEDATASTRUCT_FIELD_HEX_COLS ) { _x = GAMEDATASTRUCT_FIELD_HEX_COLS-1; }
		if ( _y >= GAMEDATASTRUCT_FIELD_HEX_ROWS ) { _y = GAMEDATASTRUCT_FIELD_HEX_ROWS-1; }
		// �Y���̏ꏊ�ɂ���`�b�v�̖����ԍ�(FIELD_HEX_ROLE)��Ԃ�
		return nb6field_hex_role.pos_y[_y].pos_x[_x];
	} else {
		return -1;
	}
}

int GetFieldHexRole(HEX_POSITION p) {
	return GetFieldHexRole(p.x, p.y);
}

// �w��̖��̖����ƍ��v����A�ʒu���X�g�𓾂�
// role �ɂ� FIELD_HEX_ROLE::* �̒l���w�肷��
vector<HEX_POSITION> GetFieldHexPosOfHexRole(int role) {
	vector<HEX_POSITION> vec;
	for (int x = 0; x < GAMEDATASTRUCT_FIELD_HEX_COLS; x++) {
		for (int y = 0; y < GAMEDATASTRUCT_FIELD_HEX_ROWS; y++) {
			if (nb6field_hex_role.pos_y[y].pos_x[x] == role) {

				HEX_POSITION pos;
				if (y % 2 == 1) {
					pos.x = (double)x + 0.5;
					pos.y = y;
				}
				else {
					pos.x = x;
					pos.y = y;
				}

				vec.push_back(pos);
			}
		}
	}

	return vec;
}



// ����w�b�N�X�ꏊ�̃`�b�v�̖���(CASTLE_HEX_ROLE)�𓾂�
int GetCastleHexRole(double x, double y) {
	// ���W�l�͑Ó��Ȕ͈͂��H
	if ( 0<=x && x< GAMEDATASTRUCT_CASTLE_HEX_COLS && 0<=y && y< GAMEDATASTRUCT_CASTLE_HEX_ROWS) {
		// y�����W���l�ɂ��āA������
		int _y = round_������(y);
		// ���̋����s�ڂ���s�ڂ��ŁA���g���炷�B�����s�Ȃ�A"��HEX"�����l��x�ɓ����Ă��邽�߁B
		if (_y % 2==1) {
			x = x - 0.5;
		}
		// x���ł��߂��l�̐�����
		int _x = round_������(x);
		// �O��菬�����Ȃ邱�Ƃ�����̂łO��
		if ( _x < 0 ) { _x = 0; }
		if ( _x >= GAMEDATASTRUCT_CASTLE_HEX_COLS ) { _x = GAMEDATASTRUCT_CASTLE_HEX_COLS-1; }
		if ( _y >= GAMEDATASTRUCT_CASTLE_HEX_ROWS ) { _y = GAMEDATASTRUCT_CASTLE_HEX_ROWS-1; }
		// �Y���̏ꏊ�ɂ���`�b�v�̖����ԍ�(CASTLE_HEX_ROLE)��Ԃ�
		return nb6castle_hex_role.pos_y[_y].pos_x[_x];
	} else {
		return -1;
	}
}


int GetCastleHexRole(HEX_POSITION p) {
	return GetCastleHexRole(p.x, p.y);
}


// �w��̍U���̖����ƍ��v����A�ʒu���X�g�𓾂�
// role �ɂ� CASTLE_HEX_ROLE::* �̒l���w�肷��
vector<HEX_POSITION> GetCastleHexPosOfHexRole(int role) {
	vector<HEX_POSITION> vec;
	for (int x = 0; x < GAMEDATASTRUCT_CASTLE_HEX_COLS; x++) {
		for (int y = 0; y < GAMEDATASTRUCT_CASTLE_HEX_ROWS; y++) {
			if (nb6castle_hex_role.pos_y[y].pos_x[x] == role) {

				HEX_POSITION pos;
				if (y % 2 == 1) {
					pos.x = (double)x + 0.5;
					pos.y = y;
				}
				else {
					pos.x = x;
					pos.y = y;
				}

				vec.push_back(pos);
			}
		}
	}

	return vec;
}



/* ���ŉE�P�o�C�g���P�̏��͉����t���Ă���
004D4DAA                          10 10 10 10 10 10 10 10          
004D4DBA  10 00 00 00 00 00 00 00 00 10 10 10 11 98 90 10  ........�E
004D4DCA  10 10 10 00 00 00 00 00 00 00 00 10 10 10 10 90  ........�E
004D4DDA  90 90 10 10 10 10 00 00 00 00 00 00 00 10 10 10  ��.......
004D4DEA  10 91 90 10 10 10 10 00 00 00 00 00 00 00 00 00  ��.........
004D4DFA  10 10 10 11 10 10 10 10 10 00 00 00 00 00 00 00  .......
004D4E0A  00 00 10 10 11 11 11 10 10 10 00 00 00 00 00 00  ........
004D4E1A  00 00 00 00 00 10 10 10 11 10 10 10 00 00 00 00  .........
004D4E2A  00 00 00 00 00 00 00 10 10 10 10 10 10 00 00 00  ..........
*/
// ����w�b�N�X�ꏊ�̃`�b�v�̉��̖���(CASTLE_HEX_ROLE)�𓾂�
// �����t���Ă��鏊�́A�E�̂P�o�C�g��1�ɂȂ�B
int GetFireHexRole(double x, double y) {
	// ���W�l�͑Ó��Ȕ͈͂��H
	if ( 0<=x && x< GAMEDATASTRUCT_CASTLE_HEX_COLS && 0<=y && y< GAMEDATASTRUCT_CASTLE_HEX_ROWS) {
		// y�����W���l�ɂ��āA������
		int _y = round_������(y);
		// ���̋����s�ڂ���s�ڂ��ŁA���g���炷�B�����s�Ȃ�A"��HEX"�����l��x�ɓ����Ă��邽�߁B
		if (_y % 2==1) {
			x = x - 0.5;
		}
		// x���ł��߂��l�̐�����
		int _x = round_������(x);
		// �O��菬�����Ȃ邱�Ƃ�����̂łO��
		if ( _x < 0 ) { _x = 0; }
		if ( _x >= GAMEDATASTRUCT_CASTLE_HEX_COLS ) { _x = GAMEDATASTRUCT_CASTLE_HEX_COLS-1; }
		if ( _y >= GAMEDATASTRUCT_CASTLE_HEX_ROWS ) { _y = GAMEDATASTRUCT_CASTLE_HEX_ROWS-1; }
		// �Y���̏ꏊ�ɂ���`�b�v�̖����ԍ�(CASTLE_HEX_ROLE)��Ԃ�
		return (nb6fire_hex_role.pos_y[_y].pos_x[_x] & 0xF) == 1;
	} else {
		return -1;
	}
}


int GetFireHexRole(HEX_POSITION p) {
	return GetFireHexRole(p.x, p.y);
}


// �w��̍U���̉����Ԃƍ��v����A�ʒu���X�g�𓾂�
// role �ɂ� CASTLE_HEX_ROLE::* �̒l���w�肷��B�w�肵�Ȃ���Ή����Ԃ̂��̂��W�߂�
vector<HEX_POSITION> GetFireeHexPosOfHexRole(int role = FIRE_HEX_ROLE::����) {
	vector<HEX_POSITION> vec;
	for (int x = 0; x < GAMEDATASTRUCT_CASTLE_HEX_COLS; x++) {
		for (int y = 0; y < GAMEDATASTRUCT_CASTLE_HEX_ROWS; y++) {
			if ((nb6fire_hex_role.pos_y[y].pos_x[x] & 0xF) == role) {

				HEX_POSITION pos;
				if (y % 2 == 1) {
					pos.x = (double)x + 0.5;
					pos.y = y;
				}
				else {
					pos.x = x;
					pos.y = y;
				}

				vec.push_back(pos);
			}
		}
	}

	return vec;
}



// ����w�b�N�X�ꏊ�̃`�b�v�̍����𓾂�
int GetFieldHexHigh(double x, double y) {
	int role = GetFieldHexRole(x, y);
	if ( role == FIELD_HEX_ROLE::�� || FIELD_HEX_ROLE::�� || FIELD_HEX_ROLE::�C ) {
		return 0;
	} else if ( role == FIELD_HEX_ROLE::��R ) {
		return 2;
	} else if ( role == FIELD_HEX_ROLE::���R ) {
		return 3;
	} else if ( role == FIELD_HEX_ROLE::���R ) {
		return 4;
	} else {
		return 1;
	}
}

int GetFieldHexHigh(HEX_POSITION p) {
	return GetFieldHexHigh(p.x, p.y);
}

// ����w�b�N�X�ꏊ�̃`�b�v�̍����𓾂�
int GetCastleHexHigh(double x, double y) {
	// ���W�l�͑Ó��Ȕ͈͂��H
	if ( 0<=x && x< GAMEDATASTRUCT_CASTLE_HEX_COLS && 0<=y && y< GAMEDATASTRUCT_CASTLE_HEX_ROWS) {
		// y�����W���l�ɂ��āA������
		int _y = round_������(y);
		// ���̋����s�ڂ���s�ڂ��ŁA���g���炷�B�����s�Ȃ�A"��HEX"�����l��x�ɓ����Ă��邽�߁B
		if (_y % 2==1) {
			x = x - 0.5;
		}
		// x���ł��߂��l�̐�����
		int _x = round_������(x);
		// �O��菬�����Ȃ邱�Ƃ�����̂łO��
		if ( _x < 0 ) { _x = 0; }
		if ( _x >= GAMEDATASTRUCT_CASTLE_HEX_COLS ) { _x = GAMEDATASTRUCT_CASTLE_HEX_COLS-1; }
		if ( _y >= GAMEDATASTRUCT_CASTLE_HEX_ROWS ) { _y = GAMEDATASTRUCT_CASTLE_HEX_ROWS-1; }
		// �Y���̏ꏊ�ɂ���`�b�v�̖����ԍ�(CASTLE_HEX_ROLE)��Ԃ�
		return nb6castle_hex_high.pos_y[_y].pos_x[_x];
	} else {
		return -1;
	}
}

int GetCastleHexHigh(HEX_POSITION p) {
	return GetCastleHexHigh(p.x, p.y);
}


// �t�ɐݒ肷��B(�Ƃ����Ă��A�ݒ肵���Ƃ��Ă��A�G���ς��Ȃ��̂ŏ�肭�����Ȃ���)
void SetFieldHexRole(double x, double y, int eHexRole) {
	// ���W�l�͑Ó��Ȕ͈͂��H
	if ( 0<=x && x< GAMEDATASTRUCT_FIELD_HEX_COLS && 0<=y && y< GAMEDATASTRUCT_FIELD_HEX_ROWS) {
		// y�����W���l�ɂ��āA������
		int _y = round_������(y);
		// ���̋����s�ڂ���s�ڂ��ŁA���g���炷�B�����s�Ȃ�A"��HEX"�����l��x�ɓ����Ă��邽�߁B
		if (_y % 2==1) {
			x = x - 0.5;
		}
		// x���ł��߂��l�̐�����
		int _x = round_������(x);
		// �O��菬�����Ȃ邱�Ƃ�����̂łO��
		if ( _x < 0 ) { _x = 0; }
		if ( _x >= GAMEDATASTRUCT_FIELD_HEX_COLS ) { _x = GAMEDATASTRUCT_FIELD_HEX_COLS-1; }
		if ( _y >= GAMEDATASTRUCT_FIELD_HEX_ROWS ) { _y = GAMEDATASTRUCT_FIELD_HEX_ROWS-1; }
		// �Y���̏ꏊ�̃`�b�v�̖����ԍ�(FIELD_HEX_ROLE)��ݒ肷��B
		nb6field_hex_role.pos_y[_y].pos_x[_x] = eHexRole;
	}
}

void SetFieldHexRole(HEX_POSITION p, int eHexRole) {
	SetFieldHexRole(p.x, p.y, eHexRole);
}

// �t�B�[���h�w�b�N�X�̍��W�����ʂ̂R�������W(����f�J���g�E���Ɠ�����)
D3D_POSITION FieldHexPos_To_WorldPos(HEX_POSITION p) {
	D3D_POSITION p_real;
	p_real.x = p.x - GAMEDATASTRUCT_FIELD_HEX_CENTER_X;
	p_real.z = (p.y -GAMEDATASTRUCT_FIELD_HEX_CENTER_Y) * ( sqrt(3.0f) / 2.0f ) * -1; // sin60��
	if ( p_real.z == 0 ) { p_real.z = 0; } // �A�z�݂��������A"-0"�ƂȂ�Ȃ��悤�ɂ���B

	// y�̍��������߂�ɂ������āA��R�Ȃ�1, ���R�Ȃ�2, ���R�Ȃ�3�Ƃ���B
	int iHexRole = GetFieldHexRole( p.x, p.y );

	if ( iHexRole == FIELD_HEX_ROLE::���R ) {
		p_real.y = 3;
	} else if ( iHexRole == FIELD_HEX_ROLE::���R ) {
		p_real.y = 2;
	} else if ( iHexRole == FIELD_HEX_ROLE::��R ) {
		p_real.y = 1;
	} else {
		p_real.y = 0;
	}
	return p_real;
}

// ���ʂ̂R�������W(����f�J���g)���t�B�[���h�w�b�N�X�̍��W��
HEX_POSITION WorldPos_To_FieldHexPos(D3D_POSITION p) {
	HEX_POSITION p_hex;
	p_hex.x = round_������(p.x + GAMEDATASTRUCT_FIELD_HEX_CENTER_X);
	p_hex.y = round_�񕪂̈�z����((2.0f / sqrt(3.0f) ) * (p.z) * -1 + GAMEDATASTRUCT_FIELD_HEX_CENTER_Y);
	return p_hex;
}

// ��w�b�N�X�̍��W�����ʂ̂R�������W(����f�J���g�E���Ɠ�����)
D3D_POSITION CastleHexPos_To_WorldPos(HEX_POSITION p) {
	D3D_POSITION p_real;
	p_real.x = p.x - GAMEDATASTRUCT_CASTLE_HEX_CENTER_X;
	p_real.z = (p.y -GAMEDATASTRUCT_CASTLE_HEX_CENTER_Y) * ( sqrt(3.0f) / 2.0f ) * -1; // sin60��
	if ( p_real.z == 0 ) { p_real.z = 0; } // �A�z�݂��������A"-0"�ƂȂ�Ȃ��悤�ɂ���B

	// y�̍��������߂�ɂ������ď�̍������𓖂Ă͂߂�B
	int iHexHigh = GetCastleHexHigh( p.x, p.y );
	p_real.y = iHexHigh;
	return p_real;
}

// ���ʂ̂R�������W(����f�J���g)����w�b�N�X�̍��W��
HEX_POSITION WorldPos_To_CastleHexPos(D3D_POSITION p) {
	HEX_POSITION p_hex;
	p_hex.x = round_������(p.x + GAMEDATASTRUCT_CASTLE_HEX_CENTER_X);
	p_hex.y = round_�񕪂̈�z����((2.0f / sqrt(3.0f) ) * (p.z) * -1 + GAMEDATASTRUCT_CASTLE_HEX_CENTER_Y);
	return p_hex;
}


// �w�b�N�X�Ԋu�̎擾�Bp1��p2�܂ŉ��e�ړ�����K�v������̂�(�S�Ă̞e�͍s������̂Ɖ��肷��)
int getHexDistance( HEX_POSITION p1, HEX_POSITION p2 ) {
	double ax = double(abs(p1.x-p2.x));
	double ay = double(abs(p1.y-p2.y));
	if ( p1.y == p2.y ) { // �����s�ɋ���Ȃ�΁c
		return round_������(ax);
	} else if ( ax*2 <= ay ) { // 2�_�Ԃ�X���W�̍���2�_�Ԃ�Y���W�̍��̔����ȉ��̏ꍇ�AY���W�̈ړ������ŊY���|�C���g�܂œ��B�ł���B
		return round_������(ay);
	} else {
		// X�̍� + Y�̍� - Y�̍��̔���
		return round_������( ax + ay - 0.5*ay );
	}
}

// ���ݏo�w���Ă���iBushouID�̈ꗗ�𓾂�B
vector<int> GetSyutujinBushouArray() {
	vector<int> bArray;

	// ���ɂ��镐����������𖞂������̂̈ړ��c�ʂ𑝂₷�B
	for ( int i=0; i<GAMEDATASTRUCT_WAR_HEX_UNIT_INFO_NUM; i++ ) {
		if ( nb6bushou_hex_unit_info[i].bushou == 0xFFFF ) { continue; } // 0xFFFF���Ƃ��̃X���b�g�ɕ����͋��Ȃ��B���̐l
		if ( nb6bushou_hex_unit_info[i].bushou > GAMEDATASTRUCT_BUSHOU_NUM ) { continue; } // �����Ŕ͈͕ۏႵ�Ă����Ă�����B���̐l
		
		int iBushouID = nb6bushou_hex_unit_info[i].bushou - 1;

		if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
			// �펀��ދp���Ă��Ȃ�
			if ( nb6bushouname[iBushouID].battleDeath == 0 && nb6bushouname[iBushouID].battleRetreat == 0 ) {
				bArray.push_back( iBushouID );
			}
		}
	}

	return bArray;
}

// ���ݕ\������Ă���}�b�v����ŁA�o�w���Ă���iBushouID�̈ꗗ�𓾂�B
vector<int> GetCurMapSyutujinBushouArray() {
	vector<int> bArray;

	if ( Is_CastleWar() ) {
		for ( int i=0; i<GAMEDATASTRUCT_CASTLE_HEX_COLS * GAMEDATASTRUCT_CASTLE_HEX_ROWS; i++ ) {
			int iBango = nb6castle_hex_unit_pos.index[i];
			if ( 1 <= iBango && iBango <= GAMEDATASTRUCT_WAR_HEX_UNIT_INFO_NUM ) {
				if ( nb6bushou_hex_unit_info[iBango-1].bushou != 0xFFFF ) {
					int iBushouID = nb6bushou_hex_unit_info[iBango-1].bushou-1;

					if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
						// �펀��ދp���Ă��Ȃ�
						if ( nb6bushouname[iBushouID].battleDeath == 0 && nb6bushouname[iBushouID].battleRetreat == 0 ) {
							bArray.push_back( iBushouID );
						}
					}
				}
			}
		}
	}
	if ( Is_FieldWar() ) {
		for ( int i=0; i<GAMEDATASTRUCT_FIELD_HEX_COLS * GAMEDATASTRUCT_FIELD_HEX_ROWS; i++ ) {
			int iBango = nb6field_hex_unit_pos.index[i];
			if ( 1 <= iBango && iBango <= GAMEDATASTRUCT_WAR_HEX_UNIT_INFO_NUM ) {
				if ( nb6bushou_hex_unit_info[iBango-1].bushou != 0xFFFF ) {
					int iBushouID = nb6bushou_hex_unit_info[iBango-1].bushou-1;

					if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
						// �펀��ދp���Ă��Ȃ�
						if ( nb6bushouname[iBushouID].battleDeath == 0 && nb6bushouname[iBushouID].battleRetreat == 0 ) {
							bArray.push_back( iBushouID );
						}
					}
				}
			}
		}
	}
	return bArray;
}


// �w��̕����h�c�̕������ǂ��ɂ���̂��H ���Ȃ��ꍇ�Ap(x=0xFFFF, y=0xFFFF) ���Ԃ��Ă���B
HEX_POSITION GetFieldHexUnitPos(int iBushouID) {

	HEX_POSITION pos( 0xFFFF, 0xFFFF );

	if ( !(0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM) ) {
		return pos;
	}
	if ( !Is_FieldWar() ) {
		return pos;
	}

	// ���ɂ��镐����������𖞂������̂̈ړ��c�ʂ𑝂₷�B
	for ( int i=0; i<GAMEDATASTRUCT_WAR_HEX_UNIT_INFO_NUM; i++ ) {
		if ( nb6bushou_hex_unit_info[i].bushou == 0xFFFF ) { continue; } // 0xFFFF���Ƃ��̃X���b�g�ɕ����͋��Ȃ��B���̐l
		if ( nb6bushou_hex_unit_info[i].bushou > GAMEDATASTRUCT_BUSHOU_NUM ) { continue; } //

		// �T���Ă镐�����A�\�����̖��ɋ���!!
		if ( iBushouID == nb6bushou_hex_unit_info[i].bushou - 1 ) {
			int x = nb6bushou_hex_unit_info[i].pos_x;
			int y = nb6bushou_hex_unit_info[i].pos_y;

			if ( x==0xFFFF ) {
				return pos;
			}

			// y�����W���l�ɂ��āA������
			pos.y = y;

			// ���̋����s�ڂ���s�ڂ��ŁA���g���炷�B�����s�Ȃ�A"���W��0.5���E�ɂ��炷�B
			if (y % 2==1) {
				pos.x = x + 0.5;
			} else {
				pos.x = x;
			}

			return pos;
		}
	}

	return pos;
}




// �w��̕����h�c�̕������ǂ��ɂ���̂��H ���Ȃ��ꍇ�Ap(x=0xFFFF, y=0xFFFF) ���Ԃ��Ă���B
HEX_POSITION GetCastleHexUnitPos(int iBushouID) {


	HEX_POSITION pos( 0xFFFF, 0xFFFF );

	if ( !(0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM) ) {
		return pos;
	}
	if ( !Is_CastleWar() ) {
		return pos;
	}

	// ���ɂ��镐����������𖞂������̂̈ړ��c�ʂ𑝂₷�B
	for ( int i=0; i<GAMEDATASTRUCT_WAR_HEX_UNIT_INFO_NUM; i++ ) {
		if ( nb6bushou_hex_unit_info[i].bushou == 0xFFFF ) { continue; } // 0xFFFF���Ƃ��̃X���b�g�ɕ����͋��Ȃ��B���̐l
		if ( nb6bushou_hex_unit_info[i].bushou > GAMEDATASTRUCT_BUSHOU_NUM ) { continue; } //

		// �T���Ă镐�����A�\�����̍U��퓬�ɋ���!!
		if ( iBushouID == nb6bushou_hex_unit_info[i].bushou - 1 ) {
			int x = nb6bushou_hex_unit_info[i].pos_x;
			int y = nb6bushou_hex_unit_info[i].pos_y;

			if ( x==0xFFFF ) {
				return pos;
			}

			// y�����W���l�ɂ��āA������
			pos.y = y;

			// ���̋����s�ڂ���s�ڂ��ŁA���g���炷�B�����s�Ȃ�A"���W��0.5���E�ɂ��炷�B
			if (y % 2==1) {
				pos.x = x + 0.5;
			} else {
				pos.x = x;
			}

			return pos;
		}
	}

	return pos;
}

HEX_VECTOR operator-(HEX_POSITION &p1, HEX_POSITION &p2) {
	HEX_VECTOR p;
	p.x = p1.x - p2.x;
	p.y = p1.y - p2.y;
	return p;
}


// ���ݐi�s���̐푈�Ɋւ���Ă���iDaimyoID�̃��X�g�B(�o�w���Ă��邩�ǂ����͊֌W�Ȃ�)
vector<int> GetCurWarJointDaimyoIDList() {
	vector<int> list;
	if ( Is_FieldWar() || Is_CastleWar() ) {
		// �Q���喼�ԍ����������āA
		for ( int ix=0; ix < GAMEDATASTRUCT_WAR_JOINT_DAIMYO_HEX_GROUP_INFO_NUM; ix++ ) {

			int iDaimyoID = (int)nb6war_joint_daimyo_hex_group_info[ix].daimyo - 1;
			// �͈͓��ł���΁A���X�g�ɒǉ�
			if ( 0 <= iDaimyoID && iDaimyoID < GAMEDATASTRUCT_DAIMYO_NUM ) {
				// �Y���̒l�����ł�list���ɂ��邩
				vector<int>::iterator iter = std::find( list.begin(), list.end() , iDaimyoID );
				// ���݂��Ȃ���΁c
				if ( iter == list.end() ) {
					// �ǉ�
					list.push_back( iDaimyoID );
				}
			}
		}
	}

	
	return list;
}

// ���ݐi�s���̐푈�Ɋւ���Ă���iGundanID�̃��X�g�B(�o�w���Ă��邩�ǂ����͊֌W�Ȃ�)
vector<int> GetCurWarJointGundanIDList() {
	vector<int> list;
	if ( Is_FieldWar() || Is_CastleWar() ) {
		// �Q���喼�ԍ����������āA
		for ( int ix=0; ix < GAMEDATASTRUCT_WAR_JOINT_DAIMYO_HEX_GROUP_INFO_NUM; ix++ ) {

			int iGundanID = (int)nb6war_joint_daimyo_hex_group_info[ix].gundan - 1;
			// �͈͓��ł���΁A���X�g�ɒǉ�
			if ( 0 <= iGundanID && iGundanID < GAMEDATASTRUCT_GUNDAN_NUM ) {
				list.push_back( iGundanID );
			}
		}
	}

	return list;
}

extern �푈�J�n�p�����^ war_start_param;
// �Y���̌R�c�̐��ɂ����闧���Ԃ�(���ڎ�鑤=0, ���ڍU�߂鑤=1, �U�߂鑤�̉��R=3, ��鑤�̉��R=4, �����R=5)
int GetWarStandPointTheGundan(int iGundanID) {

	if ( !(0 <= iGundanID && iGundanID < GAMEDATASTRUCT_GUNDAN_NUM ) ) {
		return WarStandPoint::unknown;
	}

	// enum { unknown = -1,  �U���e�R = 1,  �U���q�R = 5,  ����e�R=0, ����q�R = 4, �����e�R=2, �����q�R=6, �U�����R�e�R=1, �U�����R�q�R=5, ������R�e�R=0, ������R�q�R=4 };

	if ( Is_FieldWar() || Is_CastleWar() ) {

		//--------------------------------- ���ڍU�����A�������́A���ږh�䑤�ł���΁A�푈�J�n�p�����^����͂����o����
		// �w��̌R�c�̑喼ID
		int iDaimyoID = nb6gundan[iGundanID].attach-1;

		// �U�����̌R�c�̑喼ID
		int iAttackDaimyoID = -1; 
		if ( 0 <= war_start_param.�U�����R�c_�R�c�ԍ�-1 && war_start_param.�U�����R�c_�R�c�ԍ�-1 < GAMEDATASTRUCT_GUNDAN_NUM ) {
			iAttackDaimyoID = nb6gundan[war_start_param.�U�����R�c_�R�c�ԍ�-1].attach-1;
		}

		// ������̌R�c�̑喼ID
		int iDefendDaimyoID = -1; 
		if ( 0 <= war_start_param.������R�c_�R�c�ԍ�-1 && war_start_param.������R�c_�R�c�ԍ�-1 < GAMEDATASTRUCT_GUNDAN_NUM ) {
			iDefendDaimyoID = nb6gundan[war_start_param.������R�c_�R�c�ԍ�-1].attach-1;
		}

		// �w��̌R�c�̑喼�͒��ڍU���̑喼
		if ( iDaimyoID == iAttackDaimyoID ) {
			return WarStandPoint::attack; // ���ڍU����
		}
		// �w��̌R�c�̑喼�͒��ڎ���̑喼
		else if ( iDaimyoID == iDefendDaimyoID ) {
			return WarStandPoint::defend; // ���ږh�䑤
		}

		//--------------------------------- ����ȊO�̏ꍇ�́AWAR_JOINT_DAIMYO_HEX_GROUP_INFO �n�̏�񂩂�H���Ă���

		// �Q���喼�ԍ����������āA�U�����̃O���[�vID�A������̃O���[�vID���o���B
		for ( int ix=0; ix < GAMEDATASTRUCT_WAR_JOINT_DAIMYO_HEX_GROUP_INFO_NUM; ix++ ) {
			// ���R�⒆���R���`�F�b�N
			int JoinGundanID = (int)nb6war_joint_daimyo_hex_group_info[ix].gundan - 1; // �Q���R�cID
			int JoinDaimyoID = (int)nb6war_joint_daimyo_hex_group_info[ix].daimyo - 1; // �Q���喼ID
			// �w��̌R�c�Ɠ����R�cID�ł���ꍇ�̂ݒl��Ԃ��B
			if ( iGundanID == JoinGundanID ) {

				// �w��̌R�c�̑喼�͒��ڍU���̑喼
				if ( JoinDaimyoID == iAttackDaimyoID ) {
					return WarStandPoint::attack; // ���ڍU����
				}
				// �w��̌R�c�̑喼�͒��ڎ���̑喼
				else if ( JoinDaimyoID == iDefendDaimyoID ) {
					return WarStandPoint::defend; // ���ږh�䑤
				}

				// 2��6�Ȃ璆���R�B2�������̐e�R�ŁA6�������̎q�R
				else if ( nb6war_joint_daimyo_hex_group_info[ix].group == 2 || nb6war_joint_daimyo_hex_group_info[ix].group == 6 ) {
					// �����R
					return WarStandPoint::chuuritsu;
				}
				// 1��5�Ȃ片�R�̍U���R�B1�����R�̐e�R�ŁA5�����R�̎q�R
				else if ( nb6war_joint_daimyo_hex_group_info[ix].group == 1 || nb6war_joint_daimyo_hex_group_info[ix].group == 5 ) {
					// ���R�̍U���R
					return WarStandPoint::attack_enngun;
				}
				// 0��4�Ȃ片�R�̎���R�B0�����R�̐e�R�ŁA4�����R�̎q�R
				else if ( nb6war_joint_daimyo_hex_group_info[ix].group == 0 || nb6war_joint_daimyo_hex_group_info[ix].group == 4 ) {
					// ���R�̎���R
					return WarStandPoint::defend_enngun;
				}
			}
		}
	}

	return WarStandPoint::unknown; // �s��
}

// �Y���̕����̐��ɂ����闧���Ԃ�(�s��=0, ���ڍU�߂鑤=1, ���ڎ�鑤=2, �U�߂鑤�̉��R=3, ��鑤�̉��R=4, �����R=5)
int GetWarStandPointTheBushou(int iBushouID) {

	// �͈̓`�F�b�N
	if ( !(0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM) ) {
		return WarStandPoint::unknown;
	}

	// �Y�������̌R�cID����
	int iGundanID = nb6bushouref[iBushouID].gundan-1;
	if ( 0 <= iGundanID && iGundanID < GAMEDATASTRUCT_GUNDAN_NUM ) {
		return GetWarStandPointTheGundan(iGundanID);
	}

	return WarStandPoint::unknown;
}

// ���݂̐�ɂ����āA�w��R�c�̑��叫(�R�c����喼�̂��Ƃł͂Ȃ��A���ɂ����Ă̊Y���R�c�̑��叫�B
int GetWarSoudaishoTheGundan(int iGundanID) {
	if ( !(0 <= iGundanID && iGundanID < GAMEDATASTRUCT_GUNDAN_NUM ) ) {
		return 0xFFFF;
	}

	if ( Is_FieldWar() || Is_CastleWar() ) {
		// �Q���喼�ԍ����������āA�U�����̃O���[�vID�A������̃O���[�vID���o���B
		for ( int ix=0; ix < GAMEDATASTRUCT_WAR_JOINT_DAIMYO_HEX_GROUP_INFO_NUM; ix++ ) {

			int iJointGundanID = (int)nb6war_joint_daimyo_hex_group_info[ix].gundan-1;

			// �w��̌R�c���������B
			if ( iGundanID == iJointGundanID ) {
				// �����ԍ�(+1�̕�)�͈̔͂Ɏ��܂��Ă���B
				if ( 0 < nb6war_joint_daimyo_hex_group_info[ix].soudaisho && nb6war_joint_daimyo_hex_group_info[ix].soudaisho <= GAMEDATASTRUCT_BUSHOU_NUM ) {
					return nb6war_joint_daimyo_hex_group_info[ix].soudaisho-1;
				// �����ԍ��͈̔͂Ɏ��܂��Ă��Ȃ��Ƃ������Ƃ́A�喼��R�c�͌��܂��Ă��邪�A���叫�����܂��Ă��Ȃ��B���j�b�g�z�u�O���Ƒ��叫�����܂�Ȃ��̂ŁA���̂悤�Ȓl�ƂȂ�B
				} else {
					return 0xFFFF;
				}
			}
		}
	}

	return 0xFFFF;
}

// ���Ȃ̂��U��퓬�Ȃ̂��H
// ���݉�ʂɏo�Ă���́A���
extern int iNextTurnCastleID;
// ���݂̐��ɂ�����^�[���o�ߐ��B���ȊO�ł�-1�ƂȂ��Ă���B
extern int iCurIncrementTurnInBattle;
bool Is_FieldWar() {
	return (iCurIncrementTurnInBattle >= 0) && iNextTurnCastleID == 0xFFFF && CommonGlobalHandle::isBattleMode;
}

// ���݉�ʂɏo�Ă���́A�U���
bool Is_CastleWar() {
	return (iCurIncrementTurnInBattle >= 0) && ( 0 <= iNextTurnCastleID-1 && iNextTurnCastleID-1 < GAMEDATASTRUCT_CASTLE_NUM ) && CommonGlobalHandle::isBattleMode; 
}


// �\�����̍U���ŉ��^�[���ڂ��B�P�`�R�D�U���łȂ��ꍇ��-1�D
int getCastleTurnInBattle() {

	if ( Is_CastleWar() ) {

		byte *turn = (byte *)GAMEDATASTRUCT_HOWMUCHTURNPROCEED_INCASTLEBATTLE;
		return *turn + 1;

	} else {
		return -1;
	}
}

// �O���Q�Ɨp
int WINAPI Extern_getCastleTurnInBattle() {
	return getCastleTurnInBattle();
}

//--------------

// �w�蕐�������ɂ����āA�s���ς݂��ǂ���
BOOL IsActionEndInBattle(int iBushouID) {
	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
		for ( int i=0; i < GAMEDATASTRUCT_WAR_HEX_UNIT_INFO_NUM; i++ ) {
			if ( nb6bushou_hex_unit_info[i].bushou == iBushouID+1 ) {
				if ( nb6bushou_hex_unit_info[i].bEnd == 0x2 ) { // �s���ςł���B
					return TRUE;
				} else {
					return FALSE;
				}
				break;
			}
		}
	}
	return FALSE;
}


// �Ώۂ̕������������Ă��邩�B
int GetKonranStatus(int iBushouID) {
	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
		for ( int i=0; i < GAMEDATASTRUCT_WAR_HEX_UNIT_INFO_NUM; i++ ) {
			if ( nb6bushou_hex_unit_info[i].bushou == iBushouID+1 ) {
				return nb6bushou_hex_unit_info[i].konran; 
			}
		}
	}
	return KONRAN_STATUS::����;
}

// �Ώۂ̕���������������B(�`�悪�����Ă��Ȃ�)
BOOL SetKonranStatus(int iBushouID, int iKonranStatus ) {
	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
		for ( int i=0; i < GAMEDATASTRUCT_WAR_HEX_UNIT_INFO_NUM; i++ ) {
			if ( nb6bushou_hex_unit_info[i].bushou == iBushouID+1 ) {
				nb6bushou_hex_unit_info[i].konran = iKonranStatus;
				// �����E�卬���E���� �̏�ԂɂȂ����̂Ȃ�c
				if ( iKonranStatus > 0 ) {
					nb6bushou_hex_unit_info[i].bEnd   = 0x2; // �s����
				}
				FuncBattleUnitStatusRepaint(iBushouID); // �`��X�V
				return TRUE;
			}
		}
	}
	return FALSE;
}

// �w�蕐�����s���ς݂Ƃ���
extern int arrRemainMoveCountAllBushou[GAMEDATASTRUCT_BUSHOU_NUM];
void setIsActionEndInBattle(int iBushouID, int isEnd=TRUE) {
	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
		for ( int i=0; i < GAMEDATASTRUCT_WAR_HEX_UNIT_INFO_NUM; i++ ) {
			if ( nb6bushou_hex_unit_info[i].bushou == iBushouID+1 ) {
				if ( isEnd ) {
					nb6bushou_hex_unit_info[i].bEnd = 0x2;  // �s���ςƂ���B
					arrRemainMoveCountAllBushou[iBushouID] = 0;
				} else {
					nb6bushou_hex_unit_info[i].bEnd = 0;  // ���s���Ƃ���B
				}
				break;
			}
		}
	}
}


//--------------MAIN�̃O���b�h�n

// �O���b�h�|�W�V�����N���X
GRID_POSITION::GRID_POSITION(int _x,int _y) {
	x=_x,
	y=_y;

	// �e�אڏ�͖�����Ԃŏ�����
	nw= 0xFFFF;
	no= 0xFFFF;
	ne= 0xFFFF;
	we= 0xFFFF;
	ea= 0xFFFF;
	sw= 0xFFFF;
	so= 0xFFFF;
	se= 0xFFFF;

}

// �O���b�h���W���u�̌��Z�B���W�`�����W�a�̃x�N�g�������߂�̂ɁA�ʏ�A�a�|�`�����邾�낤����B
GRID_VECTOR operator-(GRID_POSITION &p1, GRID_POSITION &p2) {
	GRID_VECTOR p;
	p.x = p1.x - p2.x;
	p.y = p1.y - p2.y;
	return p;
}

// ��h�c����A���C���O���b�h��̃|�W�V�����𓾂�B����O���b�h��0,0�Bmapdataobgk�̃O���b�h������΂킩��₷���B
// �P�̃O���b�h�ɂ́A�P�̏邵���Ȃ��B
GRID_POSITION getCastlePosInMainGrid(int iCastleID) {
	GRID_POSITION pos(0, 0);
	if ( 0 <= iCastleID && iCastleID < GAMEDATASTRUCT_CASTLE_NUM ) {
		int iGridX = (nb6castlemax[iCastleID].x + 1) / 5; // �O�I���W���Ȃ̂Ł{�P���ĂT�Ŋ���B�w�����͂P�̃O���b�h�̒��ɂT�̏ꏊ������B
		int iGridY = (nb6castlemax[iCastleID].y + 1) / 3; // �O�I���W���Ȃ̂Ł{�P���ĂR�Ŋ���B�x�����͂P�̃O���b�h�̒��ɂR�̏ꏊ������B

		pos.x = iGridX;
		pos.y = iGridY;

		// �אڂ����̂h�c������Ă����B
		for (int iCastleID = 0; iCastleID < GAMEDATASTRUCT_CASTLE_NUM; iCastleID++) {
			int iFX = (nb6castlemax[iCastleID].x + 1) / 5; // �O�I���W���Ȃ̂Ł{�P���ĂT�Ŋ���B�w�����͂P�̃O���b�h�̒��ɂT�̏ꏊ������B
			int iFY = (nb6castlemax[iCastleID].y + 1) / 3; // �O�I���W���Ȃ̂Ł{�P���ĂR�Ŋ���B�x�����͂P�̃O���b�h�̒��ɂR�̏ꏊ������B

			if ( iFX - pos.x == -1 && iFY - pos.y == -1 ) { // �k��
				pos.nw = iCastleID + 1;
			} else if ( iFX - pos.x == 0 && iFY - pos.y == -1 ) { // �k
				pos.no = iCastleID + 1;
			} else if ( iFX - pos.x == 1 && iFY - pos.y == -1 ) { // �k��
				pos.ne = iCastleID + 1;
			} else if ( iFX - pos.x == -1 && iFY - pos.y == 0 ) { // ��
				pos.we = iCastleID + 1;
			} else if ( iFX - pos.x == 1 && iFY - pos.y == 0 ) { // ��
				pos.ea = iCastleID + 1;
			} else if ( iFX - pos.x == -1 && iFY - pos.y == 1 ) { // �쐼
				pos.sw = iCastleID + 1;
			} else if ( iFX - pos.x == 0 && iFY - pos.y == 1 ) { // ��
				pos.so = iCastleID + 1;
			} else if ( iFX - pos.x == 1 && iFY - pos.y == 1 ) { // �쓌
				pos.se = iCastleID + 1;
			}
		}

		return pos;
	}
	return pos;
}

// �w��̏�ɑ΂��ėאڂ��Ă����̃��X�g�𓾂�
vector<int> getRinsetsuCastles(int iCastleID) {
	// �܂��́A�w���̃O���b�h�̏ꏊ�𓾂�
	GRID_POSITION CenterPos = getCastlePosInMainGrid(iCastleID);

	vector<int> RinsetsuCastles;

	// �אڂ��Ă������x�N�g���ɒǉ����Ă䂭
	for ( int iTargetID = 0; iTargetID < GAMEDATASTRUCT_CASTLE_NUM; iTargetID++ ) {
		// �������g�̓p�X
		if ( iCastleID == iTargetID ) {
			continue;
		}

		GRID_POSITION TargetPos = getCastlePosInMainGrid(iTargetID);
		// �אڂ��Ă���B
		if ( abs( CenterPos.x - TargetPos.x ) <= 1 && abs( CenterPos.y - TargetPos.y) <= 1) {
			RinsetsuCastles.push_back( iTargetID );
		}
	}
	return RinsetsuCastles;
}


char* nb6kanitypelist[GAMEDATASTRUCT_KANITYPE_NUM] = {
	"�����",
	"�]���",
	"�����",
	"�]���",
	"���O��",
	"�]�O��",
	"���l�ʏ�",
	"���l�ʉ�",
	"�]�l�ʏ�",
	"�]�l�ʉ�",
	"���܈ʏ�",
	"���܈ʉ�",
	"�]�܈ʏ�",
	"�]�܈ʉ�",
	"���Z�ʏ�",
	"���Z�ʉ�",
	"�]�Z�ʏ�",
	"�]�Z�ʉ�",
	"�����ʏ�",
	"�����ʉ�",
	"�]���ʏ�",
	"�]���ʉ�",
	"�����ʏ�",
};

char* getKaniTypeName(byte type) {
	return nb6kanitypelist[type];
}




// ��E�̎�ނ̕�����ւ̃|�C���^�̃A�h���X�ꗗ 
int GAMEDATASTRUCT_POSITIONTYPELIST_POINTER[GAMEDATASTRUCT_POSITIONTYPE_NUM] = {};

// ��E�^�C�v��ID���當����𓾂�
char* getPositionTypeName(byte type) {
	char *pszPotisionType = NULL;
	if (type==0) {
		return NULL;
	}
	if ( type < GAMEDATASTRUCT_POSITIONTYPE_NUM ) {
		pszPotisionType = (char *)(GAMEDATASTRUCT_POSITIONTYPELIST_POINTER[type]);
		return pszPotisionType;
	}
	return  NULL;
}

int DeNormalizeDaimyoYakusyokuValue(int iYakusyokuValue) {
	switch (iYakusyokuValue) {
	case 1:
		return 0x01;
	case 2:
		return 0x02;
	case 3:
		return 0x04;
	case 4:
		return 0x08;
	case 5:
		return 0x10;
	case 6:
		return 0x20;
	case 7:
		return 0x40;
	default:
		return 0;
	}
}

// �喼ID���琳�K�����ꂽ��EID�𓾂�
int getNormalizedDaimyoPosition(int iDaimyoID) {
	switch (nb6daimyo[iDaimyoID].position) {
		case 0x01:
			return 1;
		case 0x02:
			return 2;
		case 0x04:
			return 3;
		case 0x08 :
			return 4;
		case 0x10:
			return 5;
		case 0x20:
			return 6;
		case 0x40:
			return 7;
		default:
			return 0;
	}
}

// �R�cID���琳�K�����ꂽ��EID�𓾂�
int getNormalizedGundanPosition(int iGundanID) {
	return nb6gundan[iGundanID].position;
}

bool tryChangeDaimyoYakusyoku(int iDaimyoID, int iYakusyokuValue ) {
	if ( 0 <= iDaimyoID && iDaimyoID < GAMEDATASTRUCT_DAIMYO_NUM) {

		if ( ��E::���� <= iYakusyokuValue && iYakusyokuValue <= ��E::�H�B�T�� ) {

			int DenormalizeyakusyokuValue = DeNormalizeDaimyoYakusyokuValue(iYakusyokuValue);
			
			// �喼�ŖړI��ID�̖�E�������Ă���l
			for (int iDID = 0; iDID < GAMEDATASTRUCT_DAIMYO_NUM; iDID++) {
				int iDaimyoYakusyokuID = getNormalizedDaimyoPosition(iDID);
				// ���̖�E�������Ă���΁A�͂��D
				if (iDaimyoYakusyokuID == iYakusyokuValue) {
					nb6daimyo[iDID].position = 0;
				}
			}

			// �R�c�ŖړI��ID�̖�E�������Ă���l
			for (int iGID = 0; iGID < GAMEDATASTRUCT_GUNDAN_NUM; iGID++) {
				int iGundanYakusyokuID = getNormalizedGundanPosition(iGID);
				// ���̖�E�������Ă���΁A�͂��D
				if (iGundanYakusyokuID == iYakusyokuValue) {
					nb6gundan[iGID].position = 0;
				}
			}
		
			// �����ɑΏۂ̖�E������B
			nb6daimyo[iDaimyoID].position = DenormalizeyakusyokuValue;

			return true;
		}
	}
		
	return false;
}


bool tryChangeGundanYakusyoku(int iGundanID, int iYakusyokuValue ) {

	if ( 0 <= iGundanID && iGundanID < GAMEDATASTRUCT_GUNDAN_NUM) {

		// �喼���g�Ȃ�A�ʏ���
		if (nb6gundan[iGundanID].number == 1) {
			int iDaimyoID = nb6gundan[iGundanID].attach-1;
			tryChangeDaimyoYakusyoku( iDaimyoID, iYakusyokuValue );
		}

		if ( ��E::���� <= iYakusyokuValue && iYakusyokuValue <= ��E::�H�B�T�� ) {

			// �喼�ŖړI��ID�̖�E�������Ă���l
			for (int iDID = 0; iDID < GAMEDATASTRUCT_DAIMYO_NUM; iDID++) {
				int iDaimyoYakusyokuID = getNormalizedDaimyoPosition(iDID);
				// ���̖�E�������Ă���΁A�͂��D
				if (iDaimyoYakusyokuID == iYakusyokuValue) {
					nb6daimyo[iDID].position = 0;
				}
			}

			// �R�c�ŖړI��ID�̖�E�������Ă���l
			for (int iGID = 0; iGID < GAMEDATASTRUCT_GUNDAN_NUM; iGID++) {
				int iGundanYakusyokuID = getNormalizedGundanPosition(iGID);
				// ���̖�E�������Ă���΁A�͂��D
				if (iGundanYakusyokuID == iYakusyokuValue) {
					nb6gundan[iGID].position = 0;
				}
			}

			// �����ɑΏۂ̖�E������B
			nb6gundan[iGundanID].position = iYakusyokuValue;

			return true;
		}
	}

	return false;
}



/// ����(���̏u��)�A�V�ċL�ɑ��݂���R�c�́u�R�c�ԍ��y�z��p�z�v�����X�g�œ���B
vector<int> getValidGundanList() {
	vector<int> list;

	// �Y���̑喼�h�c�ɑ�����R�c�����X�g�ɉ�����
	for ( int iGundanID=0; iGundanID < GAMEDATASTRUCT_GUNDAN_NUM; iGundanID++ ) {
		if ( nb6gundan[iGundanID].leader !=  0xFFFF ) {
			list.push_back(iGundanID);
		}
	}
	
	return list;
}



// �R�c�ɏ������镐�����X�g�𓾂�
vector<int> getGundanBushouList(int iGundanID) {
	// ����ȊO�͉������ꂸ�ɕԂ��B
	vector<int> list;

	for (int i = 0; i<GAMEDATASTRUCT_BUSHOU_NUM; i++) {
		if (nb6bushouref[i].gundan - 1 == iGundanID) {
			if (nb6bushouname[i].State <= 2) { // �喼, �R�c��, ���� �ŃJ�E���g
				list.push_back(i);
			}
		}
	}
	return list;
}

// �Ƃ���喼���͂̓����R�c���X�g�𓾂�B
vector<int> getGovernedGundanList(int iDaimyoID) {
	// ����ȊO�͉������ꂸ�ɕԂ��B
	vector<int> list;

	if ( 0 <= iDaimyoID && iDaimyoID < GAMEDATASTRUCT_DAIMYO_NUM ) {

		// �Y���̑喼�h�c�ɑ�����R�c�����X�g�ɉ�����
		for ( int iGundanID=0; iGundanID<GAMEDATASTRUCT_GUNDAN_NUM; iGundanID++ ) {
			if ( nb6gundan[iGundanID].attach-1 ==  iDaimyoID) {
				list.push_back(iGundanID);
			}
		}
	}

	return list;
}

// �R�c�ɏ������镺���𓾂�
int getGundanHeisuCnt(int iGundanID) {
	int iSoldierCnt = 0;
	for ( int i=0; i<GAMEDATASTRUCT_BUSHOU_NUM; i++) {
		if ( nb6bushouref[i].gundan-1 == iGundanID ) {
			if ( nb6bushouname[i].State <= 2 ) { // �喼, �R�c��, ���� �ŃJ�E���g
				iSoldierCnt += nb6bushouref[i].soldier;
			}
		}
	}
	return iSoldierCnt;
}

// ��ɏ������镺���𓾂�
int getCastleHeisuCnt(int iCastleID) {
	int iSoldierCnt = 0;
	for ( int i=0; i<GAMEDATASTRUCT_BUSHOU_NUM; i++) {
		if ( nb6bushouref[i].castle-1 == iCastleID ) {
			if ( nb6bushouname[i].State <= 2 ) { // �喼, �R�c��, ���� �ŃJ�E���g
				iSoldierCnt += nb6bushouref[i].soldier;
			}
		}
	}
	return iSoldierCnt;
}



// �Ƃ���喼���͂̓����R�c���肠�Ĕԍ����X�g�𓾂�B
vector<int> getGovernedGundanWariateNumList(int iDaimyoID) {
	// ����ȊO�͉������ꂸ�ɕԂ��B
	vector<int> list;

	if ( 0 <= iDaimyoID && iDaimyoID < GAMEDATASTRUCT_DAIMYO_NUM ) {

		// �Y���̑喼�h�c�ɑ�����R�c�����X�g�ɉ�����
		for ( int iGundanID=0; iGundanID<GAMEDATASTRUCT_GUNDAN_NUM; iGundanID++ ) {
			if ( nb6gundan[iGundanID].attach-1 ==  iDaimyoID) {
				list.push_back(nb6gundan[iGundanID].number);
			}
		}
	}
	
	std::sort(list.begin(),list.end());//�����\�[�g

	return list;
}



// �Ƃ���喼�z���̌R�c���ɂ���R�c�u���v�����ԍ��̃��X�g�𓾂�B
vector<int> getNotGovernedGundanWariateNumList(int iDaimyoID) {

	// ���蓖�Ă��Ă���̂ɁA�P������
	int wariate_array[8] = {0,0,0,0,0,0,0,0};
	
	if ( 0 <= iDaimyoID && iDaimyoID < GAMEDATASTRUCT_DAIMYO_NUM ) {

		// �Y���̑喼�h�c�ɑ�����R�c�����X�g�ɉ�����
		for ( int iGundanID=0; iGundanID<GAMEDATASTRUCT_GUNDAN_NUM; iGundanID++ ) {
			if ( nb6gundan[iGundanID].attach-1 ==  iDaimyoID) {
				int g =  nb6gundan[iGundanID].number;
				if ( 1 <= g && g <= 8 ) {
					// ���蓖�Ă��Ă���̂ɁA�P������
					wariate_array[g-1] = 1;
				}
			}
		}
	}

	vector<int> list;
	for ( int ix=0; ix<8; ix++ ) {
		if (wariate_array[ix] == 0) { // �R�c�������Ȃ����
			list.push_back(ix+1);
		}
	}

	std::sort(list.begin(),list.end());//�����\�[�g

	return list;
}

// �R�c�C���ԍ��̕ύX(����)�����݂�B���s�����false
bool tryChangeGundanWariateNum(int iGundanID, int iNewWariateNum) {

	// ��P�R�c�͑喼���g�B�ύX�͖����B
	if (nb6gundan[iGundanID].number == 1) {
		return false;
	}
	
	if ( !( 1<= iNewWariateNum && iNewWariateNum <= 8) ) {
		return false;
	}

	// �����̌R�c�̑喼��ID
	int iDaimyoID = nb6gundan[iGundanID].attach - 1;

	// �喼�����ŋ󂫃X���b�g�ƂȂ��Ă���C���ԍ��ꗗ
	if (0 <= iDaimyoID && iDaimyoID < GAMEDATASTRUCT_DAIMYO_NUM) {
		vector<int> sameDaimyoGundanList = getGovernedGundanList(iDaimyoID);
		int iOldNinmeiNum = nb6gundan[iGundanID].number; // ���܂ł̔C���ԍ�

		for( vector<int>::iterator it = sameDaimyoGundanList.begin(); it != sameDaimyoGundanList.end(); it++ ) {
			int iGID = *it;

			// ���łɕʌR�c�ŗ��p����Ă���Ȃ��
			if (nb6gundan[iGID].number == iNewWariateNum) {
				nb6gundan[iGID].number = iOldNinmeiNum;
				break;
			}
		}

		// �����͎w��̌R�c�ԍ��ƂȂ�B
		nb6gundan[iGundanID].number = iNewWariateNum;

		return true;
	}

	return false;

}


// �w��̌R�c�̏�������喼�𐨗͂��z���āA�ύX����B
bool setGundanReAttachDaimyo(int iGundanID, int iNewDaimyoID) {

	// �V�喼�������ł͂Ȃ��B
	if ( ! (0 <= iNewDaimyoID && iNewDaimyoID < GAMEDATASTRUCT_DAIMYO_NUM) ) {
		return false;
	}

	// �R�c���S�����܂��Ă���Ȃ�΁A
	vector<int> glist = getGovernedGundanWariateNumList(iNewDaimyoID);
	if ( glist.size() == 8 ) { // �R�c�͑S�����܂��Ă���
		// �����ł��Ȃ�
		return false;
	}

	if ( 0 <= iGundanID && iGundanID < GAMEDATASTRUCT_GUNDAN_NUM ) {

		// �w��̌R�c�����ݏ������Ă���喼�ԍ������߂�
		int iDaimyoID = nb6gundan[iGundanID].attach-1;
		// �V������̑喼���A���������Ă���喼�ł���΁A�������邱�Ƃ͂Ȃ��B
		if ( iDaimyoID == iNewDaimyoID) {
			return true;
		}

		// �R�c���蓖�Ĕԍ����P�Ԃ͑喼���g�B����͌Œ�B���̏ꍇ�A�����������s�Ƃ���B
		if ( nb6gundan[iGundanID].number == 1 ) {
			return false;
		}

		// ���݁A�V�喼�ɏ������Ă��āAnext��0xFFFF�ɂȂ��Ă���R�c���A�Ԃ牺����ׂ������N���X�g�̍Ōゾ�B
		// �Ԃ牺���邱�Ƃ��o���邩�A���O�Ƀ`�F�b�N���Ă����B
		int iLinkEndGundanID = -1;
		for ( int iCurGundanID=0; iCurGundanID < GAMEDATASTRUCT_GUNDAN_NUM; iCurGundanID++ ) {
			// 
			int iCurDaimyoID = nb6gundan[iCurGundanID].attach-1;

			// �w��̐V�喼�ɏ������Ă���Ȃ�΁A
			if ( iCurDaimyoID == iNewDaimyoID) {
				// ���̌R�c�������̂ł���΁A�������ŏI�R�c�ԍ�
				if ( nb6gundan[iCurGundanID].next == 0xFFFF ) {
					iLinkEndGundanID = iCurGundanID+1;
				}
				
			}

		}

		// ���ꂪ�͈͂ɓ����Ă��Ȃ��Ȃ�΁A�V�喼�̌R�c�����N���X�g�́A��������������Ȃ�����񂪉��Ă���B
		// �����I��
		if ( ! (0 < iLinkEndGundanID && iLinkEndGundanID <= GAMEDATASTRUCT_GUNDAN_NUM ) ) {
			return false;
		}

		/* �@���R�c
		      ��
		   ��-��-��-��

		   �Ƃ����̂�

		   ��-��-��

		   �Ƃ���B
		*/

		int iPrevID = nb6gundan[iGundanID].prev;
		int iNextID = nb6gundan[iGundanID].next;

		// ������prev�͗L���ȌR�c���B
		if ( 0 < iPrevID && iPrevID <= GAMEDATASTRUCT_GUNDAN_NUM ) {

			// �������擪�ł͂Ȃ��R�c�ŁA
			// �P�O�̌R�c�̎��̃����N�悪���R�c�̏ꍇ
			if ( iPrevID != 0xFFFF && nb6gundan[(iPrevID-1)].next == iGundanID+1 ) {
				// ���R�c�ł͂Ȃ��A���̌R�c�ւƌq��������B
				nb6gundan[(iPrevID-1)].next = iNextID;
			}
		}

		// ������next�͗L���ȌR�c���B
		if ( 0 < iNextID && iNextID <= GAMEDATASTRUCT_GUNDAN_NUM ) {
			// �P��̌R�c�̑O�̃����N�悪���R�c�̏ꍇ
			if ( iNextID != 0xFFFF && nb6gundan[(iNextID-1)].prev == iGundanID+1 ) {
				// ���R�c�ł͂Ȃ��A�O�̌R�c�ւƌq��������B
				nb6gundan[(iNextID-1)].prev = iPrevID;
			}
		}


		/* �@
		     
		   ��-��

		   �Ƃ����̂�

		   ��-��-��

		   �Ƃ���B
		*/



		nb6gundan[iLinkEndGundanID-1].next = iGundanID+1; // �����N���X�g�̍Ō�̌R�c��next�����R�c��

		nb6gundan[iGundanID].prev = iLinkEndGundanID; // ���R�c�̑O�̌R�c�́A�R�c�̃����N���X�g�̍Ō�̌R�c

		// ���R�c�́A�R�c���X�g�̈�ԍŌ�ɂ������B���R�c��next�͋��Ȃ�
		nb6gundan[iGundanID].next = 0xFFFF; // 

		// ��������������
		{
			nb6gundan[iGundanID].number = 8; // ���̔ԍ��Ƃ����킯�ɂ͂����Ȃ��̂ŁA������̂��߂ɁA�Ƃ肠�����A�ł������m���ŋ󂢂Ă�Z�[�t�Ȃ��̂����

			vector<int> milist = getNotGovernedGundanWariateNumList(iNewDaimyoID);
			if ( milist.size() > 0 ) {
				nb6gundan[iGundanID].number = milist[0]; // ��ԍ��̍ŏ��̂��̂���荞��
			}

			nb6gundan[iGundanID].attach = iNewDaimyoID+1;
		}

		for ( int iBushouID=0; iBushouID < GAMEDATASTRUCT_BUSHOU_NUM; iBushouID++) {
			// �w��̌R�c�ɏ������Ă��镐���́A�S�āA�V�喼�ɏ�����ύX����B
			if ( nb6bushouref[iBushouID].gundan== iGundanID+1 ) {
				nb6bushouref[iBushouID].attach = iNewDaimyoID+1;
				nb6bushouref[iBushouID].work = 0; // �m���N�����Z�b�g
				nb6bushouref[iBushouID].rise = 0xFFFF; // �����惊�Z�b�g
				nb6bushouref[iBushouID].bRise = 0; // ������ԂȂ�
				nb6bushouref[iBushouID].grudge = 0; // �⍦�Ȃ�
			}
		}

		return true;

	}

	return false;
}






MAKEGUNDANKOUHOINFO Get_MakeGundanKouhoInfo(int m_iDaimyoID) {
	MAKEGUNDANKOUHOINFO ret;
	ret.m_iDaimyoID = m_iDaimyoID;
	ret.vGundanIDList = vector<int>();
	ret.vCastleIDList = vector<int>();
	ret.vBushouIDList = vector<int>();
	ret.CanMake = FALSE;

	if (!(0 <= m_iDaimyoID && m_iDaimyoID < GAMEDATASTRUCT_DAIMYO_NUM)) {
		OutputDebugStream("m_iDaimyoID���͈͊O");
		return ret;
	}

	bool is_gundan_has_empty_slot = false;
	for (int iGundanID = 0; iGundanID < GAMEDATASTRUCT_GUNDAN_NUM; iGundanID++) {
		if (nb6gundan[iGundanID].attach == 0xFFFF && // �����喼�ԍ�
			nb6gundan[iGundanID].residence == 0xFFFF &&
			nb6gundan[iGundanID].next == 0xFFFF) {
			is_gundan_has_empty_slot = true;
			break;
		}
	}
	if (is_gundan_has_empty_slot == false) {
		OutputDebugStream("�R�c�X���b�g���S�Ė��܂��Ă���");
		return ret;
	}

	int iDaimyoBushouID = nb6daimyo[m_iDaimyoID].attach - 1; // �喼�̕����ԍ�
	// ����ŊY���R�c�Ƀ^�[�������Ԃ̊댯�ȃ^�C�~���O�B���̃^�C�~���O�͏��F�҂��������Ă��Ȃ��̂ŁA�V�݂ł��Ȃ����̂Ƃ���B
	if (nb6bushouname[iDaimyoBushouID].battleDeath || (nb6bushouname[iDaimyoBushouID].State == 7)) { // ���S
		return ret;
	}

	// ���蓖�Č��͗v����ɁA�܂����蓖�Ă��Ă��Ȃ��R�c�ԍ��̂���
	ret.vGundanIDList = getNotGovernedGundanWariateNumList(m_iDaimyoID);

	// �w��̑喼���x�z���Ă���S��̃��X�g
	auto daimyoHasCastleList = getDaimyoHasCastleList(m_iDaimyoID);

	// ���ɂ���̂́A�w��̑喼�Ǝx�z��̂����A�u�喼�v���u�R�c���v�����Ȃ���B
	// �����A�u��{���v�̂��̂Ɍ���
	for each(int iCastleID in daimyoHasCastleList) {
		if (nb6castle[iCastleID].honjo == 0) { //�{�� ���� (0:��{��, �喼�̖{��=1, �R�c���̖{��=2);
			ret.vCastleIDList.push_back(iCastleID);
		}
	}

	for (int iBushouID = 0; iBushouID < GAMEDATASTRUCT_BUSHOU_NUM; iBushouID++) {

		// �܂��Ώۂ̕����́A�w��̑喼�Ƃ̐l�Ԃł���B(�喼�܂�)
		if (nb6bushouref[iBushouID].attach - 1 == m_iDaimyoID) {

			// �����ł���B(=�喼��R�c����q���ł͂Ȃ��A�������A�o�ꂵ�Ă����ʕ���)
			if (nb6bushouname[iBushouID].State == 2) { // 2:����
				// �펀���ł͂Ȃ�
				if (!nb6bushouname[iBushouID].battleDeath) {
					ret.vBushouIDList.push_back(iBushouID);
				}
			}
		}
	}

	// �R�c�����łɂW����
	if (ret.vGundanIDList.size() == 0) {
		ret.CanMake = FALSE;
		return ret;
	}

	// �V�R�c�����}�������悤�ȗ]������͑��݂��Ȃ�
	if (ret.vCastleIDList.size() == 0) {
		ret.CanMake = FALSE;
		return ret;
	}

	// ���������R�c���ɔC�����ׂ��Ώۂ̈�ʕ��������݂��Ȃ�
	if (ret.vBushouIDList.size() == 0) {
		ret.CanMake = FALSE;
		return ret;
	}

	ret.CanMake = TRUE;

	return ret;
}


int Set_MakeGundan(int _m_iBushouID, int _m_iCastleID, MAKEGUNDANKOUHOINFO check) {
	if (check.CanMake == FALSE) {
		OutputDebugStream("MAKEGUNDANKOUHOINFO.CanMake �� FALSE");
		return 0xFFFF;
	}

	// ���O������
	auto& bushous = check.vBushouIDList;
	auto& castles = check.vCastleIDList;
	auto& gwariates = check.vGundanIDList;
	if (bushous.size() == 0 || castles.size() == 0 || gwariates.size() == 0){
		OutputDebugStream("MAKEGUNDANKOUHOINFO�̒��g�f�[�^���s��");
		return 0xFFFF;
	}

	auto bitr = std::find(bushous.begin(), bushous.end(), _m_iBushouID);
	// �������ȊO���w�肵�Ă���B
	if (bitr == bushous.end()) {
		OutputDebugStream("_m_iBushouID �� MAKEGUNDANKOUHOINFO.vBushouIDList �ɂȂ�");
		return 0xFFFF;
	}

	auto citr = std::find(castles.begin(), castles.end(), _m_iCastleID);
	// ����ȊO���w�肵�Ă���B
	if (citr == castles.end()) {
		OutputDebugStream("_m_iCastleID �� MAKEGUNDANKOUHOINFO..vCastleIDList �ɂȂ�");
		return 0xFFFF;
	}

	// �����X���b�g��T��
	int iSelfGundanID = 0xFFFF;
	for (int iGundanID = 0; iGundanID < GAMEDATASTRUCT_GUNDAN_NUM; iGundanID++) {
		if (nb6gundan[iGundanID].attach == 0xFFFF && // �����喼�ԍ�
			nb6gundan[iGundanID].residence == 0xFFFF &&  // ��������
			nb6gundan[iGundanID].next == 0xFFFF) {
			iSelfGundanID = iGundanID;
			break;
		}
	}

	// �Ȃ��������B���S�`�F�b�N�p�R�c�V�݌���̎擾�̌�ASet_�R�c�V�݂��Ăԃ^�C�~���O������Ă���̂��낤�B
	if (iSelfGundanID == 0xFFFF) {
		return 0xFFFF;
	}


	// �܂��́A�R�c�ꗗ�̒����玩�����Ԃ牺����ׂ��R�c��T���B
	// ����́u�����喼�ԍ��v�ɏ������Ă��Ȃ���unext�v��65535 ���Ȃ킿�A
	// �����N���X�g�̖����ɏ������Ă���R�c�ł���B
	int iPrevGundanID = 0xFFFF;
	for (int iGundanID = 0; iGundanID < GAMEDATASTRUCT_GUNDAN_NUM; iGundanID++) {
		// �喼����v
		if (nb6gundan[iGundanID].attach == check.m_iDaimyoID + 1) { // �����喼�ԍ�
			// �Ώۂ̑喼���̌R�c�ꗗ�̂����A�����̌R�c
			if (nb6gundan[iGundanID].next == 0xFFFF) {
				iPrevGundanID = iGundanID;
			}
		}
	}


	if (iPrevGundanID == 0xFFFF) {
		return 0xFFFF;
	}

	// �ȏ���N���A���Ă���΁A�܂����v���낤�B

	// -------------------------------------------------------------------
	// �P�O�̌R�c�̎������g�Ƃ���
	nb6gundan[iPrevGundanID].next = iSelfGundanID + 1;


	// -------------------------------------------------------------------
	// �����̑O�̌R�c�Ƃ��āA�����N���X�g�����̍Ō�̌R�c�ԍ�������
	nb6gundan[iSelfGundanID].prev = iPrevGundanID + 1;
	// �������g�������N���X�g�̍Ō�B
	nb6gundan[iSelfGundanID].next = 0xFFFF;

	// �󂢂Ă��邤���ŁA��ԎႢ�ԍ�������B
	nb6gundan[iSelfGundanID].number = gwariates[0];

	nb6gundan[iSelfGundanID].attach = check.m_iDaimyoID + 1;

	nb6gundan[iSelfGundanID].residence = _m_iCastleID + 1;

	nb6gundan[iSelfGundanID].leader = _m_iBushouID + 1; // �R�c���y�����ԍ��z

	nb6gundan[iSelfGundanID].act = 50;

	tryChangeGundanYakusyoku(iSelfGundanID, 0);

	nb6gundan[iSelfGundanID].money = 1000;
	nb6gundan[iSelfGundanID].rise = 1000;

	nb6gundan[iSelfGundanID].gun = 100;
	nb6gundan[iSelfGundanID].horse = 100;



	// -------------------------------------------------------------------
	// ��̏��⃊���N���X�g��ύX����
	nb6castle[_m_iCastleID].attach = iSelfGundanID + 1;

	nb6castle[_m_iCastleID].honjo = 2; //�{�� ���� (0:��{��, �喼�̖{��=1, �R�c���̖{��=2);

	// �������g(��)�������N���X�g����O���B
	// �O�̏�́u���̏�v���A���g�ł͂Ȃ��A�u���g�̎��̏�v�ɂ���B
	if (nb6castle[_m_iCastleID].prev != 0 && nb6castle[_m_iCastleID].prev != 0xFFFF) {
		int _prev = nb6castle[_m_iCastleID].prev - 1;
		nb6castle[_prev].next = nb6castle[_m_iCastleID].next;
	}

	// ���̏�́u�O�̏�v���A���g�ł͂Ȃ��A�u���g�̑O�̏�v�ɂ���B
	if (nb6castle[_m_iCastleID].next != 0 && nb6castle[_m_iCastleID].next != 0xFFFF) {
		int _next = nb6castle[_m_iCastleID].next - 1;
		nb6castle[_next].prev = nb6castle[_m_iCastleID].prev;
	}


	// �V�݂Ȃ̂Ŏ������擪�͊m��
	nb6castle[_m_iCastleID].prev = 0;
	// �V�݂Ȃ̂Ŏ������Ō�͊m��
	nb6castle[_m_iCastleID].next = 0xFFFF;

	// -------------------------------------------------------------------

	// -------------------------------------------------------------------
	// �{�l�̃p�����[�^
	// �Ώۂ̕����́A�Y����ւ̈ړ�
	setBushouTransport(_m_iBushouID, _m_iCastleID);

	setGenekiToJyosyu(_m_iBushouID);

	nb6bushouname[_m_iBushouID].State = 1; // �R�c��

	// �Y���̏�ɋ���l�B�̏����R�c�̏�������
	for (int iBushouID = 0; iBushouID < GAMEDATASTRUCT_BUSHOU_NUM; iBushouID++) {
		if (nb6bushouref[iBushouID].attach == check.m_iDaimyoID + 1) { // �����喼�ԍ�
			if (nb6bushouref[iBushouID].castle == _m_iCastleID + 1) {
				if (nb6bushouname[iBushouID].State == 1 || nb6bushouname[iBushouID].State == 2) { // �R�c���������͌���
					// �펀���ł͂Ȃ�
					if (!nb6bushouname[iBushouID].battleDeath) {
						nb6bushouref[iBushouID].gundan = iSelfGundanID + 1;
					}
				}
			}
		}
	}

	return iSelfGundanID;
}





// �Ɩ�����߂Ă���A���S(�Ƃ������R�Â��f�[�^)���s���Ȃ̂ŁA
// �Ƃ肠�����A�ȈՂȂ��̂ő�p
static int _getKamonAtBorn(int iBushouID) {

	if (!(0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM)) {
		OutputDebugStream("�����ԍ����͈͊O");
		return 0; // �Ԃ��悤���Ȃ��̂ŁA��U�M���̂ɂ��Ă����B
	}

	// ���K�Ɩ�S�̂̔z��(�A���A
	map<int, int> regular_kamons_remain_list;
	for (int k = 0; k < GAMEDATASTRUCT_KAMON_REGULAR_NUM; k++) {
		regular_kamons_remain_list.insert(map<int, int>::value_type(k, k));
	}

	// ���łɑ喼���g���Ă���Ɩ���A���K�̗]��Ɩ䃊�X�g���珜�����邱�ƂŁA���g�p�̂��̂����ɂ���
	for (int iDaimyoID = 0; iDaimyoID < GAMEDATASTRUCT_DAIMYO_NUM; iDaimyoID++) {

		int kamon = nb6daimyo[iDaimyoID].symbol;

		// �Ώۂ̉Ɩ�ԍ������X�g�ɂȂ��Ȃ�A(���K�Ɩ�ȊO�Ȃ瑊��ɂ��Ȃ�)
		regular_kamons_remain_list.erase(kamon);
	}

	int bloodNormalized = nb6bushouref[iBushouID].blood;
	if (bloodNormalized >= GAMEDATASTRUCT_KAMON_REGULAR_NUM) { bloodNormalized = GAMEDATASTRUCT_KAMON_REGULAR_NUM - 1; }

	// ���K�̗]��Ɩ䃊�X�g�ɁA�喼�̕����̌��ؔԍ��Ɠ����Ɩ�ԍ������g�p�ł��܂��Ă�Ȃ炻��ɂ��Ă���(���ؔԍ��ƉƖ�ԍ���72���炢�܂ł͈�v���Ă��邽��)
	if (regular_kamons_remain_list.count(bloodNormalized) > 0) {
		; // �������Ȃ��BbloodNormalized�̂��̂܂܂̐��l���̗p
	}
	// ���K���X�g���ɁA�喼�̕����̌��ؔԍ��Ɠ����Ɩ�ԍ��������ꍇ�́A���K�̗]��Ɩ䃊�X�g(�̗]���Ă��)
	else {
		// ��`
		auto itr = regular_kamons_remain_list.begin();

		bloodNormalized = itr->first;
	}

	return bloodNormalized;
}




int Set_IndependentGundan(int _iGundanID) {
	if (!(0 <= _iGundanID && _iGundanID < GAMEDATASTRUCT_GUNDAN_NUM)) {
		return 0xFFFF;
	}

	int iSelfDaimyoID = 0xFFFF;
	for (int iDaimyoID = 0; iDaimyoID < GAMEDATASTRUCT_DAIMYO_NUM; iDaimyoID++) {
		// �����Ă���Ƃ��������͈ȉ��̂Q�ł킩�邾�낤�B
		if (nb6daimyo[iDaimyoID].attach == 0xFFFF && // �喼�̕����ԍ�
			nb6daimyo[iDaimyoID].gundan == 0xFFFF) {
			iSelfDaimyoID = iDaimyoID;
			break;
		}
	}

	// �󂫂��Ȃ�������_��
	if (iSelfDaimyoID == 0xFFFF) {
		return 0xFFFF;
	}

	int iNewDaimyoBushouID = nb6gundan[_iGundanID].leader - 1; // �R�c���̕����ԍ�
	int iNewDaimyoCastleID = nb6gundan[_iGundanID].residence - 1; // ������ԍ�
	// �R�c���ȊO���w�肳��Ă���B�����A�喼���g��(��P�R�c������)�R�c�ԍ����w�肳��Ă���B
	if (nb6bushouname[iNewDaimyoBushouID].State != 1) { // �R�c��
		return 0xFFFF;
	}

	// ���Ŏ���ł��āA���C����ʂɖ߂�O�B�_��
	if (nb6bushouname[iNewDaimyoBushouID].battleDeath) { // �펀
		return 0xFFFF;
	}

	// ----------------------------------------------------
	// �S�~�|��
	nb6daimyo[iSelfDaimyoID].retire = 0xFFFF;
	nb6daimyo[iSelfDaimyoID].hostile = 0xFF;	// �G�Α喼
	nb6daimyo[iSelfDaimyoID].friendship = 0xFF; // �F�D�喼
	nb6daimyo[iSelfDaimyoID].friendship = 50;   // ���l�F�D�x
	nb6daimyo[iSelfDaimyoID].bPropagate = 0;  // �z����
	nb6daimyo[iSelfDaimyoID].hyotei = 0;      // �]��
	nb6daimyo[iSelfDaimyoID].bMerchantAngry = 0;      // ���l���{���Ăď��l�s��

	if (nb6bushouref[iNewDaimyoBushouID].job == 0x60) { // ����
		nb6daimyo[iSelfDaimyoID].tyotei = 30; // ����
	}
	else {
		nb6daimyo[iSelfDaimyoID].tyotei = 0; // ����
	}

	nb6daimyo[iSelfDaimyoID].symbol = _getKamonAtBorn(iNewDaimyoBushouID);

	tryChangeDaimyoYakusyoku(iSelfDaimyoID, 0);

	// ----------------------------------------------------
	// �Ώۂ̌R�c���́A�l�Ƃ��đ喼�ƂȂ�
	nb6bushouname[iNewDaimyoBushouID].State = 0; // 0:�喼
	nb6bushouname[iNewDaimyoBushouID].position = 1; // 1:�喼

	// �܂��͐V�݂����喼�Ƃ̑喼�́A�w��̌R�c����V�ݑ喼�̕�����
	nb6daimyo[iSelfDaimyoID].attach = iNewDaimyoBushouID + 1; // �喼�y�����ԍ��z
	// �喼�̖{��P�R�c�ƂȂ�
	nb6daimyo[iSelfDaimyoID].gundan = _iGundanID + 1;

	// ----------------------------------------------------
	// ���݂̌R�c�̏����喼���A�V�݂̑喼ID�ɋA��
	nb6gundan[_iGundanID].attach = iSelfDaimyoID + 1; // �����喼�y�喼�ԍ��z

	// �����ԍ��͂P�Œ�
	nb6gundan[_iGundanID].number = 1;

	// �R�c���̖{���������Ƃ������́A�喼�̖{���֊i�グ
	nb6daimyo[iSelfDaimyoID].castle = iNewDaimyoCastleID + 1; // ��������y��ԍ��z
	nb6castle[iNewDaimyoCastleID].honjo = 1; //  �{��::�喼;

	// ----------------------------------------------------
	// �����N���X�g�̕ύX�B�����̌R�c�����A����܂ł̑喼�z���̌R�c�̃����N���X�g����폜����

	// �������g(�R�c)�������N���X�g����O���B
	// �O�̌R�c�́u���̌R�c�v���A���g�ł͂Ȃ��A�u���g�̎��̌R�c�v�ɂ���B
	if (nb6gundan[_iGundanID].prev != 0 && nb6gundan[_iGundanID].prev != 0xFFFF) {
		int _prev = nb6gundan[_iGundanID].prev - 1;
		nb6gundan[_prev].next = nb6gundan[_iGundanID].next;
	}

	// ���̌R�c�́u�O�̌R�c�v���A���g�ł͂Ȃ��A�u���g�̑O�̌R�c�v�ɂ���B
	if (nb6gundan[_iGundanID].next != 0 && nb6gundan[_iGundanID].next != 0xFFFF) {
		int _next = nb6gundan[_iGundanID].next - 1;
		nb6gundan[_next].prev = nb6gundan[_iGundanID].prev;
	}


	// �V�݂Ȃ̂Ŏ������擪�͊m��
	nb6gundan[_iGundanID].prev = 0;
	// �V�݂Ȃ̂Ŏ������Ō�͊m��
	nb6gundan[_iGundanID].next = 0xFFFF;

	// ----------------------------------------------------
	// �R�c�z�������̏�������(�������܂�)�̏����喼���A�V�ݑ喼�ւ̕ύX����
	for (int iBushouID = 0; iBushouID < GAMEDATASTRUCT_BUSHOU_NUM; iBushouID++) {
		if (nb6bushouref[iBushouID].gundan == _iGundanID + 1) {
			// �펀���ł͂Ȃ�
			if (!nb6bushouname[iBushouID].battleDeath) { // �펀
				nb6bushouref[iBushouID].attach = iSelfDaimyoID + 1;
				nb6bushouref[iBushouID].work = 0; // �m���N��
			}
		}
	}

	// ----------------------------------------------------
	// �����n
	for (int iTargetDaimyoID = 0; iTargetDaimyoID < GAMEDATASTRUCT_DAIMYO_NUM; iTargetDaimyoID++) {
		// �F�D�͂܂�Ȃ�
		setYuukouKankei(iSelfDaimyoID, iTargetDaimyoID, 50);
		// �����⍥���̓��Z�b�g
		setDoumeiKankei(iSelfDaimyoID, iTargetDaimyoID, FALSE);
		setKoninKankei(iSelfDaimyoID, iTargetDaimyoID, FALSE);
	}
	return iSelfDaimyoID;
}



// �Ƃ���喼���ł̂`�R�c�̏���a�R�c�ւƃA�^�b�`�ύX
bool Set_ChangeCastleAttachGundan(int _l_CastleID, int _l_GundanID) {

	if (!(0 <= _l_CastleID && _l_CastleID < GAMEDATASTRUCT_CASTLE_NUM)) {
		OutputDebugStream("��ԍ����͈͊O");
		return false;
	}

	if (nb6castle[_l_CastleID].honjo == 2 || nb6castle[_l_CastleID].honjo == 1) { // 2:�R�c���̖{�� 1:�喼�̖{��
		OutputDebugStream("�喼��R�c���̖{���n�͏����҂��ړ��ł��Ȃ�");
		return false;
	}

	if (!(0 <= _l_GundanID && _l_GundanID < GAMEDATASTRUCT_GUNDAN_NUM)) {
		OutputDebugStream("�R�c�ԍ����͈͊O");
		return false;
	}

	int iGundanID = nb6castle[_l_CastleID].attach - 1; // �����R�c�R�c�ԍ� -1

	if (!(0 <= iGundanID && iGundanID < GAMEDATASTRUCT_GUNDAN_NUM)) {
		OutputDebugStream("��ԍ��������炭��������");
		return false;
	}

	int iOrgDaimyoID = nb6gundan[iGundanID].attach - 1; // �����喼�ԍ�
	int iDstDaimyoID = nb6gundan[_l_GundanID].attach - 1;

	if (iOrgDaimyoID != iDstDaimyoID) {
		OutputDebugStream("��ԍ��ƐV�R�c�ԍ��������喼�̂��̂ł͂Ȃ�");
		return false;
	}

	// -------------------------------------------------------------------
	// �܂��A���̌R�c�̏郊���N���X�g����A�����̏���O��
	nb6castle[_l_CastleID].attach = _l_GundanID + 1;  // �����R�c�̌R�c�ԍ�

	// �������g(��)�������N���X�g����O���B
	// �O�̏�́u���̏�v���A���g�ł͂Ȃ��A�u���g�̎��̏�v�ɂ���B
	if (nb6castle[_l_CastleID].prev != 0 && nb6castle[_l_CastleID].prev != 0xFFFF) {
		int _prev = nb6castle[_l_CastleID].prev - 1;
		nb6castle[_prev].next = nb6castle[_l_CastleID].next;
	}

	// ���̏�́u�O�̏�v���A���g�ł͂Ȃ��A�u���g�̑O�̏�v�ɂ���B
	if (nb6castle[_l_CastleID].next != 0 && nb6castle[_l_CastleID].next != 0xFFFF) {
		int _next = nb6castle[_l_CastleID].next - 1;
		nb6castle[_next].prev = nb6castle[_l_CastleID].prev;
	}

	// -------------------------------------------------------------------
	// ���ɁA�V�R�c�̏郊���N���X�g�̍Ō�̏��T��
	int iTailCastleID = 0xFFFF;
	for (int iCastleID = 0; iCastleID < GAMEDATASTRUCT_CASTLE_NUM; iCastleID++) {
		// �Ώۂ̏�̏����R�c���A�w��̐V�R�c�ł���
		if (nb6castle[iCastleID].attach - 1 == _l_GundanID) {
			// ���̏邪�����N���X�g�̍Ō�̏�ł���Ȃ��
			if (nb6castle[iCastleID].next == 0xFFFF) {
				iTailCastleID = iCastleID;
				break;
			}
		}
	}

	if (iTailCastleID == 0xFFFF) {
		OutputDebugStream("�ُ펖��!! ��̃����N���X�g�����Ă���!!");
		return false;
	}

	// �����͐V�R�c�ł̓����N���X�g�̖����͊m��Ȃ̂Ŏ��̏�͂Ȃ�
	nb6castle[_l_CastleID].next = 0xFFFF;

	// �����̂P�O�́A����܂Ŗ�����������̔ԍ�
	nb6castle[_l_CastleID].prev = iTailCastleID + 1;

	// ����܂Ń����N���X�g�̖�����������̎��̏邪����
	nb6castle[iTailCastleID].next = _l_CastleID + 1;

	// -------------------------------------------------------------------
	// ���̏�ɂ��镐���B�̏�����ύX����
	// �Y���̏�ɋ���l�B�̏����R�c�̏�������
	for (int iBushouID = 0; iBushouID < GAMEDATASTRUCT_BUSHOU_NUM; iBushouID++) {
		if (nb6bushouref[iBushouID].attach == iOrgDaimyoID + 1) {  // �����喼�ԍ�
			if (nb6bushouref[iBushouID].castle == _l_CastleID + 1) { // ��������
				if (nb6bushouname[iBushouID].State == 1 || nb6bushouname[iBushouID].State == 2) { // 1:�R�c�� 2:����
					// �펀���ł͂Ȃ�
					if ( ! nb6bushouname[iBushouID].battleDeath) {
						nb6bushouref[iBushouID].gundan = _l_GundanID + 1;
					}
				}
			}
		}
	}

	return true;

}




// �S�L���喼�̃��X�g��Ԃ��B�^�[���喼���g�������Ă�̂Œ��ӁB
vector<int> getDaimyoList(BOOL isIncludeTurnDaimyo) {
	vector<int> list;
	for ( int iDaimyoID=0; iDaimyoID < GAMEDATASTRUCT_DAIMYO_NUM; iDaimyoID++ ) {
		// �^�[���喼�͊܂܂Ȃ�
		if ( !isIncludeTurnDaimyo ) {
			// �^�[���喼�Ȃ�p�X
			if ( iDaimyoID == nb6turn.daimyo-1 ) {
				continue;
			}
		}
		// �����ƕ������[�߂Ă���喼�Ƃ̂�
		if ( nb6daimyo[iDaimyoID].attach != 0xFFFF ) {
			list.push_back(iDaimyoID);
		}
	}
	return list;
}



// �`�喼(�O�n�܂�ł̔z��p�w��)�Ƃa�喼�Ƃ̓����֌W�j
// 004CF213  05 55 55 55 95    �Ƃ���̂́A004CF213 [0 5 5 5 5 5 5 5 9 5] �Ƃ����悤��1�ԖځA2�ԖځA3�Ԗڂ̑���c�Ƃ��������ƂȂ�B  
// ��������E�������͂ǂ��ɂ���̂��Ƃ����ƁA2�l�̑g�ݍ��킹�̂����A�傫�����̑喼�ԍ��p�̓����\���̂ɓ����Ă���B
// �Ⴆ�΁A�喼�ԍ�21��25�Ƃ���ƁA�F�D����21�Ԃ̕���25�Ԗڂɓ����Ă���A�����⍥���́A25�Ԃ̕���21�Ԗڂɓ����Ă���Ƃ����킯���B


// ����getDoumeiKankei�����ŋ��܂�l�́A���̔z���INDEX�ł��邽�߁A������g���Ď��ۂ̒l�ɒ����B
int YuukouValueArray[] = {0, 10, 20, 30, 40, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 100, 0xFFFF};


byte* getYuukouKannkeiPointer(int& iDaimyoA_ID, int& iDaimyoB_ID) {

	// �͈̓`�F�b�N
	if ( 0 <= iDaimyoA_ID && iDaimyoA_ID < GAMEDATASTRUCT_DAIMYO_NUM &&
 		 0 <= iDaimyoB_ID && iDaimyoB_ID < GAMEDATASTRUCT_DAIMYO_NUM ) {
	} else {
		return NULL;
	}

	// �喼�`�Ƒ喼�a�ŁA�喼�`�̕����l���傫���ꍇ�A����ւ���B
	// �Ȃ��Ȃ�F�D�֌W�̒l�́A�喼�`�����������̑g�ݍ��킹�e�[�u���ɓ����Ă��āA�喼�`���傫�����̑g�ݍ��킹�e�[�u���ɂ͍����⓯���̒l�������Ă��邩��B
	if (iDaimyoA_ID > iDaimyoB_ID ) {
		// �Q����ւ�
		iDaimyoA_ID ^= iDaimyoB_ID;
		iDaimyoB_ID ^= iDaimyoA_ID;
		iDaimyoA_ID ^= iDaimyoB_ID;
	}

	byte *p = (byte *)GAMEDATASTRUCT_DOUMEI_ADDRESS;
	
	// �ȉ�2�l�̑喼�h�c���A�����֌W�l�������Ă��郁�����̃A�h���X�����߂�
	p += iDaimyoA_ID * ( GAMEDATASTRUCT_DAIMYO_NUM / 2 ); // �������P�̂`�喼������A�a�喼�ւ̓������P�o�C�g�ŕ\������Ă���΁A
															 // �V�Q�喼����̂ŁA�V�Q�o�C�g�ŕ\�������B
															 // �Ƃ��낪���ۂɂ́A4�r�b�g�ŕ\�������̂ŁA
															 // �P�̑喼�ɂԂ牺����\���̃f�[�^�Ƃ��ẮA�����̂R�U�o�C�g�ł���B

	p += iDaimyoB_ID / 2;									 // ��Ɠ������R�ƂȂ�B�P�̑喼��(�����֌W�\����)�̒��ł�
															 // �P�̑���喼�Ƃ̓����l�Ɏg���Ă���͔̂��o�C�g�B
															 // ����ă|�C���^�̈ʒu�Ƃ��ẮA/2 ���Ă����A����������ŁA��ŉ��Z��U�蕪����B

	return p;
}

int getYuukouKankei(int iDaimyoA_ID, int iDaimyoB_ID) {

	// �Y���̗F�D�֌W��񂪑��݂���A�h���X�𓾂�B�`�Ƃa�͒l����������邩������Ȃ��B���ӁB
	byte *p = getYuukouKannkeiPointer( iDaimyoA_ID, iDaimyoB_ID );
	if (!p) return 0;

	// �A�h���X�͋��܂������A�P�̃A�h���X��1�o�C�g���ɂ́A�Q�̑喼�������Ă���B
	// �喼�ԍ��������Ȃ��ʌ��A�����Ȃ牺�ʌ��ł���B
	int YuukouIX = 0;
	// ����喼��������
	if ( iDaimyoB_ID % 2==0 ) {								 // �����ł���΁A�㔼���̃r�b�g�Ƃ́�����[0 5] �̂���[0]��Ԃ�
		YuukouIX = (*p) >> 4;

	// ����喼�����										 // ��ł���΁A�������̃r�b�g�Ƃ́�����[0 5] �̂���[5]��Ԃ�
	} else {
		YuukouIX = 0x0F & (*p);
	}

	return YuukouValueArray[YuukouIX];
}

// �F�D�֌W�l���Z�b�g����B
void setYuukouKankei(int iDaimyoA_ID, int iDaimyoB_ID, int iValue) {

	// �Y���̗F�D�֌W��񂪑��݂���A�h���X�𓾂�B�`�Ƃa�͒l����������邩������Ȃ��B���ӁB
	byte *p = getYuukouKannkeiPointer( iDaimyoA_ID, iDaimyoB_ID );
	if (!p) return;


	// �����̓����@�C�̂ŗF�D�֌W�l�Ƃ��ĂO���w�肵�����̂Ƃ���B100�I�[�o�[��100�ɂ��Ƃ��B
	if ( iValue < 0 )	{ iValue = 0;	}
	if ( iValue > 100 ) { iValue = 100; }

	int YuukouIX = 0;
	// �Z�b�g�������F�D�֌W�l�ƁA�F�D�֌W�l�̔z����r���āA�����Ƃ��߂��؎̂Ēl��index�����߂�B
	// �Ⴆ�ΗF�D�֌W�l�Ƃ���53�Ƃ����悤�Ɏw�肳��Ă��A���̂悤�Ȓl�ɂ͏o���Ȃ��̂ŁA50�Ƃ���킯���B
	// ������50�́AYuukouValueArray��index==5�ɑ��݂���B����5�̂悤�Ȓl�����߂����B
	for ( int i=0; i < sizeof(YuukouValueArray)/sizeof(YuukouValueArray[0]); i++) {
		if ( iValue < YuukouValueArray[i] ) {

			YuukouIX = i-1;	// �l���������̂ŁA�P�O��index�ɂ��Ƃ��B

			if ( YuukouIX < 0 ) { YuukouIX = 0; }  // ���_�゠�肦�Ȃ����A���������킩���̂ňꉞ�B
			break;
		}
	}

	// �A�h���X�͋��܂������A�P�̃A�h���X��1�o�C�g���ɂ́A�Q�̑喼�������Ă���B
	// �喼�ԍ��������Ȃ��ʌ��A�����Ȃ牺�ʌ��ł���B
	// ����喼��������
	if ( iDaimyoB_ID % 2==0 ) {								 // �����ł���΁A�㔼���̃r�b�g�Ƃ́�����[0 5] �̂���[0]��Ԃ�
		YuukouIX = YuukouIX << 4;
		// ��ʃo�C�g��YuukouIX���V�t�g�������̂��A���ʃo�C�g�͉��ʃo�C�g�̕����������o��
		*p = YuukouIX | (0x0F & (*p));

	// ����喼�����										 // ��ł���΁A�������̃r�b�g�Ƃ́�����[0 5] �̂���[5]��Ԃ�
	} else {
		// ��ʃo�C�g�͂��̂܂܍̗p�ŁA���ʃo�C�g�́AYuukouIX���̂܂܂ŁB
		*p = (0xF0 & (*p)) | YuukouIX;
	}
}

byte* getDoumeiKankeiPointer(int& iDaimyoA_ID, int& iDaimyoB_ID) {

	// �͈̓`�F�b�N
	if ( 0 <= iDaimyoA_ID && iDaimyoA_ID < GAMEDATASTRUCT_DAIMYO_NUM &&
 		 0 <= iDaimyoB_ID && iDaimyoB_ID < GAMEDATASTRUCT_DAIMYO_NUM ) {
	} else {
		return NULL;
	}

	// �喼�`�Ƒ喼�a�ŁA�喼�`�̕����l���������ꍇ�A����ւ���B
	// �Ȃ��Ȃ瓯���֌W�̒l�́A�喼�`���傫�����̑g�ݍ��킹�e�[�u���ɓ����Ă��邩��B
	if (iDaimyoA_ID < iDaimyoB_ID ) {
		// �Q����ւ�
		iDaimyoA_ID ^= iDaimyoB_ID;
		iDaimyoB_ID ^= iDaimyoA_ID;
		iDaimyoA_ID ^= iDaimyoB_ID;
	}


	byte *p = (byte *)GAMEDATASTRUCT_DOUMEI_ADDRESS;
	
	// �ȉ�2�l�̑喼�h�c���A�����֌W�l�������Ă��郁�����̃A�h���X�����߂�
	p += iDaimyoA_ID * ( GAMEDATASTRUCT_DAIMYO_NUM / 2 ); // �������P�̂`�喼������A�a�喼�ւ̓������P�o�C�g�ŕ\������Ă���΁A
															 // �V�Q�喼����̂ŁA�V�Q�o�C�g�ŕ\�������B
															 // �Ƃ��낪���ۂɂ́A4�r�b�g�ŕ\�������̂ŁA
															 // �P�̑喼�ɂԂ牺����\���̃f�[�^�Ƃ��ẮA�����̂R�U�o�C�g�ł���B

	p += iDaimyoB_ID / 2;									 // ��Ɠ������R�ƂȂ�B�P�̑喼��(�����֌W�\����)�̒��ł�
															 // �P�̑���喼�Ƃ̓����l�Ɏg���Ă���͔̂��o�C�g�B
															 // ����ă|�C���^�̈ʒu�Ƃ��ẮA/2 ���Ă����A����������ŁA��ŉ��Z��U�蕪����B
	return p;
}

int isDoumeiKankei(int iDaimyoA_ID, int iDaimyoB_ID) {

	// �Y���̓����֌W��񂪑��݂���A�h���X�𓾂�B�`�Ƃa�͒l����������邩������Ȃ��B���ӁB
	byte *p = getDoumeiKankeiPointer( iDaimyoA_ID, iDaimyoB_ID );
	if (!p) return 0;

	// �A�h���X�͋��܂������A�P�̃A�h���X��1�o�C�g���ɂ́A�Q�̑喼�������Ă���B
	// �喼�ԍ��������Ȃ��ʌ��A�����Ȃ牺�ʌ��ł���B
	// ����喼��������
	if ( iDaimyoB_ID % 2==0 ) {								 // �����ł���΁A�㔼���̃r�b�g
		return 0x10 & (*p);
	// ����喼�����										 // ��ł���΁A�������̃r�b�g
	} else {
		return 0x01 & (*p);
	}
}
int isKoninKankei(int iDaimyoA_ID, int iDaimyoB_ID) {

	// �Y���̍����֌W��񂪑��݂���A�h���X�𓾂�(�����Ɠ����ꏊ)�B�`�Ƃa�͒l����������邩������Ȃ��B���ӁB
	byte *p = getDoumeiKankeiPointer( iDaimyoA_ID, iDaimyoB_ID );
	if (!p) return 0;

	// �A�h���X�͋��܂������A�P�̃A�h���X��1�o�C�g���ɂ́A�Q�̑喼�������Ă���B
	// �喼�ԍ��������Ȃ��ʌ��A�����Ȃ牺�ʌ��ł���B

	if ( iDaimyoB_ID % 2==0 ) {								 // �����ł���΁A�㔼���̃r�b�g
		return (0x20 & (*p)) > 0;							 // �Q�Ƃ̔�r�ł͊ԈႤ��������Ȃ��̂ŁA�ꉞBOOL���ۂ����Ă����B
	// ����喼�����										 // ��ł���΁A�������̃r�b�g
	} else {
		return (0x02 & (*p)) > 0;							 // �Q�Ƃ̔�r�ł͊ԈႤ��������Ȃ��̂ŁA�ꉞBOOL���ۂ����Ă����B
	}
}

void setDoumeiKankei(int iDaimyoA_ID, int iDaimyoB_ID, BOOL doDoumei) {

	// �Y���̓����֌W��񂪑��݂���A�h���X�𓾂�B�`�Ƃa�͒l����������邩������Ȃ��B���ӁB
	byte *p = getDoumeiKankeiPointer( iDaimyoA_ID, iDaimyoB_ID );
	if (!p) return;
	
	// �A�h���X�͋��܂������A�P�̃A�h���X��1�o�C�g���ɂ́A�Q�̑喼�������Ă���B
	// �喼�ԍ��������Ȃ��ʌ��A�����Ȃ牺�ʌ��ł���B
	// ����喼��������
	if ( iDaimyoB_ID % 2==0 ) {								 // �����ł���΁A�㔼���̃r�b�g
		if (doDoumei) {
			*p = *p | 0x10;
		}
		else {
			*p = *p & 0xEF;
		}
		// ����喼�����										 // ��ł���΁A�������̃r�b�g
	} else {
		if (doDoumei) {
			*p = *p | 0x01;
		}
		else {
			*p = *p & 0x0E;
		}
	}
}


void setKoninKankei(int iDaimyoA_ID, int iDaimyoB_ID, BOOL doKonin) {

	// �Y���̍����֌W��񂪑��݂���A�h���X�𓾂�(�����Ɠ����ꏊ)�B�`�Ƃa�͒l����������邩������Ȃ��B���ӁB
	byte *p = getDoumeiKankeiPointer( iDaimyoA_ID, iDaimyoB_ID );
	if (!p) return;
	
	// �A�h���X�͋��܂������A�P�̃A�h���X��1�o�C�g���ɂ́A�Q�̑喼�������Ă���B
	// �喼�ԍ��������Ȃ��ʌ��A�����Ȃ牺�ʌ��ł���B
	// ����喼��������
	if ( iDaimyoB_ID % 2==0 ) {								 // �����ł���΁A�㔼���̃r�b�g
		if (doKonin) {
			*p = *p | 0x20;
		}
		else {
			*p = *p & 0xEF;
		}
		// ����喼�����										 // ��ł���΁A�������̃r�b�g
	} else {
		if (doKonin) {
			*p = *p | 0x02;
		}
		else {
			*p = *p & 0x0E;
		}
	}
}




int DaimyoHasCastleArray[GAMEDATASTRUCT_DAIMYO_NUM] = {}; // �e�X�̑喼�����鐔

//�ő吨��(�ł������鐔������)�喼�𓾂�
int GetMaximumInfluenceDaimyo() {
	// �܂��A�ꉞ�Ă΂��x�Ƀ��Z�b�g
	for (int iDaimyoID=0; iDaimyoID < GAMEDATASTRUCT_DAIMYO_NUM; iDaimyoID++ ) {
		DaimyoHasCastleArray[iDaimyoID] = 0;
	}

	for (int iCastleID=0; iCastleID < GAMEDATASTRUCT_CASTLE_NUM; iCastleID++ ) {
		// �����喼�ԍ��ɏ]���āA�J�E���g�𑝂₵�Ă䂭�B
		int iGundanID = nb6castle[iCastleID].attach-1;
		if ( iGundanID < GAMEDATASTRUCT_GUNDAN_NUM ) { // �N�ɂ��������ĂȂ���͂��̏����Œe�����
			int iDaimyoID = nb6gundan[iGundanID].attach-1;
			DaimyoHasCastleArray[iDaimyoID]++;
		}
	}

	// �ő�̏鎝���̑喼ID��T��
	int iMaximumInfluenceDaimyoID = 0;

	// ��r����Ȃ̂�1�X�^�[�g�ŗǂ�
	for (int iDaimyoID=1; iDaimyoID < GAMEDATASTRUCT_DAIMYO_NUM; iDaimyoID++ ) {
		if ( DaimyoHasCastleArray[iDaimyoID] > DaimyoHasCastleArray[iMaximumInfluenceDaimyoID] ) {
			iMaximumInfluenceDaimyoID = iDaimyoID;
		}
	}

	return iMaximumInfluenceDaimyoID;
}

/*
004D1490  C0 FD 4C 00 B4 FD 4C 00 A8 FD 4C 00 9C FD 4C 00  ��L.��L.��L.�EL.
004D14A0  8C FD 4C 00 80 FD 4C 00 70 FD 4C 00 60 FD 4C 00  �EL.��L.p�L.`�L.
004D14B0  54 FD 4C 00 48 FD 4C 00                          T�L.H�L.
*/
char *getGoyouSyouninMei(int iGoyousyouninID) {
	if ( 0 <= iGoyousyouninID && iGoyousyouninID <= GAMEDATASTRUCT_SYOUNIN_NUM ) {
		int *pArrayGoyouSyoninName = (int *)GAMEDATASTRUCT_GOYOUSYOUNINNAME_ARRAY;
		return (char *)(pArrayGoyouSyoninName[iGoyousyouninID]);
	} else {
		return "";
	}
}











// �ƕ�̎�ނ̕�����ւ̃|�C���^�̃A�h���X�ꗗ
int GAMEDATASTRUCT_KAHOUTYPELIST_POINTER[GAMEDATASTRUCT_KAHOUTYPE_NUM] = {NULL};




// �ƕ�^�C�v��ID���當����𓾂�
char* getKahouTypeName(byte type) {
	char *pszKahouType = NULL;
	if ( type < GAMEDATASTRUCT_KAHOUTYPE_NUM ) {
		pszKahouType = (char *)(GAMEDATASTRUCT_KAHOUTYPELIST_POINTER[type]);
		return pszKahouType;
	}
	return NULL;
}


// �Z�[�u�f�[�^�̃t�@�C����
char *psznb6savedatafilename = (char *)GAMEDATASTRUCT_SAVEDATAFILENAME_ADDRESS;



// �V�C���𓾂� 0:���� 1:�܂� 2:�J 3:��
byte getWeather() {

	byte *weather = (byte *)GAMEDATASTRUCT_WEATHER_ADDRESSS;

	return *weather;
}



// �V�C�̗\��
void reserveWeather(int eTenki, int iContinueTurn) {
	iReservedWeatherOnChangeWeather.clear();
	if ( getRemainTurnInBattle() > 0 ) {
		TReservedTenki tk = { getRemainTurnInBattle(), eTenki, iContinueTurn };
		iReservedWeatherOnChangeWeather.push_back(tk);
	}
}
// �V�C�̗\��
void WINAPI Extern_reserveWeather(int eTenki, int iContinueTurn) {
	reserveWeather(eTenki, iContinueTurn);
}


// �푈�ł��łɉ��^�[���o�߂�����
byte getHowMuchTurnProceesInBattle() {

	byte *turn = (byte *)GAMEDATASTRUCT_HOWMUCHTURNPROCEED_INBATTLE;

	return *turn;
}

// �푈�ŉ��^�[���c���Ă��邩�B�������ł͂Ȃ��A�����v�Z
extern int iCurIncrementTurnInBattle;
int getRemainTurnInBattle() {
	// �퓬�^�[�����ȊO�ł���΁A-1��Ԃ��B
	if (iCurIncrementTurnInBattle < 0) {
		return iCurIncrementTurnInBattle;
	}
	// �푈���Ȃ�A30����o�߃^�[�������������̂�Ԃ��B
	return 31-iCurIncrementTurnInBattle;
}

// �O���Q�Ɨp
int WINAPI Extern_getRemainTurnInBattle() {
	return getRemainTurnInBattle();
}


// �S�C�`���ς݂��ǂ���
bool isTeppouDenrai() {
	byte *bTeppouDenrai = (byte *)GAMEDATASTRUCT_EVENT_GROUP1_ADDRESSS;

	int isTeppouDenrai = *bTeppouDenrai & 0x01; // (= b0b00000001) 

	if (isTeppouDenrai) { return true; } else { return false; }
}

// �S�C�`������߂�
void UnsetTeppouDenrai() {
	byte *bTeppouDenrai = (byte *)GAMEDATASTRUCT_EVENT_GROUP1_ADDRESSS;

	*bTeppouDenrai = *bTeppouDenrai & 0xFE; // ( & 11111101 ) ���邱�ƂŁA�S�C�`�������������B 
}


// �L���X�g���`���ς݂��ǂ���
bool isChristDenrai() {
	byte *bChristDenrai = (byte *)GAMEDATASTRUCT_EVENT_GROUP1_ADDRESSS;

	int isChristDenrai = *bChristDenrai & 0x02; // (= b0b00000010) 

	if (isChristDenrai) { return true; } else { return false; }
}

// ���y�邪���ݍς݂��ǂ���
bool isAzuchiCastleComplete() {
	byte *bAzuchiCastleComplete = (byte *)GAMEDATASTRUCT_EVENT_GROUP1_ADDRESSS;

	int isAzuchiCastleComplete = *bAzuchiCastleComplete & 0x08; // (= b0b00001000) 

	if (isAzuchiCastleComplete) { return true; } else { return false; }
}

// ���邪���ݍς݂��ǂ���
bool isOsakaCastleComplete() {
	byte *bOsakaCastleComplete = (byte *)GAMEDATASTRUCT_EVENT_GROUP2_ADDRESSS;

	int isOsakaCastleComplete = *bOsakaCastleComplete & 0x40; // (= b0b01000000) 

	if (isOsakaCastleComplete) { return true; } else { return false; }
}

// �L���X�g���`������߂�
void UnsetChristDenrai() {
	byte *bChristDenrai = (byte *)GAMEDATASTRUCT_EVENT_GROUP1_ADDRESSS;

	*bChristDenrai = *bChristDenrai & 0xFD; // ( & 11111101 ) ���邱�ƂŁA�S�C�`�������������B 
}



// SE�͗L�����H
bool isSEValid() {

	byte bSEValid = 0;

	bSEValid = nb6settings.se; // (= b0b00000001) 

	if (bSEValid) { return true; } else { return false; }

}

// BGM�͗L�����H
bool isBGMValid() {

	byte bBGMValid = 0;

	bBGMValid = nb6settings.bgm; // (= b0b00000010) 

	if (bBGMValid) { return true; } else { return false; }
}

// ���[�r�[�͗L�����H
bool isMovieValid() {

	byte bMovieValid = 0;

	bool bMovieInValid = nb6settings.movie_is_off; // (= b0b00010000) 

	if (!bMovieInValid) { return true; } else { return false; }

}

// _INMM���[�h���H
bool is_InmmMode() {
	char *winmm = (char *)GAMEDATASTRUCT_WINMM_DLL_NAME_ADDRESS;
	if ( strcmp( winmm, "_INMM.dll") == 0 ) {
		return true;
	} else if ( TSModCommand::Environment::cmdBGMMode == 1 || TSModCommand::Environment::cmdBGMMode == 2 ) {
		return true;
	}
	return false;
}


// �f�����[�h���H
bool is_DemoMode() {
	for ( int iPG=0; iPG<GAMEDATASTRUCT_PLAYERS8_NUM; iPG++) {
		// �v���C���[���S�����Ă���R�c�̂h�c���m��
		int iGundanID = nb6players8[iPG].gundan;

		// �P�ł��v���C���S���R�c�ԍ���0xFFFF�ȊO�̒l������΁A�f�����[�h�ł͂Ȃ��B
		if ( iGundanID != 0xFFFF ) {
			return false;
		}
	}

	// �S��0xFFFF�Ȃ�f�����[�h
	return true;
}


// �����̗֔z��B(�v�Z�p)
int arrayAisho[32] = {8,9,10,11,12,13,14,15,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,0,1,2,3,4,5,6,7};

// A������B�����̑����̍����𓾂�
// �ŏ�0(�����ō�) �ő�8(�����ň�)
int getAishoDiff(int iAishoA, int iAishoB) {

	// �܂��AA�����̑����ʒu�𒆉�������
	int iAAishoIXInArray = 0;
	for (int iAishoID=8; iAishoID<8+GAMEDATASTRUCT_AISHOU_NUM; iAishoID++ ) {
		// A�̑����l����������A������A�̈ʒu
		if ( arrayAisho[iAishoID] == iAishoA ) {
			iAAishoIXInArray = iAishoID;
		}
	}

	// �s���Ȑ����̏ꍇ�́A�������͍ł��������̂Ƃ���B
	if ( iAAishoIXInArray == 0 ) { return 8; }

	// B�����̑����ʒu��A������-8����X�^�[�g
	int iBAishoIXInArray = 0;
	for (int iAishoID=iAAishoIXInArray-8; iAishoID<iAAishoIXInArray+8; iAishoID++ ) {
		// A�̑����l����������A������A�̈ʒu
		if ( arrayAisho[iAishoID] == iAishoB ) {
			iBAishoIXInArray = iAishoID;
		}
	}
	
	// �s���Ȑ����̏ꍇ�́A�������͍ł��������̂Ƃ���B
	if ( iBAishoIXInArray == 0 ) { return 8; }

	// �Q�̑����̈ʒu�̍�����������
	return abs(iBAishoIXInArray-iAAishoIXInArray);
}


/*
 * 2�l�̕���ID���瑊���̈Ⴂ�𓾂�B
 * �ŏ�0(�����ō�) �ő�8(�����ň�)
 */
int getBushouAishoDiff(int iBushouAID, int iBushouBID) {
	// ����A������B���͈͓��Ɏ��܂��Ă���
	if ( 0 <= iBushouAID && iBushouAID < GAMEDATASTRUCT_BUSHOU_NUM &&  0 <= iBushouBID && iBushouBID < GAMEDATASTRUCT_BUSHOU_NUM ) {
		// �����̑����̍���Ԃ�
		return getAishoDiff( nb6bushouref[iBushouAID].affinity, nb6bushouref[iBushouBID].affinity );
	}
	// ��Ԉ����l��Ԃ��Ă���
	else {
		return 8;
	}
}









