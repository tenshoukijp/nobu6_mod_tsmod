#pragma once

#include "WinTarget.h"
#include "GameDataStruct.h"

extern byte nb6AvgParam_Gov[GAMEDATASTRUCT_REGISTED_BUSHOU_NUM]; // ê≠é°
extern byte nb6AvgParam_Int[GAMEDATASTRUCT_REGISTED_BUSHOU_NUM]; // íqñd
extern byte nb6AvgParam_Bat[GAMEDATASTRUCT_REGISTED_BUSHOU_NUM]; // êÌì¨
extern byte nb6AvgParam_Cmd[GAMEDATASTRUCT_REGISTED_BUSHOU_NUM]; // ìùó¶
extern byte nb6AvgParam_Amb[GAMEDATASTRUCT_REGISTED_BUSHOU_NUM]; // ñÏñ]
extern byte nb6AvgParam_Chm[GAMEDATASTRUCT_REGISTED_BUSHOU_NUM]; // ñ£óÕ
extern byte nb6AvgParam_Edu[GAMEDATASTRUCT_REGISTED_BUSHOU_NUM]; // ã≥ó{
extern byte nb6AvgParam_Gir[GAMEDATASTRUCT_REGISTED_BUSHOU_NUM]; // ã`óù

extern byte nb6AvgParam_Bat_Ashigaru[GAMEDATASTRUCT_REGISTED_BUSHOU_NUM]; // ë´åy
extern byte nb6AvgParam_Bat_Kiba[GAMEDATASTRUCT_REGISTED_BUSHOU_NUM];     // ãRîn
extern byte nb6AvgParam_Bat_Teppou[GAMEDATASTRUCT_REGISTED_BUSHOU_NUM];   // ìSñC
extern byte nb6AvgParam_Bat_Suigun[GAMEDATASTRUCT_REGISTED_BUSHOU_NUM];   // êÖåR
extern byte nb6AvgParam_Bat_Yumi[GAMEDATASTRUCT_REGISTED_BUSHOU_NUM];     // ã|
extern byte nb6AvgParam_Bat_Yari[GAMEDATASTRUCT_REGISTED_BUSHOU_NUM];     // ëÑ

extern byte nb6AvgParam_Nougyou[GAMEDATASTRUCT_REGISTED_BUSHOU_NUM]; // î_ã∆
extern byte nb6AvgParam_Syougyou[GAMEDATASTRUCT_REGISTED_BUSHOU_NUM]; // è§ã∆
extern byte nb6AvgParam_Chikujo[GAMEDATASTRUCT_REGISTED_BUSHOU_NUM]; // ízèÈ
extern byte nb6AvgParam_Gaikou[GAMEDATASTRUCT_REGISTED_BUSHOU_NUM]; // äOå
extern byte nb6AvgParam_Touyou[GAMEDATASTRUCT_REGISTED_BUSHOU_NUM]; // ìoóp

void SetBushouAvgParam(int iFlagValue);

void SetBushouAvgParamAppearanceBushou(int iBushouID);


