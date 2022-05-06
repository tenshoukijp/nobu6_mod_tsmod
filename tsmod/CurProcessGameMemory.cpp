#include "WinTarget.h"
#include <mmsystem.h>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <functional>
#include <type_traits>

#include "CommonGlobalHandle.h"
#include "GameDataStruct.h"
#include "TSModCommand.h"
#include "OnViewingBattleMenuInBattle.h"
#include "EXServiceExecute.h"
#include "MessageInflection.h"
#include "MessageInflectionEnding.h"
#include "VariableNameRegistAndJudge.h"
#include "SoundServer.h"
#include "FactoryGeneralBushouName.h"
#include "DijkstraCastle.h"


using namespace std;



static DWORD preTimeGetTime = 0;


void copyAllBushouSoldiersOriginToCopy() {

	// ���m�������A�������と���ݒl�ۑ��Ƃ��ăR�s�[
	for ( int iBushouID=0; iBushouID < GAMEDATASTRUCT_BUSHOU_NUM; iBushouID++ ) {
		nb6bushoucopy[iBushouID].soldier = nb6bushouref[iBushouID].soldier;
	}
}



// �ȉ��͌���1��ǂݍ��݌n�B�����������肷�邽�߁A��O�͂���B
void readUpdateGameDataStructSoldierMax() {
	// ����MAX���
	ReadProcessMemory( 
		CommonGlobalHandle::curProcess,
		(LPCVOID)(GAMEDATASTRUCT_SOLDIERMAX_ADDRESS), 
		nb6soldiermax, 
		sizeof(nb6soldiermax),
		NULL);
}

void writeUpdateGameDataStructSoldierMax() {
	// ����MAX��񏑂�����
	WriteProcessMemory( 
		CommonGlobalHandle::curProcess,
		(LPVOID)(GAMEDATASTRUCT_SOLDIERMAX_ADDRESS), 
		nb6soldiermax, 
		sizeof(nb6soldiermax),
		NULL);
}


extern int ServiceExecuteRegenerationAllSaveBeginSoldier[GAMEDATASTRUCT_BUSHOU_NUM];
extern int ServiceExecuteRegenerationAllIncrSoldierInBattle[GAMEDATASTRUCT_BUSHOU_NUM];


// �܂��ǂݍ��񂾂��Ƃ��Ȃ�
void setCurProcessGameMemory() {

	// �\���̂̒�`�̎��O�R���p�C���`�F�b�N
	static_assert(sizeof(SAVEDATA_EX) == 299708, "SAVEDATA_EX�̃T�C�Y�������Ă��Ȃ�");
	static_assert(sizeof(SAVEDATA_EX_COMMON) == 4000, "SAVEDATA_EX_COMMON�̃T�C�Y�������Ă��Ȃ�");


	if ( (sizeof(SAVEDATA_EX) != 299708) || (sizeof(SAVEDATA_EX_COMMON) != 4000) ) {
		MessageBox(NULL, "�Z�[�u�f�[�^�T�C�Y�s��", "�Z�[�u�f�[�^�T�C�Y�s��", NULL);
	}


	DWORD curTimeGetTime = timeGetTime();
	int RefleshTime = 5000; // �_�b

	// �K��̊Ԋu���Ԃ��߂���
	if ( int(curTimeGetTime - preTimeGetTime) > int(RefleshTime) ) {

		preTimeGetTime = curTimeGetTime;
		
		// �^�C�~���O�����ēǂݍ��ݍX�V����K�v������Q�[���f�[�^�Q���X�V
		copyAllBushouSoldiersOriginToCopy();

		//----�ȉ��͂P�񂾂��ǂݍ��݌n

		// ��E����
		// ��E���ނ�exe����̌Œ�ǂݍ��݂Ȃ̂�1�񂾂��ǂݍ���
		if ( ! GAMEDATASTRUCT_POSITIONTYPELIST_POINTER[1] ) {
			ReadProcessMemory( 
				CommonGlobalHandle::curProcess,
				(LPCVOID)(GAMEDATASTRUCT_POSITIONTYPELIST_POINTER_ADDRESS), 
				GAMEDATASTRUCT_POSITIONTYPELIST_POINTER, 
				4 * GAMEDATASTRUCT_POSITIONTYPE_NUM,
				NULL);
		}

		// �ƕ󕪗�
		// �ƕ󕪗ނ�exe����̌Œ�ǂݍ��݂Ȃ̂�1�񂾂��ǂݍ���
		if ( ! GAMEDATASTRUCT_KAHOUTYPELIST_POINTER[1] ) {
			ReadProcessMemory( 
				CommonGlobalHandle::curProcess,
				(LPCVOID)(GAMEDATASTRUCT_KAHOUTYPELIST_POINTER_ADDRESS), 
				GAMEDATASTRUCT_KAHOUTYPELIST_POINTER, 
				4 * GAMEDATASTRUCT_KAHOUTYPE_NUM,
				NULL);
		}


		// ����MAX���́A�Œ�ǂݍ��݂Ǝv����̂�1�񂾂��ǂݍ���
		if ( nb6soldiermax[1].soldiermax < 1 ) { // [0] �� ������0�Ȃ̂Łc

			// �������̕�����ǂ�ł���
			readUpdateGameDataStructSoldierMax();

			// TSMod.txt�Ŏw�肪����Ώ㏑������B
			if ( TSModCommand::AllBushou::cmdSoldierBaseMaxOfDaimy != 0) {
				nb6soldiermax[1].soldiermax			= TSModCommand::AllBushou::cmdSoldierBaseMaxOfDaimy;
				nb6soldiermax_custom[1].soldiermax	= TSModCommand::AllBushou::cmdSoldierBaseMaxOfDaimy + 20;
			}
			if ( TSModCommand::AllBushou::cmdSoldierBaseMaxOfSyuku != 0 ) {
				nb6soldiermax[2].soldiermax			= TSModCommand::AllBushou::cmdSoldierBaseMaxOfSyuku;
				nb6soldiermax_custom[2].soldiermax	= TSModCommand::AllBushou::cmdSoldierBaseMaxOfSyuku + 20;
			}
			if ( TSModCommand::AllBushou::cmdSoldierBaseMaxOfKarou != 0 ) {
				nb6soldiermax[3].soldiermax			= TSModCommand::AllBushou::cmdSoldierBaseMaxOfKarou;
				nb6soldiermax_custom[3].soldiermax	= TSModCommand::AllBushou::cmdSoldierBaseMaxOfKarou + 20;
			}
			if ( TSModCommand::AllBushou::cmdSoldierBaseMaxOfBusyo != 0 ) {
				nb6soldiermax[4].soldiermax			= TSModCommand::AllBushou::cmdSoldierBaseMaxOfBusyo;
				nb6soldiermax_custom[4].soldiermax	= TSModCommand::AllBushou::cmdSoldierBaseMaxOfBusyo + 20;
			}
			if ( TSModCommand::AllBushou::cmdSoldierBaseMaxOfSamra != 0 ) {
				nb6soldiermax[5].soldiermax			= TSModCommand::AllBushou::cmdSoldierBaseMaxOfSamra;
				nb6soldiermax_custom[5].soldiermax	= TSModCommand::AllBushou::cmdSoldierBaseMaxOfSamra + 20;
			}
			if ( TSModCommand::AllBushou::cmdSoldierBaseMaxOfAshig != 0 ) {
				nb6soldiermax[6].soldiermax			= TSModCommand::AllBushou::cmdSoldierBaseMaxOfAshig;
				nb6soldiermax_custom[6].soldiermax	= TSModCommand::AllBushou::cmdSoldierBaseMaxOfAshig + 20;
			}

			// �������m���Ɋւ��鑀�삪����ꍇ�́A1��A�b�v�f�[�g�̂��߂ɏ���������
			if ( TSModCommand::AllBushou::cmdPositionInfluenceSoldierMax != 0 ) {
				nb6soldiermax[1].soldiermax = nb6soldiermax_custom[1].soldiermax; // �喼
				nb6soldiermax[2].soldiermax = nb6soldiermax_custom[2].soldiermax; // �h�V
				nb6soldiermax[3].soldiermax = nb6soldiermax_custom[3].soldiermax; // �ƘV
				nb6soldiermax[4].soldiermax = nb6soldiermax_custom[4].soldiermax; // ����
				nb6soldiermax[5].soldiermax = nb6soldiermax_custom[5].soldiermax; // ���叫
				nb6soldiermax[6].soldiermax = nb6soldiermax_custom[6].soldiermax; // ���y��
			}
			// �������ɔ��f
			writeUpdateGameDataStructSoldierMax();
		}
		

		// �f�t�H���g�́u�S�C�U���v�u��C�U���v�u�ʏ�U���v�Ȃǂ�����������R�}���h
		// �ό`�R�}���h�̉��n���̂�������ƒ�������Ƃ�������
		rewriteDefaultBattleCommand();

		// �܂����j���[��ǉ����Ă��Ȃ��Ȃ�΁A���j���[��ǉ�����B
		if ( !isAddMenuItems ) {
			isAddMenuItems = true;

			// �܂��̓L�[�𔲂��o���ēo�^����B(�\�[�g���邽��)
			vector<string> menuitems;
			for ( map<HMODULE, string>::iterator it = mapListEspecialModule.begin(); it != mapListEspecialModule.end(); it++ ) {
				menuitems.push_back( it->second );
			}
			// �\�[�g����B
			sort( menuitems.begin(), menuitems.end(), less<string>() );

			// ���j���[�A�C�e���Ƃ��Ēǉ�����B
			for ( vector<string>::iterator it = menuitems.begin(); it != menuitems.end(); it++ ) {
				AddMenuItem(*it, MENU_ITEM_MODULE_END_ID );
			}
			// �Ō�ɃZ�p���[�^������
			AddMenuItem("---", MENU_ITEM_MODULE_END_ID );

		}

		//------------------------------�@�����܂ł������@-----------------------------------------
		// RoleArrayOfBetweenPosToPos( GetFieldHexUnitPos(188) , GetFieldHexUnitPos(215) );
		// ���W -3,0,0 �̉����B1�b��3,0,0�̕����ւƉ����������x���V�e�B�����B
		/*
		HEX_POSITION p1 = GetCastleHexUnitPos(0xBD-1);
		D3D_POSITION e1 = CastleHexPos_To_WorldPos( p1 );
		OutputDebugStream("(%f,%f,%f)", e1.x, e1.y, e1.z );
		*/
	}
}