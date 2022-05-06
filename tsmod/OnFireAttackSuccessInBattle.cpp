#include "WinTarget.h"
#include <time.h>


#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"
#include "FuncPlayResourceWav.h"

void OnTenshouExeFireBattleAttackSuccessExecute() {

	// ‰¹‚ğ–Â‚ç‚·B	715	‰ÎU‚ß‰¹ 
	FuncPlaySoundWav(715);
}

