#define _CRT_SECURE_NO_WARNINGS

#include "EXBushou_All.h"
#include "GameDataStruct.h"
#include "GameDataStruct.h"
#include "TSModCommand.h"

// ランスロットタルタロス
EXBushou_LancelotTartare EXB_LancelotTartare;

#define EXB_TempleCommandNum 7
#define EXB_ZenobiaMemberNum 7

// ★以下仮の処理
EXBushou EXB_TempleCommand[EXB_TempleCommandNum]; // ヴォラック～ヴェルサリア
bool setEXB_TempleCommand = false;
// ★以上仮の処理


EXBushou EXB_ZenobiaMember[EXB_ZenobiaMemberNum]; // ウォーレン～デネブ
bool setEXB_ZenobiaMember = false;


EXBushou_ShuiHidechiyo EXB_ShuiHidechiyo;
bool setEXB_NiouMember = false; // 仁王メンバー



// 対象の武将がロスローリアンかどうか
bool IsLoslorien(int iBushouID) {
	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
		char szFullName[20] = "";
		strcat(szFullName, nb6bushouname[iBushouID].familyname);
		strcat(szFullName, nb6bushouname[iBushouID].fastname);
	
		for ( int i=0; i<EXB_TempleCommandNum; i++ ) {
			if ( strcmp(szFullName, EXB_TempleCommand[i].getName())==0 ) {
				return true;
			}
		}
	}
	return false;
}

void EXBushou_All_SetEXBushouInit() {

	// テンプルコマンドが未設定
	// ★以下仮の処理
	if ( !setEXB_TempleCommand ) {
		setEXB_TempleCommand = true;

		// バールゼフォン
		EXB_TempleCommand[0].setName("");

		// ウィンザルフ
		EXB_TempleCommand[1].setName("");

		// バルバス
		EXB_TempleCommand[2].setName("");

		// マルティム
		EXB_TempleCommand[3].setName("㌘㌧㌔㌢㍍");

		// ハボリム
		EXB_TempleCommand[4].setName("");

		// オズマ
		EXB_TempleCommand[5].setName("ёжзийк");

		// ベルサリア
		EXB_TempleCommand[6].setName("");

		// ランスロットタルタロスをスーパーモードに
		EXB_LancelotTartare.setSuperMode(true);

		// この面子をスーパーモードに
		for (int i=0; i<EXB_TempleCommandNum; i++) {
			EXB_TempleCommand[i].setSuperMode(true);
		}
	}

	// ゼノビアからやってきた者達
	if ( !setEXB_ZenobiaMember ) {
		setEXB_ZenobiaMember = true;

		// ウォーレン
		EXB_ZenobiaMember[0].setName("");
		int iWoBushouID = EXB_ZenobiaMember[0].getNb6BushouIX();
		if ( 0 <= iWoBushouID && iWoBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
			nb6savedata_ex.bushou_ex[iWoBushouID].aptitudeyumi = 1;
		}

		// カノープス
		EXB_ZenobiaMember[1].setName("");
		EXB_ZenobiaMember[1].setLifeTimes(3);

		// ギルバルド
		EXB_ZenobiaMember[2].setName("");

		// ミルディン
		EXB_ZenobiaMember[3].setName("");

		// ギルダス
		EXB_ZenobiaMember[4].setName("шщъ");

		// デネブ
		EXB_ZenobiaMember[5].setName("");
		EXB_ZenobiaMember[5].setLifeTimes(3);

		// アルビレオ・ラドラム
		EXB_ZenobiaMember[6].setName("");
		EXB_ZenobiaMember[6].setLifeTimes(3);

		// この面子をスーパーモードに
		for (int i=0; i<EXB_ZenobiaMemberNum; i++) {
			EXB_TempleCommand[i].setSuperMode(true);
		}

	}

	if (!setEXB_NiouMember) {
		setEXB_NiouMember = true;

		// 朱伊秀千代をスーパーモードに
		EXB_ShuiHidechiyo.setSuperMode(true);

	}

	// ★以上仮の処理
}

// 歳をとるのを遅くする処理。あくまでTSMod.txtで有効にしている時だけ。
void EXBushou_All_CheckLifeTimes() {

	// ランスロットタルタロス
	EXB_LancelotTartare.checkTimesNB6BushouLife();

	// ★以下仮の処理
	for (int i=0; i<sizeof(EXB_TempleCommand)/sizeof(EXB_TempleCommand[0]); i++) {
		EXB_TempleCommand[i].checkTimesNB6BushouLife();
	}
	for (int i=0; i<sizeof(EXB_ZenobiaMember)/sizeof(EXB_ZenobiaMember[0]); i++) {
		EXB_ZenobiaMember[i].checkTimesNB6BushouLife();
	}
	// ★以上仮の処理
}




void EXBushou_All_CheckRegeneration() {

	// ここに来るたびに、兵士のリジェネチェック
	EXB_LancelotTartare.checkRegenerationNB6BushouSoldier(3000);

	// ★以下仮の処理
	for (int i=0; i<sizeof(EXB_TempleCommand)/sizeof(EXB_TempleCommand[0]); i++) {
		EXB_TempleCommand[i].checkRegenerationNB6BushouSoldier(20000);
	}
	// ★以上仮の処理

	// ランスロットタルタロスは大砲が壊れても補給が効く
	EXB_LancelotTartare.checkRestoreNB6BushouTaihou();
}



void EXBushou_All_Execute() {
	
	if ( TSModCommand::UltraBushou::cmdExists != 0) {

		// 名前の登録
		EXBushou_All_SetEXBushouInit();

		// リジェネチェック
		EXBushou_All_CheckRegeneration();
	}
}


// 特殊武将用のパラメタ反映
void SetExBushouAvgParam(int iSigBushouID) {
	// iBushouID == -1 なら全員分をチェック

	if ( iSigBushouID == -1 ) {

		for ( int iBushouID=0; iBushouID < GAMEDATASTRUCT_BUSHOU_NUM; iBushouID++ ) {
			// ウォーレンならば…杖を得意に
			if ( strcmp(nb6bushouname[iBushouID].familyname, "")==0 && strcmp(nb6bushouname[iBushouID].fastname, "")==0 ) {
				nb6savedata_ex.bushou_ex[iBushouID].aptitudetue = 5; // 杖だけＳに

				nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi = 2;
				nb6savedata_ex.bushou_ex[iBushouID].aptitudeyari = 1;
				nb6savedata_ex.bushou_ex[iBushouID].aptitudeono = 1;
				nb6savedata_ex.bushou_ex[iBushouID].aptitudemuchi = 1;
				nb6savedata_ex.bushou_ex[iBushouID].aptitudetuchi = 0;
				nb6savedata_ex.bushou_ex[iBushouID].aptitudeken = 0;

			// ベルサリア…杖を得意に
			} else if ( strcmp(nb6bushouname[iBushouID].familyname, "")==0 && strcmp(nb6bushouname[iBushouID].fastname, "")==0 ) {
				nb6savedata_ex.bushou_ex[iBushouID].aptitudetue = 4; // 杖だけＡに

				nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi = 3; // 弓も準ずる
				nb6savedata_ex.bushou_ex[iBushouID].aptitudeyari = 1;
				nb6savedata_ex.bushou_ex[iBushouID].aptitudeono = 1;
				nb6savedata_ex.bushou_ex[iBushouID].aptitudemuchi = 1;
				nb6savedata_ex.bushou_ex[iBushouID].aptitudetuchi = 0;
				nb6savedata_ex.bushou_ex[iBushouID].aptitudeken = 0;

			// デネブならば…杖を得意に
			} else if ( strcmp(nb6bushouname[iBushouID].familyname, "")==0 && strcmp(nb6bushouname[iBushouID].fastname, "")==0 ) {
				nb6savedata_ex.bushou_ex[iBushouID].aptitudetue = 5; // 杖だけＳに

				nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi = 2;
				nb6savedata_ex.bushou_ex[iBushouID].aptitudeyari = 1;
				nb6savedata_ex.bushou_ex[iBushouID].aptitudeono = 1;
				nb6savedata_ex.bushou_ex[iBushouID].aptitudemuchi = 1;
				nb6savedata_ex.bushou_ex[iBushouID].aptitudetuchi = 0;
				nb6savedata_ex.bushou_ex[iBushouID].aptitudeken = 0;

			// ラドラムならば…杖を得意に
			} else if ( strcmp(nb6bushouname[iBushouID].familyname, "")==0 && strcmp(nb6bushouname[iBushouID].fastname, "")==0 ) {
				nb6savedata_ex.bushou_ex[iBushouID].aptitudetue = 5; // 杖だけＳに

				nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi = 2;
				nb6savedata_ex.bushou_ex[iBushouID].aptitudeyari = 1;
				nb6savedata_ex.bushou_ex[iBushouID].aptitudeono = 1;
				nb6savedata_ex.bushou_ex[iBushouID].aptitudemuchi = 1;
				nb6savedata_ex.bushou_ex[iBushouID].aptitudetuchi = 0;
				nb6savedata_ex.bushou_ex[iBushouID].aptitudeken = 0;

			// ギルバルドならば…鞭を得意に
			} else if ( strcmp(nb6bushouname[iBushouID].familyname, "")==0 && strcmp(nb6bushouname[iBushouID].fastname, "")==0 ) {
				nb6savedata_ex.bushou_ex[iBushouID].aptitudemuchi = 5;

				nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi = 2;
				nb6savedata_ex.bushou_ex[iBushouID].aptitudeyari = 2;
				nb6savedata_ex.bushou_ex[iBushouID].aptitudeono = 2;
				nb6savedata_ex.bushou_ex[iBushouID].aptitudetuchi = 2;
				nb6savedata_ex.bushou_ex[iBushouID].aptitudetue = 0;
				nb6savedata_ex.bushou_ex[iBushouID].aptitudeken = 1;

			// カノープスならば…斧と槍を得意に
			} else if ( strcmp(nb6bushouname[iBushouID].familyname, "")==0 && strcmp(nb6bushouname[iBushouID].fastname, "")==0 ) {
				nb6savedata_ex.bushou_ex[iBushouID].aptitudeyari = 4; // 槍はＡ
				nb6savedata_ex.bushou_ex[iBushouID].aptitudeono = 4; // 斧もＡ

				nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi = 2;
				nb6savedata_ex.bushou_ex[iBushouID].aptitudemuchi = 2;
				nb6savedata_ex.bushou_ex[iBushouID].aptitudetuchi = 2;
				nb6savedata_ex.bushou_ex[iBushouID].aptitudetue = 0; 
				nb6savedata_ex.bushou_ex[iBushouID].aptitudeken = 2;

			// タルタロスならば…剣を得意に
			} else if ( isLancelotTartare(iBushouID) ) {
				nb6savedata_ex.bushou_ex[iBushouID].aptitudeken = 5; // 剣はＳ 剣は普通は設定しない。

				nb6savedata_ex.bushou_ex[iBushouID].aptitudeyari = 2;
				nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi = 2; 
				nb6savedata_ex.bushou_ex[iBushouID].aptitudeono = 2;
				nb6savedata_ex.bushou_ex[iBushouID].aptitudemuchi = 1;
				nb6savedata_ex.bushou_ex[iBushouID].aptitudetuchi = 1;
				nb6savedata_ex.bushou_ex[iBushouID].aptitudetue = 0; 

			// バールゼフォンならば…槍を得意に
			} else if ( strcmp(nb6bushouname[iBushouID].familyname, "")==0 && strcmp(nb6bushouname[iBushouID].fastname, "")==0 ) {
				nb6savedata_ex.bushou_ex[iBushouID].aptitudeyari = 5; // 槍はＳ

				nb6savedata_ex.bushou_ex[iBushouID].aptitudeken = 3;
				nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi = 2;
				nb6savedata_ex.bushou_ex[iBushouID].aptitudeono = 2;
				nb6savedata_ex.bushou_ex[iBushouID].aptitudemuchi = 1;
				nb6savedata_ex.bushou_ex[iBushouID].aptitudetuchi = 1;
				nb6savedata_ex.bushou_ex[iBushouID].aptitudetue = 0; 

			// バルバスならば…ハンマー(槌)を得意に
			} else if ( strcmp(nb6bushouname[iBushouID].familyname, "")==0 && strcmp(nb6bushouname[iBushouID].fastname, "")==0 ) {
				nb6savedata_ex.bushou_ex[iBushouID].aptitudetuchi = 5; // 槌はＳ
				nb6savedata_ex.bushou_ex[iBushouID].aptitudeono = 4; // 斧はＡ

				nb6savedata_ex.bushou_ex[iBushouID].aptitudeken = 3;
				nb6savedata_ex.bushou_ex[iBushouID].aptitudeyari = 2;
				nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi = 1;
				nb6savedata_ex.bushou_ex[iBushouID].aptitudemuchi = 0;
				nb6savedata_ex.bushou_ex[iBushouID].aptitudetue = 0; 

			// オズマ・モー・グラシャスならば…鞭が得意に
			} else if (	strcmp( nb6bushouname[iBushouID].familyname, "ёжз") == 0 && strcmp( nb6bushouname[iBushouID].fastname, "ийк") == 0 ) {
				nb6savedata_ex.bushou_ex[iBushouID].aptitudemuchi = 5;

				nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi = 3; // 弓も準ずる
				nb6savedata_ex.bushou_ex[iBushouID].aptitudeyari = 2;
				nb6savedata_ex.bushou_ex[iBushouID].aptitudeken = 2;
				nb6savedata_ex.bushou_ex[iBushouID].aptitudeono = 0;
				nb6savedata_ex.bushou_ex[iBushouID].aptitudetuchi = 0;
				nb6savedata_ex.bushou_ex[iBushouID].aptitudetue = 0;

			// 朱伊秀千代ならば…弓が得意に
			} else if (isShuiHidechiyo(iBushouID)) {
				nb6savedata_ex.bushou_ex[iBushouID].aptitudeken = 1; 

				nb6savedata_ex.bushou_ex[iBushouID].aptitudeyari = 3; // 槍はＢ
				nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi = 5; // 弓はＳ
				nb6savedata_ex.bushou_ex[iBushouID].aptitudeono = 0;
				nb6savedata_ex.bushou_ex[iBushouID].aptitudemuchi = 0;
				nb6savedata_ex.bushou_ex[iBushouID].aptitudetuchi = 0;
				nb6savedata_ex.bushou_ex[iBushouID].aptitudetue = 2; // 杖はＣ

			}
		}
	}
}
