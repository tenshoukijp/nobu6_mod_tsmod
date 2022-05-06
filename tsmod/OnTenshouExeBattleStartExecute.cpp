#include "GameDataStruct.h"
#include "ScenarioMod.h"


�푈�J�n�p�����^ war_start_param;


// �푈�������������ɁA�d�|����ꂽ��ԍ��ƁA�d�|�����R�cID���n���Ă���B
// @iRecieveCastleID : �U�߂�ꂽ��̏�ID
// @iRecieveGundanID : �U�߂�ꂽ���̌R�c��ID
// @iRecieveBushouID : �U�߂�ꂽ���̌R�c���̕���ID
// @iAttackGundanID  : �U�߂����̌R�cID
// @iAttackBushouID  : �U�߂����̌R�c���̕���ID
void OnTenshouExeBattleStartExecute(int iRecieveCastleID ) {

	// �ȉ��悭�g���ԍ��̏���

	// �U�߂�ꂽ�邪�͈͊O�Ȃ�_��
	if ( ! (0 <= iRecieveCastleID && iRecieveCastleID < GAMEDATASTRUCT_CASTLE_NUM) ) {
		return;
	}

	// �U�߂�ꂽ���̌R�c�h�c�𓾂�
	int iRecieveGundanID = nb6castle[iRecieveCastleID].attach-1;

	// �U�߂�ꂽ���̌R�c���̕����h�c�𓾂�
	int iRecieveBushouID = nb6gundan[iRecieveGundanID].leader-1;

	int iAttackGundanID = nb6turn.gundan-1;
	// �^�[���R�c�������Ƃ����肦�Ȃ��͂������ꉞ�c
	if ( ! (0 <= iAttackGundanID && iAttackGundanID < GAMEDATASTRUCT_GUNDAN_NUM ) ) {
		return;
	}

	// �U�߂����̌R�c���̕����h�c�𓾂�
	int iAttackBushouID = nb6gundan[iAttackGundanID].leader-1;

	
	// ScenarioMod�̐푈�J�n���ɓn�����߁B
	war_start_param.��ԍ� = iRecieveCastleID+1;				// �W�I�̏�ԍ�
	war_start_param.������R�c_�R�c�ԍ� = iRecieveGundanID + 1;		// ������̌R�c�ԍ�
	war_start_param.������R�c��_�����ԍ� = iRecieveBushouID + 1;
	war_start_param.�U�����R�c_�R�c�ԍ�  = iAttackGundanID + 1;		// ������̌R�c�ԍ�
	war_start_param.�U�����R�c��_�����ԍ�  = iAttackBushouID + 1;

	// ���������ŉ����B
	// OutputDebugStream("%s, %s, %s", nb6castle[iRecieveCastleID].name, nb6bushouname[iRecieveBushouID].fastname, nb6bushouname[iAttackBushouID].fastname );

	// ScenarioMod�̐푈�J�n���̃��\�b�h�ďo���B
	if ( p_snOnBattleStart ) {
		((PFNSNONBATTLESTART) p_snOnBattleStart)(&war_start_param);
	}
}
