#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"
#include "TSModCommand.h"
#include "ScenarioMod.h"


extern bool isUseCurBushouAmeDeppou; // この武将が雨鉄砲を使うかどうか

/*
004BB488  00 00 00 00 88 EA 90 C4 8D 55 8C 82 00 00 00 00  ....一斉攻撃....
004BB498  91 E5 96 43 00 00 00 00 00 00 00 00 93 53 96 43  大砲........鉄砲
004BB4A8  00 00 00 00 00 00 00 00 93 CB 8C 82 00 00 00 00  ........突撃....
004BB4B8  8D 55 8C 82 00 00 00 00 00 00 00 00 00 00 00 00  攻撃............
*/

bool isRewriteDefaultBattleCommand = false;
// デフォルトの「通常攻撃、一斉攻撃、鉄砲攻撃、大砲攻撃」などを書き換え
void rewriteDefaultBattleCommand() {

	if ( isRewriteDefaultBattleCommand == false ) {

		// 「通常攻撃」を「攻撃」とする。
		WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(0x4BB4B8), "攻撃\x00\x00\x00\x00", 8, NULL); // アドレスなので4バイト

		// 「鉄砲攻撃」を「鉄砲」とする。
		WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(0x4BB4A4), "鉄砲\x00\x00\x00\x00", 8, NULL); // アドレスなので4バイト

		// 「大砲攻撃」を「大砲」とする。
		WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(0x4BB498), "大砲\x00\x00\x00\x00", 8, NULL); // アドレスなので4バイト

		isRewriteDefaultBattleCommand = true;

	}
}


/*           ┌通常攻撃    ┌突撃(ここのアドレスを、状況によって書き換えれば良い)
004BB500  B8 B4 4B 00 B0 B4 4B 00 A4 B4 4B 00 98 B4 4B 00  ｸｴK.ｰｴK.､ｴK.亢K.
004BB510  8C B4 4B 00 00 00 00 00 84 B4 4B 00 7C B4 4B 00  原K.....╋K.|ｴK.
004BB520  74 B4 4B 00 6C B4 4B 00 64 B4 4B 00 5C B4 4B 00  tｴK.lｴK.dｴK.\ｴK.
004BB530  54 B4 4B                                         TｴK
*/

// メニューを出そうとしている武将のID(配列使用よりも1大きい数字)
int iCurBushouIDOnViewingBattleMenuInBattle1 = -1;





#define SCENARIOMOD_BATTLE_CUSTOM_LABEL_NUM		5


char pBufBattleCustomMenuInBattle[SCENARIOMOD_BATTLE_CUSTOM_LABEL_NUM][20] = {"","","","",""};

// 「カスタム」を指す、新たなアドレス５つ
int iNewAdressBufBattleCustomMenuInBattle[SCENARIOMOD_BATTLE_CUSTOM_LABEL_NUM] = {
	(int)(pBufBattleCustomMenuInBattle[0]),
	(int)(pBufBattleCustomMenuInBattle[1]),
	(int)(pBufBattleCustomMenuInBattle[2]),
	(int)(pBufBattleCustomMenuInBattle[3]),
	(int)(pBufBattleCustomMenuInBattle[4]),
};




#define COMMAND_LEVEL_NUM 7

//--------------------- 攻撃 -----------------------


// 新たなコマンド名の配列
char pBufBattleMenuAttackInBattle[COMMAND_LEVEL_NUM][10] = {"攻撃","攻撃･弐","攻撃･参","攻撃･四","攻撃･極","6","攻撃･極"};

// 「攻撃」を指す、新たなアドレス
int iNewAdressBufBattleAttackMenuInBattle[COMMAND_LEVEL_NUM] = {
	(int)(pBufBattleMenuAttackInBattle[0]),
	(int)(pBufBattleMenuAttackInBattle[1]),
	(int)(pBufBattleMenuAttackInBattle[2]),
	(int)(pBufBattleMenuAttackInBattle[3]),
	(int)(pBufBattleMenuAttackInBattle[4]),
	(int)(pBufBattleMenuAttackInBattle[5]),
	(int)(pBufBattleMenuAttackInBattle[6]),
};

//--------------------- 攻撃 -----------------------

// 「攻撃」を指す、元のアドレス
int iOrgAdressBufBattleAttackMenuInBattle = 0x4BB4B8;

// 「攻撃」の文字列を指すポインタ値が入っているアドレス
int pOrgAddressBattleAttackMenuInBattle = 0x4BB500;



//--------------------- 一斉攻撃 -----------------------

// 新たなコマンド名の配列
char pBufBattleMenuIsseiInBattle[COMMAND_LEVEL_NUM][14] = {"一斉攻撃","一斉攻撃･弐","一斉攻撃･参","一斉攻撃･四","一斉攻撃･極","6","一斉攻撃･極"};

// 「一斉攻撃」を指す、新たなアドレス
int iNewAdressBufBattleIsseiMenuInBattle[COMMAND_LEVEL_NUM] = {
	(int)(pBufBattleMenuIsseiInBattle[0]),
	(int)(pBufBattleMenuIsseiInBattle[1]),
	(int)(pBufBattleMenuIsseiInBattle[2]),
	(int)(pBufBattleMenuIsseiInBattle[3]),
	(int)(pBufBattleMenuIsseiInBattle[4]),
	(int)(pBufBattleMenuIsseiInBattle[5]),
	(int)(pBufBattleMenuIsseiInBattle[6]),
};

//--------------------- 一斉攻撃 -----------------------

// 「一斉攻撃」を指す、元のアドレス
int iOrgAdressBufBattleIsseiMenuInBattle = 0x4BB48C;

// 「一斉攻撃」の文字列を指すポインタ値が入っているアドレス
int pOrgAddressBattleIsseiMenuInBattle = 0x4BB510;




//--------------------- 騎馬突撃系 -----------------------

// 新たなコマンド名の配列
char pBufBattleMenuKiTotsuInBattle[COMMAND_LEVEL_NUM][10] = {"騎突","騎突･弐","騎突･参","騎突･四","騎突･極","6","騎鉄突･極"};

// 「騎突極」を指す、新たなアドレス
int iNewAdressBufBattleKiTotsuMenuInBattle[COMMAND_LEVEL_NUM] = {
	(int)(pBufBattleMenuKiTotsuInBattle[0]),
	(int)(pBufBattleMenuKiTotsuInBattle[1]),
	(int)(pBufBattleMenuKiTotsuInBattle[2]),
	(int)(pBufBattleMenuKiTotsuInBattle[3]),
	(int)(pBufBattleMenuKiTotsuInBattle[4]),
	(int)(pBufBattleMenuKiTotsuInBattle[5]),
	(int)(pBufBattleMenuKiTotsuInBattle[6]),
};

//--------------------- 突撃系 -----------------------

// 新たなコマンド名の配列
char pBufBattleMenuTotsugekiInBattle[COMMAND_LEVEL_NUM][10] = {"突撃","突撃･弐","突撃･参","突撃･四","突撃･極","6","突撃･真"};

// 「騎突極」を指す、新たなアドレス
int iNewAdressBufBattleTotsugekiMenuInBattle[COMMAND_LEVEL_NUM] = {
	(int)(pBufBattleMenuTotsugekiInBattle[0]),
	(int)(pBufBattleMenuTotsugekiInBattle[1]),
	(int)(pBufBattleMenuTotsugekiInBattle[2]),
	(int)(pBufBattleMenuTotsugekiInBattle[3]),
	(int)(pBufBattleMenuTotsugekiInBattle[4]),
	(int)(pBufBattleMenuTotsugekiInBattle[5]),
	(int)(pBufBattleMenuTotsugekiInBattle[6]),
};

//--------------------- 突撃共通 -----------------------

// 「突撃」を指す、元のアドレス
int iOrgAdressBufBattleTotsugekiMenuInBattle = 0x4BB4B0;

// 「突撃」の文字列を指すポインタ値が入っているアドレス
int pOrgAddressBattleTotsugekiMenuInBattle = 0x4BB504;


//--------------------- 鉄砲系 -----------------------

// 新たなコマンド名の配列
char pBufBattleMenuTeppouInBattle[COMMAND_LEVEL_NUM][10] = {"鉄砲","鉄砲･弐","鉄砲･参","鉄砲･四","鉄砲･極","6","鉄砲･真"};

// 「鉄砲系」を指す、新たなアドレス
int iNewAdressBufBattleTeppouMenuInBattle[COMMAND_LEVEL_NUM] = {
	(int)(pBufBattleMenuTeppouInBattle[0]),
	(int)(pBufBattleMenuTeppouInBattle[1]),
	(int)(pBufBattleMenuTeppouInBattle[2]),
	(int)(pBufBattleMenuTeppouInBattle[3]),
	(int)(pBufBattleMenuTeppouInBattle[4]),
	(int)(pBufBattleMenuTeppouInBattle[5]),
	(int)(pBufBattleMenuTeppouInBattle[6]),
};

//--------------------- 雨鉄砲系 -----------------------

// 新たなコマンド名の配列
char pBufBattleMenuAmeTeppouInBattle[COMMAND_LEVEL_NUM][10] = {"雨鉄砲","雨鉄砲･弐","雨鉄砲･参","雨鉄砲･四","雨鉄砲･極","6","雨鉄砲･真"};

// 「雨鉄砲系」を指す、新たなアドレス
int iNewAdressBufBattleAmeTeppouMenuInBattle[COMMAND_LEVEL_NUM] = {
	(int)(pBufBattleMenuAmeTeppouInBattle[0]),
	(int)(pBufBattleMenuAmeTeppouInBattle[1]),
	(int)(pBufBattleMenuAmeTeppouInBattle[2]),
	(int)(pBufBattleMenuAmeTeppouInBattle[3]),
	(int)(pBufBattleMenuAmeTeppouInBattle[4]),
	(int)(pBufBattleMenuAmeTeppouInBattle[5]),
	(int)(pBufBattleMenuAmeTeppouInBattle[6]),
};

//--------------------- 弓系 -----------------------

// 新たなコマンド名の配列
char pBufBattleMenuYumiInBattle[COMMAND_LEVEL_NUM][10] = {"弓","弓･弐","弓･参","弓･四","弓･極","6","弓･真"};

// 「弓系」を指す、新たなアドレス
int iNewAdressBufBattleYumiMenuInBattle[COMMAND_LEVEL_NUM] = {
	(int)(pBufBattleMenuYumiInBattle[0]),
	(int)(pBufBattleMenuYumiInBattle[1]),
	(int)(pBufBattleMenuYumiInBattle[2]),
	(int)(pBufBattleMenuYumiInBattle[3]),
	(int)(pBufBattleMenuYumiInBattle[4]),
	(int)(pBufBattleMenuYumiInBattle[5]),
	(int)(pBufBattleMenuYumiInBattle[6]),
};


//--------------------- 雨弓系 -----------------------

// 新たなコマンド名の配列
char pBufBattleMenuAmeYumiInBattle[COMMAND_LEVEL_NUM][10] = {"雨弓","雨弓･弐","雨弓･参","雨弓･四","雨弓･極","6","雨弓･真"};

// 「雨弓系」を指す、新たなアドレス
int iNewAdressBufBattleAmeYumiMenuInBattle[COMMAND_LEVEL_NUM] = {
	(int)(pBufBattleMenuAmeYumiInBattle[0]),
	(int)(pBufBattleMenuAmeYumiInBattle[1]),
	(int)(pBufBattleMenuAmeYumiInBattle[2]),
	(int)(pBufBattleMenuAmeYumiInBattle[3]),
	(int)(pBufBattleMenuAmeYumiInBattle[4]),
	(int)(pBufBattleMenuAmeYumiInBattle[5]),
	(int)(pBufBattleMenuAmeYumiInBattle[6]),
};


//--------------------- 鉄砲共通系 -----------------------

// 「鉄砲攻撃」を指す、元のアドレス
int iOrgAdressBufBattleTeppouMenuInBattle = 0x4BB4A4;

// 「鉄砲攻撃」の文字列を指すポインタ値が入っているアドレス
int pOrgAddressBattleTeppouMenuInBattle = 0x4BB508;



//--------------------- 大砲系 -----------------------

// 新たなコマンド名の配列
char pBufBattleMenuTaihouInBattle[COMMAND_LEVEL_NUM][10] = {"大砲","大砲･弐","大砲･参","大砲･四","大砲･極","6","大砲･真"};

// 「大砲系」を指す、新たなアドレス
int iNewAdressBufBattleTaihouMenuInBattle[COMMAND_LEVEL_NUM] = {
	(int)(pBufBattleMenuTaihouInBattle[0]),
	(int)(pBufBattleMenuTaihouInBattle[1]),
	(int)(pBufBattleMenuTaihouInBattle[2]),
	(int)(pBufBattleMenuTaihouInBattle[3]),
	(int)(pBufBattleMenuTaihouInBattle[4]),
	(int)(pBufBattleMenuTaihouInBattle[5]),
	(int)(pBufBattleMenuTaihouInBattle[6]),
};

//--------------------- 雨大砲系 -----------------------


// 新たなコマンド名の配列
char pBufBattleMenuAmeTaihouInBattle[COMMAND_LEVEL_NUM][10] = {"雨大砲","雨大砲･弐","雨大砲･参","雨大砲･四","雨大砲･極","6","雨大砲･真"};

// 「雨大砲系」を指す、新たなアドレス
int iNewAdressBufBattleAmeTaihouMenuInBattle[COMMAND_LEVEL_NUM] = {
	(int)(pBufBattleMenuAmeTaihouInBattle[0]),
	(int)(pBufBattleMenuAmeTaihouInBattle[1]),
	(int)(pBufBattleMenuAmeTaihouInBattle[2]),
	(int)(pBufBattleMenuAmeTaihouInBattle[3]),
	(int)(pBufBattleMenuAmeTaihouInBattle[4]),
	(int)(pBufBattleMenuAmeTaihouInBattle[5]),
	(int)(pBufBattleMenuAmeTaihouInBattle[6]),
};

//--------------------- 大砲共通系 -----------------------

// 「大砲攻撃」を指す、元のアドレス
int iOrgAdressBufBattleTaihouMenuInBattle = 0x4BB498;

// 「大砲攻撃」の文字列を指すポインタ値が入っているアドレス
int pOrgAddressBattleTaihouMenuInBattle = 0x4BB50C;

void OnTenshouExeViewingBattleMenuInBattle1Execute() {

	// 武将IDが一定の範囲(それ以外の値が入ってくることもあるためチェック)
	if (0 <= iCurBushouIDOnViewingBattleMenuInBattle1-1 && iCurBushouIDOnViewingBattleMenuInBattle1-1 < GAMEDATASTRUCT_BUSHOU_NUM) {
	} else {
		return;
	}


	// (通常)の文字列の変更。今のところ、変更していないので、必ずデフォルトへと戻すのみ。(前の人やScenarioModが書き換えていた時のため)
	if ( 1 ) {
		WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleAttackMenuInBattle), &iOrgAdressBufBattleAttackMenuInBattle, 4, NULL); // アドレスなので4バイト
		WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleIsseiMenuInBattle), &iOrgAdressBufBattleIsseiMenuInBattle, 4, NULL); // アドレスなので4バイト
		WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleTotsugekiMenuInBattle), &iOrgAdressBufBattleTotsugekiMenuInBattle, 4, NULL); // アドレスなので4バイト
		WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleTeppouMenuInBattle), &iOrgAdressBufBattleTeppouMenuInBattle, 4, NULL); // アドレスなので4バイト
		WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleTaihouMenuInBattle), &iOrgAdressBufBattleTaihouMenuInBattle, 4, NULL); // アドレスなので4バイト
	}


	// 騎馬隊か騎馬鉄砲隊である
	if (nb6bushouref[iCurBushouIDOnViewingBattleMenuInBattle1-1].form==1 ||nb6bushouref[iCurBushouIDOnViewingBattleMenuInBattle1-1].form==3) {

		// 戦才が120(=240)を超える場合、｢騎突真」が出る。 INDEXは1引いてるの注意
		if (getBushouBat(iCurBushouIDOnViewingBattleMenuInBattle1-1) >= 120 &&			// 戦闘が240以上
			getBushouAptitude(iCurBushouIDOnViewingBattleMenuInBattle1-1, APTITUDE_KIBA) >= 5 &&
			isLancelotTartare(iCurBushouIDOnViewingBattleMenuInBattle1-1) ) {

			// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
			WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleTotsugekiMenuInBattle), &(iNewAdressBufBattleKiTotsuMenuInBattle[6]), 4, NULL); // アドレスなので4バイト

		//戦才が110(=220)を超える場合、｢騎突極」が出る。 INDEXは1引いてるの注意
		} else if (getBushouBat(iCurBushouIDOnViewingBattleMenuInBattle1-1) >= 110 &&   // 戦闘が110(=220)以上
			getBushouAptitude(iCurBushouIDOnViewingBattleMenuInBattle1-1, APTITUDE_KIBA) >= 5 ) {   // 騎馬がＳある

			// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
			WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleTotsugekiMenuInBattle), &(iNewAdressBufBattleKiTotsuMenuInBattle[4]), 4, NULL); // アドレスなので4バイト

		//戦才が90(=180)を超える場合、｢騎突四」が出る。 INDEXは1引いてるの注意
		} else if (getBushouBat(iCurBushouIDOnViewingBattleMenuInBattle1-1) >= 90 &&   // 戦闘が90(=180)以上
			getBushouAptitude(iCurBushouIDOnViewingBattleMenuInBattle1-1, APTITUDE_KIBA) >= 5 ) {   // 騎馬がＳある

			// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
			WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleTotsugekiMenuInBattle), &(iNewAdressBufBattleKiTotsuMenuInBattle[3]), 4, NULL); // アドレスなので4バイト

		//戦才が80(=160)を超える場合、｢騎突参」が出る。 INDEXは1引いてるの注意
		} else if (getBushouBat(iCurBushouIDOnViewingBattleMenuInBattle1-1) >= 70 &&   // 戦闘が70(=140)以上
			getBushouAptitude(iCurBushouIDOnViewingBattleMenuInBattle1-1, APTITUDE_KIBA) >= 4 ) {   // 騎馬がＡある

			// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
			WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleTotsugekiMenuInBattle), &(iNewAdressBufBattleKiTotsuMenuInBattle[2]), 4, NULL); // アドレスなので4バイト

		//戦才が70(=140)を超える場合、｢騎突弐」が出る。 INDEXは1引いてるの注意
		} else if (getBushouBat(iCurBushouIDOnViewingBattleMenuInBattle1-1) >= 50 &&   // 戦闘が50(=100)以上
			getBushouAptitude(iCurBushouIDOnViewingBattleMenuInBattle1-1, APTITUDE_KIBA) >= 2 ) {   // 騎馬がＣある

			// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
			WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleTotsugekiMenuInBattle), &(iNewAdressBufBattleKiTotsuMenuInBattle[1]), 4, NULL); // アドレスなので4バイト

		// 異なる場合は騎馬突撃とする。
		} else {	 

			// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
			WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleTotsugekiMenuInBattle), &(iNewAdressBufBattleKiTotsuMenuInBattle[0]), 4, NULL); // アドレスなので4バイト
		}

	// 足軽隊か鉄砲隊である
	} else if (nb6bushouref[iCurBushouIDOnViewingBattleMenuInBattle1-1].form==0 ||nb6bushouref[iCurBushouIDOnViewingBattleMenuInBattle1-1].form==2) {


		//戦才が110(=220)を超える場合、｢突撃極」が出る。 INDEXは1引いてるの注意
		if (getBushouBat(iCurBushouIDOnViewingBattleMenuInBattle1-1) >= 110 &&   // 戦闘が110(=220)以上
			getBushouAptitude(iCurBushouIDOnViewingBattleMenuInBattle1-1, APTITUDE_ASIGARU) >= 5 ) {   // 足軽がＳある

			// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
			WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleTotsugekiMenuInBattle), &(iNewAdressBufBattleTotsugekiMenuInBattle[4]), 4, NULL); // アドレスなので4バイト

		//戦才が90(=180)を超える場合、｢突撃四」が出る。 INDEXは1引いてるの注意
		} else if (getBushouBat(iCurBushouIDOnViewingBattleMenuInBattle1-1) >= 90 &&   // 戦闘が90(=180)以上
			getBushouAptitude(iCurBushouIDOnViewingBattleMenuInBattle1-1, APTITUDE_ASIGARU) >= 5 ) {   // 足軽がＳある

			// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
			WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleTotsugekiMenuInBattle), &(iNewAdressBufBattleTotsugekiMenuInBattle[3]), 4, NULL); // アドレスなので4バイト

		//戦才が80(=160)を超える場合、｢突撃参」が出る。 INDEXは1引いてるの注意
		} else if (getBushouBat(iCurBushouIDOnViewingBattleMenuInBattle1-1) >= 70 &&   // 戦闘が70(=140)以上
			getBushouAptitude(iCurBushouIDOnViewingBattleMenuInBattle1-1, APTITUDE_ASIGARU) >= 4 ) {   // 足軽がＡある

			// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
			WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleTotsugekiMenuInBattle), &(iNewAdressBufBattleTotsugekiMenuInBattle[2]), 4, NULL); // アドレスなので4バイト

		//戦才が70(=140)を超える場合、｢突撃弐」が出る。 INDEXは1引いてるの注意
		} else if (getBushouBat(iCurBushouIDOnViewingBattleMenuInBattle1-1) >= 50 &&   // 戦闘が50(=100)以上
			getBushouAptitude(iCurBushouIDOnViewingBattleMenuInBattle1-1, APTITUDE_ASIGARU) >= 2 ) {   // 足軽がＣある

			// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
			WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleTotsugekiMenuInBattle), &(iNewAdressBufBattleTotsugekiMenuInBattle[1]), 4, NULL); // アドレスなので4バイト

		// 異なる場合は突撃と元へと戻す。
		} else {	 

			// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
			WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleTotsugekiMenuInBattle), &(iNewAdressBufBattleTotsugekiMenuInBattle[0]), 4, NULL); // アドレスなので4バイト
		}


	// 異なる場合は元へと戻す
	} else {

		// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
		WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleTotsugekiMenuInBattle), &iOrgAdressBufBattleTotsugekiMenuInBattle, 4, NULL); // アドレスなので4バイト
	}


	//　雨鉄砲を使う条件が整っていて、かつ天気が雨の場合(鉄砲隊もしくは騎馬鉄砲隊)
	if ( isUseCurBushouAmeDeppou && getWeather() == 2 && (nb6bushouref[iCurBushouIDOnViewingBattleMenuInBattle1-1].form == 2||nb6bushouref[iCurBushouIDOnViewingBattleMenuInBattle1-1].form == 3)) {

		// 戦才が110(=220)を超える場合、｢鉄砲極」が出る。 INDEXは1引いてるの注意
		if (getBushouBat(iCurBushouIDOnViewingBattleMenuInBattle1-1) >= 110 &&    // 戦闘が110(=220)以上
			getBushouAptitude(iCurBushouIDOnViewingBattleMenuInBattle1-1, APTITUDE_TEPPOU) >= 5 ) {  // 鉄砲がＳある

			// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
			WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleTeppouMenuInBattle), &(iNewAdressBufBattleAmeTeppouMenuInBattle[4]), 4, NULL); // アドレスなので4バイト

		// 戦才が90(=180)を超える場合、｢鉄砲極」が出る。 INDEXは1引いてるの注意
		} else if (getBushouBat(iCurBushouIDOnViewingBattleMenuInBattle1-1) >= 90 &&    // 戦闘が90(=180)以上
			getBushouAptitude(iCurBushouIDOnViewingBattleMenuInBattle1-1, APTITUDE_TEPPOU) >= 5 ) {  // 鉄砲がＳある

			// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
			WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleTeppouMenuInBattle), &(iNewAdressBufBattleAmeTeppouMenuInBattle[3]), 4, NULL); // アドレスなので4バイト

		// 戦才が80(=160)を超える場合、｢鉄砲参」が出る。 INDEXは1引いてるの注意
		} else if (getBushouBat(iCurBushouIDOnViewingBattleMenuInBattle1-1) >= 70 &&    // 戦闘が70(=140)以上
			getBushouAptitude(iCurBushouIDOnViewingBattleMenuInBattle1-1, APTITUDE_TEPPOU) >= 4 ) {  // 鉄砲がＡある

			// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
			WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleTeppouMenuInBattle), &(iNewAdressBufBattleAmeTeppouMenuInBattle[2]), 4, NULL); // アドレスなので4バイト

		// 戦才が70(=140)を超える場合、｢鉄砲弐」が出る。 INDEXは1引いてるの注意
		} else if (getBushouBat(iCurBushouIDOnViewingBattleMenuInBattle1-1) >= 50 &&    // 戦闘が50(=100)以上
			getBushouAptitude(iCurBushouIDOnViewingBattleMenuInBattle1-1, APTITUDE_TEPPOU) >= 2 ) {  // 鉄砲がＣある

			// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
			WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleTeppouMenuInBattle), &(iNewAdressBufBattleAmeTeppouMenuInBattle[1]), 4, NULL); // アドレスなので4バイト


		// 異なる場合は元へと戻す
		} else {

			// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
			WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleTeppouMenuInBattle), &(iNewAdressBufBattleAmeTeppouMenuInBattle[0]), 4, NULL); // アドレスなので4バイト
		}
	}


	//------------------------弓系(鉄砲による書き換えを再度変更するのでここに入れること)

	else if ( (nb6bushouref[iCurBushouIDOnViewingBattleMenuInBattle1-1].form == 0||nb6bushouref[iCurBushouIDOnViewingBattleMenuInBattle1-1].form == 1) ) {
		// 雨以外
		if ( getWeather() != 2 ) {

			// 戦才が110(=220)を超える場合、｢弓極」が出る。 INDEXは1引いてるの注意
			if (getBushouBat(iCurBushouIDOnViewingBattleMenuInBattle1-1) >= 110 &&    // 戦闘が110(=220)以上
				nb6savedata_ex.bushou_ex[iCurBushouIDOnViewingBattleMenuInBattle1-1].aptitudeyumi >= 5 ) {  // 弓がＳある

				// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
				WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleTeppouMenuInBattle), &(iNewAdressBufBattleYumiMenuInBattle[4]), 4, NULL); // アドレスなので4バイト

			// 戦才が90(=180)を超える場合、｢弓極」が出る。 INDEXは1引いてるの注意
			} else if (getBushouBat(iCurBushouIDOnViewingBattleMenuInBattle1-1) >= 90 &&    // 戦闘が90(=180)以上
				nb6savedata_ex.bushou_ex[iCurBushouIDOnViewingBattleMenuInBattle1-1].aptitudeyumi >= 5 ) {   // 弓がＳある

				// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
				WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleTeppouMenuInBattle), &(iNewAdressBufBattleYumiMenuInBattle[3]), 4, NULL); // アドレスなので4バイト

			// 戦才が80(=160)を超える場合、｢弓参」が出る。 INDEXは1引いてるの注意
			} else if (getBushouBat(iCurBushouIDOnViewingBattleMenuInBattle1-1) >= 70 &&    // 戦闘が70(=140)以上
				nb6savedata_ex.bushou_ex[iCurBushouIDOnViewingBattleMenuInBattle1-1].aptitudeyumi >= 4 ) {  // 弓がＡある

				// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
				WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleTeppouMenuInBattle), &(iNewAdressBufBattleYumiMenuInBattle[2]), 4, NULL); // アドレスなので4バイト

			// 戦才が70(=140)を超える場合、｢弓弐」が出る。 INDEXは1引いてるの注意
			} else if (getBushouBat(iCurBushouIDOnViewingBattleMenuInBattle1-1) >= 50 &&    // 戦闘が50(=100)以上
				nb6savedata_ex.bushou_ex[iCurBushouIDOnViewingBattleMenuInBattle1-1].aptitudeyumi >= 3 ) {  // 弓がＢある

				// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
				WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleTeppouMenuInBattle), &(iNewAdressBufBattleYumiMenuInBattle[1]), 4, NULL); // アドレスなので4バイト

			// 異なる場合は元へと戻す
			} else {

				// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
				WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleTeppouMenuInBattle), &(iNewAdressBufBattleYumiMenuInBattle[0]), 4, NULL); // アドレスなので4バイト
			}

		// 雨の時
		} else {
			// 戦才が110(=220)を超える場合、｢雨弓極」が出る。 INDEXは1引いてるの注意
			if (getBushouBat(iCurBushouIDOnViewingBattleMenuInBattle1-1) >= 110 &&    // 戦闘が110(=220)以上
				nb6savedata_ex.bushou_ex[iCurBushouIDOnViewingBattleMenuInBattle1-1].aptitudeyumi >= 5 ) {  // 雨弓がＳある

				// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
				WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleTeppouMenuInBattle), &(iNewAdressBufBattleAmeYumiMenuInBattle[4]), 4, NULL); // アドレスなので4バイト

			// 戦才が90(=180)を超える場合、｢雨弓極」が出る。 INDEXは1引いてるの注意
			} else if (getBushouBat(iCurBushouIDOnViewingBattleMenuInBattle1-1) >= 90 &&    // 戦闘が90(=180)以上
				nb6savedata_ex.bushou_ex[iCurBushouIDOnViewingBattleMenuInBattle1-1].aptitudeyumi >= 5 ) {   // 雨弓がＳある

				// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
				WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleTeppouMenuInBattle), &(iNewAdressBufBattleAmeYumiMenuInBattle[3]), 4, NULL); // アドレスなので4バイト

			// 戦才が80(=160)を超える場合、｢雨弓参」が出る。 INDEXは1引いてるの注意
			} else if (getBushouBat(iCurBushouIDOnViewingBattleMenuInBattle1-1) >= 70 &&    // 戦闘が70(=140)以上
				nb6savedata_ex.bushou_ex[iCurBushouIDOnViewingBattleMenuInBattle1-1].aptitudeyumi >= 4 ) {  // 雨弓がＡある

				// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
				WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleTeppouMenuInBattle), &(iNewAdressBufBattleAmeYumiMenuInBattle[2]), 4, NULL); // アドレスなので4バイト

			// 戦才が70(=140)を超える場合、｢雨弓弐」が出る。 INDEXは1引いてるの注意
			} else if (getBushouBat(iCurBushouIDOnViewingBattleMenuInBattle1-1) >= 50 &&    // 戦闘が50(=100)以上
				nb6savedata_ex.bushou_ex[iCurBushouIDOnViewingBattleMenuInBattle1-1].aptitudeyumi >= 3 ) {  // 雨弓がＢある

				// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
				WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleTeppouMenuInBattle), &(iNewAdressBufBattleAmeYumiMenuInBattle[1]), 4, NULL); // アドレスなので4バイト

			// 異なる場合は元へと戻す
			} else {

				// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
				WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleTeppouMenuInBattle), &(iNewAdressBufBattleAmeYumiMenuInBattle[0]), 4, NULL); // アドレスなので4バイト
			}
		}

	// 普通に鉄砲が使える状況の場合
	} else {

		// 戦才が110(=220)を超える場合、｢鉄砲極」が出る。 INDEXは1引いてるの注意
		if (getBushouBat(iCurBushouIDOnViewingBattleMenuInBattle1-1) >= 110 &&    // 戦闘が110(=220)以上
			getBushouAptitude(iCurBushouIDOnViewingBattleMenuInBattle1-1, APTITUDE_TEPPOU) >= 5 ) {  // 鉄砲がＳある

			// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
			WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleTeppouMenuInBattle), &(iNewAdressBufBattleTeppouMenuInBattle[4]), 4, NULL); // アドレスなので4バイト

		// 戦才が90(=180)を超える場合、｢鉄砲極」が出る。 INDEXは1引いてるの注意
		} else if (getBushouBat(iCurBushouIDOnViewingBattleMenuInBattle1-1) >= 90 &&    // 戦闘が90(=180)以上
			getBushouAptitude(iCurBushouIDOnViewingBattleMenuInBattle1-1, APTITUDE_TEPPOU) >= 5 ) {  // 鉄砲がＳある

			// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
			WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleTeppouMenuInBattle), &(iNewAdressBufBattleTeppouMenuInBattle[3]), 4, NULL); // アドレスなので4バイト

		// 戦才が80(=160)を超える場合、｢鉄砲参」が出る。 INDEXは1引いてるの注意
		} else if (getBushouBat(iCurBushouIDOnViewingBattleMenuInBattle1-1) >= 70 &&    // 戦闘が70(=140)以上
			getBushouAptitude(iCurBushouIDOnViewingBattleMenuInBattle1-1, APTITUDE_TEPPOU) >= 4 ) {  // 鉄砲がＡある

			// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
			WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleTeppouMenuInBattle), &(iNewAdressBufBattleTeppouMenuInBattle[2]), 4, NULL); // アドレスなので4バイト

		// 戦才が70(=140)を超える場合、｢鉄砲弐」が出る。 INDEXは1引いてるの注意
		} else if (getBushouBat(iCurBushouIDOnViewingBattleMenuInBattle1-1) >= 50 &&    // 戦闘が50(=100)以上
			getBushouAptitude(iCurBushouIDOnViewingBattleMenuInBattle1-1, APTITUDE_TEPPOU) >= 2 ) {  // 鉄砲がＣある

			// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
			WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleTeppouMenuInBattle), &(iNewAdressBufBattleTeppouMenuInBattle[1]), 4, NULL); // アドレスなので4バイト


		// 異なる場合は元へと戻す
		} else {

			// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
			WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleTeppouMenuInBattle), &iOrgAdressBufBattleTeppouMenuInBattle, 4, NULL); // アドレスなので4バイト
		}

	}


		//　雨大砲を使う条件が整っていて、かつ天気が雨の場合
	if ( isUseCurBushouAmeDeppou && getWeather() == 2 ) {

		// 戦才が110(=220)を超える場合、｢鉄砲極」が出る。 INDEXは1引いてるの注意
		if (getBushouBat(iCurBushouIDOnViewingBattleMenuInBattle1-1) >= 110 &&    // 戦闘が110(=220)以上
			getBushouAptitude(iCurBushouIDOnViewingBattleMenuInBattle1-1, APTITUDE_TEPPOU) >= 5 ) {  // 鉄砲がＳある

			// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
			WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleTaihouMenuInBattle), &(iNewAdressBufBattleAmeTaihouMenuInBattle[4]), 4, NULL); // アドレスなので4バイト

		// 戦才が90(=180)を超える場合、｢鉄砲極」が出る。 INDEXは1引いてるの注意
		} else if (getBushouBat(iCurBushouIDOnViewingBattleMenuInBattle1-1) >= 90 &&    // 戦闘が90(=180)以上
			getBushouAptitude(iCurBushouIDOnViewingBattleMenuInBattle1-1, APTITUDE_TEPPOU) >= 5 ) {  // 鉄砲がＳある

			// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
			WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleTaihouMenuInBattle), &(iNewAdressBufBattleAmeTaihouMenuInBattle[3]), 4, NULL); // アドレスなので4バイト

		// 戦才が80(=160)を超える場合、｢鉄砲参」が出る。 INDEXは1引いてるの注意
		} else if (getBushouBat(iCurBushouIDOnViewingBattleMenuInBattle1-1) >= 70 &&    // 戦闘が70(=140)以上
			getBushouAptitude(iCurBushouIDOnViewingBattleMenuInBattle1-1, APTITUDE_TEPPOU) >= 4 ) {  // 鉄砲がＡある

			// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
			WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleTaihouMenuInBattle), &(iNewAdressBufBattleAmeTaihouMenuInBattle[2]), 4, NULL); // アドレスなので4バイト

		// 戦才が70(=140)を超える場合、｢鉄砲弐」が出る。 INDEXは1引いてるの注意
		} else if (getBushouBat(iCurBushouIDOnViewingBattleMenuInBattle1-1) >= 50 &&    // 戦闘が50(=100)以上
			getBushouAptitude(iCurBushouIDOnViewingBattleMenuInBattle1-1, APTITUDE_TEPPOU) >= 2 ) {  // 鉄砲がＣある

			// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
			WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleTaihouMenuInBattle), &(iNewAdressBufBattleAmeTaihouMenuInBattle[1]), 4, NULL); // アドレスなので4バイト


		// 異なる場合は元へと戻す
		} else {

			// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
			WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleTaihouMenuInBattle), &(iNewAdressBufBattleAmeTaihouMenuInBattle[0]), 4, NULL); // アドレスなので4バイト
		}

	// 普通に大砲が使える状況の場合
	} else {

		// 戦才が110(=220)を超える場合、｢鉄砲極」が出る。 INDEXは1引いてるの注意
		if (getBushouBat(iCurBushouIDOnViewingBattleMenuInBattle1-1) >= 110 &&    // 戦闘が110(=220)以上
			getBushouAptitude(iCurBushouIDOnViewingBattleMenuInBattle1-1, APTITUDE_TEPPOU) >= 5 ) {  // 鉄砲がＳある

			// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
			WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleTaihouMenuInBattle), &(iNewAdressBufBattleTaihouMenuInBattle[4]), 4, NULL); // アドレスなので4バイト

		// 戦才が90(=180)を超える場合、｢鉄砲極」が出る。 INDEXは1引いてるの注意
		} else if (getBushouBat(iCurBushouIDOnViewingBattleMenuInBattle1-1) >= 90 &&    // 戦闘が90(=180)以上
			getBushouAptitude(iCurBushouIDOnViewingBattleMenuInBattle1-1, APTITUDE_TEPPOU) >= 5 ) {  // 鉄砲がＳある

			// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
			WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleTaihouMenuInBattle), &(iNewAdressBufBattleTaihouMenuInBattle[3]), 4, NULL); // アドレスなので4バイト

		// 戦才が80(=160)を超える場合、｢鉄砲参」が出る。 INDEXは1引いてるの注意
		} else if (getBushouBat(iCurBushouIDOnViewingBattleMenuInBattle1-1) >= 70 &&    // 戦闘が70(=140)以上
			getBushouAptitude(iCurBushouIDOnViewingBattleMenuInBattle1-1, APTITUDE_TEPPOU) >= 4 ) {  // 鉄砲がＡある

			// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
			WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleTaihouMenuInBattle), &(iNewAdressBufBattleTaihouMenuInBattle[2]), 4, NULL); // アドレスなので4バイト

		// 戦才が70(=140)を超える場合、｢鉄砲弐」が出る。 INDEXは1引いてるの注意
		} else if (getBushouBat(iCurBushouIDOnViewingBattleMenuInBattle1-1) >= 50 &&    // 戦闘が50(=100)以上
			getBushouAptitude(iCurBushouIDOnViewingBattleMenuInBattle1-1, APTITUDE_TEPPOU) >= 2 ) {  // 鉄砲がＣある

			// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
			WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleTaihouMenuInBattle), &(iNewAdressBufBattleTaihouMenuInBattle[1]), 4, NULL); // アドレスなので4バイト


		// 異なる場合は元へと戻す
		} else {

			// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
			WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleTaihouMenuInBattle), &iOrgAdressBufBattleTaihouMenuInBattle, 4, NULL); // アドレスなので4バイト
		}

	}


	// 武将IDが一定の範囲(それ以外の値が入ってくることもあるためチェック)
	if (0 <= iCurBushouIDOnViewingBattleMenuInBattle1-1 && iCurBushouIDOnViewingBattleMenuInBattle1-1 < GAMEDATASTRUCT_BUSHOU_NUM) {
		// シナリオモッドの対象メソッドが有効であれば…
		if ( p_snOnViewingBattleMenuInBattle ) {

			((PFNSNONVIEWINGBATTLEMENUINBATTLE) p_snOnViewingBattleMenuInBattle)(iCurBushouIDOnViewingBattleMenuInBattle1,
					pBufBattleCustomMenuInBattle[0],
					pBufBattleCustomMenuInBattle[1],
					pBufBattleCustomMenuInBattle[2],
					pBufBattleCustomMenuInBattle[3],
					pBufBattleCustomMenuInBattle[4]);

			for ( int n=0; n<SCENARIOMOD_BATTLE_CUSTOM_LABEL_NUM; n++ ) {
				if ( strlen( pBufBattleCustomMenuInBattle[n] ) > 0 ) {
					if ( n==0 ) {
						WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleAttackMenuInBattle), &(iNewAdressBufBattleCustomMenuInBattle[n]), 4, NULL); // アドレスなので4バイト
					} else if ( n==1 ) {
						WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleIsseiMenuInBattle), &(iNewAdressBufBattleCustomMenuInBattle[n]), 4, NULL); // アドレスなので4バイト
					} else if ( n==2 ) {
						WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleTotsugekiMenuInBattle), &(iNewAdressBufBattleCustomMenuInBattle[n]), 4, NULL); // アドレスなので4バイト
					} else if ( n==3 ) {
						WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleTeppouMenuInBattle), &(iNewAdressBufBattleCustomMenuInBattle[n]), 4, NULL); // アドレスなので4バイト
					} else if ( n==4 ) {
						WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleTaihouMenuInBattle), &(iNewAdressBufBattleCustomMenuInBattle[n]), 4, NULL); // アドレスなので4バイト
					}
				}
			}
		}
	}

}


/*
0042B482  |. 33F6           XOR     ESI, ESI
0042B484  |. B9 00B54B00    MOV     ECX, TENSHOU.004BB500
				└ここを JMP TSMod.OnTSExeViewingBattleMenuInBattle1 と書き換えてやる実際の処理
0042B489  |. 8B55 14        MOV     EDX, DWORD PTR SS:[EBP+14]
*/
int pTenshouExeJumpFromToOnTenshouExeViewingBattleMenuInBattle1		=0x42B484; // 関数はこのアドレスから、OnTenshouExeViewingBattleMenuInBattle1へとジャンプしてくる。
int pTenshouExeReturnLblFromOnTenshouExeViewingBattleMenuInBattle1	=0x42B489; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeViewingBattleMenuInBattle1() {
	// スタックにためておく
	__asm {
		// 武将のIDを保存しておく
		mov  iCurBushouIDOnViewingBattleMenuInBattle1, edi;

		// 元々TENSHOU.EXE内にあったものをここで
		mov  ecx, 4BB500h

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}
	
	// メニューの中身の文字列を置き換えたり
	OnTenshouExeViewingBattleMenuInBattle1Execute();

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

		jmp pTenshouExeReturnLblFromOnTenshouExeViewingBattleMenuInBattle1
	}
}

/*
0042B482  |. 33F6           XOR     ESI, ESI
0042B484  |. B9 00B54B00    MOV     ECX, TENSHOU.004BB500
				└ここを JMP TSMod.OnTSExeViewingBattleMenuInBattle1 と書き換えてやる実際の処理
0042B489  |. 8B55 14        MOV     EDX, DWORD PTR SS:[EBP+14]
*/
char cmdOnTenshouExeJumpFromViewingBattleMenuInBattle1[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ターンがチェンジしたタイミング
void WriteAsmJumperOnTenshouExeViewingBattleMenuInBattle1() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeViewingBattleMenuInBattle1;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeViewingBattleMenuInBattle1 + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeViewingBattleMenuInBattle1  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromViewingBattleMenuInBattle1+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeViewingBattleMenuInBattle1), cmdOnTenshouExeJumpFromViewingBattleMenuInBattle1, 5, NULL); //5バイトのみ書き込む
}




//------------------------------------------------------------------------------------------------------



void OnTenshouExeViewingBattleMenuInBattle2Execute() {
	// (今は何もなし)
	;
}

/*
0044848B  |. 74 10          JE      SHORT TENSHOU.0044849D
0044848D  |. A1 00B54B00    MOV     EAX, DWORD PTR DS:[4BB500]
				└ここを JMP TSMod.OnTSExeViewingBattleMenuInBattle2 と書き換えてやる実際の処理
00448492  |. 8D5E 01        LEA     EBX, DWORD PTR DS:[ESI+1]
*/
int pTenshouExeJumpFromToOnTenshouExeViewingBattleMenuInBattle2		=0x44848D; // 関数はこのアドレスから、OnTenshouExeViewingBattleMenuInBattle2へとジャンプしてくる。
int pTenshouExeReturnLblFromOnTenshouExeViewingBattleMenuInBattle2	=0x448492; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeViewingBattleMenuInBattle2() {
	// スタックにためておく
	__asm {
		// 元々TENSHOU.EXE内にあったものをここで
		mov  eax, DWORD PTR DS:[4BB500h]

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}
	
	// こちらでは原則は何もしない
	OnTenshouExeViewingBattleMenuInBattle2Execute();

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

		jmp pTenshouExeReturnLblFromOnTenshouExeViewingBattleMenuInBattle2
	}
}

/*
0044848B  |. 74 10          JE      SHORT TENSHOU.0044849D
0044848D  |. A1 00B54B00    MOV     EAX, DWORD PTR DS:[4BB500]
				└ここを JMP TSMod.OnTSExeViewingBattleMenuInBattle2 と書き換えてやる実際の処理
00448492  |. 8D5E 01        LEA     EBX, DWORD PTR DS:[ESI+1]
*/
char cmdOnTenshouExeJumpFromViewingBattleMenuInBattle2[6]="\xE9";
// 元の命令が6バイト、以後の関数で生まれる命令が合計５バイトなので、NOPが最後に付け足されている。


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeViewingBattleMenuInBattle2() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeViewingBattleMenuInBattle2;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeViewingBattleMenuInBattle2 + 5 );
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeViewingBattleMenuInBattle2  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromViewingBattleMenuInBattle2+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeViewingBattleMenuInBattle2), cmdOnTenshouExeJumpFromViewingBattleMenuInBattle2, 5, NULL); // 6バイトのみ書き込む 元の命令が6バイト、新規の命令が５バイトなので、NOPが付け足されている。
}


