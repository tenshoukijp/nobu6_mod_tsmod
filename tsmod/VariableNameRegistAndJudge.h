#pragma once

#include "GameDataStruct.h"


#define NAME_VARIATION_NUM	11

// �Y���̕����h�c�̕����́A�I���W�i�������i��ɖ��O�Ŕ���j���ǂ����B
bool IsOriginalRegistedBushou(int iBushouID);

// �����̃o���G�[�V������
extern char *nb6bushou_name_variation[GAMEDATASTRUCT_REGISTED_BUSHOU_NUM][NAME_VARIATION_NUM]; // 1300�l��11�̃o���G�[�V�������A�P�̖��O������ő�27�����܂ŁB
