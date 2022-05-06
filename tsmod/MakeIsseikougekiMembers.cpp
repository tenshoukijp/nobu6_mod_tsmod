#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"


vector<int> vListIsseiKougekiMembers;
void MakeIsseikougekiMembers(int iBushouID, int iTargetID) {
	// �����o�̃N���A
	vListIsseiKougekiMembers.clear();

	if ( !(0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM) ) {
		return;
	}
	if ( !(0 <= iTargetID && iTargetID < GAMEDATASTRUCT_BUSHOU_NUM) ) {
		return;
	}

	// ��풆
	if ( Is_FieldWar() ) {
		// ���݂̃}�b�v�ɏo�w���Ă��镐�����X�g�S��
		vector<int> blist = GetCurMapSyutujinBushouArray();

		// ����̕�����POSITION�𓾂�
		HEX_POSITION posTarget = GetFieldHexUnitPos(iTargetID);

		// �Y���̕����̐��ɂ����闧���Ԃ�(�s��=0, ���ڍU�߂鑤=1, ���ڎ�鑤=2, �U�߂鑤�̉��R=3, ��鑤�̉��R=4, �����R=5)
		int iBushouIDTachiba = GetWarStandPointTheBushou(iBushouID);


		// ���X�g�Łc ��čU�����Ă����l��T���B
		for ( vector<int>::iterator it = blist.begin(); it != blist.end(); it++ ) {

			int iSearchID = *it;

			// �������g�̓p�X
			if ( iBushouID == iSearchID ) { continue; }

			// �T�[�`�Ώۂ̕����̈ʒu
			HEX_POSITION posSearch = GetFieldHexUnitPos(iSearchID);

			// �T�[�`�Ώۂ̕����̗���
			int iSearchIDTachiba = GetWarStandPointTheBushou(iSearchID);

			// �w�b�N�X�Ԋu���P�̕����B�����אڂ��Ă��镐��
			if ( getHexDistance( posTarget, posSearch ) == 1 ) {

				// �U�߂鑤�������́A�U�߂鑤�̉��R�ł���(���ꂪ��čU���Ŋ����Y���\�Ȑl)
				if ( (iBushouIDTachiba == 1 || iBushouIDTachiba == 3) && (iSearchIDTachiba == 1 || iSearchIDTachiba== 3) ) {
					vListIsseiKougekiMembers.push_back(iSearchID);
				}
				// ��鑤�������́A��鑤�̉��R�ł���(���ꂪ��čU���Ŋ����Y���\�Ȑl)
				else if ( (iBushouIDTachiba == 2 || iBushouIDTachiba == 4) && (iSearchIDTachiba == 2 || iSearchIDTachiba== 4) ) {
					vListIsseiKougekiMembers.push_back(iSearchID);
				}
			}
		}
	}

	// �U��킿�イ
	if ( Is_CastleWar() ) {
		// ���݂̃}�b�v�ɏo�w���Ă��镐�����X�g�S��
		vector<int> blist = GetCurMapSyutujinBushouArray();

		// ����̕�����POSITION�𓾂�
		HEX_POSITION posTarget = GetCastleHexUnitPos(iTargetID);

		// �Y���̕����̐��ɂ����闧���Ԃ�(�s��=0, ���ڍU�߂鑤=1, ���ڎ�鑤=2, �U�߂鑤�̉��R=3, ��鑤�̉��R=4, �����R=5)
		int iBushouIDTachiba = GetWarStandPointTheBushou(iBushouID);


		// ���X�g�Łc ��čU�����Ă����l��T���B
		for ( vector<int>::iterator it = blist.begin(); it != blist.end(); it++ ) {

			int iSearchID = *it;

			// �������g�̓p�X
			if ( iBushouID == iSearchID ) { continue; }

			// �T�[�`�Ώۂ̕����̈ʒu
			HEX_POSITION posSearch = GetCastleHexUnitPos(iSearchID);

			// �T�[�`�Ώۂ̕����̗���
			int iSearchIDTachiba = GetWarStandPointTheBushou(iSearchID);

			// �w�b�N�X�Ԋu���P�̕����B�����אڂ��Ă��镐��
			if ( getHexDistance( posTarget, posSearch ) == 1 ) {

				// �U�߂鑤�������́A�U�߂鑤�̉��R�ł���(���ꂪ��čU���Ŋ����Y���\�Ȑl)
				if ( (iBushouIDTachiba == 1 || iBushouIDTachiba == 3) && (iSearchIDTachiba == 1 || iSearchIDTachiba== 3) ) {
					vListIsseiKougekiMembers.push_back(iSearchID);
				}
				// ��鑤�������́A��鑤�̉��R�ł���(���ꂪ��čU���Ŋ����Y���\�Ȑl)
				else if ( (iBushouIDTachiba == 2 || iBushouIDTachiba == 4) && (iSearchIDTachiba == 2 || iSearchIDTachiba== 4) ) {
					vListIsseiKougekiMembers.push_back(iSearchID);
				}
			}
		}
	}


}