#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "VariableNameRegistAndJudge.h"

// ����1300�l�̕����̕��ϓo�p
byte nb6AvgParam_Touyou[GAMEDATASTRUCT_REGISTED_BUSHOU_NUM] = {
4,
3,
3,
3,
3,
4,
3,
3,
3,
3,
4,
3,
3,
4,
4,
1,
3,
3,
3,
3,
2,
1,
3,
4,
3,
3,
4,
3,
2,
4,
3,
3,
1,
2,
1,
4,
1,
3,
3,
3,
3,
4,
2,
1,
1,
3,
3,
3,
3,
3,
3,
1,
3,
3,
3,
4,
2,
3,
3,
3,
3,
1,
4,
3,
3,
4,
3,
1,
3,
3,
3,
1,
3,
3,
3,
3,
3,
2,
2,
2,
1,
3,
1,
1,
2,
2,
2,
3,
2,
3,
3,
2,
2,
2,
3,
1,
3,
1,
1,
3,
3,
3,
1,
4,
3,
1,
3,
2,
3,
2,
3,
3,
1,
3,
3,
2,
1,
2,
2,
3,
4,
3,
1,
3,
1,
2,
3,
1,
1,
2,
1,
1,
2,
3,
4,
2,
1,
1,
3,
1,
1,
1,
1,
2,
3,
3,
1,
2,
1,
1,
1,
3,
3,
1,
1,
3,
1,
3,
2,
3,
3,
1,
2,
1,
1,
2,
1,
3,
1,
2,
1,
3,
1,
1,
1,
1,
2,
1,
3,
1,
1,
1,
1,
1,
2,
1,
2,
1,
1,
1,
1,
1,
2,
3,
3,
1,
1,
3,
2,
3,
2,
2,
3,
1,
1,
2,
1,
1,
1,
1,
3,
1,
1,
1,
2,
3,
2,
3,
2,
1,
2,
1,
3,
2,
1,
2,
1,
2,
2,
1,
1,
1,
1,
2,
1,
1,
3,
1,
1,
3,
3,
2,
1,
3,
1,
1,
1,
2,
2,
2,
1,
2,
2,
2,
1,
3,
3,
1,
1,
1,
3,
1,
3,
1,
1,
1,
1,
3,
2,
2,
2,
1,
3,
1,
1,
1,
3,
3,
3,
3,
1,
1,
1,
2,
1,
2,
2,
3,
2,
3,
1,
2,
1,
3,
2,
3,
3,
1,
3,
2,
2,
1,
1,
1,
1,
2,
1,
3,
1,
3,
2,
1,
2,
2,
1,
3,
2,
1,
3,
2,
2,
2,
3,
1,
3,
1,
1,
1,
3,
2,
1,
2,
3,
3,
2,
2,
1,
1,
1,
2,
2,
2,
1,
1,
1,
1,
3,
1,
1,
1,
2,
3,
1,
2,
3,
2,
3,
1,
2,
3,
2,
4,
1,
3,
3,
3,
2,
3,
3,
3,
3,
3,
3,
3,
3,
3,
3,
4,
3,
1,
3,
1,
2,
2,
4,
1,
2,
1,
1,
3,
1,
3,
1,
1,
1,
1,
2,
1,
2,
3,
1,
1,
1,
3,
1,
3,
1,
1,
2,
1,
1,
1,
1,
1,
3,
1,
2,
1,
1,
1,
2,
2,
1,
2,
3,
2,
1,
1,
2,
2,
1,
2,
2,
3,
1,
2,
3,
1,
3,
1,
2,
1,
2,
3,
1,
1,
2,
2,
1,
1,
1,
1,
2,
1,
1,
1,
2,
1,
3,
2,
1,
2,
1,
2,
2,
1,
1,
3,
2,
3,
1,
3,
2,
2,
3,
1,
2,
3,
2,
3,
3,
1,
1,
1,
2,
1,
1,
1,
2,
2,
1,
1,
2,
2,
1,
1,
2,
3,
1,
2,
1,
1,
1,
1,
2,
2,
3,
2,
1,
3,
1,
1,
1,
3,
1,
1,
2,
1,
1,
1,
1,
3,
3,
1,
2,
3,
1,
1,
3,
1,
2,
3,
1,
1,
1,
1,
1,
1,
2,
4,
2,
1,
1,
1,
3,
1,
1,
2,
1,
1,
1,
1,
3,
1,
2,
1,
3,
1,
1,
2,
1,
1,
1,
1,
3,
1,
1,
3,
1,
2,
3,
1,
1,
2,
3,
1,
1,
3,
3,
1,
1,
1,
1,
2,
2,
1,
3,
1,
3,
2,
1,
3,
1,
1,
1,
1,
2,
2,
1,
1,
1,
1,
2,
1,
3,
1,
1,
2,
3,
2,
4,
1,
1,
3,
1,
1,
1,
2,
3,
2,
1,
1,
1,
3,
1,
2,
2,
1,
1,
1,
2,
1,
2,
3,
1,
1,
3,
1,
1,
1,
2,
1,
1,
3,
3,
3,
2,
1,
2,
1,
2,
2,
3,
3,
3,
1,
2,
1,
1,
1,
1,
1,
1,
3,
2,
3,
2,
1,
1,
3,
1,
3,
1,
1,
2,
1,
1,
3,
2,
1,
1,
1,
1,
2,
1,
2,
2,
1,
1,
1,
1,
2,
2,
3,
1,
1,
1,
1,
1,
2,
3,
1,
1,
1,
1,
1,
1,
2,
2,
1,
1,
1,
3,
1,
1,
3,
1,
3,
1,
2,
1,
1,
3,
1,
1,
1,
1,
1,
1,
1,
3,
1,
1,
2,
1,
1,
1,
1,
3,
2,
1,
2,
1,
1,
1,
1,
1,
1,
2,
2,
3,
1,
2,
3,
4,
3,
2,
2,
1,
2,
2,
1,
1,
3,
3,
3,
1,
1,
3,
4,
1,
1,
1,
1,
1,
1,
1,
3,
3,
4,
3,
3,
2,
1,
1,
1,
1,
1,
1,
2,
1,
2,
1,
3,
4,
1,
1,
3,
2,
1,
3,
3,
2,
1,
1,
3,
3,
3,
3,
2,
1,
2,
1,
1,
1,
1,
3,
1,
1,
2,
1,
2,
1,
1,
1,
3,
1,
1,
3,
1,
2,
1,
2,
3,
1,
1,
1,
1,
2,
1,
1,
1,
1,
2,
1,
1,
2,
2,
1,
1,
3,
2,
4,
1,
1,
2,
1,
3,
1,
2,
1,
1,
1,
1,
2,
2,
3,
1,
3,
1,
2,
1,
1,
1,
1,
1,
2,
1,
1,
1,
1,
1,
2,
1,
2,
1,
2,
1,
1,
2,
1,
2,
1,
1,
2,
2,
1,
1,
1,
1,
2,
2,
1,
3,
3,
1,
2,
2,
1,
1,
1,
1,
3,
2,
1,
1,
1,
3,
1,
1,
1,
3,
4,
2,
3,
1,
1,
2,
3,
3,
1,
2,
1,
1,
1,
2,
1,
1,
1,
1,
1,
1,
1,
3,
1,
3,
1,
1,
1,
1,
1,
1,
3,
3,
1,
1,
1,
1,
1,
1,
2,
3,
3,
3,
2,
3,
2,
2,
1,
2,
1,
1,
3,
1,
1,
2,
1,
1,
2,
1,
2,
3,
1,
2,
1,
2,
1,
3,
1,
1,
2,
3,
1,
1,
1,
2,
2,
2,
1,
2,
4,
2,
1,
3,
1,
4,
2,
3,
1,
3,
4,
4,
1,
3,
1,
1,
1,
4,
2,
2,
2,
1,
4,
1,
3,
4,
1,
3,
1,
3,
1,
4,
2,
3,
1,
3,
2,
2,
1,
3,
1,
4,
3,
2,
3,
1,
2,
1,
3,
3,
1,
1,
3,
3,
3,
2,
3,
1,
3,
2,
2,
2,
3,
3,
1,
1,
1,
2,
3,
2,
1,
3,
1,
3,
3,
3,
1,
3,
2,
2,
1,
3,
3,
3,
1,
2,
1,
2,
1,
4,
4,
3,
1,
3,
3,
2,
3,
2,
2,
2,
4,
4,
2,
4,
2,
3,
3,
2,
2,
2,
3,
2,
3,
2,
2,
1,
2,
2,
2,
3,
2,
4,
1,
2,
2,
1,
1,
3,
1,
2,
1,
1,
1,
1,
1,
1,
1,
3,
2,
1,
1,
1,
1,
1,
2,
2,
2,
2,
3,
2,
1,
2,
1,
1,
2,
2,
2,
3,
3,
2,
2,
3,
3,
3,
3,
2,
1,
1,
1,
1,
2,
1,
1,
1,
2,
4,
2,
2,
2,
2,
3,
4,
1,
2,
2,
3,
2,
2,
2,
2,
2,
2,
2,
2,
1,
2,
1,
2,
2,
1,
2,
2,
1,
1,
1,
2,
4,
4,
3,
3,
3,
2,
2,
2,
2,
1,
2,
2,
1,
2,
3,
3,
3,
3,
2,
2,
3,
3,
2,
2,
2,
1,
3,
3,
2,
3,
2,
1,
1,
1,
2,
2,
2,
1,
3,
3,
3,
4,
1,
1,
2,
3,
1,
1,
1,
2,
2,
3,
4,
1,
1,
3,
2,
3,
3,
1,
1,
1,
2,
2,
2,
2,
2,
1,
1,
2,
4,
3,
3,
3,
3,
3,
3,
2,
4,
1,
3,
2,
1,
2,
1,
1,
3,
1,
3,
2,
3,
1,
2,
1
};