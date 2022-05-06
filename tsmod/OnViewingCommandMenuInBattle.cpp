#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"
#include "TSModCommand.h"
#include "ScenarioMod.h"





//------------------------------------------------------------------------------------------------------

/*　　　　　 ┌技能　　　　┌内応		 ┌鼓舞    　  ┌退却
004CD508  [9C D4 4C] 00 [B0 AF 4B] 00 [94 D4 4C] 00 [8C D4 4C] 00  慟L.ｰｯK.番L.菰L.
			 ┌城外
004CD518  [84 D4 4C] 00                                      ・L..


004CD494  8C DB 95 91 00 00 00 00 8B 5A 94 5C 00 00 00 00  鼓舞....技能....



004BB508  A4 B4 4B 00 98 B4 4B 00 8C B4 4B 00 00 00 00 00  ､ｴK.亢K.原K.....
004BB518  [84 B4 4B] 00 [7C B4 4B] 00 [74 B4 4B] 00 [6C B4 4B] 00  ╋K.|ｴK.tｴK.lｴK.
             └ 一喝　　　└徴兵 　     └混乱        └火攻め
004BB528  [64 B4 4B] 00 [5C B4 4B] 00 [54 B4 4B] 00 00 00 00 00  dｴK.\ｴK.TｴK.....
   　　　　　└ 暗殺	  └煽動		└挑発
004BB538  4C B4 4B 00 44                                LｴK.D

004CD484  8F E9 8A 4F 00 00 00 00 91 DE 8B 70 00 00 00 00  城外....退却....
004CD494  8C DB 95 91 00 00 00 00 8B 5A 94 5C 00 00 00 00  鼓舞....技能....


004BB454  92 A7 94 AD 00 00 00 00 90 F8 93 AE 00 00 00 00  挑発....煽動....
004BB464  88 C3 8E 45 00 00 00 00 89 CE 8D 55 82 DF 00 00  暗殺....火攻め..
004BB474  8D AC 97 90 00 00 00 00 92 A5 95 BA 00 00 00 00  混乱....徴兵....
004BB484  88 EA 8A 85 00 00 00 00                          一喝....一斉
*/

// 現在の武将ID
int iCurBushouIDOnViewingCommandMenuInBattle = -1;




#define SCENARIOMOD_BASE_CUSTOM_LABEL_NUM		12


char pBufBaseCustomMenuInBattle[SCENARIOMOD_BASE_CUSTOM_LABEL_NUM][20] = {"","","","","","","","","","","",""};

// 「カスタム」を指す、新たなアドレス５つ
int iNewAdressBufBaseCustomMenuInBattle[SCENARIOMOD_BASE_CUSTOM_LABEL_NUM] = {
	(int)(pBufBaseCustomMenuInBattle[0]),
	(int)(pBufBaseCustomMenuInBattle[1]),
	(int)(pBufBaseCustomMenuInBattle[2]),
	(int)(pBufBaseCustomMenuInBattle[3]),
	(int)(pBufBaseCustomMenuInBattle[4]),
	(int)(pBufBaseCustomMenuInBattle[5]),
	(int)(pBufBaseCustomMenuInBattle[6]),
	(int)(pBufBaseCustomMenuInBattle[7]),
	(int)(pBufBaseCustomMenuInBattle[8]),
	(int)(pBufBaseCustomMenuInBattle[9]),
	(int)(pBufBaseCustomMenuInBattle[10]),
	(int)(pBufBaseCustomMenuInBattle[11]),
};


//--------------------- 共通系 -----------------------

#define COMMAND_LEVEL_NUM 7

//--------------------- 鼓舞系 -----------------------
char pBufBattleMenuTokukreiXInBattle[] = "総督励";

// 「総督励」を指す、新たなアドレス
int iNewAdressBufBattleTokureiMenuInBattle = (int)pBufBattleMenuTokukreiXInBattle;

// 「鼓舞」を指す、元のアドレス
int iOrgAdressBufBattleKobuMenuInBattle = 0x4CD494;

// 「鼓舞」の文字列を指すポインタ値が入っているアドレス
int pOrgAddressBattleKobuMenuInBattle = 0x4CD510;


//--------------------- 城外系 -----------------------
// 「城外」を指す、元のアドレス
int iOrgAdressBufBattleJogaiMenuInBattle = 0x4CD484;

// 「城外」の文字列を指すポインタ値が入っているアドレス
int pOrgAddressBattleJogaiMenuInBattle = 0x4CD518;


//--------------------- 退却系 -----------------------
// 「退却」を指す、元のアドレス
int iOrgAdressBufBattleTaikyakuMenuInBattle = 0x4CD48C;

// 「退却」の文字列を指すポインタ値が入っているアドレス
int pOrgAddressBattleTaikyakuMenuInBattle = 0x4CD514;



//--------------------- 内応系 -----------------------
// 「内応」を指す、元のアドレス
int iOrgAdressBufBattleNaiouMenuInBattle = 0x4BAFB0;

// 「内応」の文字列を指すポインタ値が入っているアドレス
int pOrgAddressBattleNaiouMenuInBattle = 0x4CD50C;


//--------------------- 技能系 -----------------------
// 新たなコマンド名の配列
char pBufCommandMenuGinouInBattle[COMMAND_LEVEL_NUM][10] = {"技能･壱","技能･弐","技能･参","技能･四","技能･極","6","技能･真"};

// 「挑発系」を指す、新たなアドレス
int iNewAdressBufCommandMenuGinouInBattle[8] = {
	(int)(pBufCommandMenuGinouInBattle[0]),
	(int)(pBufCommandMenuGinouInBattle[1]),
	(int)(pBufCommandMenuGinouInBattle[2]),
	(int)(pBufCommandMenuGinouInBattle[3]),
	(int)(pBufCommandMenuGinouInBattle[4]),
	(int)(pBufCommandMenuGinouInBattle[5]),
	(int)(pBufCommandMenuGinouInBattle[6]),
	(int)(pBufCommandMenuGinouInBattle[7]),
};


// 「技能」を指す、元のアドレス
int iOrgAdressBufBattleGinouMenuInBattle = 0x4CD49C;

// 「技能」の文字列を指すポインタ値が入っているアドレス
int pOrgAddressBattleGinouMenuInBattle = 0x4CD508;



//--------------------- 挑発系 -----------------------


// 新たなコマンド名の配列
char pBufCommandMenuChohatsuInBattle[COMMAND_LEVEL_NUM][10] = {"挑発","挑発･弐","挑発･参","挑発･四","挑発･極","6","挑発･真"};

// 「挑発系」を指す、新たなアドレス
int iNewAdressBufCommandMenuChohatsuInBattle[COMMAND_LEVEL_NUM] = {
	(int)(pBufCommandMenuChohatsuInBattle[0]),
	(int)(pBufCommandMenuChohatsuInBattle[1]),
	(int)(pBufCommandMenuChohatsuInBattle[2]),
	(int)(pBufCommandMenuChohatsuInBattle[3]),
	(int)(pBufCommandMenuChohatsuInBattle[4]),
	(int)(pBufCommandMenuChohatsuInBattle[5]),
	(int)(pBufCommandMenuChohatsuInBattle[6]),
};

// 「挑発」を指す、元のアドレス
int iOrgAdressBufBattleChohatsuMenuInBattle = 0x4BB454;

// 「挑発」の文字列を指すポインタ値が入っているアドレス
int pOrgAddressBattleChohatsuMenuInBattle = 0x4BB530;



// 新たなコマンド名の配列
char pBufCommandMenuSendouInBattle[COMMAND_LEVEL_NUM][10] = {"煽動","煽動･弐","煽動･参","煽動･四","煽動･極","6","煽動･真"};

// 「煽動系」を指す、新たなアドレス
int iNewAdressBufCommandMenuSendouInBattle[COMMAND_LEVEL_NUM] = {
	(int)(pBufCommandMenuSendouInBattle[0]),
	(int)(pBufCommandMenuSendouInBattle[1]),
	(int)(pBufCommandMenuSendouInBattle[2]),
	(int)(pBufCommandMenuSendouInBattle[3]),
	(int)(pBufCommandMenuSendouInBattle[4]),
	(int)(pBufCommandMenuSendouInBattle[5]),
	(int)(pBufCommandMenuSendouInBattle[6]),
};

// 「煽動」を指す、元のアドレス
int iOrgAdressBufBattleSendouMenuInBattle = 0x4BB45C;

// 「煽動」の文字列を指すポインタ値が入っているアドレス
int pOrgAddressBattleSendouMenuInBattle = 0x4BB52C;





//--------------------- 混乱系 -----------------------

// 新たなコマンド名の配列
char pBufCommandMenuKonranInBattle[COMMAND_LEVEL_NUM][10] = {"混乱","混乱･弐","混乱･参","混乱･四","混乱･極","6","混乱･真"};

// 「混乱系」を指す、新たなアドレス
int iNewAdressBufCommandMenuKonranInBattle[COMMAND_LEVEL_NUM] = {
	(int)(pBufCommandMenuKonranInBattle[0]),
	(int)(pBufCommandMenuKonranInBattle[1]),
	(int)(pBufCommandMenuKonranInBattle[2]),
	(int)(pBufCommandMenuKonranInBattle[3]),
	(int)(pBufCommandMenuKonranInBattle[4]),
	(int)(pBufCommandMenuKonranInBattle[5]),
	(int)(pBufCommandMenuKonranInBattle[6]),
};

// 「混乱」を指す、元のアドレス
int iOrgAdressBufBattleKonranMenuInBattle = 0x4BB474;

// 「混乱」の文字列を指すポインタ値が入っているアドレス
int pOrgAddressBattleKonranMenuInBattle = 0x4BB520;


//--------------------- 火攻系 -----------------------

// 新たなコマンド名の配列
char pBufCommandMenuHizemeInBattle[COMMAND_LEVEL_NUM][10] = {"火攻め","火攻･弐","火攻･参","火攻･四","火攻･極","6","火攻･真"};

// 「火攻系」を指す、新たなアドレス
int iNewAdressBufCommandMenuHizemeInBattle[COMMAND_LEVEL_NUM] = {
	(int)(pBufCommandMenuHizemeInBattle[0]),
	(int)(pBufCommandMenuHizemeInBattle[1]),
	(int)(pBufCommandMenuHizemeInBattle[2]),
	(int)(pBufCommandMenuHizemeInBattle[3]),
	(int)(pBufCommandMenuHizemeInBattle[4]),
	(int)(pBufCommandMenuHizemeInBattle[5]),
	(int)(pBufCommandMenuHizemeInBattle[6]),
};

//--------------------- 忍者火攻系 -----------------------

// 新たなコマンド名の配列
char pBufCommandMenuKatonInBattle[COMMAND_LEVEL_NUM][10] = {"火遁","火遁･弐","火遁･参","火遁･四","火遁･極","6","火遁･真"};

// 「火遁系」を指す、新たなアドレス
int iNewAdressBufCommandMenuKatonInBattle[COMMAND_LEVEL_NUM] = {
	(int)(pBufCommandMenuKatonInBattle[0]),
	(int)(pBufCommandMenuKatonInBattle[1]),
	(int)(pBufCommandMenuKatonInBattle[2]),
	(int)(pBufCommandMenuKatonInBattle[3]),
	(int)(pBufCommandMenuKatonInBattle[4]),
	(int)(pBufCommandMenuKatonInBattle[5]),
	(int)(pBufCommandMenuKatonInBattle[6]),
};


// 「火攻」を指す、元のアドレス
int iOrgAdressBufBattleHizemeMenuInBattle = 0x4BB46C;

// 「火攻」の文字列を指すポインタ値が入っているアドレス
int pOrgAddressBattleHizemeMenuInBattle = 0x4BB524;


//--------------------- 暗殺系 -----------------------

// 新たなコマンド名の配列
char pBufCommandMenuAnsatsuInBattle[COMMAND_LEVEL_NUM][10] = {"暗殺","暗殺･弐","暗殺･参","暗殺･四","暗殺･極","6","暗殺･真"};

// 「暗殺系」を指す、新たなアドレス
int iNewAdressBufCommandMenuAnsatsuInBattle[COMMAND_LEVEL_NUM] = {
	(int)(pBufCommandMenuAnsatsuInBattle[0]),
	(int)(pBufCommandMenuAnsatsuInBattle[1]),
	(int)(pBufCommandMenuAnsatsuInBattle[2]),
	(int)(pBufCommandMenuAnsatsuInBattle[3]),
	(int)(pBufCommandMenuAnsatsuInBattle[4]),
	(int)(pBufCommandMenuAnsatsuInBattle[5]),
	(int)(pBufCommandMenuAnsatsuInBattle[6]),
};

// 「暗殺」を指す、元のアドレス
int iOrgAdressBufBattleAnsatsuMenuInBattle = 0x4BB464;

// 「暗殺」の文字列を指すポインタ値が入っているアドレス
int pOrgAddressBattleAnsatsuMenuInBattle = 0x4BB528;


//--------------------- 徴兵系 -----------------------

// 新たなコマンド名の配列
char pBufCommandMenuChoheiInBattle[COMMAND_LEVEL_NUM][10] = {"徴兵","徴兵･弐","徴兵･参","徴兵･四","徴兵･極","6","徴兵･真"};

// 「徴兵系」を指す、新たなアドレス
int iNewAdressBufCommandMenuChoheiInBattle[COMMAND_LEVEL_NUM] = {
	(int)(pBufCommandMenuChoheiInBattle[0]),
	(int)(pBufCommandMenuChoheiInBattle[1]),
	(int)(pBufCommandMenuChoheiInBattle[2]),
	(int)(pBufCommandMenuChoheiInBattle[3]),
	(int)(pBufCommandMenuChoheiInBattle[4]),
	(int)(pBufCommandMenuChoheiInBattle[5]),
	(int)(pBufCommandMenuChoheiInBattle[6]),
};

// 「徴兵」を指す、元のアドレス
int iOrgAdressBufBattleChoheiMenuInBattle = 0x4BB47C;

// 「徴兵」の文字列を指すポインタ値が入っているアドレス
int pOrgAddressBattleChoheiMenuInBattle = 0x4BB51C;


//--------------------- 一喝系 -----------------------

// 新たなコマンド名の配列
char pBufCommandMenuIkkatsuInBattle[COMMAND_LEVEL_NUM][10] = {"一喝","一喝･弐","一喝･参","一喝･四","一喝･極","6","一喝･真"};

// 「一喝系」を指す、新たなアドレス
int iNewAdressBufCommandMenuIkkatsuInBattle[COMMAND_LEVEL_NUM] = {
	(int)(pBufCommandMenuIkkatsuInBattle[0]),
	(int)(pBufCommandMenuIkkatsuInBattle[1]),
	(int)(pBufCommandMenuIkkatsuInBattle[2]),
	(int)(pBufCommandMenuIkkatsuInBattle[3]),
	(int)(pBufCommandMenuIkkatsuInBattle[4]),
	(int)(pBufCommandMenuIkkatsuInBattle[5]),
	(int)(pBufCommandMenuIkkatsuInBattle[6]),
};

// 「一喝」を指す、元のアドレス
int iOrgAdressBufBattleIkkatsuMenuInBattle = 0x4BB484;

// 「一喝」の文字列を指すポインタ値が入っているアドレス
int pOrgAddressBattleIkkatsuMenuInBattle = 0x4BB518;

//-------------------------------------------


void OnTenshouExeViewingCommandMenuInBattleExecute() {

	// 武将範囲チェック
	if (0 <= iCurBushouIDOnViewingCommandMenuInBattle-1 && iCurBushouIDOnViewingCommandMenuInBattle-1 < GAMEDATASTRUCT_BUSHOU_NUM) {
	} else {
		return;
	}

	if ( 1 ) {
		// 鼓舞の文字列の変更。今のところ、変更していないので、必ずデフォルトへと戻すのみ。(前の人やScenarioModが書き換えていた時のため)
		WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleKobuMenuInBattle), &iOrgAdressBufBattleKobuMenuInBattle, 4, NULL); // アドレスなので4バイト
		// 城外の文字列の変更。今のところ、変更していないので、必ずデフォルトへと戻すのみ。(前の人やScenarioModが書き換えていた時のため)
		WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleJogaiMenuInBattle), &iOrgAdressBufBattleJogaiMenuInBattle, 4, NULL); // アドレスなので4バイト
		// 退却の文字列の変更。今のところ、変更していないので、必ずデフォルトへと戻すのみ。(前の人やScenarioModが書き換えていた時のため)
		WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleTaikyakuMenuInBattle), &iOrgAdressBufBattleTaikyakuMenuInBattle, 4, NULL); // アドレスなので4バイト
		// 内応の文字列の変更。今のところ、変更していないので、必ずデフォルトへと戻すのみ。(前の人やScenarioModが書き換えていた時のため)
		WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleNaiouMenuInBattle), &iOrgAdressBufBattleNaiouMenuInBattle, 4, NULL); // アドレスなので4バイト
		// 技能の文字列の変更。今のところ、変更していないので、必ずデフォルトへと戻すのみ。(前の人やScenarioModが書き換えていた時のため)
		WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleGinouMenuInBattle), &iOrgAdressBufBattleGinouMenuInBattle, 4, NULL); // アドレスなので4バイト
		// 挑発の文字列の変更。今のところ、変更していないので、必ずデフォルトへと戻すのみ。(前の人やScenarioModが書き換えていた時のため)
		WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleChohatsuMenuInBattle), &iOrgAdressBufBattleChohatsuMenuInBattle, 4, NULL); // アドレスなので4バイト
		// 扇動の文字列の変更。今のところ、変更していないので、必ずデフォルトへと戻すのみ。(前の人やScenarioModが書き換えていた時のため)
		WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleSendouMenuInBattle), &iOrgAdressBufBattleSendouMenuInBattle, 4, NULL); // アドレスなので4バイト
		// 混乱の文字列の変更。今のところ、変更していないので、必ずデフォルトへと戻すのみ。(前の人やScenarioModが書き換えていた時のため)
		WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleKonranMenuInBattle), &iOrgAdressBufBattleKonranMenuInBattle, 4, NULL); // アドレスなので4バイト
		// 火攻めの文字列の変更。今のところ、変更していないので、必ずデフォルトへと戻すのみ。(前の人やScenarioModが書き換えていた時のため)
		WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleHizemeMenuInBattle), &iOrgAdressBufBattleHizemeMenuInBattle, 4, NULL); // アドレスなので4バイト
		// 暗殺の文字列の変更。今のところ、変更していないので、必ずデフォルトへと戻すのみ。(前の人やScenarioModが書き換えていた時のため)
		WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleAnsatsuMenuInBattle), &iOrgAdressBufBattleAnsatsuMenuInBattle, 4, NULL); // アドレスなので4バイト
		// 徴兵の文字列の変更。今のところ、変更していないので、必ずデフォルトへと戻すのみ。(前の人やScenarioModが書き換えていた時のため)
		WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleChoheiMenuInBattle), &iOrgAdressBufBattleChoheiMenuInBattle, 4, NULL); // アドレスなので4バイト
		// 一喝の文字列の変更。今のところ、変更していないので、必ずデフォルトへと戻すのみ。(前の人やScenarioModが書き換えていた時のため)
		WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleIkkatsuMenuInBattle), &iOrgAdressBufBattleIkkatsuMenuInBattle, 4, NULL); // アドレスなので4バイト
	}

	
	// 技能の文字列の変更。技能数に応じて、文字列を変更する。
	if ( 1 ) {
		WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleGinouMenuInBattle), &iOrgAdressBufBattleGinouMenuInBattle, 4, NULL); // アドレスなので4バイト
	}

	// ランスロット・タルタロスである。
	if ( isLancelotTartare(iCurBushouIDOnViewingCommandMenuInBattle-1) ) {

		// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
		WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleKobuMenuInBattle), &iNewAdressBufBattleTokureiMenuInBattle, 4, NULL); // アドレスなので4バイト

	// 他は鼓舞
	} else {
		// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
		WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleKobuMenuInBattle), &iOrgAdressBufBattleKobuMenuInBattle, 4, NULL); // アドレスなので4バイト
	}

	//.-----------------------------------------------------
	//智謀関連が一定条件を超える場合、｢挑発極・混乱極」が出る。 INDEXは1引いてるの注意
	if (getBushouInt(iCurBushouIDOnViewingCommandMenuInBattle-1) >= 110 &&   // 智謀が110(=220)以上
		nb6bushouref[iCurBushouIDOnViewingCommandMenuInBattle-1].maxint >= 110) { // 智才が110(=220)以上

		// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
		WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleChohatsuMenuInBattle), &(iNewAdressBufCommandMenuChohatsuInBattle[4]), 4, NULL); // アドレスなので4バイト
		WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleKonranMenuInBattle), &(iNewAdressBufCommandMenuKonranInBattle[4]), 4, NULL); // アドレスなので4バイト

	//智謀関連が一定条件を超える場合、｢挑発四・混乱四」が出る。 INDEXは1引いてるの注意
	} else if (getBushouInt(iCurBushouIDOnViewingCommandMenuInBattle-1) >= 90 &&   // 智謀が90(=180)以上
		nb6bushouref[iCurBushouIDOnViewingCommandMenuInBattle-1].maxint >= 100) { // 智才が100(=200)以上

		// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
		WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleChohatsuMenuInBattle), &(iNewAdressBufCommandMenuChohatsuInBattle[3]), 4, NULL); // アドレスなので4バイト
		WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleKonranMenuInBattle), &(iNewAdressBufCommandMenuKonranInBattle[3]), 4, NULL); // アドレスなので4バイト


	//智謀関連が一定条件を超える場合、｢挑発参・混乱参」が出る。 INDEXは1引いてるの注意
	} else if (getBushouInt(iCurBushouIDOnViewingCommandMenuInBattle-1) >= 70 &&   // 智謀が70(=140)以上
		nb6bushouref[iCurBushouIDOnViewingCommandMenuInBattle-1].maxint >= 85) { // 智才が85(=170)以上

		// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
		WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleChohatsuMenuInBattle), &(iNewAdressBufCommandMenuChohatsuInBattle[2]), 4, NULL); // アドレスなので4バイト
		WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleKonranMenuInBattle), &(iNewAdressBufCommandMenuKonranInBattle[2]), 4, NULL); // アドレスなので4バイト


	//智謀関連が一定条件を超える場合、｢挑発弐・混乱弐」が出る。 INDEXは1引いてるの注意
	} else if (getBushouInt(iCurBushouIDOnViewingCommandMenuInBattle-1) >= 50 &&   // 智謀が50(=100)以上
		nb6bushouref[iCurBushouIDOnViewingCommandMenuInBattle-1].maxint >= 70) { // 智才が70(=140)以上

		// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
		WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleChohatsuMenuInBattle), &(iNewAdressBufCommandMenuChohatsuInBattle[1]), 4, NULL); // アドレスなので4バイト
		WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleKonranMenuInBattle), &(iNewAdressBufCommandMenuKonranInBattle[1]), 4, NULL); // アドレスなので4バイト

	// 他はデフォに戻す
	} else {
		// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
		WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleChohatsuMenuInBattle), &iOrgAdressBufBattleChohatsuMenuInBattle, 4, NULL); // アドレスなので4バイト
		WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleKonranMenuInBattle), &iOrgAdressBufBattleKonranMenuInBattle, 4, NULL); // アドレスなので4バイト
	}


	//.-----------------------------------------------------
	//智謀と魅力関連が一定条件を超える場合、｢徴兵極」が出る。 INDEXは1引いてるの注意
	if (getBushouInt(iCurBushouIDOnViewingCommandMenuInBattle-1) >= 110 &&   // 智謀が110(=220)以上
		nb6bushouref[iCurBushouIDOnViewingCommandMenuInBattle-1].maxint >= 110 && // 智才が110(=220)以上
		nb6bushouref[iCurBushouIDOnViewingCommandMenuInBattle-1].charm >= 110		// 魅力が110以上
		) { 

		// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
		WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleChoheiMenuInBattle), &(iNewAdressBufCommandMenuChoheiInBattle[4]), 4, NULL); // アドレスなので4バイト

	//智謀と魅力関連が一定条件を超える場合、｢徴兵四」が出る。 INDEXは1引いてるの注意
	} else if (getBushouInt(iCurBushouIDOnViewingCommandMenuInBattle-1) >= 90 &&   // 智謀が90(=180)以上
		nb6bushouref[iCurBushouIDOnViewingCommandMenuInBattle-1].maxint >= 100 &&	 // 智才が100(=200)以上
		nb6bushouref[iCurBushouIDOnViewingCommandMenuInBattle-1].charm >= 90) {	// 魅力が90以上

		// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
		WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleChoheiMenuInBattle), &(iNewAdressBufCommandMenuChoheiInBattle[3]), 4, NULL); // アドレスなので4バイト


	//智謀と魅力関連が一定条件を超える場合、｢徴兵参」が出る。 INDEXは1引いてるの注意
	} else if (getBushouInt(iCurBushouIDOnViewingCommandMenuInBattle-1) >= 70 &&   // 智謀が70(=140)以上
		nb6bushouref[iCurBushouIDOnViewingCommandMenuInBattle-1].maxint >= 85 &&	 // 智才が85(=170)以上
		nb6bushouref[iCurBushouIDOnViewingCommandMenuInBattle-1].charm >= 70) {	// 魅力が70以上

		// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
		WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleChoheiMenuInBattle), &(iNewAdressBufCommandMenuChoheiInBattle[2]), 4, NULL); // アドレスなので4バイト


	//智謀と魅力関連が一定条件を超える場合、｢徴兵弐」が出る。 INDEXは1引いてるの注意
	} else if (getBushouInt(iCurBushouIDOnViewingCommandMenuInBattle-1) >= 50 &&   // 智謀が50(=100)以上
		nb6bushouref[iCurBushouIDOnViewingCommandMenuInBattle-1].maxint >= 70 &&  // 智才が70(=140)以上
		nb6bushouref[iCurBushouIDOnViewingCommandMenuInBattle-1].charm >= 50) {	// 魅力が50以上

		// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
		WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleChoheiMenuInBattle), &(iNewAdressBufCommandMenuChoheiInBattle[1]), 4, NULL); // アドレスなので4バイト

	// 他はデフォに戻す
	} else {
		// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
		WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleChoheiMenuInBattle), &iOrgAdressBufBattleChoheiMenuInBattle, 4, NULL); // アドレスなので4バイト
	}



	//.-----------------------------------------------------
	//智謀と野望関連が一定条件を超える場合、｢一喝極・煽動極」が出る。 INDEXは1引いてるの注意
	if (getBushouInt(iCurBushouIDOnViewingCommandMenuInBattle-1) >= 110 &&   // 智謀が110(=220)以上
		nb6bushouref[iCurBushouIDOnViewingCommandMenuInBattle-1].maxint >= 110 && // 智才が110(=220)以上
		nb6bushouref[iCurBushouIDOnViewingCommandMenuInBattle-1].ambition >= 110		// 野望が110以上
		) { 

		// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
		WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleIkkatsuMenuInBattle), &(iNewAdressBufCommandMenuIkkatsuInBattle[4]), 4, NULL); // アドレスなので4バイト
		WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleSendouMenuInBattle), &(iNewAdressBufCommandMenuSendouInBattle[4]), 4, NULL); // アドレスなので4バイト

	//智謀と野望関連が一定条件を超える場合、｢一喝四・煽動四」が出る。 INDEXは1引いてるの注意
	} else if (getBushouInt(iCurBushouIDOnViewingCommandMenuInBattle-1) >= 90 &&   // 智謀が90(=180)以上
		nb6bushouref[iCurBushouIDOnViewingCommandMenuInBattle-1].maxint >= 100 &&	 // 智才が100(=200)以上
		nb6bushouref[iCurBushouIDOnViewingCommandMenuInBattle-1].ambition >= 90) {	// 野望が90以上

		// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
		WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleIkkatsuMenuInBattle), &(iNewAdressBufCommandMenuIkkatsuInBattle[3]), 4, NULL); // アドレスなので4バイト
		WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleSendouMenuInBattle), &(iNewAdressBufCommandMenuSendouInBattle[3]), 4, NULL); // アドレスなので4バイト


	//智謀と野望関連が一定条件を超える場合、｢一喝参・煽動参」が出る。 INDEXは1引いてるの注意
	} else if (getBushouInt(iCurBushouIDOnViewingCommandMenuInBattle-1) >= 70 &&   // 智謀が70(=140)以上
		nb6bushouref[iCurBushouIDOnViewingCommandMenuInBattle-1].maxint >= 85 &&	 // 智才が85(=170)以上
		nb6bushouref[iCurBushouIDOnViewingCommandMenuInBattle-1].ambition >= 70) {	// 野望が70以上

		// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
		WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleIkkatsuMenuInBattle), &(iNewAdressBufCommandMenuIkkatsuInBattle[2]), 4, NULL); // アドレスなので4バイト
		WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleSendouMenuInBattle), &(iNewAdressBufCommandMenuSendouInBattle[2]), 4, NULL); // アドレスなので4バイト


	//智謀と野望関連が一定条件を超える場合、｢一喝弐・煽動弐」が出る。 INDEXは1引いてるの注意
	} else if (getBushouInt(iCurBushouIDOnViewingCommandMenuInBattle-1) >= 50 &&   // 智謀が50(=100)以上
		nb6bushouref[iCurBushouIDOnViewingCommandMenuInBattle-1].maxint >= 70 &&  // 智才が70(=140)以上
		nb6bushouref[iCurBushouIDOnViewingCommandMenuInBattle-1].ambition >= 50) {	// 野望が50以上

		// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
		WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleIkkatsuMenuInBattle), &(iNewAdressBufCommandMenuIkkatsuInBattle[1]), 4, NULL); // アドレスなので4バイト
		WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleSendouMenuInBattle), &(iNewAdressBufCommandMenuSendouInBattle[1]), 4, NULL); // アドレスなので4バイト

	// 他はデフォに戻す
	} else {
		// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
		WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleIkkatsuMenuInBattle), &iOrgAdressBufBattleIkkatsuMenuInBattle, 4, NULL); // アドレスなので4バイト
		WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleSendouMenuInBattle), &iOrgAdressBufBattleSendouMenuInBattle, 4, NULL); // アドレスなので4バイト
	}



	//.-----------------------------------------------------
	// 忍者以外であれば…
	if (nb6bushouref[iCurBushouIDOnViewingCommandMenuInBattle-1].job != 0x10 ) {

		//智謀関連が一定条件を超える場合、｢火攻極」が出る。 INDEXは1引いてるの注意
		if (getBushouInt(iCurBushouIDOnViewingCommandMenuInBattle-1) >= 110 &&   // 智謀が110(=220)以上
			nb6bushouref[iCurBushouIDOnViewingCommandMenuInBattle-1].maxint >= 110) { // 智才が110(=220)以上

			// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
			WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleHizemeMenuInBattle), &(iNewAdressBufCommandMenuHizemeInBattle[4]), 4, NULL); // アドレスなので4バイト

		//智謀関連が一定条件を超える場合、｢火攻四」が出る。 INDEXは1引いてるの注意
		} else if (getBushouInt(iCurBushouIDOnViewingCommandMenuInBattle-1) >= 90 &&   // 智謀が90(=180)以上
			nb6bushouref[iCurBushouIDOnViewingCommandMenuInBattle-1].maxint >= 100) { // 智才が100(=200)以上

			// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
			WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleHizemeMenuInBattle), &(iNewAdressBufCommandMenuHizemeInBattle[3]), 4, NULL); // アドレスなので4バイト


		//智謀関連が一定条件を超える場合、｢火攻参」が出る。 INDEXは1引いてるの注意
		} else if (getBushouInt(iCurBushouIDOnViewingCommandMenuInBattle-1) >= 70 &&   // 智謀が70(=140)以上
			nb6bushouref[iCurBushouIDOnViewingCommandMenuInBattle-1].maxint >= 85) { // 智才が85(=170)以上

			// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
			WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleHizemeMenuInBattle), &(iNewAdressBufCommandMenuHizemeInBattle[2]), 4, NULL); // アドレスなので4バイト


		//智謀関連が一定条件を超える場合、｢火攻弐」が出る。 INDEXは1引いてるの注意
		} else if (getBushouInt(iCurBushouIDOnViewingCommandMenuInBattle-1) >= 50 &&   // 智謀が50(=100)以上
			nb6bushouref[iCurBushouIDOnViewingCommandMenuInBattle-1].maxint >= 70) { // 智才が70(=140)以上

			// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
			WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleHizemeMenuInBattle), &(iNewAdressBufCommandMenuHizemeInBattle[1]), 4, NULL); // アドレスなので4バイト

		// 他はデフォに戻す
		} else {
			// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
			WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleHizemeMenuInBattle), &iOrgAdressBufBattleHizemeMenuInBattle, 4, NULL); // アドレスなので4バイト
		}


		//.-----------------------------------------------------
		// 暗殺系…
		//智謀と野望が一定条件を超える場合、｢暗殺四」が出る。 INDEXは1引いてるの注意
		if (getBushouInt(iCurBushouIDOnViewingCommandMenuInBattle-1) >= 110 &&   // 智謀が110(=220)以上
			nb6bushouref[iCurBushouIDOnViewingCommandMenuInBattle-1].maxint >= 110 && // 智才が110(=220)以上
			nb6bushouref[iCurBushouIDOnViewingCommandMenuInBattle-1].ambition >= 110) { // 野望が110以上

			// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
			WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleAnsatsuMenuInBattle), &(iNewAdressBufCommandMenuAnsatsuInBattle[3]), 4, NULL); // アドレスなので4バイト

		//智謀と野望が一定条件を超える場合、｢暗殺三」が出る。 INDEXは1引いてるの注意
		} else if (getBushouInt(iCurBushouIDOnViewingCommandMenuInBattle-1) >= 90 &&   // 智謀が90(=180)以上
			nb6bushouref[iCurBushouIDOnViewingCommandMenuInBattle-1].maxint >= 100 && // 智才が100(=200)以上
			nb6bushouref[iCurBushouIDOnViewingCommandMenuInBattle-1].ambition >= 90) { // 野望が90以上

			// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
			WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleAnsatsuMenuInBattle), &(iNewAdressBufCommandMenuAnsatsuInBattle[2]), 4, NULL); // アドレスなので4バイト

		//智謀と野望が一定条件を超える場合、｢暗殺弐」が出る。 INDEXは1引いてるの注意
		} else if (getBushouInt(iCurBushouIDOnViewingCommandMenuInBattle-1) >= 70 &&   // 智謀が70(=140)以上
			nb6bushouref[iCurBushouIDOnViewingCommandMenuInBattle-1].maxint >= 85 && // 智才が85(=170)以上
			nb6bushouref[iCurBushouIDOnViewingCommandMenuInBattle-1].ambition >= 70) { // 野望が70以上

			// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
			WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleAnsatsuMenuInBattle), &(iNewAdressBufCommandMenuAnsatsuInBattle[1]), 4, NULL); // アドレスなので4バイト

		// 他はデフォに戻す
		} else {
			// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
			WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleAnsatsuMenuInBattle), &iOrgAdressBufBattleAnsatsuMenuInBattle, 4, NULL); // アドレスなので4バイト
		}


	//.-----------------------------------------------------
	// 忍者であれば…(火攻と火遁が異なる)
	} else {

		//智謀関連が一定条件を超える場合、｢火遁極」が出る。 INDEXは1引いてるの注意
		if (getBushouInt(iCurBushouIDOnViewingCommandMenuInBattle-1) >= 110 &&   // 智謀が110(=220)以上
			nb6bushouref[iCurBushouIDOnViewingCommandMenuInBattle-1].maxint >= 110) { // 智才が110(=220)以上

			// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
			WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleHizemeMenuInBattle), &(iNewAdressBufCommandMenuKatonInBattle[4]), 4, NULL); // アドレスなので4バイト

		//智謀関連が一定条件を超える場合、｢火遁四」が出る。 INDEXは1引いてるの注意
		} else if (getBushouInt(iCurBushouIDOnViewingCommandMenuInBattle-1) >= 90 &&   // 智謀が90(=180)以上
			nb6bushouref[iCurBushouIDOnViewingCommandMenuInBattle-1].maxint >= 100) { // 智才が100(=200)以上

			// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
			WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleHizemeMenuInBattle), &(iNewAdressBufCommandMenuKatonInBattle[3]), 4, NULL); // アドレスなので4バイト


		//智謀関連が一定条件を超える場合、｢火遁参」が出る。 INDEXは1引いてるの注意
		} else if (getBushouInt(iCurBushouIDOnViewingCommandMenuInBattle-1) >= 70 &&   // 智謀が70(=140)以上
			nb6bushouref[iCurBushouIDOnViewingCommandMenuInBattle-1].maxint >= 85) { // 智才が85(=170)以上

			// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
			WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleHizemeMenuInBattle), &(iNewAdressBufCommandMenuKatonInBattle[2]), 4, NULL); // アドレスなので4バイト


		//智謀関連が一定条件を超える場合、｢火遁弐」が出る。 INDEXは1引いてるの注意
		} else if (getBushouInt(iCurBushouIDOnViewingCommandMenuInBattle-1) >= 50 &&   // 智謀が50(=100)以上
			nb6bushouref[iCurBushouIDOnViewingCommandMenuInBattle-1].maxint >= 70) { // 智才が70(=140)以上

			// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
			WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleHizemeMenuInBattle), &(iNewAdressBufCommandMenuKatonInBattle[1]), 4, NULL); // アドレスなので4バイト

		// 他はデフォに戻す(火攻の場合は火遁とする)
		} else {
			// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
			WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleHizemeMenuInBattle), &(iNewAdressBufCommandMenuKatonInBattle[0]), 4, NULL); // アドレスなので4バイト
		}

		//.-----------------------------------------------------
		// 暗殺系…
		//智謀と野望が一定条件を超える場合、｢暗殺極」が出る。 INDEXは1引いてるの注意
		if (getBushouInt(iCurBushouIDOnViewingCommandMenuInBattle-1) >= 110 &&   // 智謀が110(=220)以上
			nb6bushouref[iCurBushouIDOnViewingCommandMenuInBattle-1].maxint >= 110 && // 智才が110(=220)以上
			nb6bushouref[iCurBushouIDOnViewingCommandMenuInBattle-1].ambition >= 110) { // 野望が110以上

			// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
			WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleAnsatsuMenuInBattle), &(iNewAdressBufCommandMenuAnsatsuInBattle[4]), 4, NULL); // アドレスなので4バイト

		//智謀と野望が一定条件を超える場合、｢暗殺四」が出る。 INDEXは1引いてるの注意
		} else if (getBushouInt(iCurBushouIDOnViewingCommandMenuInBattle-1) >= 90 &&   // 智謀が90(=180)以上
			nb6bushouref[iCurBushouIDOnViewingCommandMenuInBattle-1].maxint >= 100 && // 智才が100(=200)以上
			nb6bushouref[iCurBushouIDOnViewingCommandMenuInBattle-1].ambition >= 50) { // 野望が50以上

			// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
			WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleAnsatsuMenuInBattle), &(iNewAdressBufCommandMenuAnsatsuInBattle[3]), 4, NULL); // アドレスなので4バイト

		//智謀が一定条件を超える場合、｢暗殺参」が出る。 INDEXは1引いてるの注意
		} else if (getBushouInt(iCurBushouIDOnViewingCommandMenuInBattle-1) >= 70 &&   // 智謀が70(=140)以上
			nb6bushouref[iCurBushouIDOnViewingCommandMenuInBattle-1].maxint >= 85 ) { // 智才が85(=170)以上

			// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
			WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleAnsatsuMenuInBattle), &(iNewAdressBufCommandMenuAnsatsuInBattle[2]), 4, NULL); // アドレスなので4バイト

		//智謀が一定条件を超える場合、｢暗殺参」が出る。 INDEXは1引いてるの注意
		} else if (getBushouInt(iCurBushouIDOnViewingCommandMenuInBattle-1) >= 50 &&   // 智謀が50(=100)以上
			nb6bushouref[iCurBushouIDOnViewingCommandMenuInBattle-1].maxint >= 70 ) { // 智才が70(=140)以上

			// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
			WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleAnsatsuMenuInBattle), &(iNewAdressBufCommandMenuAnsatsuInBattle[1]), 4, NULL); // アドレスなので4バイト


		// 他はデフォに戻す
		} else {
			// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
			WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleAnsatsuMenuInBattle), &iOrgAdressBufBattleAnsatsuMenuInBattle, 4, NULL); // アドレスなので4バイト
		}


	}


	// 武将IDが一定の範囲(それ以外の値が入ってくることもあるためチェック)
	if (0 <= iCurBushouIDOnViewingCommandMenuInBattle-1 && iCurBushouIDOnViewingCommandMenuInBattle-1 < GAMEDATASTRUCT_BUSHOU_NUM) {
		// シナリオモッドの対象メソッドが有効であれば…
		if ( p_snOnViewingCommandMenuInBattle ) {
			((PFNSNONVIEWINGCOMMANDMENUINBATTLE) p_snOnViewingCommandMenuInBattle)(iCurBushouIDOnViewingCommandMenuInBattle, 
					pBufBaseCustomMenuInBattle[0],
					pBufBaseCustomMenuInBattle[1],
					pBufBaseCustomMenuInBattle[2],
					pBufBaseCustomMenuInBattle[3],
					pBufBaseCustomMenuInBattle[4],
					pBufBaseCustomMenuInBattle[5],
					pBufBaseCustomMenuInBattle[6],
					pBufBaseCustomMenuInBattle[7],
					pBufBaseCustomMenuInBattle[8],
					pBufBaseCustomMenuInBattle[9],
					pBufBaseCustomMenuInBattle[10],
					pBufBaseCustomMenuInBattle[11]
			);
			for ( int n=0; n<SCENARIOMOD_BASE_CUSTOM_LABEL_NUM; n++ ) {
				if ( strlen( pBufBaseCustomMenuInBattle[n] ) > 0 ) {
					if ( n==0 ) {
						WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleKobuMenuInBattle), &(iNewAdressBufBaseCustomMenuInBattle[n]), 4, NULL); // アドレスなので4バイト
					} else if ( n==1 ) {
						WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleJogaiMenuInBattle), &(iNewAdressBufBaseCustomMenuInBattle[n]), 4, NULL); // アドレスなので4バイト
					} else if ( n==2 ) {
						WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleTaikyakuMenuInBattle), &(iNewAdressBufBaseCustomMenuInBattle[n]), 4, NULL); // アドレスなので4バイト
					} else if ( n==3 ) {
						WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleNaiouMenuInBattle), &(iNewAdressBufBaseCustomMenuInBattle[n]), 4, NULL); // アドレスなので4バイト
					} else if ( n==4 ) {
						WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleGinouMenuInBattle), &(iNewAdressBufBaseCustomMenuInBattle[n]), 4, NULL); // アドレスなので4バイト
					} else if ( n==5 ) {
						WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleChohatsuMenuInBattle), &(iNewAdressBufBaseCustomMenuInBattle[n]), 4, NULL); // アドレスなので4バイト
					} else if ( n==6 ) {
						WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleSendouMenuInBattle), &(iNewAdressBufBaseCustomMenuInBattle[n]), 4, NULL); // アドレスなので4バイト
					} else if ( n==7 ) {
						WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleKonranMenuInBattle), &(iNewAdressBufBaseCustomMenuInBattle[n]), 4, NULL); // アドレスなので4バイト
					} else if ( n==8 ) {
						WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleHizemeMenuInBattle), &(iNewAdressBufBaseCustomMenuInBattle[n]), 4, NULL); // アドレスなので4バイト
					} else if ( n==9 ) {
						WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleAnsatsuMenuInBattle), &(iNewAdressBufBaseCustomMenuInBattle[n]), 4, NULL); // アドレスなので4バイト
					} else if ( n==10 ) {
						WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleChoheiMenuInBattle), &(iNewAdressBufBaseCustomMenuInBattle[n]), 4, NULL); // アドレスなので4バイト
					} else if ( n==11 ) {
						WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pOrgAddressBattleIkkatsuMenuInBattle), &(iNewAdressBufBaseCustomMenuInBattle[n]), 4, NULL); // アドレスなので4バイト
					}
				}
			}
		}
	}

}

/*
0046A7FA  |. 68 08D54C00    PUSH    TENSHOU.004CD508
0046A7FF  |. 56             PUSH    ESI
0046A800  |. BE FFFF0000    MOV     ESI, 0FFFF
				└ここを JMP TSMod.OnTSExeViewingCommandMenuInBattle と書き換えてやる実際の処理
0046A805  |. 56             PUSH    ESI
0046A806  |. 56             PUSH    ESI
0046A807  |. E8 BF970300    CALL    TENSHOU.004A3FCB
0046A80C  |. 83C4 18        ADD     ESP, 18
0046A80F  |. 3BC6           CMP     EAX, ESI
*/
int pTenshouExeJumpFromToOnTenshouExeViewingCommandMenuInBattle		=0x46A800; // 関数はこのアドレスから、OnTenshouExeViewingCommandMenuInBattleへとジャンプしてくる。
int pTenshouExeReturnLblFromOnTenshouExeViewingCommandMenuInBattle	=0x46A805; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeViewingCommandMenuInBattle() {
	// スタックにためておく
	__asm {
		mov  iCurBushouIDOnViewingCommandMenuInBattle, ebx;

		// 元々TENSHOU.EXE内にあったものをここで
		mov esi, 0xFFFF

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}
	
	// コマンド文字列の置換え
	OnTenshouExeViewingCommandMenuInBattleExecute();

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

		jmp pTenshouExeReturnLblFromOnTenshouExeViewingCommandMenuInBattle
	}
}

/*
0046A7FA  |. 68 08D54C00    PUSH    TENSHOU.004CD508
0046A7FF  |. 56             PUSH    ESI
0046A800  |. BE FFFF0000    MOV     ESI, 0FFFF
				└ここを JMP TSMod.OnTSExeViewingCommandMenuInBattle と書き換えてやる実際の処理
0046A805  |. 56             PUSH    ESI
0046A806  |. 56             PUSH    ESI
0046A807  |. E8 BF970300    CALL    TENSHOU.004A3FCB
0046A80C  |. 83C4 18        ADD     ESP, 18
0046A80F  |. 3BC6           CMP     EAX, ESI
*/
char cmdOnTenshouExeJumpFromViewingCommandMenuInBattle[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ターンがチェンジしたタイミング
void WriteAsmJumperOnTenshouExeViewingCommandMenuInBattle() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeViewingCommandMenuInBattle;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeViewingCommandMenuInBattle + 5 );
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeViewingCommandMenuInBattle  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromViewingCommandMenuInBattle+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeViewingCommandMenuInBattle), cmdOnTenshouExeJumpFromViewingCommandMenuInBattle, 5, NULL); //5バイトのみ書き込む
}


