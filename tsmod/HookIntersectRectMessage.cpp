#include "WinTarget.h"
#include <fstream>
#include <mmsystem.h>

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "EXBushou_All.h"
#include "MessageN6PPatch.h"
#include "ScenarioMessageN6PPatch.h"
#include "DialogKahou.h"
#include "EXServiceExecute.h"
#include "ScenarioMod.h"
#include "TenShougiMod.h"
#include "FuncPlayBGM.h"

using namespace std;

// BGMMM.libの中にある
extern int BGMmm_GetShouldBeRePlayLastestBGM();


CurProcessMemory CPM;
DialogKahou DK;

DWORD preSnModTimeGetTime = 0;

extern void RetsudenEditorExtRepaintCheck(CONST RECT *lprcSrc1, CONST RECT *lprcSrc2);

BOOL Hook_IntersectRectMessage(
  LPRECT lprcDst,        // 交差部分が格納されるバッファ
  CONST RECT *lprcSrc1,  // 最初の長方形
  CONST RECT *lprcSrc2   // 2 番目の長方形
)
{

	// まだゲームがはじまってなければ…
	if ( CommonGlobalHandle::isNobunaganoYabouGameStart == false ) {
		return TRUE;
	}

	if ( CommonGlobalHandle::isNobunaganoYabouGameEnd ) {
		return TRUE;
	}

	// メモリ⇒CurProcessMemoryのCPM値をセット
	CPM.setVariableFromMemory();

	// 現在のGameDataStructを読み込んでセット。
	setCurProcessGameMemory();

	// 特殊武将の機能を全員分まとめて
	EXBushou_All_Execute();

	// 一般武将の機能を全員分まとめて
	EXSuperService_Regeneration();

	// 勲功に関するフィルター。特殊な属性を持つ武将に関して、勲功があがらなくしたり、特別に下げたりなど。
	EXSuperService_FilterMerits();

	// 武将名、メッセージ、付属情報
	ReWritePatchMessageN6P( CPM.getCurVarPtr(LOC1), CPM.getCurVarPtr(LOC4), NULL );

	// 家宝関連の処理
	DK.CheckDialogKahou( lprcSrc1, lprcSrc2 );

	RetsudenEditorExtRepaintCheck(lprcSrc1, lprcSrc2);

	// BGMが再生を終えたようだ。再度再生するべきである。
	if ( BGMmm_GetShouldBeRePlayLastestBGM() ) {
		int iBGM = getPlayingBGM();
		/*
		-- - - - - - - - - - - - １３曲分布表
		--
		--                       01 : －
		--                       02 : －
		--                       03 : [東北地方][関東地方]
		--                       04 : [近畿地方][外交コマンド]
		--                       05 : [戦争・春][戦争・冬]
		--                       06 : [東海地方][甲信地方][北陸地方]
		--                       07 : [戦争・秋][戦争・信長][戦争・名将][初期設定][本能寺の変][大坂城]
		--                       08 : [山陽地方][山陰地方][環境コマンド]
		--                       09 : [京都御所][海外貿易港][講義コマンド][披露コマンド]
		--                       10 : [四国地方][九州地方]
		--                       11 : [全国統一][安土城][戦争コマンド][披露・茶会][披露・御前試合][披露・問答][戦闘・勝利][戦闘・敗北]
		--                       12 : [戦争・夏][戦争・緊迫][新幕府]
		--                       13 : －
		--                       14 : [死亡・滅亡]
		*/
		// 11 : のうち、[戦闘・勝利][戦闘・敗北] だけはループしない。
		if ( iBGM == 11 && (Is_FieldWar() || Is_CastleWar()) ) {
			// この時だけはなにもしない。
		} else {
			FuncStopBGM();
			FuncPlayBGM(iBGM);
		}
	}
	
	// ScenarioModのOnTickを呼び出す。1秒に5回目安。
	if ( p_snOnTick ) {
		DWORD curSnModTimeGetTime = timeGetTime();
		int RefleshTime = 200; // ㍉秒

		// 規定の間隔時間が過ぎた
		if ( int(curSnModTimeGetTime - preSnModTimeGetTime) > int(RefleshTime) ) {

			// 初めての実行の時
			if ( preSnModTimeGetTime == 0 ) {
				p_snOnTick(curSnModTimeGetTime, 0);
			// それ以外
			} else {
				p_snOnTick(curSnModTimeGetTime, curSnModTimeGetTime-preSnModTimeGetTime); // 経過時間を渡しつつ呼出し。
			}

			preSnModTimeGetTime = curSnModTimeGetTime;
		}
	}

	return TRUE;
}





