#include "CommonGlobalHandle.h"
#include "EXServiceExecute.h"
#include "GameDataStruct.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "TSModCommand.h"
#include "TSMod.h"


// ���V�[�Y���A�]�v�ɁA����Ă���肷�鏈��
void EXSuperService_CheckOtherDaimyosGetMoneyAndRiseOnSeasonChange() {

	// �����͂ւ̑����֘A�̃R�}���h���L���Ȃ��
	if ( TSModCommand::MyselfGundan::cmdGetMoneyAndRiseOnSeasonChangeLv > 0 ) {

		// �e�R�c�ɑ΂��āA�S�C��n�𑝂₷
		for (int iGundanNum=0; iGundanNum < GAMEDATASTRUCT_GUNDAN_NUM; iGundanNum++) {

			// ���閼�i���o�[��0xffff���Ƃ��̌R�c�͑��݂��Ă��Ȃ��B���ցB
			if (nb6gundan[iGundanNum].residence == 0xFFFF ) {
				continue;
			}
			// �Ώۂ������͂̌R�c�ł���� �p�X
			if ( !isPlayerGundan(iGundanNum) ) {
				continue;
			}

			// �I�[�o�[�t���[�h�~�̂���INT�^�փR�s�[
			int money = nb6gundan[iGundanNum].money;
			// �R�}���h���x���ɍ��킹�đ���
			money += TSModCommand::MyselfGundan::cmdGetMoneyAndRiseOnSeasonChangeLv * 50 * nb6progressseason; // �o�ߋG��
			// �I�[�o�[�t���[��}����
			if ( money > GAMEDATASTRUCT_MONEY_MAX) { money = GAMEDATASTRUCT_MONEY_MAX; }
			// �i�[
			nb6gundan[iGundanNum].money = money;


			// �I�[�o�[�t���[�h�~�̂���INT�^�փR�s�[
			int rise = nb6gundan[iGundanNum].rise;
			// �R�}���h���x���ɍ��킹�đ���
			rise += TSModCommand::MyselfGundan::cmdGetMoneyAndRiseOnSeasonChangeLv * 50 * nb6progressseason; // �o�ߋG��
			// �I�[�o�[�t���[��}����
			if ( rise > GAMEDATASTRUCT_RISE_MAX) { rise = GAMEDATASTRUCT_RISE_MAX; }
			// �i�[
			nb6gundan[iGundanNum].rise = rise;
		}
	}

	// �����͂ւ̑����֘A�̃R�}���h���L���Ȃ��
	if ( TSModCommand::OthersGundan::cmdGetMoneyAndRiseOnSeasonChangeLv > 0 ) {

		// �e�R�c�ɑ΂��āA�����ƕĂ𑝂₷�B
		for (int iGundanNum=0; iGundanNum < GAMEDATASTRUCT_GUNDAN_NUM; iGundanNum++) {

			// ���閼�i���o�[��0xffff���Ƃ��̌R�c�͑��݂��Ă��Ȃ��B���ցB
			if (nb6gundan[iGundanNum].residence == 0xFFFF ) {
				continue;
			}
			// �Ώۂ��v���C���[�̌R�c�ł���� �p�X
			if ( isPlayerGundan(iGundanNum) ) {
				continue;
			}

			// �I�[�o�[�t���[�h�~�̂���INT�^�փR�s�[
			int money = nb6gundan[iGundanNum].money;
			// �R�}���h���x���ɍ��킹�đ���
			money += TSModCommand::OthersGundan::cmdGetMoneyAndRiseOnSeasonChangeLv * 50 * nb6progressseason; // �o�ߋG��
			// �I�[�o�[�t���[��}����
			if ( money > GAMEDATASTRUCT_MONEY_MAX) { money = GAMEDATASTRUCT_MONEY_MAX; }
			// �i�[
			nb6gundan[iGundanNum].money = money;


			// �I�[�o�[�t���[�h�~�̂���INT�^�փR�s�[
			int rise = nb6gundan[iGundanNum].rise;
			// �R�}���h���x���ɍ��킹�đ���
			rise += TSModCommand::OthersGundan::cmdGetMoneyAndRiseOnSeasonChangeLv * 50 * nb6progressseason; // �o�ߋG��
			// �I�[�o�[�t���[��}����
			if ( rise > GAMEDATASTRUCT_RISE_MAX) { rise = GAMEDATASTRUCT_RISE_MAX; }
			// �i�[
			nb6gundan[iGundanNum].rise = rise;
		}
	}
}

