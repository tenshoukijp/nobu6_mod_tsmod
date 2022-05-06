// hexmap.n6p �� ���� NB6HexMapData�Ɋi�[����B
#include "GameDataStruct.h"
#include "ls11_mod.h"

#define HEXMAP_CASTLE_GRAPH_STARTID 952 // hexmap.nb6 �̒��́A���}�b�v�ƍU���}�b�v�ɕ�����Ă��邪�A��}�b�v�̎n�܂�ԍ��̊J�n�_�������B
									  // �ŏ���0�`(34*14)-1�܂ł����}�b�v�̃O���\���A3*14�`3*14*2-1�܂ł������f�[�^
									  // �����34*14*2=952����}�b�v�f�[�^�̊J�n�_�ł���B
									  // ��O�I��1594�łɑ�� �� ���y�̃O���\���}�b�v�������Ă���

#define HEXMAP_CASTLE_ROLE_STARTID HEXMAP_CASTLE_GRAPH_STARTID + GAMEDATASTRUCT_CASTLE_NUM
									  // ��̖����̎n�܂�|�C���g�B��O���t�B�b�N�\���ԍ��̍ŏ��ɁA�鐔�𑫂�������

#define HEXMAP_CASTLE_HIGH_STARTID HEXMAP_CASTLE_GRAPH_STARTID + GAMEDATASTRUCT_CASTLE_NUM*2
									  // ��̍����̎n�܂�|�C���g�B��O���t�B�b�N�\���ԍ��̍ŏ��ɁA�鐔*2�𑫂�������


int CastleHighSum[GAMEDATASTRUCT_CASTLE_NUM] = {}; // �e��}�b�v�̑S�`�b�v�̍����̍��v�B
int SuperCastleHighSum = 0; // �e��}�b�v�̑S�`�b�v�̍����̍��v�B���y �� ���� ��p�B

// ���̍��ڂ̓f�t�H���g��hexmap.nb6���炾�ƃ������W�J���傫���Ȃ肷����tenshou.exe�������Ȃ��Ȃ�
int setHexMapNB6ToHexMapArray() {

	// HexMap.NB6�̑S�f�[�^���i�[
	vector<vector<byte>> vHexMapNB6Data;

	// ���k���ꂽLS11�t�H�[�}�b�g���f�R�[�h����֐�
	char *szHexMapName = GetHexMapName() + 2; // �擪�ɂ���uA:HEXMAP.NB6�v�́uA:�v�̕����J�b�g

	int result = ls11_DecodePack(szHexMapName, NULL, &vHexMapNB6Data );

	if ( vHexMapNB6Data.size() >= 1597 ) { // ����̍�����hexmap.1596�Ȃ̂ŁAhexmap.000�`hexmap.1596 �̌v1597���K�v�B
		for ( int i=0; i<GAMEDATASTRUCT_CASTLE_NUM;i++ ) {

			vector<byte> vCastleHigh = vHexMapNB6Data[HEXMAP_CASTLE_HIGH_STARTID+i]; // �����̃f�[�^��[952+214*2]=1380 ����X�^�[�g

			int iHigh=0; // �Y���̏�̍����̕���
			// �Y���}�b�v�̍�����S�ďW�v����
			for ( vector<byte>::iterator it=vCastleHigh.begin(); it != vCastleHigh.end(); it++ ) {
				iHigh += *it;
			}
			CastleHighSum[i] = iHigh; // �Y����̏��Ƃ��Ċi�[
		}

		vector<byte> vOOsakaCastleHigh = vHexMapNB6Data[1596]; // ���y �� ���̍������B���ʂɂ����ɑ��݂���B

		int iHigh=0; // �Y���̏�̍����̕���
		// �Y���}�b�v�̍�����S�ďW�v����
		for ( vector<byte>::iterator it=vOOsakaCastleHigh.begin(); it != vOOsakaCastleHigh.end(); it++ ) {
			iHigh += *it;
		}
		SuperCastleHighSum = iHigh; // �Y����̏��Ƃ��Ċi�[
	}

	return result;
}


// ��}�b�v�������S�x���o���B
int getCastleMapSolidDegre(int iCastleID) {
	if ( 0 <= iCastleID && iCastleID < GAMEDATASTRUCT_CASTLE_NUM ) {
		// �ΎR�{�莛�̏ꏊ�ŁA����ɂȂ��Ă���
		if (iCastleID==167 && strcmp(nb6castle[iCastleID].name, "���")==0 ) {
			return SuperCastleHighSum;
		}
		// �ω����̏ꏊ�ň��y�ɂȂ��Ă���
		else if (iCastleID==136 && strcmp(nb6castle[iCastleID].name, "���y")==0 ) {
			return SuperCastleHighSum;
		}
		else {
			// ���S�x��Ԃ��B
			return CastleHighSum[iCastleID];
		}
	}

	return 0;
}




// A:HEXMAP.NB6�݂����Ȗ��O���Ԃ��Ă���BA:HEXMAP.N6P�����ۂɂ͂ǂ��ɏ�����Ă��邩�͒肩�ł͂Ȃ����A�f�t�H���g�ł́AGAMEDATASTRUCT_HEXMAPN6P_REF_ADDRESS �̃A�h���X�ɑ��݂���B
// MapDataObgk.dll�Ȃǂł́A���̏ꏊ��DLL�����w�������悤�ɕς��Ă���B
// GetHexMapName�֐��Ȃ�A�ǂ��ɂ��낤�Ƃ��A�Y���̖��O���擾�\�B
char *GetHexMapName() {
	char *szHexMapName = NULL;

	// �܂��AGAMEDATASTRUCT_MAPDATAN6P_REF_ADDRESS1 �̃A�h���X��INT�l���AszHexMapName�|�C���^�̃A�h���X�ƂȂ�ׂ��B�����A���̃A�h���X�ɁAmapdata.n6p�Ƃ����������ւ̃|�C���^�������Ă���B
	int *iAddress = NULL;
	iAddress = (int *)GAMEDATASTRUCT_HEXMAPNB6_REF_ADDRESS;
	szHexMapName = (char *)(*iAddress);

	return szHexMapName;
}
