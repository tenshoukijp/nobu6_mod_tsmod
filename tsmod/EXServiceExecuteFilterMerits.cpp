#include "WinTarget.h"

#include "EXBushou.h"
#include "EXBushou_All.h"
#include "GameDataStruct.h"
#include "TSModCommand.h"




// 勲功に関する特殊なフィルターサービス
void EXSuperService_FilterMerits() {


	// 忍者は勲功がたまらず、出世出来ないに関するフラグが有効
	if (TSModCommand::AllBushou::cmdNinjaNotHaveAWayOfPromotion ) {

		// 武将に対して検索する
		for (int iTargetBushouID=0; iTargetBushouID<GAMEDATASTRUCT_BUSHOU_NUM; iTargetBushouID++) {

			// 対象の武将が忍者であれば… 
			if (nb6bushouref[iTargetBushouID].job == 0x10) { // 0x10==忍者 GameDataStruct.hを見よ

				// 書き換える新たな値は勲功最低値
				WORD rewrite_merits = 0;

				// 宿老～足軽頭までなら･･･ (大名や隠居は書き換えない)
				if ( 2 <= nb6bushouname[iTargetBushouID].position && nb6bushouname[iTargetBushouID].position <= 6 ) {

					// 身分が変動していれば･･･身分を足軽頭に戻す
					if ( nb6bushouname[iTargetBushouID].position != 6 ) { // 6 足軽頭･･･GameDataStructを見よ

						// 強制的に足軽頭へ
						nb6bushouname[iTargetBushouID].position = 6;
					}

					// 該当部将の勲功値が、なんらかの変動をしていれば･･･ 勲功を０に書き換える
					if ( nb6bushouref[iTargetBushouID].merits != rewrite_merits ) { 

						nb6bushouref[iTargetBushouID].merits = 0;
					}

				} 

			} // end 忍者なら
		} // end for
	} // end TSModCommandフラグ
}

