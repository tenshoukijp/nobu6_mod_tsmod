#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"
#include "ScenarioMod.h"

// �U�������s����钼�O�ɍs�����I�����ꂽ��
// 
namespace SelectingBattleAction {
	// Tenshou.exe �� TSMod���ł̔���
	enum {
		�U�� = 0,
		�ˌ� = 1, �R�n�ˌ� = 1,
		�S�C = 2, �| = 2,
		��C = 3,
		��čU�� = 4,
	};
}


// �d�v�ȃC�x���g�n���h��!! ����L������ϔC�L����(NPC�܂�)���A�s�������s�������ɑI�������B
// �����Ȃǁu�����̈ӎv�łȂ��v���ɂ́A���s����Ȃ��B
void OnSelectingAttackEventHandlerInBattle(int iBushouID, int iTargetID, int iSelectActionID, BOOL IsMoveBefore) {

	if ( p_snOnSelectingBattleAction ) {

		�U�����@���莞�p�����^ param;
		param.�ړ��O = IsMoveBefore;
		param.�U�����@�ԍ� = iSelectActionID;

		// TSMOD�ł̋|�ԍ���SCENARIOMOD�ł̋|�ԍ��B
		p_snOnSelectingBattleAction(iBushouID, iTargetID, iSelectActionID, &param);
	}
}