#pragma once

#include "WinTarget.h"
#include "GameDataStruct.h"

extern byte nb6AvgParam_Gov[GAMEDATASTRUCT_REGISTED_BUSHOU_NUM]; // ����
extern byte nb6AvgParam_Int[GAMEDATASTRUCT_REGISTED_BUSHOU_NUM]; // �q�d
extern byte nb6AvgParam_Bat[GAMEDATASTRUCT_REGISTED_BUSHOU_NUM]; // �퓬
extern byte nb6AvgParam_Cmd[GAMEDATASTRUCT_REGISTED_BUSHOU_NUM]; // ����
extern byte nb6AvgParam_Amb[GAMEDATASTRUCT_REGISTED_BUSHOU_NUM]; // ��]
extern byte nb6AvgParam_Chm[GAMEDATASTRUCT_REGISTED_BUSHOU_NUM]; // ����
extern byte nb6AvgParam_Edu[GAMEDATASTRUCT_REGISTED_BUSHOU_NUM]; // ���{
extern byte nb6AvgParam_Gir[GAMEDATASTRUCT_REGISTED_BUSHOU_NUM]; // �`��

extern byte nb6AvgParam_Bat_Ashigaru[GAMEDATASTRUCT_REGISTED_BUSHOU_NUM]; // ���y
extern byte nb6AvgParam_Bat_Kiba[GAMEDATASTRUCT_REGISTED_BUSHOU_NUM];     // �R�n
extern byte nb6AvgParam_Bat_Teppou[GAMEDATASTRUCT_REGISTED_BUSHOU_NUM];   // �S�C
extern byte nb6AvgParam_Bat_Suigun[GAMEDATASTRUCT_REGISTED_BUSHOU_NUM];   // ���R
extern byte nb6AvgParam_Bat_Yumi[GAMEDATASTRUCT_REGISTED_BUSHOU_NUM];     // �|
extern byte nb6AvgParam_Bat_Yari[GAMEDATASTRUCT_REGISTED_BUSHOU_NUM];     // ��

extern byte nb6AvgParam_Nougyou[GAMEDATASTRUCT_REGISTED_BUSHOU_NUM]; // �_��
extern byte nb6AvgParam_Syougyou[GAMEDATASTRUCT_REGISTED_BUSHOU_NUM]; // ����
extern byte nb6AvgParam_Chikujo[GAMEDATASTRUCT_REGISTED_BUSHOU_NUM]; // �z��
extern byte nb6AvgParam_Gaikou[GAMEDATASTRUCT_REGISTED_BUSHOU_NUM]; // �O��
extern byte nb6AvgParam_Touyou[GAMEDATASTRUCT_REGISTED_BUSHOU_NUM]; // �o�p

void SetBushouAvgParam(int iFlagValue);

void SetBushouAvgParamAppearanceBushou(int iBushouID);


