#define _CRT_SECURE_NO_WARNINGS

#include "MapData.h"


// A:MAPDATA.N6P�݂����Ȗ��O���Ԃ��Ă���BA:MAPDATA.N6P�����ۂɂ͂ǂ��ɏ�����Ă��邩�͒肩�ł͂Ȃ����A�f�t�H���g�ł́AGAMEDATASTRUCT_MAPDATAN6P_NAME_ADDRESS �̃A�h���X�ɑ��݂���B
// MapDataObgk.dll�Ȃǂł́A���̏ꏊ��DLL�����w�������悤�ɕς��Ă���B
// GetMapDataName�֐��Ȃ�A�ǂ��ɂ��낤�Ƃ��A�Y���̖��O���擾�\�B
char *GetMapDataName() {
	char *szMapDataName = NULL;

	// �܂��AGAMEDATASTRUCT_MAPDATAN6P_REF_ADDRESS1 �̃A�h���X��INT�l���AszMapDataName�|�C���^�̃A�h���X�ƂȂ�ׂ��B�����A���̃A�h���X�ɁAmapdata.n6p�Ƃ����������ւ̃|�C���^�������Ă���B
	int *iAddress = NULL;
	iAddress = (int *)GAMEDATASTRUCT_MAPDATAN6P_REF_ADDRESS1;
	szMapDataName = (char *)(*iAddress);

	return szMapDataName;
}

// A:MAPDATA.N6P�݂����ɢA:���擪�ɕt���邱��
void SetMapDataName(char *szNewName) {
	// �|�C���^�𓾂�
	char *pMapDataName = GetMapDataName();
	// �Y���̃|�C���^�Ɍ��X���݂��Ă��镶���񐔂�MAX���Ƃ��ē���ւ���B
	strcpy( pMapDataName, szNewName); // �f�t�H���g��TENSHOU.EXE���Ȃ�13�����܂Ō���AMapDataObgkMod�ɑ΂��ĂȂ�A31�����܂ł�����B
}

