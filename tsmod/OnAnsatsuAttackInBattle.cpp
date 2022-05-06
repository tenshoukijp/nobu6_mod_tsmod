#include "WinTarget.h"
#include <time.h>

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"
#include "FuncPlayResourceWav.h"
#include "TSModCommand.h"
#include "WaitTimeProcedure.h"
#include "ScenarioMod.h"

int isAnsatsuSuccessOnAttackInBattleExecute = -1;			// 暗殺が成功したかどうか,eaxレジスタの値を上書きするためにある
int iOrgBushouIDOfAnsatsuSuruGawa    = -1;					// レジスタからわたってきた暗殺する側の武将ID
int iOrgBushouIDOfAnsatsuSareruGawa  = -1;					// レジスタからわたってきた暗殺される側の武将ID
int iNewBushouIDOfAnsatsuSuruGawa	 = -1;					// 暗殺する側の武将ID(加工用)
int iNewBushouIDOfAnsatsuSareruGawa  = -1;					// 暗殺される側の武将ID(加工用)
int iStatusAntiAnsatsuMessageModeOnAttackInBattleExecute = -1;   // アンチ暗殺メッセージモード

BOOL iStatusAntiAnsatsIsOverWriteByScenarioMod = FALSE;

// 季節の変わり目（相場の変わり目）で呼び出され、季節の暗殺カウントがリセットされる。
void EXSuperService_AnsatsuCntReset() {

	// 全員分の暗殺カウントをリセット。
	for ( int iBushouID=0; iBushouID < GAMEDATASTRUCT_BUSHOU_NUM; iBushouID++ ) {
		// 該当武将の今季節の暗殺実行カウントを+1
		nb6savedata_ex.bushou_ex[iBushouID].ansatsu_cnt = 0;
	}
}


// アンチ暗殺システム、および、暗殺ブロックシステム
void OnTenshouExeAnsatsuAttackInBattleExecute() {

	// 元の値をコピー
	iNewBushouIDOfAnsatsuSuruGawa	= iOrgBushouIDOfAnsatsuSuruGawa;
	iNewBushouIDOfAnsatsuSareruGawa = iOrgBushouIDOfAnsatsuSareruGawa;

	// ここを通過する度に逆暗殺メッセージモードはリセットする。
	iStatusAntiAnsatsuMessageModeOnAttackInBattleExecute = 0;

	// ScenarioModによってオーバーライドされたことはリセットする。
	iStatusAntiAnsatsIsOverWriteByScenarioMod = FALSE;

	// 両方とも武将番号が正規な番号に収まっている時。下の条件がいろいろとややこしいので、ScenarioModの条件を先に出して、
	// 変更がなかったら、続きを行う。
	if ( 0 <= iNewBushouIDOfAnsatsuSuruGawa-1 && iNewBushouIDOfAnsatsuSuruGawa-1 < GAMEDATASTRUCT_BUSHOU_NUM  &&
		0 <= iNewBushouIDOfAnsatsuSareruGawa-1 && iNewBushouIDOfAnsatsuSareruGawa-1 < GAMEDATASTRUCT_BUSHOU_NUM 
		) {
		// ScenarioModのカスタム条件で軍師を設定する
		if ( p_snOnCustomCondition ) {
			// メイン画面での暗殺の成否																									┌実行者	　┌受動者			┌現在予定されている結果         ┌BOOL値への変換
			int isScenarioModDeathResult = ((PFNSNONCUSTOMCONDITION) p_snOnCustomCondition)("AS条件::戦争::暗殺受動者死亡可否", iNewBushouIDOfAnsatsuSuruGawa, iNewBushouIDOfAnsatsuSareruGawa, (isAnsatsuSuccessOnAttackInBattleExecute!=0), "", "", 0.0, 0.0);
			if ( isScenarioModDeathResult != CUSTOM_CONDITION_NOMODIFY ) {
				// 暗殺の成否を上書きする。
				isAnsatsuSuccessOnAttackInBattleExecute = isScenarioModDeathResult;
				iStatusAntiAnsatsIsOverWriteByScenarioMod = TRUE;
			}
		}
	}

	// 両方とも武将番号が正規な番号に収まっている時。かつ、ScenarioModでオーバーライドされていない時
	if ( iStatusAntiAnsatsIsOverWriteByScenarioMod == FALSE &&
		0 <= iNewBushouIDOfAnsatsuSuruGawa-1 && iNewBushouIDOfAnsatsuSuruGawa-1 < GAMEDATASTRUCT_BUSHOU_NUM  &&
		0 <= iNewBushouIDOfAnsatsuSareruGawa-1 && iNewBushouIDOfAnsatsuSareruGawa-1 < GAMEDATASTRUCT_BUSHOU_NUM 
		) {

		// 該当武将の今季節の暗殺実行カウントを+1
		nb6savedata_ex.bushou_ex[iNewBushouIDOfAnsatsuSuruGawa-1].ansatsu_cnt += 1;

		// 暗殺される側がランスロット・タルタロスである。
		if (isLancelotTartare(iNewBushouIDOfAnsatsuSareruGawa - 1)) {

			// 暗殺する側とされる側を入れ替える
			int tmp = iNewBushouIDOfAnsatsuSuruGawa;
			iNewBushouIDOfAnsatsuSuruGawa = iNewBushouIDOfAnsatsuSareruGawa;
			iNewBushouIDOfAnsatsuSareruGawa = tmp;

			// 逆暗殺は必ず成功する。
			isAnsatsuSuccessOnAttackInBattleExecute = 1;

			// 逆暗殺メッセージモードスタート
			iStatusAntiAnsatsuMessageModeOnAttackInBattleExecute = 1;

			{
				// ｼｭｼｭ
				FuncPlaySoundWav(617);
			}

		// 暗殺される側が朱伊秀千代である。
		} else if (isShuiHidechiyo(iNewBushouIDOfAnsatsuSareruGawa - 1)) {

			// 暗殺する側とされる側を入れ替える
			int tmp = iNewBushouIDOfAnsatsuSuruGawa;
			iNewBushouIDOfAnsatsuSuruGawa = iNewBushouIDOfAnsatsuSareruGawa;
			iNewBushouIDOfAnsatsuSareruGawa = tmp;

			// 逆暗殺は必ず成功する。
			isAnsatsuSuccessOnAttackInBattleExecute = 1;

			// 逆暗殺メッセージモードスタート
			iStatusAntiAnsatsuMessageModeOnAttackInBattleExecute = 1;

			{
				// ｼｭｼｭ
				FuncPlaySoundWav(617);
			}


		// 暗殺される側が騎士・魔術師・魔族ならば…
		} else if ( nb6bushouref[iNewBushouIDOfAnsatsuSareruGawa-1].job == 0xA0 ||
					nb6bushouref[iNewBushouIDOfAnsatsuSareruGawa-1].job == 0xB0 || 
					nb6bushouref[iNewBushouIDOfAnsatsuSareruGawa-1].job == 0xC0 ) {

			// 暗殺が成功することがない
			isAnsatsuSuccessOnAttackInBattleExecute = 0;

		// 暗殺する側がランスロットタルタロスならば…
		} else if ( isLancelotTartare(iNewBushouIDOfAnsatsuSuruGawa-1) ) {

			;
			// 何もフィルター制限やアンチシステムなし

		// 暗殺する側が騎士・魔術師・魔族ならば…
		} else if ( 
			nb6bushouref[iNewBushouIDOfAnsatsuSuruGawa-1].job == 0xA0 ||
			nb6bushouref[iNewBushouIDOfAnsatsuSuruGawa-1].job == 0xB0 ||
			nb6bushouref[iNewBushouIDOfAnsatsuSuruGawa-1].job == 0xC0 ) {

			;
			// 何もフィルター制限やアンチシステムなし

		// 暗殺する側が朱伊ならば…
		}
		else if (isShuiHidechiyo(iNewBushouIDOfAnsatsuSuruGawa - 1)) {

			;
			// 何もフィルター制限やアンチシステムなし

		// その他一般武将同士
		} else {

			// 暗殺返しおよび暗殺率調整のオプションが有効ならば…
			if ( TSModCommand::AllBushou::cmdAntiAssassinationInBattle ) {

				// n=該当武将の暗殺実行回数
				int n = nb6savedata_ex.bushou_ex[iNewBushouIDOfAnsatsuSuruGawa-1].ansatsu_cnt;

				// nが1以上、逆暗殺が発動し始める。
				// 暗殺を繰り返すほど、逆暗殺が発動しやすくなる。
				if ( n >= 1 && rand() % 50 <= n ) {
					// 暗殺する側とされる側を入れ替える
					int tmp = iNewBushouIDOfAnsatsuSuruGawa;
					iNewBushouIDOfAnsatsuSuruGawa = iNewBushouIDOfAnsatsuSareruGawa;
					iNewBushouIDOfAnsatsuSareruGawa = tmp;

					// 逆暗殺は必ず成功する。
					isAnsatsuSuccessOnAttackInBattleExecute = 1;

					// 逆暗殺メッセージモードスタート
					iStatusAntiAnsatsuMessageModeOnAttackInBattleExecute = 1;
					{
						// ｼｭｼｭ
						FuncPlaySoundWav( 617 );
					}


				// nが2以上で、1/nの確率で暗殺フィルターがかからない。それ以外は暗殺失敗。(rand()%n==0の時だけ暗殺判定に0フィルターを掛けない)
				// 暗殺を繰り返すほど、暗殺は失敗しやすくなる。
				} else if ( n >= 2 && (rand() % n != 0) ) {
					// 暗殺が成功することがない
					isAnsatsuSuccessOnAttackInBattleExecute = 0;

				} 
			}
		}
	}
}


/*
この瞬間
ESI：暗殺する側
EDI：暗殺される側
00433992  |. E8 F4950700    CALL    TENSHOU.004ACF8B
				└ここを JMP TSMod.OnTSExeAnsatsuAttackInBattle と書き換えてやる実際の処理
00433997  |. 83C4 04        ADD     ESP, 4
0043399A  |. 85C0           TEST    EAX, EAX
*/
int pTenshouExeJumpFromToOnTenshouExeAnsatsuAttackInBattle		=0x433992; // 関数はこのアドレスから、OnTenshouExeAnsatsuAttackInBattleへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeAnsatsuAttackInBattle  =0x4ACF8B; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeAnsatsuAttackInBattle	=0x433997; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeAnsatsuAttackInBattle() {
	// スタックにためておく
	__asm {

		// 元々TENSHOU.EXE内にあったものをここで
		call pTenshouExeJumpCallFromToOnTenshouExeAnsatsuAttackInBattle

		mov isAnsatsuSuccessOnAttackInBattleExecute, eax // 現在予定されている結果
		mov iOrgBushouIDOfAnsatsuSuruGawa, esi 
		mov iOrgBushouIDOfAnsatsuSareruGawa, edi

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}
	
	// ゲームデータをプレイヤーターンが変わった段階で読み込み更新しておく
	OnTenshouExeAnsatsuAttackInBattleExecute();

	// スタックに引き出す
	__asm {
		pop edi
		pop esi
		pop ebp
		pop esp
		pop edx
		pop ecx
		pop ebx
		pop eax

		// 暗殺の成否情報を上書き
		mov eax, isAnsatsuSuccessOnAttackInBattleExecute
		// 
		mov esi, iNewBushouIDOfAnsatsuSuruGawa
		mov edi, iNewBushouIDOfAnsatsuSareruGawa

		jmp pTenshouExeReturnLblFromOnTenshouExeAnsatsuAttackInBattle
	}
}

/*
004487DB   . 57             PUSH    EDI
004487DC   . 57             PUSH    EDI
004487DD   . 57             PUSH    EDI
004487DE   . 6A 0A          PUSH    0A
004487E0   > E8 044C0400    CALL    TENSHOU.0048D3E9
				└ここを JMP TSMod.OnTSExeAnsatsuAttackInBattle と書き換えてやる実際の処理
004487E5   . 83C4 10        ADD     ESP, 10
004487E8   . EB 0B          JMP     SHORT TENSHOU.004487F5
*/
char cmdOnTenshouExeJumpFromAnsatsuAttackInBattle[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeAnsatsuAttackInBattle() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeAnsatsuAttackInBattle;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeAnsatsuAttackInBattle + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeAnsatsuAttackInBattle  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromAnsatsuAttackInBattle+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeAnsatsuAttackInBattle), cmdOnTenshouExeJumpFromAnsatsuAttackInBattle, 5, NULL); //5バイトのみ書き込む
}




