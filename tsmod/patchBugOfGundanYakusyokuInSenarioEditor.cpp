#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "GameDataStruct.h"
#include "TSModCommand.h"

using namespace std;

// �V�i���I�G�f�B�^���R�c�̖�E�ԍ��ɑ΂��Ģ15��Ƃ����Ӗ��s���Ȓl��ݒ肵�Ă���B
// ����������I�ɢ0��ɒ����B
void patchBugOfGundanYakusyokuInSenarioEditor() {
	for ( int i=0; i<GAMEDATASTRUCT_GUNDAN_NUM; i++ ) {
		if ( nb6gundan[i].position == 15 ) {
			nb6gundan[i].position = 0;
		}
	}
}