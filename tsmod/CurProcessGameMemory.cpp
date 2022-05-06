#include "WinTarget.h"
#include <mmsystem.h>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <functional>
#include <type_traits>

#include "CommonGlobalHandle.h"
#include "GameDataStruct.h"
#include "TSModCommand.h"
#include "OnViewingBattleMenuInBattle.h"
#include "EXServiceExecute.h"
#include "MessageInflection.h"
#include "MessageInflectionEnding.h"
#include "VariableNameRegistAndJudge.h"
#include "SoundServer.h"
#include "FactoryGeneralBushouName.h"
#include "DijkstraCastle.h"


using namespace std;



static DWORD preTimeGetTime = 0;


void copyAllBushouSoldiersOriginToCopy() {

	// 兵士数だけ、メモリ上→現在値保存としてコピー
	for ( int iBushouID=0; iBushouID < GAMEDATASTRUCT_BUSHOU_NUM; iBushouID++ ) {
		nb6bushoucopy[iBushouID].soldier = nb6bushouref[iBushouID].soldier;
	}
}



// 以下は原則1回読み込み系。書き換えたりするため、例外はある。
void readUpdateGameDataStructSoldierMax() {
	// 兵数MAX情報
	ReadProcessMemory( 
		CommonGlobalHandle::curProcess,
		(LPCVOID)(GAMEDATASTRUCT_SOLDIERMAX_ADDRESS), 
		nb6soldiermax, 
		sizeof(nb6soldiermax),
		NULL);
}

void writeUpdateGameDataStructSoldierMax() {
	// 兵数MAX情報書き換え
	WriteProcessMemory( 
		CommonGlobalHandle::curProcess,
		(LPVOID)(GAMEDATASTRUCT_SOLDIERMAX_ADDRESS), 
		nb6soldiermax, 
		sizeof(nb6soldiermax),
		NULL);
}


extern int ServiceExecuteRegenerationAllSaveBeginSoldier[GAMEDATASTRUCT_BUSHOU_NUM];
extern int ServiceExecuteRegenerationAllIncrSoldierInBattle[GAMEDATASTRUCT_BUSHOU_NUM];


// まだ読み込んだことがない
void setCurProcessGameMemory() {

	// 構造体の定義の事前コンパイルチェック
	static_assert(sizeof(SAVEDATA_EX) == 299708, "SAVEDATA_EXのサイズが合っていない");
	static_assert(sizeof(SAVEDATA_EX_COMMON) == 4000, "SAVEDATA_EX_COMMONのサイズが合っていない");


	if ( (sizeof(SAVEDATA_EX) != 299708) || (sizeof(SAVEDATA_EX_COMMON) != 4000) ) {
		MessageBox(NULL, "セーブデータサイズ不正", "セーブデータサイズ不正", NULL);
	}


	DWORD curTimeGetTime = timeGetTime();
	int RefleshTime = 5000; // ㍉秒

	// 規定の間隔時間が過ぎた
	if ( int(curTimeGetTime - preTimeGetTime) > int(RefleshTime) ) {

		preTimeGetTime = curTimeGetTime;
		
		// タイミングを見て読み込み更新する必要があるゲームデータ群を更新
		copyAllBushouSoldiersOriginToCopy();

		//----以下は１回だけ読み込み系

		// 役職分類
		// 役職分類はexeからの固定読み込みなので1回だけ読み込み
		if ( ! GAMEDATASTRUCT_POSITIONTYPELIST_POINTER[1] ) {
			ReadProcessMemory( 
				CommonGlobalHandle::curProcess,
				(LPCVOID)(GAMEDATASTRUCT_POSITIONTYPELIST_POINTER_ADDRESS), 
				GAMEDATASTRUCT_POSITIONTYPELIST_POINTER, 
				4 * GAMEDATASTRUCT_POSITIONTYPE_NUM,
				NULL);
		}

		// 家宝分類
		// 家宝分類はexeからの固定読み込みなので1回だけ読み込み
		if ( ! GAMEDATASTRUCT_KAHOUTYPELIST_POINTER[1] ) {
			ReadProcessMemory( 
				CommonGlobalHandle::curProcess,
				(LPCVOID)(GAMEDATASTRUCT_KAHOUTYPELIST_POINTER_ADDRESS), 
				GAMEDATASTRUCT_KAHOUTYPELIST_POINTER, 
				4 * GAMEDATASTRUCT_KAHOUTYPE_NUM,
				NULL);
		}


		// 兵数MAX情報は、固定読み込みと思われるので1回だけ読み込む
		if ( nb6soldiermax[1].soldiermax < 1 ) { // [0] は ずっと0なので…

			// メモリの方から読んでくる
			readUpdateGameDataStructSoldierMax();

			// TSMod.txtで指定があれば上書きする。
			if ( TSModCommand::AllBushou::cmdSoldierBaseMaxOfDaimy != 0) {
				nb6soldiermax[1].soldiermax			= TSModCommand::AllBushou::cmdSoldierBaseMaxOfDaimy;
				nb6soldiermax_custom[1].soldiermax	= TSModCommand::AllBushou::cmdSoldierBaseMaxOfDaimy + 20;
			}
			if ( TSModCommand::AllBushou::cmdSoldierBaseMaxOfSyuku != 0 ) {
				nb6soldiermax[2].soldiermax			= TSModCommand::AllBushou::cmdSoldierBaseMaxOfSyuku;
				nb6soldiermax_custom[2].soldiermax	= TSModCommand::AllBushou::cmdSoldierBaseMaxOfSyuku + 20;
			}
			if ( TSModCommand::AllBushou::cmdSoldierBaseMaxOfKarou != 0 ) {
				nb6soldiermax[3].soldiermax			= TSModCommand::AllBushou::cmdSoldierBaseMaxOfKarou;
				nb6soldiermax_custom[3].soldiermax	= TSModCommand::AllBushou::cmdSoldierBaseMaxOfKarou + 20;
			}
			if ( TSModCommand::AllBushou::cmdSoldierBaseMaxOfBusyo != 0 ) {
				nb6soldiermax[4].soldiermax			= TSModCommand::AllBushou::cmdSoldierBaseMaxOfBusyo;
				nb6soldiermax_custom[4].soldiermax	= TSModCommand::AllBushou::cmdSoldierBaseMaxOfBusyo + 20;
			}
			if ( TSModCommand::AllBushou::cmdSoldierBaseMaxOfSamra != 0 ) {
				nb6soldiermax[5].soldiermax			= TSModCommand::AllBushou::cmdSoldierBaseMaxOfSamra;
				nb6soldiermax_custom[5].soldiermax	= TSModCommand::AllBushou::cmdSoldierBaseMaxOfSamra + 20;
			}
			if ( TSModCommand::AllBushou::cmdSoldierBaseMaxOfAshig != 0 ) {
				nb6soldiermax[6].soldiermax			= TSModCommand::AllBushou::cmdSoldierBaseMaxOfAshig;
				nb6soldiermax_custom[6].soldiermax	= TSModCommand::AllBushou::cmdSoldierBaseMaxOfAshig + 20;
			}

			// もし兵士数に関する操作がある場合は、1回アップデートのために書き換える
			if ( TSModCommand::AllBushou::cmdPositionInfluenceSoldierMax != 0 ) {
				nb6soldiermax[1].soldiermax = nb6soldiermax_custom[1].soldiermax; // 大名
				nb6soldiermax[2].soldiermax = nb6soldiermax_custom[2].soldiermax; // 宿老
				nb6soldiermax[3].soldiermax = nb6soldiermax_custom[3].soldiermax; // 家老
				nb6soldiermax[4].soldiermax = nb6soldiermax_custom[4].soldiermax; // 部将
				nb6soldiermax[5].soldiermax = nb6soldiermax_custom[5].soldiermax; // 侍大将
				nb6soldiermax[6].soldiermax = nb6soldiermax_custom[6].soldiermax; // 足軽頭
			}
			// メモリに反映
			writeUpdateGameDataStructSoldierMax();
		}
		

		// デフォルトの「鉄砲攻撃」「大砲攻撃」「通常攻撃」などを書き換えるコマンド
		// 変形コマンドの下地自体をちょっと調整するという感じ
		rewriteDefaultBattleCommand();

		// まだメニューを追加していないならば、メニューを追加する。
		if ( !isAddMenuItems ) {
			isAddMenuItems = true;

			// まずはキーを抜き出して登録する。(ソートするため)
			vector<string> menuitems;
			for ( map<HMODULE, string>::iterator it = mapListEspecialModule.begin(); it != mapListEspecialModule.end(); it++ ) {
				menuitems.push_back( it->second );
			}
			// ソートする。
			sort( menuitems.begin(), menuitems.end(), less<string>() );

			// メニューアイテムとして追加する。
			for ( vector<string>::iterator it = menuitems.begin(); it != menuitems.end(); it++ ) {
				AddMenuItem(*it, MENU_ITEM_MODULE_END_ID );
			}
			// 最後にセパレータを入れる
			AddMenuItem("---", MENU_ITEM_MODULE_END_ID );

		}

		//------------------------------　ここまでが処理　-----------------------------------------
		// RoleArrayOfBetweenPosToPos( GetFieldHexUnitPos(188) , GetFieldHexUnitPos(215) );
		// 座標 -3,0,0 の音源。1秒で3,0,0の方向へと音源が動くベロシティを持つ。
		/*
		HEX_POSITION p1 = GetCastleHexUnitPos(0xBD-1);
		D3D_POSITION e1 = CastleHexPos_To_WorldPos( p1 );
		OutputDebugStream("(%f,%f,%f)", e1.x, e1.y, e1.z );
		*/
	}
}