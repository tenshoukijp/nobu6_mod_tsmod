#include "GameDataStruct.h"
#include "CommonGlobalHandle.h"
#include "MessageInflection.h"
#include "MessageInflectionSurfix.h"
#include "MessageInflectionEnding.h"
#include "FuncFukidashiDialog.h"
#include "FuncMessageDialog.h"
#include "FuncMoveCamera.h"
#include "FuncFadeInOut.h"
#include "FuncSelectBushouMultiDialog.h"
#include "TenShougiMod.h"

// �V����Mod.dll�̃��W���[���n���h��
extern HMODULE hTenShougiModDll;

int isTenShougiModeDialog = 0; // �V�������[�h�̃_�C�A���O(���ꕺ�ȂȂǂ̕\�L�Ǘ��ɗ��p����)

extern void ModifyBushouDaniTitle(); // �������X�g�o���ۂ̃J�����^�C�g�����u���ꕺ�ԁv���u�i�ʁv�ւƕύX����B
extern void InitModifyBushouListTitle(); // �������X�g�o���ۂ̃J�����^�C�g�����u���ꕺ���v�Ƃ����f�t�H���g��Ԃɐݒ�B�N�����ɂ��Ă΂��B


// �G�߂̑ւ���(�v���C���^�[��������Ă�����)�ɁA�R�t�Ƃ̉�b�ɂ���āA�V�������n�܂邱�Ƃ�����B
void EXServieExecute_TenShougiCheck() {

	// �V�����n�̃t�@�C���������I�ɑ��݂��Ă��Ȃ��Ȃ�΁A�������Ȃ��B
	if ( ! IsTenShougiModFilesExists() ) {
		return;
	}

	// TenShougiMod.dll������͂��Ȃ̂ŁA�ǂݍ���
	if (!hTenShougiModDll) {
		hTenShougiModDll = LoadLibrary("TenShougiMod.dll");
	}

	// ���݂͂��邪�Ȃ����ǂ߂Ȃ��B�_��
	if (!hTenShougiModDll) { return; }

	// �V���������łɊJ���Ă���Ȃ�΁A�������Ȃ��B
	if ( p_sgIsTenShougiFormShown ) {
		if ( p_sgIsTenShougiFormShown() ) {
			return;
		}
	}

	int iTurnGundanID = nb6turn.gundan-1;

	// �R�c���͈͂Ɏ��܂��Ă��Ȃ���΁A�������Ȃ��B
	if (! (0 <= iTurnGundanID && iTurnGundanID < GAMEDATASTRUCT_GUNDAN_NUM) ) {
		return;
	}

	// �R�t�̕����ԍ�index
	int iGBushouID = getGunshiBushouID( iTurnGundanID );

	// �R�t�����Ȃ���΁A�������Ȃ�
	if ( !(0 <= iGBushouID && iGBushouID < GAMEDATASTRUCT_BUSHOU_NUM) ) {
		return;
	}

	// �R�t�����S���Ă���Ȃ�΁A�������Ȃ��B
	if ( nb6bushouname[iGBushouID].State == 7 || nb6bushouname[iGBushouID].battleDeath == 1 ) {
		return;
	}

	int iTurnDaimyoID = nb6turn.daimyo-1;

	// �喼���͈͂Ɏ��܂��Ă��Ȃ���΁A�������Ȃ��B
	if (! (0 <= iTurnDaimyoID && iTurnDaimyoID < GAMEDATASTRUCT_DAIMYO_NUM) ) {
		return;
	}

	// �喼�̕����ԍ�index
	int iDBushouID = nb6daimyo[iTurnDaimyoID].attach - 1;

	// �喼�����Ȃ���΁A�������Ȃ�
	if ( !(0 <= iDBushouID && iDBushouID < GAMEDATASTRUCT_BUSHOU_NUM) ) {
		return;
	}

	// �喼�����S���Ă���Ȃ�΁A�������Ȃ��B
	if ( nb6bushouname[iDBushouID].State == 7 || nb6bushouname[iDBushouID].battleDeath == 1 ) {
		return;
	}

	int iCastleID = nb6daimyo[iTurnDaimyoID].castle -1;

	// �喼�̋���
	if ( !(0 <= iCastleID && iCastleID < GAMEDATASTRUCT_CASTLE_NUM) ) {
		return;
	}

	//----------------------�喼�ƌR�t��������

	// ���͂��X�͍ő吨�͂ł���B����āA�]�T�̂�������Ȃ̂ŁA�������w���B
	if ( nb6year.season==0 ) {
		FuncFadeOut();
		FuncMoveCamera(iCastleID);
		FuncFadeIn();

		stringstream ssmsg1;
		ssmsg1 
			<< �����聓(iGBushouID, iDBushouID) << "\x0A"
			<< "�����̎w�����߂Ȃ�" << "\x0A"
			<< "������" << ���ł�����(iGBushouID, iDBushouID);
		FuncLeftNormalFukidashiMessage(iGBushouID, (char *)ssmsg1.str().c_str());

		int result = FuncOkCancelDialog( "�N���Ə������w���܂����H");

		// �I�𕐏��_�C�A���O�ɕ\�����邽�߂̕����̃��X�g
		// �N��P�T�ۈȏ�ŁA����łȂ��l�B
		vector<int> bushou_list; 
		for ( int iBushouID = 0; iBushouID < GAMEDATASTRUCT_BUSHOU_NUM; iBushouID++) {
			// �R�c�ԍ�������
			if ( nb6bushouref[iBushouID].gundan-1 == iTurnGundanID ) {
				// ���ɂĎ���ł��Ȃ��B����łȂ��B�喼�ł͂Ȃ��B
				if ( nb6bushouname[iBushouID].State != 7 && nb6bushouname[iBushouID].State != 0 && nb6bushouname[iBushouID].battleDeath != 1 ) {
					if ( getYearsOld(iBushouID) >= 15 ) { // 15�ۈȏ�ł���B
						bushou_list.push_back(iBushouID);
					}
				}
			}
		}
		vector<int> result_list;

		if ( result == 1 ) {  // YES�̎�
			// �������X�g�B�I���\�ő�l�����w��B�ꉞ�W�l�܂ŁB�f�t�H���g�͂悭���肪���ȂT�l�B
			isTenShougiModeDialog = 1; // ���ꕺ�Ȃ̕������i�ʂƂȂ�B
			ModifyBushouDaniTitle(); // ���ꕺ���̕����̃J���������u���́v�Ƃ���B

			result_list = FuncSelectBushouMultiDialog(bushou_list, Filter::Bushou::���ꕺ��, 1 );

			InitModifyBushouListTitle(); // ���ꕺ���̕����̃J���������u���ꕺ�ԁv�Ɩ߂��B
			isTenShougiModeDialog = 0; // ���ꕺ�Ȃ̕����̕\�L��߂��B
		}

		if ( result_list.size() > 0 ) {
			int iWBushouID = result_list[0];
			stringstream ssmsg4;
			ssmsg4
				<< "�ʔ�����" << ���ł���(iDBushouID, iGBushouID, 0xFFFF) << ���ȁi�Q�j��(iDBushouID, iGBushouID, 0xFFFF) <<  "\x0A"
				<< "��������" << �����聓(iDBushouID, iWBushouID) << "��\x0A"
				<< "����" << �����܂��傤��(iDBushouID, iGBushouID);
			FuncRightNormalFukidashiMessage(iDBushouID, (char *)ssmsg4.str().c_str());

			TenShougiModBgn(iDBushouID, iWBushouID);
		} else {
			stringstream ssmsg4;
			ssmsg4
				<< "�ʔ�����" << ���ł���(iDBushouID, iGBushouID, 0xFFFF) << "��\x0A"
				<< "�ʂ̋@���" << �����܂��傤��(iDBushouID, iGBushouID);
			FuncRightNormalFukidashiMessage(iDBushouID, (char *)ssmsg4.str().c_str());
		}

		// �����o������
		FuncLeftNormalFukidashiDelete();
		FuncRightNormalFukidashiDelete();
	}
}




// �������X�g���̃^�C�g���̕�����╝��ύX����B
static char szStringSpecialDani[] = "����";
static void ModifyBushouDaniTitle() {

	// ��q�R�����g�́u�R�S�v�̕�����̃|�C���^���u�i�ʁv���w�����̂Ƃ���B
	int iAdress = (int)szStringSpecialDani;
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(0x4C9084), &iAdress, 4, NULL);

	// ����4�o�C�g(�S�p2������)�ɏ���������B
	int iKitetsuWidth=0x4;
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(0x4C9088), &iKitetsuWidth, 4, NULL);
}


static char* szBushouDaniList[] = { "�܋�", "�l��", "�O��", "��", "�ꋉ", "���i", "��i", "�O�i", "�l�i", "�ܒi", "�Z�i", "���i", "���i", "��i", "���l" };
 // �w��̒i�ʂ𓾂�B
char *getSpecialDaniStringOfBushou(int iBushouID) {
	// �͈͂ɓ����Ă���B
	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM) {

		// 
		int power = GetShogiPower(iBushouID);

		return szBushouDaniList[power];


	} else {
		return "�@"; // �ꉞ�Ԃ���΂Ȃ��悤NULL�ł͂Ȃ��S�p�󔒂�Ԃ��Ă���
	}
}

// �Ώۂ̊��m�̊��͂�Ԃ��B
int GetShogiPower(int iBushouID) {
	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM) {

		// �����̒i�ʃp���[�����߂�B(����+�퓬+�q�d)*���{
		int power1 = nb6bushouref[iBushouID].maxgov + nb6bushouref[iBushouID].maxint + nb6savedata_ex.bushou_ex[iBushouID].education + nb6savedata_ex.bushou_ex[iBushouID].command;
		int power2 = nb6bushouref[iBushouID].maxgov + nb6bushouref[iBushouID].maxbat + nb6bushouref[iBushouID].maxint + nb6bushouref[iBushouID].ambition;
		
		int power = 0;

		power = max(power1, power2);

		power += (
					getBushouAptitude(iBushouID, APTITUDE_ASIGARU)+
					getBushouAptitude(iBushouID, APTITUDE_KIBA)+
					getBushouAptitude(iBushouID, APTITUDE_TEPPOU)+
					getBushouAptitude(iBushouID, APTITUDE_SUIGUN)+
					max( getBushouAptitude(iBushouID, APTITUDE_YUMI) ,  getBushouAptitude(iBushouID, APTITUDE_YARI) )
				  ) * 10;

		power = int(power / 55); // �ő��650������ƂȂ̂ŁA55�Ŋ����Ă݂�B11�������

		power = min(power, sizeof(szBushouDaniList)/sizeof(szBushouDaniList[0]) ); // �ő�ł��z��̂ǂꂩ�͎w�������悤��

		return power;
	} else {
		return 0;
	}
}