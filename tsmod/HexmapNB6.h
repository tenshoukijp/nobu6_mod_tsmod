#pragma once

#pragma pack(1)

int setHexMapNB6ToHexMapArray();
char *GetHexMapName();


// 004C8DE2  48 45 58 4D 41 50 2E 4E 42 36 00 00 00 00 41 3A  HEXMAP.NB6....A:
#define GAMEDATASTRUCT_HEXMAPNB6_NAME_ADDRESS	0x4C8DE2	// hexmap.nb6�Ƃ������O�̃A�h���X�B�004D2CB8  A:MAPDATA.N6P...��Ƃ���NULL���R����̂ŁA�Q�����܂łȂ璷���o����B����ȏ�Ȃ����������ꍇ�ɂ͎Q�ƌ��̃A�h���X(tenshou.exe���ɂQ��������)�𓮓I�ɕύX����K�v������B

#define GAMEDATASTRUCT_HEXMAPNB6_REF_ADDRESS	0x498999	// hexmap.nb6�Ƃ������O�̃A�h���X���Q�Ƃ��Ă���ꏊ�P��
