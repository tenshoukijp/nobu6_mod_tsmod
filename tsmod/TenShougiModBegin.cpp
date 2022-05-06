#include "GameDataStruct.h"
#include "CommonGlobalHandle.h"
#include "MessageInflection.h"
#include "MessageInflectionSurfix.h"
#include "MessageInflectionEnding.h"
#include "FuncFukidashiDialog.h"
#include "FuncMessageDialog.h"
#include "FuncMoveCamera.h"
#include "FuncFadeInOut.h"
#include "FuncSelectBushouMultiDialog.h"
#include "TenShougiMod.h"

// 天将棋Mod.dllのモジュールハンドル
extern HMODULE hTenShougiModDll;

int isTenShougiModeDialog = 0; // 天将棋モードのダイアログ(特殊兵科などの表記管理に利用する)

extern void ModifyBushouDaniTitle(); // 武将リスト出す際のカラムタイトルを「特殊兵態」→「段位」へと変更する。
extern void InitModifyBushouListTitle(); // 武将リスト出す際のカラムタイトルを「特殊兵隊」というデフォルト状態に設定。起動時にも呼ばれる。


// 季節の替わり目(プレイヤターンが回ってきた時)に、軍師との会話によって、天将棋が始まることがある。
void EXServieExecute_TenShougiCheck() {

	// 天将棋系のファイルが物理的に存在していないならば、何もしない。
	if ( ! IsTenShougiModFilesExists() ) {
		return;
	}

	// TenShougiMod.dllがあるはずなので、読み込み
	if (!hTenShougiModDll) {
		hTenShougiModDll = LoadLibrary("TenShougiMod.dll");
	}

	// 存在はするがなぜか読めない。ダメ
	if (!hTenShougiModDll) { return; }

	// 天将棋をすでに開いているならば、何もしない。
	if ( p_sgIsTenShougiFormShown ) {
		if ( p_sgIsTenShougiFormShown() ) {
			return;
		}
	}

	int iTurnGundanID = nb6turn.gundan-1;

	// 軍団が範囲に収まっていなければ、何もしない。
	if (! (0 <= iTurnGundanID && iTurnGundanID < GAMEDATASTRUCT_GUNDAN_NUM) ) {
		return;
	}

	// 軍師の武将番号index
	int iGBushouID = getGunshiBushouID( iTurnGundanID );

	// 軍師が居なければ、何もしない
	if ( !(0 <= iGBushouID && iGBushouID < GAMEDATASTRUCT_BUSHOU_NUM) ) {
		return;
	}

	// 軍師が死亡しているならば、何もしない。
	if ( nb6bushouname[iGBushouID].State == 7 || nb6bushouname[iGBushouID].battleDeath == 1 ) {
		return;
	}

	int iTurnDaimyoID = nb6turn.daimyo-1;

	// 大名が範囲に収まっていなければ、何もしない。
	if (! (0 <= iTurnDaimyoID && iTurnDaimyoID < GAMEDATASTRUCT_DAIMYO_NUM) ) {
		return;
	}

	// 大名の武将番号index
	int iDBushouID = nb6daimyo[iTurnDaimyoID].attach - 1;

	// 大名が居なければ、何もしない
	if ( !(0 <= iDBushouID && iDBushouID < GAMEDATASTRUCT_BUSHOU_NUM) ) {
		return;
	}

	// 大名が死亡しているならば、何もしない。
	if ( nb6bushouname[iDBushouID].State == 7 || nb6bushouname[iDBushouID].battleDeath == 1 ) {
		return;
	}

	int iCastleID = nb6daimyo[iTurnDaimyoID].castle -1;

	// 大名の居城
	if ( !(0 <= iCastleID && iCastleID < GAMEDATASTRUCT_CASTLE_NUM) ) {
		return;
	}

	//----------------------大名と軍師が揃った

	// もはや我々は最大勢力である。よって、余裕のよっちゃんなので、将棋を指す。
	if ( nb6year.season==0 ) {
		FuncFadeOut();
		FuncMoveCamera(iCastleID);
		FuncFadeIn();

		stringstream ssmsg1;
		ssmsg1 
			<< ％相手％(iGBushouID, iDBushouID) << "\x0A"
			<< "将棋の指し初めなど" << "\x0A"
			<< "いかが" << ％ですか％(iGBushouID, iDBushouID);
		FuncLeftNormalFukidashiMessage(iGBushouID, (char *)ssmsg1.str().c_str());

		int result = FuncOkCancelDialog( "誰かと将棋を指しますか？");

		// 選択武将ダイアログに表示するための武将のリスト
		// 年齢１５際以上で、死んでない人。
		vector<int> bushou_list; 
		for ( int iBushouID = 0; iBushouID < GAMEDATASTRUCT_BUSHOU_NUM; iBushouID++) {
			// 軍団番号が同じ
			if ( nb6bushouref[iBushouID].gundan-1 == iTurnGundanID ) {
				// 戦場にて死んでいない。死んでない。大名ではない。
				if ( nb6bushouname[iBushouID].State != 7 && nb6bushouname[iBushouID].State != 0 && nb6bushouname[iBushouID].battleDeath != 1 ) {
					if ( getYearsOld(iBushouID) >= 15 ) { // 15際以上である。
						bushou_list.push_back(iBushouID);
					}
				}
			}
		}
		vector<int> result_list;

		if ( result == 1 ) {  // YESの時
			// 武将リスト。選択可能最大人数を指定。一応８人まで。デフォルトはよくありがちな５人。
			isTenShougiModeDialog = 1; // 特殊兵科の部分が段位となる。
			ModifyBushouDaniTitle(); // 特殊兵隊の部分のカラム名を「棋力」とする。

			result_list = FuncSelectBushouMultiDialog(bushou_list, Filter::Bushou::特殊兵科, 1 );

			InitModifyBushouListTitle(); // 特殊兵隊の部分のカラム名を「特殊兵態」と戻す。
			isTenShougiModeDialog = 0; // 特殊兵科の部分の表記を戻す。
		}

		if ( result_list.size() > 0 ) {
			int iWBushouID = result_list[0];
			stringstream ssmsg4;
			ssmsg4
				<< "面白そう" << ％です％(iDBushouID, iGBushouID, 0xFFFF) << ％な（２）％(iDBushouID, iGBushouID, 0xFFFF) <<  "\x0A"
				<< "さっそく" << ％相手％(iDBushouID, iWBushouID) << "と\x0A"
				<< "勝負" << ％しましょう％(iDBushouID, iGBushouID);
			FuncRightNormalFukidashiMessage(iDBushouID, (char *)ssmsg4.str().c_str());

			TenShougiModBgn(iDBushouID, iWBushouID);
		} else {
			stringstream ssmsg4;
			ssmsg4
				<< "面白そう" << ％です％(iDBushouID, iGBushouID, 0xFFFF) << "が\x0A"
				<< "別の機会に" << ％しましょう％(iDBushouID, iGBushouID);
			FuncRightNormalFukidashiMessage(iDBushouID, (char *)ssmsg4.str().c_str());
		}

		// 吹き出し閉じる
		FuncLeftNormalFukidashiDelete();
		FuncRightNormalFukidashiDelete();
	}
}




// 武将リスト時のタイトルの文字列や幅を変更する。
static char szStringSpecialDani[] = "棋力";
static void ModifyBushouDaniTitle() {

	// 上述コメントの「騎鉄」の文字列のポインタを「段位」を指すものとする。
	int iAdress = (int)szStringSpecialDani;
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(0x4C9084), &iAdress, 4, NULL);

	// 幅を4バイト(全角2文字分)に書き換える。
	int iKitetsuWidth=0x4;
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(0x4C9088), &iKitetsuWidth, 4, NULL);
}


static char* szBushouDaniList[] = { "五級", "四級", "三級", "二級", "一級", "初段", "二段", "三段", "四段", "五段", "六段", "七段", "八段", "九段", "名人" };
 // 指定の段位を得る。
char *getSpecialDaniStringOfBushou(int iBushouID) {
	// 範囲に入っている。
	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM) {

		// 
		int power = GetShogiPower(iBushouID);

		return szBushouDaniList[power];


	} else {
		return "　"; // 一応ぶっ飛ばないようNULLではなく全角空白を返しておく
	}
}

// 対象の棋士の棋力を返す。
int GetShogiPower(int iBushouID) {
	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM) {

		// 武将の段位パワーを求める。(政治+戦闘+智謀)*教養
		int power1 = nb6bushouref[iBushouID].maxgov + nb6bushouref[iBushouID].maxint + nb6savedata_ex.bushou_ex[iBushouID].education + nb6savedata_ex.bushou_ex[iBushouID].command;
		int power2 = nb6bushouref[iBushouID].maxgov + nb6bushouref[iBushouID].maxbat + nb6bushouref[iBushouID].maxint + nb6bushouref[iBushouID].ambition;
		
		int power = 0;

		power = max(power1, power2);

		power += (
					getBushouAptitude(iBushouID, APTITUDE_ASIGARU)+
					getBushouAptitude(iBushouID, APTITUDE_KIBA)+
					getBushouAptitude(iBushouID, APTITUDE_TEPPOU)+
					getBushouAptitude(iBushouID, APTITUDE_SUIGUN)+
					max( getBushouAptitude(iBushouID, APTITUDE_YUMI) ,  getBushouAptitude(iBushouID, APTITUDE_YARI) )
				  ) * 10;

		power = int(power / 55); // 最大で650ちょっとなので、55で割ってみる。11ちょっと

		power = min(power, sizeof(szBushouDaniList)/sizeof(szBushouDaniList[0]) ); // 最大でも配列のどれかは指し示すように

		return power;
	} else {
		return 0;
	}
}