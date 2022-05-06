#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "GameDataStruct.h"
#include "TSModCommand.h"

using namespace std;

// シナリオエディタが軍団の役職番号に対して｢15｣という意味不明な値を設定している。
// これを強制的に｢0｣に直す。
void patchBugOfGundanYakusyokuInSenarioEditor() {
	for ( int i=0; i<GAMEDATASTRUCT_GUNDAN_NUM; i++ ) {
		if ( nb6gundan[i].position == 15 ) {
			nb6gundan[i].position = 0;
		}
	}
}