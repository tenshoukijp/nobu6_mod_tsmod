#pragma once

#include "GameDataStruct.h"

// 004D2CB8  41 3A 4D 41 50 44 41 54 41 2E 4E 36 50 00 00 00  A:MAPDATA.N6P...
#define GAMEDATASTRUCT_MAPDATAN6P_NAME_ADDRESS	0x4D2CB8	// mapdata.n6p�Ƃ������O�̃A�h���X�B�004D2CB8  A:MAPDATA.N6P...��Ƃ���NULL���R����̂ŁA�Q�����܂łȂ璷���o����B����ȏ�Ȃ����������ꍇ�ɂ͎Q�ƌ��̃A�h���X(tenshou.exe���ɂQ��������)�𓮓I�ɕύX����K�v������B

#define GAMEDATASTRUCT_MAPDATAN6P_REF_ADDRESS1	0x490B60	// mapdata.n6p�Ƃ������O�̃A�h���X���Q�Ƃ��Ă���ꏊ�P��
#define GAMEDATASTRUCT_MAPDATAN6P_REF_ADDRESS2	0x490C79	// mapdata.n6p�Ƃ������O�̃A�h���X���Q�Ƃ��Ă���ꏊ�Q��


// mapdata.n6p�����̖��O�𓾂�(�ς����Ă���΁A����𓾂�)
char *GetMapDataName();


// mapdata.n6p�ɕς��āA�ʂ̂��̂�ݒ�
void SetMapDataName(char *szNewName);

