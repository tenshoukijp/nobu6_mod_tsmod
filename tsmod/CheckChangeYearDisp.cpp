#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"


bool CheckChangeYearDisplay() {

	// スタート年月が改変されているならば、拡張設定の｢元号表記｣は｢西暦表記｣に強制的に切り替える
	if ( getStartYearAD() != 1454 ) {
		// 元号表示ではなく、西暦表示
		nb6settingsex.yeardisp = 0;

		return true;
	}
	return false;
}