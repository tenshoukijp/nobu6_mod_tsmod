#include "WinTarget.h"
#include <time.h>


#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"
#include "FuncPlayResourceWav.h"

void OnTenshouExeFireBattleAttackSuccessExecute() {

	// ����炷�B	715	�΍U�߉� 
	FuncPlaySoundWav(715);
}

