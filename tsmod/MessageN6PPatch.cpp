#define _CRT_SECURE_NO_WARNINGS

#include "WinTarget.h"
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>

#include "TSMod.h"
#include "TSModCommand.h"
#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"
#include "ScenarioDataStruct.h"
#include "ScenarioMessageN6PPatch.h"
#include "MessageN6PTemplate.h"
#include "MessageN6PPatch.h"
#include "MessageInflection.h"
#include "MessageInflectionSurfix.h"
#include "MessageInflectionEnding.h"
#include "OnFireAttackSuccessInBattle.h"
#include "SoundServer.h"
#include "WaitTimeProcedure.h"
#include "FuncPlayResourceWav.h"


using namespace std;




// 噴出しメッセージが更新されてコピーされたスタンバイタイミング
bool MessageN6PPatch::isOnTenshouExeMessageCopyUsed = false;


int isMessageLOCMode = -1; // LOC中心モード



MessageN6PPatch::MessageN6PPatch( char *szBName, char *szCurOrigMsg, HOtherInfo* hOtherInfo ) {
	m_iReWritePercentForce = -1;
	m_isCurOrigMsgReWrited = false;
	strcpy_s(m_szCurOrigMsg, szCurOrigMsg);
	m_hOtherInfo = hOtherInfo;

	// szBNameにはデフォルトで｢苗字･名前   ○○○家｣みたいな文字が入っているので、苗字+名前だけとする。
	strncpy( m_szBName, szBName, 12 );
	// 末尾の半角空白を除去する
	for ( int i=sizeof(m_szBName)-1; i>4; i--) {
		if (m_szBName[i] == ' ') {
			m_szBName[i] = '\0';
		}
	}
	m_szBName[12]=NULL;
}


char* MessageN6PPatch::getBName() {
	return m_szBName;
}


bool MessageN6PPatch::isBName(char* szTargetName) {
	return (
		strcmp(m_szBName, szTargetName)==0
	);
}


BOOL MessageN6PPatch::isMsg( char* szMsgReg, Matches* matches ) {
	// 該当の文字パターンが一致するかどうか
	return ( 
		// strstr( m_szCurOrigMsg, szMsgReg ) != NULL 
		OnigMatch( m_szCurOrigMsg, szMsgReg, matches )
	);
}


bool MessageN6PPatch::rwMsg( char* szMsgReg, char *szCurReWriteMsg, int iReWritePercent ) {
	// すでに他でマッチしてたら処理する必要がない…はず…
	if ( m_isCurOrigMsgReWrited && (iReWritePercent != m_iReWritePercentForce) ) { 
		return false;
	}

	// ターゲットのメッセージと合致すれば
	if ( isMsg( szMsgReg ) ) {

		if ( isMessageLOCMode == 4 ) {
			// LOC4とLOC5が同じ内容であれば、コピーフラグを立てる
			bool bMsgMustCopyTo5Flag = false;
			if ( strcmp( (char *)CPM.getMemoryAddress(LOC4), (char *)CPM.getMemoryAddress(LOC5)) == 0 ) {
				bMsgMustCopyTo5Flag = true; // LOC4とLOC5はメッセージが同じなので両方とも書き換えなければならない。
			}
			bool bMsgMustCopyTo2Flag = false;
			if ( strcmp( (char *)CPM.getMemoryAddress(LOC4), (char *)CPM.getMemoryAddress(LOC2)) == 0 ) {
				bMsgMustCopyTo2Flag = true; // LOC4とLOC2はメッセージが同じなので両方とも書き換えなければならない。
			}
			// 書き込み											　　　　┌メッセージのメモリアドレス
			WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(CPM.getMemoryAddress(LOC4)), szCurReWriteMsg, strlen(szCurReWriteMsg)+1, NULL);
			if ( bMsgMustCopyTo5Flag ) {
				WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(CPM.getMemoryAddress(LOC5)), szCurReWriteMsg, strlen(szCurReWriteMsg)+1, NULL);
			}
			if ( bMsgMustCopyTo2Flag ) {
				WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(CPM.getMemoryAddress(LOC2)), szCurReWriteMsg, strlen(szCurReWriteMsg)+1, NULL);
			}
		}
		else if ( isMessageLOCMode == 2 ) {

			// 書き込み											　　　　┌メッセージのメモリアドレス
			WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(CPM.getMemoryAddress(LOC4)), szCurReWriteMsg, strlen(szCurReWriteMsg)+1, NULL);
			WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(CPM.getMemoryAddress(LOC2)), szCurReWriteMsg, strlen(szCurReWriteMsg)+1, NULL);
		}
		strcpy(CPM.getPreVarPtr(LOC1) ,szCurReWriteMsg);
		m_isCurOrigMsgReWrited = true;

		return true;
	}
	return false;
}


// 逆暗殺メッセージモード
extern int iStatusAntiAnsatsuMessageModeOnAttackInBattleExecute;
extern int iOrgBushouIDOfAnsatsuSareruGawa;
extern int iOrgBushouIDOfAnsatsuSuruGawa;

// 商人交渉時に商人だった人の武将ID(-1して使うこと)
extern int iCurBushouIDInCommerceMessageJudge;
// 商人交渉時に商人に対商人要のメッセージを言わせるかどうか
extern bool isUseSyoninMsgInCommerceSuccessJudge;

// 煽動で混乱させた人数によって全員煽動のメッセージを変更する
extern int iSendouKonranTargetConranCnt;
// 煽動で混乱させた主体(張本人)となる武将ID
extern int iSendouKonranMultiKonranBushouID;

bool isCurScenarioModReWrite = false;




extern char szMessageBuildLastestWordName[];
extern int isOnCurGungiFukidashi;		// 今回の吹き出しが｢評定｣吹き出しかどうか。。
extern int isOnCurGunshiFukidashi;		// 軍師助言の吹き出しかどうか
extern int iTeppouHanekaesiInTeppouDeath; // 鉄砲を弾き返したモードかどうか。

// メインの書き換え
bool ReWritePatchMessageN6P(char *szBName, char *szCurOrigMsg, HOtherInfo* hOtherInfo) {

	// 軍師助言の吹き出しはここを通さず別の場所がある。
	if ( isOnCurGunshiFukidashi ) {
		isOnCurGunshiFukidashi = FALSE;
		MessageN6PPatch::isOnTenshouExeMessageCopyUsed = true;
		return false;
	}

	// 評定吹き出しフラグが立ってれば、ここでは何もしない。書き換えても表示に間に合わないからだ。
	if ( isOnCurGungiFukidashi ) {
		isOnCurGungiFukidashi = FALSE;
		MessageN6PPatch::isOnTenshouExeMessageCopyUsed = true;
		return false;
	}

	isMessageLOCMode = 4; // デフォルトは4

	/*
	 * 毎フレーム処理すると重いので、ここで制御
	 * かなりのフレームタイミングでここにながれるので、ここで新たなメッセージがセットされているかどうかをチェックし
	 * セットされていなければ、特に処理する必要がないと判断する。
	 */
	
	if ( nb6fukidashirelatepersons->i1stPersonID <= 0 || 532 < nb6fukidashirelatepersons->i1stPersonID ) { // 530番までが普通の武将
		return false;
	}

	// コピータイミングのメッセージを利用していないならば…
	if ( ! MessageN6PPatch::isOnTenshouExeMessageCopyUsed ) {

		// 該当メッセージを利用したというフラグを立てる
		MessageN6PPatch::isOnTenshouExeMessageCopyUsed = true;

		// ScenarioModが書き換えたという情報はここでリセット。
		isCurScenarioModReWrite = false;

	// すでにコピータイミングのメッセージを利用していたならば、何も処理せず返す
	} else {
		return false;
	}


	// ここで名前を作り出す。
	char name[256] = "";

	if ( nb6fukidashirelatepersons->i1stPersonID <= 532 ) {
		strcat( name, nb6bushouname[nb6fukidashirelatepersons->i1stPersonID-1].familyname);
		strcat( name, nb6bushouname[nb6fukidashirelatepersons->i1stPersonID-1].fastname);
	}


	/*	
	ofsClassMessageN6PPatch << "-------------------------------------------------------" << endl;

	// 以下テスト用。第１人者、第２人者、第３人者
	nb6fukidashirelatepersons->i1stPersonID;
	nb6fukidashirelatepersons->i2ndPersonID;
	nb6fukidashirelatepersons->i3rdPersonID;
	ofsClassMessageN6PPatch << nb6bushouname[nb6fukidashirelatepersons->i1stPersonID-1].fastname << endl;
	if (  nb6fukidashirelatepersons->i2ndPersonID <= 0 || 532 < nb6fukidashirelatepersons->i2ndPersonID ) {
	} else {
		ofsClassMessageN6PPatch << nb6bushouname[nb6fukidashirelatepersons->i2ndPersonID-1].fastname << ":" << nb6bushouname[nb6fukidashirelatepersons->i2ndPersonID-1].fastname  << endl;
	}
	ofsClassMessageN6PPatch << "Main4" << name << ":" << CPM.getCurVarPtr(LOC4) << endl;
	ofsClassMessageN6PPatch << "Main4" << name << ":" << CPM.getCurVarPtr(LOC5) << endl;

	/*
	 * サブの方(商人など)をチェック
	 */

	/*
	 * 以下、いろいろと状況を判断し、メッセージ置き換え。
	 */

	/*
		□□□□□□□□□
		□□□□□□□□□
		□□□□□□□□□
	*/
	// どうもメッセージ４はパラメタタイプで、メッセージ２にその展開されたものがあるようだ…
	if ( isTheParamMsgIsIncludeVerifyMsg( (char *)CPM.getCurVarPtr(LOC2), (char *)CPM.getCurVarPtr(LOC4) ) ) {
		strcpy( szCurOrigMsg, (char *)CPM.getCurVarPtr(LOC2) );
		strcpy( (char *)CPM.getCurVarPtr(LOC4), (char *)CPM.getCurVarPtr(LOC2) );
		// 展開済みのものを展開前のパラメータ状態のものに上書きしてしまう。
		isMessageLOCMode = 2;	// メッセージ格納場所の２が強いモード
	}

	MessageN6PPatch oCurMsg = MessageN6PPatch( name, szCurOrigMsg,   hOtherInfo );

	/*
	ofsClassMessageN6PPatch << "4" << CPM.getCurVarPtr(LOC4) << endl;
	ofsClassMessageN6PPatch << "2" << CPM.getCurVarPtr(LOC2) << endl;
	*/

	/*
	 * 商人との交渉。OnCommerceBenzetsuJudge,OnCommerceSuccessJudge関連
	 */ 
	// 商売人との交渉だったら書き換える 
	if (isUseSyoninMsgInCommerceSuccessJudge && oCurMsg.isMsg("浅からぬ仲…その値で")) {
		isUseSyoninMsgInCommerceSuccessJudge = false;

		char szCommerseMsg1[70] = "%s殿とは\x0A商売人の仲…その値で\x0A手を打ちましょう";
		char szCommerseMsg2[70] = "";
		wsprintf(szCommerseMsg2, szCommerseMsg1, nb6bushouname[iCurBushouIDInCommerceMessageJudge-1].fastname);
		oCurMsg.rwMsg( "浅からぬ仲…その値で", szCommerseMsg2 );
	}

	/*
	 * 火攻め系。メッセージ置き換えとともに、メラメラ音を鳴らす。OnFireBattleAttackSuccessInBattle系
	 */
	// 共通。火責めであれば…
	else if ( oCurMsg.isMsg(  吹き出しウィンドウ::戦争::火攻め::REG01 ) ||
		 oCurMsg.isMsg(  吹き出しウィンドウ::戦争::火攻め::REG02 ) ||
		 oCurMsg.isMsg(  吹き出しウィンドウ::戦争::火攻め::REG03 ) ||
		 oCurMsg.isMsg(  吹き出しウィンドウ::戦争::火攻め::REG04 ) ) {

		 // この中ではメッセージの書き換えを行わないこと!! タルタロスが書き換えられなくなるため!!
		 // 火攻め成功時の処理
		 OnTenshouExeFireBattleAttackSuccessExecute();
	}

	// 内応したら、敵だったら戦闘中全技能使えるのフラグがＯＮだったら処理をする。
	else if ( oCurMsg.isMsg (  吹き出しウィンドウ::戦争::内応::REG01 ) || oCurMsg.isMsg(  吹き出しウィンドウ::戦争::内応::REG02 ) ) {
		// 内応成功の噴出しウィンドウであれば…
		EXSuperService_IfCanAllAlibityInBattleSeiryokuChange( nb6fukidashirelatepersons->i1stPersonID-1 , nb6fukidashirelatepersons->i2ndPersonID-1 );
	}


	/*
	 * 煽動系。煽動の内容を変化させたので、それに対応するメッセージ。OnSendouKonran****系。
	 */
	// 煽動関連
	// 混乱させたのが７隊以上
	if ( iSendouKonranTargetConranCnt >= 7 ) {

		// どれかのメッセージは確実なので置き換えの
		char *pBushouName = nb6bushouname[iSendouKonranMultiKonranBushouID-1].fastname;
		int iBushouSex = nb6bushouname[iSendouKonranMultiKonranBushouID-1].sex;
		int iBushouTone = nb6bushouref[iSendouKonranMultiKonranBushouID-1].tone;
		char szAnsatsuMsg[256] = "";
		/*
		（自口=1or3or自女）各々方、油断めさるな
		（自口=2）者ども、油断するな
		（他）皆の者、油断するな
		*/
		// ランスロット･タルタロス
		if ( oCurMsg.isBName("" ) ) {
			oCurMsg.rwMsg( 吹き出しウィンドウ::戦争::煽動::REG01,		"ほぉ…壮観なり\x0A戦況を確実に見極め\x0A各個に撃破せよ" );
			oCurMsg.rwMsg( 吹き出しウィンドウ::戦争::煽動::REG02,		"よし！　今こそ\x0A戦術的転機だ！\x0A敵の混乱の隙を突け！" );

		} else if ( iBushouTone==1 || iBushouTone==3 || iBushouSex== 1 ) {
			wsprintf( szAnsatsuMsg, "皆いきり立っておるわ\x0A各々方、油断めさるな" );
			oCurMsg.rwMsg( 吹き出しウィンドウ::戦争::煽動::REG01, szAnsatsuMsg);
			oCurMsg.rwMsg( 吹き出しウィンドウ::戦争::煽動::REG02, "周囲は大混乱じゃ…\x0Aとはいえ\x0A何とも恐ろしげよのう" );

		} else if ( iBushouTone==2 ) {
			wsprintf( szAnsatsuMsg, "皆いきり立っておるわ\x0A者ども、油断するな" );
			oCurMsg.rwMsg( 吹き出しウィンドウ::戦争::煽動::REG01, szAnsatsuMsg);
			oCurMsg.rwMsg( 吹き出しウィンドウ::戦争::煽動::REG02, "周囲は大混乱じゃ…\x0Aとはいえ\x0A何とも恐ろしげよのう" );

		} else {
			wsprintf( szAnsatsuMsg, "皆いきり立っておるわ\x0A皆の者、油断するな" );
			oCurMsg.rwMsg( 吹き出しウィンドウ::戦争::煽動::REG01, szAnsatsuMsg);
			oCurMsg.rwMsg( 吹き出しウィンドウ::戦争::煽動::REG02, "周囲は大混乱じゃ…\x0Aとはいえ\x0A何とも恐ろしげよのう" );
		}

	// 混乱させたのが２隊以上
	} else if ( iSendouKonranTargetConranCnt >= 1 ) {

		// ランスロット･タルタロス
		if ( oCurMsg.isBName("" ) ) {
			oCurMsg.rwMsg( 吹き出しウィンドウ::戦争::煽動::REG01,		"ふっ\x0A造作もないことだ…\x0A順次掃討にかかれ！" );
			oCurMsg.rwMsg( 吹き出しウィンドウ::戦争::煽動::REG02,		"兵は鬼道なり\x0Aこの機に乗じ\x0A敵の混乱の隙を突け！" );
		
		} else {
			// 煽動は成功したが、混乱させた人数としては少数
			// どれかのメッセージは確実なので置き換えの
			char *pBushouName = nb6bushouname[iSendouKonranMultiKonranBushouID-1].fastname;
			int iBushouSex = nb6bushouname[iSendouKonranMultiKonranBushouID-1].sex;
			int iBushouTone = nb6bushouref[iSendouKonranMultiKonranBushouID-1].tone;
			char szAnsatsuMsg[256] = "";

			if ( iBushouSex==1 ) { 
				oCurMsg.rwMsg( 吹き出しウィンドウ::戦争::煽動::REG01, "よし！　相手は\x0A分別がつかなく\x0Aなったようですね" );
				oCurMsg.rwMsg( 吹き出しウィンドウ::戦争::煽動::REG02, "よし！\x0Aこれぞ転機よ！\x0Aこの混乱の隙を突け！" );
			} else if ( iBushouTone==2 || iBushouTone==4) {
				oCurMsg.rwMsg( 吹き出しウィンドウ::戦争::煽動::REG01, "よし！　相手は\x0A分別がつかなく\x0Aなったようだな" );
				oCurMsg.rwMsg( 吹き出しウィンドウ::戦争::煽動::REG02, "よし！\x0Aこれぞ転機だ！\x0Aこの混乱の隙を突け！" );
			} else {
				oCurMsg.rwMsg( 吹き出しウィンドウ::戦争::煽動::REG01, "よし！　相手は\x0A分別がつかなく\x0Aなったようじゃな" );
				oCurMsg.rwMsg( 吹き出しウィンドウ::戦争::煽動::REG02, "よし！\x0Aこれぞ転機じゃ！\x0Aこの混乱の隙を突け！" );
			}
		}

	// 混乱させたのが０隊
	} else {
		// ランスロット･タルタロス
		if ( oCurMsg.isBName("" ) ) {
			oCurMsg.rwMsg( 吹き出しウィンドウ::戦争::煽動::REG01,		"むむ…\x0A煽動できる\x0A相手がおらぬわ" );
			oCurMsg.rwMsg( 吹き出しウィンドウ::戦争::煽動::REG02,		"むむ…\x0A煽動できる\x0A相手がおらぬわ" );

		} else {
			// どれかのメッセージは確実なので置き換えの
			char *pBushouName = nb6bushouname[iSendouKonranMultiKonranBushouID-1].fastname;
			int iBushouSex = nb6bushouname[iSendouKonranMultiKonranBushouID-1].sex;
			int iBushouTone = nb6bushouref[iSendouKonranMultiKonranBushouID-1].tone;

			if ( iBushouTone==1 || iBushouTone==3 || iBushouSex== 1 ) {
				oCurMsg.rwMsg( 吹き出しウィンドウ::戦争::煽動::REG01, "むむ…\x0A煽動できる\x0A相手がおりませぬ");
				oCurMsg.rwMsg( 吹き出しウィンドウ::戦争::煽動::REG02, "むむ…\x0A煽動できる\x0A相手がおりませぬ");

			} else if ( iBushouTone==2 ) {
				oCurMsg.rwMsg( 吹き出しウィンドウ::戦争::煽動::REG01, "むむ…\x0A煽動できる\x0A相手がおらぬ");
				oCurMsg.rwMsg( 吹き出しウィンドウ::戦争::煽動::REG02, "むむ…\x0A煽動できる\x0A相手がおらぬ");

			} else {
				oCurMsg.rwMsg( 吹き出しウィンドウ::戦争::煽動::REG01, "むむ…\x0A煽動できる\x0A相手がおらぬな");
				oCurMsg.rwMsg( 吹き出しウィンドウ::戦争::煽動::REG02, "むむ…\x0A煽動できる\x0A相手がおらぬな");
			}
		}

	}

	// 鉄砲のメッセージを矢のメッセージに変更する。
	if ( nb6fukidashirelatepersons->i1stPersonID <= GAMEDATASTRUCT_BUSHOU_NUM ) {
		int iBushouID = nb6fukidashirelatepersons->i1stPersonID-1;
		if ( nb6bushouref[iBushouID].form == 0 || nb6bushouref[iBushouID].form == 1 ) {

			string msg = string("矢のつるべ射ちに\x0A関しては、この") + nb6bushouname[iBushouID].fastname + string("\x0A一二を争う腕前じゃ");
			char szmsg[100] = "";
			strcpy(szmsg, msg.c_str() );
			oCurMsg.rwMsg( "新しき時代を告げる\x0A",	szmsg );
			oCurMsg.rwMsg( "のこのこ出て来たわ\x0A",	"弓矢隊の威力\x0A思い知らせてやるわ！\x0A放てえっ！" );
			oCurMsg.rwMsg( "まだじゃ…もっと\x0A",		"まだじゃ…もっと\x0A十分に引きつけて…\x0Aよし、放てえっ！" );
			oCurMsg.rwMsg( "わざわざ標的になりに\x0A",	"わざわざ矢の的に\x0Aなりにきおったわ…\x0A放てえっ！" );
			oCurMsg.rwMsg( "弾の嵐を\x0A",				"矢の雨を\x0A見舞ってくれるわ\x0A一斉に放てえっ！" );
		}

	}

	// 本丸落城のメッセージ。ショートカット版。
	if ( TSModCommand::AllCastle::cmdShortCutFallCastle ) {
		if ( nb6fukidashirelatepersons->i1stPersonID <= GAMEDATASTRUCT_BUSHOU_NUM ) {
			if ( Is_CastleWar() ) {
				int iBushouID = nb6fukidashirelatepersons->i1stPersonID-1;
				extern int iCheckRojoBushouNum; // 籠城側の城内に居る人数

				if ( oCurMsg.isMsg( 吹き出しウィンドウ::戦争::落城::REG01) || 
					 oCurMsg.isMsg( 吹き出しウィンドウ::戦争::落城::REG02) ||
					 oCurMsg.isMsg( 吹き出しウィンドウ::戦争::落城::REG03) ) {
					HEX_POSITION my2DPos = GetCastleHexUnitPos(iBushouID); // 自分の位置
					int iRole = GetCastleHexRole(my2DPos); // その部分の役割
					if ( iRole != CASTLE_HEX_ROLE::本丸 ) { // 本丸以外に居たのに落城した。
	
						Matches matches;
						if ( oCurMsg.isMsg("(本丸一番乗りじゃ)\\s(.+)", &matches ) ) {
							string s1 = "敵に戦う力なしじゃ\x0A";
							string s2 = matches[2];
							string reg01 = s1+s2;
							oCurMsg.rwMsg( 吹き出しウィンドウ::戦争::落城::REG01, (char *)reg01.c_str() );
						} else if ( oCurMsg.isMsg("(本丸一番乗りの名誉は)\\s(.+?\x0A)(.+)", &matches ) ) {
							extern int iNextTurnCastleID;   // 表示籠城戦中の城番号(+1の方)
							int iCastleID = iNextTurnCastleID-1;
							string s1 = "もはや勝負は決した\x0A";
							string s2 = matches[2];
							string s3 = "この城、頂戴した！";
							if ( 0 <= iCastleID && iCastleID < GAMEDATASTRUCT_CASTLE_NUM ) {
								s3 = string("") + nb6castle[iCastleID].name + getCastleTypeName(nb6castle[iCastleID].title) + "、頂いた！";
							}
							string reg02 = s1+s2+s3;
							oCurMsg.rwMsg( 吹き出しウィンドウ::戦争::落城::REG02, (char *)reg02.c_str() );
						} else if ( oCurMsg.isMsg("本丸一番乗りぞーっ！") ) {
							string reg03;
							if ( rand()%2 == 0 ) {
								reg03 = ％ははは％(iBushouID,iBushouID,0xFFFF) + "\x0A" + "敵に戦う力なし\x0A" + "城は我らのものじゃ";
							} else {
								reg03 = string("敵は降参したようじゃ\x0A") + "城はこの" + nb6bushouname[iBushouID].fastname + "が\x0A" + "頂戴" + ％します％(iBushouID, iBushouID, 0xFFFF);
							}
							oCurMsg.rwMsg( 吹き出しウィンドウ::戦争::落城::REG03, (char *)reg03.c_str() );
							oCurMsg.rwMsg( "武将として\x0Aひとまわり大きく", (char *)reg03.c_str() );
							oCurMsg.rwMsg( "奉公の術\x0A開眼した", (char *)reg03.c_str() );
							oCurMsg.rwMsg( "今まで感じたことなき\x0A新しき力", (char *)reg03.c_str() );
						}
					}
				}
			}
		}
	}


	/*
	 * ランスロットタルタロスの他の状況でのメッセージ置き換え
	 */
	// ランスロット･タルタロス
	if ( oCurMsg.isBName("" ) ) {

		/*
		 * タルタロスが鉄砲を跳ね返され死亡した
		 */
		if ( iTeppouHanekaesiInTeppouDeath == 1 ) {
			iTeppouHanekaesiInTeppouDeath = 0;
			oCurMsg.rwMsg( "む、無念…\x0A",		"なんと！　鉄砲を弾き\x0A私に致命の一打を\x0A与えたというのか…" );
			oCurMsg.rwMsg( "ここで死ぬの\x0A",		"なんと！　鉄砲を弾き\x0A私に致命の一打を\x0A与えたというのか…" );
			oCurMsg.rwMsg( "これほどの恐怖を\x0A",		"なんと！　鉄砲を弾き\x0A私に致命の一打を\x0A与えたというのか…" );
			oCurMsg.rwMsg( "に立ち向かった\x0A",		"なんと！　鉄砲を弾き\x0A私に致命の一打を\x0A与えたというのか…" );
			oCurMsg.rwMsg( "次は平和な時代に\x0A",		"なんと！　鉄砲を弾き\x0A私に致命の一打を\x0A与えたというのか…" );

		/*
		 * タルタロスが鉄砲を跳ね返した時のセリフ
		 */
		} else if ( iTeppouHanekaesiInTeppouDeath == 2 ) {
			iTeppouHanekaesiInTeppouDeath = 0;
			oCurMsg.rwMsg( "あの世で\x0A",		"無駄なことを…\x0Aこの暗黒騎士団長\x0A鉄砲では倒せぬ！" );
			oCurMsg.rwMsg( "首だけとなり果てたか\x0A",		"見よ愚民の末路を！\x0A鉄砲ごときに慢心し\x0A自ら以て滅びるのだ…" );
			oCurMsg.rwMsg( "同じ時に生きたことを\x0A",		"ふふふ…狙いは悪くない\x0Aだが身の丈に合わぬ\x0A試みは身を亡ぼすのみ" );
		}


		// 鼓舞系のメッセージ
		if ( oCurMsg.isMsg( "風林火山の旗印\x0Aとくと" ) ) {
			if ( rand()%2 == 0 ) {
				oCurMsg.rwMsg( "風林火山の旗印\x0Aとくと",		"このアンビシオン\x0Aその身でしかと\x0A味わうがよい！" );
			} else {
				oCurMsg.rwMsg( "風林火山の旗印\x0Aとくと",		"ローディスに逆らう\x0A愚か者め…、わが奥義\x0A受けてみよッ！" );
			}
		}
		oCurMsg.rwMsg( "戦いはこれから",				"敵軍を掌握せよ\x0Aロスローリアンの矜持\x0Aと真髄しかと示せ！" );
		oCurMsg.rwMsg( "皆の者、奮い立て",				"皆の者、良いか！\x0A我らの任務は重大だ\x0A戦力損失は回避せよ！" );
		oCurMsg.rwMsg( "勝てば極楽",					"所詮、敵軍は\x0A我々に遠い劣等民族\x0Aその違いを示すのだ！" );
		oCurMsg.rwMsg( "者ども、ひるむな",				"皆の者、敵に与えるだ\x0Aけでよい…我らに敗北\x0Aするという特権を！" );
//		oCurMsg.rwMsg( "者ども、ひるむな",				"敵は我らに支配される\x0Aを望んでおる！　その\x0A願い叶えてやれい！" );
		oCurMsg.rwMsg( "兵どもよ、今一度",				"これ以上、奴らを\x0A野放しにはできん\x0A速やかに制圧せよ！" );
		
		// 鼓舞すでにMAX
		oCurMsg.rwMsg( "わが隊はすでに",		"我が兵士達はすでに\x0A士気旺盛である\x0A今こそ好機ぞ！" );
		
		// これ以上徴兵が不可能
		oCurMsg.rwMsg( "わが隊の兵士を",		"ロスローリアンは\x0A少数精鋭が旨\x0A余計な兵は要るまい" );

		// 退却時のメッセージ
		oCurMsg.rwMsg( "もはやこれまで",		"むむ…！\x0Aこの私にプレッシャー\x0Aを与えるとは…" );

		// 大砲破壊時に修復のメッセージその１
		// 壊れたので、新たな大砲に番号を付ける。番号は二番～十番
		int iTaihouIX = (rand() % 9);
		char szTaihouNo[7][3] = {"二","三","四","五","六","七","八"};
		char szTaihouMessage[70] = "";
		// ランダムで番号を選んで、メッセージに合体
		wsprintfA(szTaihouMessage, "むむ、大砲が壊れたか！\x0A替わりの%s番砲を\x0A急ぎこれに持てい！", szTaihouNo[iTaihouIX]);
		oCurMsg.rwMsg( "やや、大砲が壊れた！",	szTaihouMessage );

		// 大砲破壊時に修復のメッセージその２
		oCurMsg.rwMsg( "もう大砲が壊れたか！",	"もう大砲が壊れたか！\x0Aうぬう…早急に\x0A修理いたせい！" );

		oCurMsg.rwMsg( "風林火山の旗印\x0Aが…",  "な…なんと！\x0Aこの私に致命の一打を\x0A与えたというのか…" );


		/* 
		 * ランスロットタルタロスの逆暗殺時のメッセージ。
		 * 逆暗殺は通常とは異なる処理をしているので、MessageN6PPatch::isOnTenshouExeMessageCopyUsed
		 * だけでは上手くいかないので、これで。OnAnsatsuAttackInBattle関連。
		 */
		// 逆暗殺でやる側（一般武将）  (==2ということは武将のIDはOrg系のレベルで大丈夫であった)
		if ( iStatusAntiAnsatsuMessageModeOnAttackInBattleExecute == 2 ) {

			//char *pBushouName = nb6bushouname[iOrgBushouIDOfAnsatsuSuruGawa-1].fastname;
			int iTmp2nd_BushouID = iOrgBushouIDOfAnsatsuSuruGawa-1;
			int iTmp1st_BushouID = iOrgBushouIDOfAnsatsuSareruGawa-1;
			string strBushouName = ％罵倒相手％(iTmp1st_BushouID, iTmp2nd_BushouID );
			char *pBushouName = (char *)strBushouName.c_str();
			char szAnsatsuMsg[256] = "";

			if ( oCurMsg.isMsg( 吹き出しウィンドウ::戦争::原暗殺::REG04 ) ||
				 oCurMsg.isMsg( 吹き出しウィンドウ::戦争::原暗殺::REG05 ) ) {
				// 逆暗殺用のメッセージ変動モードは終了
				iStatusAntiAnsatsuMessageModeOnAttackInBattleExecute = 0;
			}

			// 逆暗殺その１
			wsprintf( szAnsatsuMsg, "愚かな%sよ…！\x0A過ぎたる野心は\x0A身を滅ぼすのみだ！", pBushouName );
			oCurMsg.rwMsg( 吹き出しウィンドウ::戦争::原暗殺::REG04,	szAnsatsuMsg );

			// 逆暗殺その２
			wsprintf( szAnsatsuMsg, "この暗黒騎士団長に\x0A暗殺を仕掛けるとは…\x0A見上げた愚かさよ！" );
			oCurMsg.rwMsg( 吹き出しウィンドウ::戦争::原暗殺::REG05,	szAnsatsuMsg );

		}		
	
	/* 
	 * 暗殺時のやられる側のメッセージ。OnAnsatsuAttackInBattle関連。
	 */
	// 逆暗殺時でやられる側 (==1ということは武将のIDはOrg系のレベルで大丈夫であった)
	} else if ( iStatusAntiAnsatsuMessageModeOnAttackInBattleExecute == 1 ) {

		// 逆暗殺用のメッセージ変動モードを次に進める
		iStatusAntiAnsatsuMessageModeOnAttackInBattleExecute = 2;

		// char *pBushouName = nb6bushouname[iOrgBushouIDOfAnsatsuSareruGawa-1].fastname;
		int iTmp2nd_BushouID = iOrgBushouIDOfAnsatsuSareruGawa-1;
		int iTmp1st_BushouID = iOrgBushouIDOfAnsatsuSuruGawa-1;
		string strBushouName = ％罵倒相手％(iTmp1st_BushouID, iTmp2nd_BushouID );
		char *pBushouName = (char *)strBushouName.c_str();
		char szAnsatsuMsg[256] = "";

		wsprintf( szAnsatsuMsg, "わ！貴様、何をする！\x0A…ぐわっ！…おのれ\x0A%sめ…見抜き…う", pBushouName);
		oCurMsg.rwMsg( 吹き出しウィンドウ::戦争::被暗殺::REG01, szAnsatsuMsg);

		wsprintf( szAnsatsuMsg, "やや…足元から槍が！\x0Aし、しまった…\x0A%sめ…見破り…ぐ", pBushouName);
		oCurMsg.rwMsg( 吹き出しウィンドウ::戦争::被暗殺::REG02, szAnsatsuMsg);

		wsprintf( szAnsatsuMsg, "う！何じゃこの痛みは\x0A…こ、これは毒矢か！\x0A%sめ…見抜き…う", pBushouName);
		oCurMsg.rwMsg( 吹き出しウィンドウ::戦争::被暗殺::REG03, szAnsatsuMsg);

		wsprintf( szAnsatsuMsg, "む！怪しげな黒装束…\x0A%sめ…先に刺客を\x0A…つ、強い…ぎゃ", pBushouName);
		oCurMsg.rwMsg( 吹き出しウィンドウ::戦争::被暗殺::REG04, szAnsatsuMsg);


	/* 
	 * 暗殺時のやる側のメッセージ。OnAnsatsuAttackInBattle関連。
	 */
	// 逆暗殺でやる側（一般武将）  (==2ということは武将のIDはOrg系のレベルで大丈夫であった)
	} else if ( iStatusAntiAnsatsuMessageModeOnAttackInBattleExecute == 2 ) {

		// タルタロス以外の場合。この条件はなぜか必要。
		if ( isLancelotTartare(iOrgBushouIDOfAnsatsuSareruGawa-1) ) {
		
			// どれかのメッセージにひっかかっていたら終了
			if ( oCurMsg.isMsg( 吹き出しウィンドウ::戦争::原暗殺::REG01 ) ||
				 oCurMsg.isMsg( 吹き出しウィンドウ::戦争::原暗殺::REG02 ) ||
				 oCurMsg.isMsg( 吹き出しウィンドウ::戦争::原暗殺::REG03 ) ||
				 oCurMsg.isMsg( 吹き出しウィンドウ::戦争::原暗殺::REG04 ) ||
				 oCurMsg.isMsg( 吹き出しウィンドウ::戦争::原暗殺::REG05 ) ) {

				// 逆暗殺用のメッセージ変動モードは終了
				iStatusAntiAnsatsuMessageModeOnAttackInBattleExecute = 0;
	
			}

			// どれかのメッセージは確実なので置き換えの
			//char *pBushouName = nb6bushouname[iOrgBushouIDOfAnsatsuSuruGawa-1].fastname;
			int iTmp2nd_BushouID = iOrgBushouIDOfAnsatsuSuruGawa-1;
			int iTmp1st_BushouID = iOrgBushouIDOfAnsatsuSareruGawa-1;
			string strBushouName = ％相手％(iTmp1st_BushouID, iTmp2nd_BushouID );
			char *pBushouName = (char *)strBushouName.c_str();

			int iBushouSex = nb6bushouname[iOrgBushouIDOfAnsatsuSareruGawa-1].sex;
			int iBushouTone = nb6bushouref[iOrgBushouIDOfAnsatsuSareruGawa-1].tone;
			char szAnsatsuMsg[256] = "";

			// 逆暗殺その１
			wsprintf( szAnsatsuMsg, "策に溺れし愚か者め\x0A闇に滅するがよい" );
			oCurMsg.rwMsg( 吹き出しウィンドウ::戦争::原暗殺::REG01,	szAnsatsuMsg );

			// 逆暗殺その２
			wsprintf( szAnsatsuMsg, "哀れなり%s…\x0Aまたこの手を汚して\x0Aしまった…", pBushouName );
			oCurMsg.rwMsg( 吹き出しウィンドウ::戦争::原暗殺::REG02,	szAnsatsuMsg );

			// 逆暗殺その３
			wsprintf( szAnsatsuMsg, "ほっほっほっ\x0A本日の茶は\x0Aいつにまして格別…" );
			oCurMsg.rwMsg( 吹き出しウィンドウ::戦争::原暗殺::REG03,	szAnsatsuMsg );

			// 逆暗殺その４
			// 女武将
			if ( iBushouSex == 1 ) {
				wsprintf( szAnsatsuMsg, "姑息なり%s！\x0A見よ、将をなくした\x0A部隊は総崩れでしょう", pBushouName );
			// 口調が粗暴
			} else if ( iBushouTone == 2 ) {
				wsprintf( szAnsatsuMsg, "猿知恵なり%s！\x0A見よ、将をなくした\x0A部隊は総崩れだろう", pBushouName );
			// 口調が高貴
			} else if ( iBushouTone == 3 ) {
				wsprintf( szAnsatsuMsg, "才知は身の仇…\x0A見よ、将をなくした\x0A部隊は総崩れじゃろう" );
			// その他の口調
			} else {
				wsprintf( szAnsatsuMsg, "策に溺れたか%s！\x0A見よ、将をなくした\x0A部隊は総崩れであろう", pBushouName );
			}
			oCurMsg.rwMsg( 吹き出しウィンドウ::戦争::原暗殺::REG04,	szAnsatsuMsg );

			// 逆暗殺その５
			wsprintf( szAnsatsuMsg, "才子才に躓く\x0Aこれが最期とは\x0Aはかないものよのう…" );
			oCurMsg.rwMsg( 吹き出しウィンドウ::戦争::原暗殺::REG05,	szAnsatsuMsg );
		}

		// 朱伊の場合。この条件はなぜか必要。
		else if (isShuiHidechiyo(iOrgBushouIDOfAnsatsuSareruGawa - 1)) {

			// どれかのメッセージにひっかかっていたら終了
			if (oCurMsg.isMsg(吹き出しウィンドウ::戦争::原暗殺::REG01) ||
				oCurMsg.isMsg(吹き出しウィンドウ::戦争::原暗殺::REG02) ||
				oCurMsg.isMsg(吹き出しウィンドウ::戦争::原暗殺::REG03) ||
				oCurMsg.isMsg(吹き出しウィンドウ::戦争::原暗殺::REG04) ||
				oCurMsg.isMsg(吹き出しウィンドウ::戦争::原暗殺::REG05)) {

				// 逆暗殺用のメッセージ変動モードは終了
				iStatusAntiAnsatsuMessageModeOnAttackInBattleExecute = 0;

			}

			// どれかのメッセージは確実なので置き換えの
			//char *pBushouName = nb6bushouname[iOrgBushouIDOfAnsatsuSuruGawa-1].fastname;
			int iTmp2nd_BushouID = iOrgBushouIDOfAnsatsuSuruGawa - 1;
			int iTmp1st_BushouID = iOrgBushouIDOfAnsatsuSareruGawa - 1;
			string strBushouName = ％相手％(iTmp1st_BushouID, iTmp2nd_BushouID);
			char *pBushouName = (char *)strBushouName.c_str();

			int iBushouSex = nb6bushouname[iOrgBushouIDOfAnsatsuSareruGawa - 1].sex;
			int iBushouTone = nb6bushouref[iOrgBushouIDOfAnsatsuSareruGawa - 1].tone;
			char szAnsatsuMsg[256] = "";

			// 逆暗殺その１
			wsprintf(szAnsatsuMsg, "策に溺れし愚か者め\x0A闇に滅するがよい");
			oCurMsg.rwMsg(吹き出しウィンドウ::戦争::原暗殺::REG01, szAnsatsuMsg);

			// 逆暗殺その２
			wsprintf(szAnsatsuMsg, "哀れなり%s…\x0Aまたこの手を汚して\x0Aしまった…", pBushouName);
			oCurMsg.rwMsg(吹き出しウィンドウ::戦争::原暗殺::REG02, szAnsatsuMsg);

			// 逆暗殺その３
			wsprintf(szAnsatsuMsg, "ほっほっほっ\x0A本日の茶は\x0Aいつにまして格別…");
			oCurMsg.rwMsg(吹き出しウィンドウ::戦争::原暗殺::REG03, szAnsatsuMsg);

			// 逆暗殺その４
			// 女武将
			if (iBushouSex == 1) {
				wsprintf(szAnsatsuMsg, "姑息なり%s！\x0A見よ、将をなくした\x0A部隊は総崩れでしょう", pBushouName);
				// 口調が粗暴
			}
			else if (iBushouTone == 2) {
				wsprintf(szAnsatsuMsg, "猿知恵なり%s！\x0A見よ、将をなくした\x0A部隊は総崩れだろう", pBushouName);
				// 口調が高貴
			}
			else if (iBushouTone == 3) {
				wsprintf(szAnsatsuMsg, "才知は身の仇…\x0A見よ、将をなくした\x0A部隊は総崩れじゃろう");
				// その他の口調
			}
			else {
				wsprintf(szAnsatsuMsg, "策に溺れたか%s！\x0A見よ、将をなくした\x0A部隊は総崩れであろう", pBushouName);
			}
			oCurMsg.rwMsg(吹き出しウィンドウ::戦争::原暗殺::REG04, szAnsatsuMsg);

			// 逆暗殺その５
			wsprintf(szAnsatsuMsg, "才子才に躓く\x0Aこれが最期とは\x0Aはかないものよのう…");
			oCurMsg.rwMsg(吹き出しウィンドウ::戦争::原暗殺::REG05, szAnsatsuMsg);
		}

	}


	if ( iTeppouHanekaesiInTeppouDeath == 1 ) {
		/*
		 以下、通常武将が鉄砲を弾き返された時のメッセージ
		*/
		extern int iTeppouAttackBushouIDInTeppouDeath; // 撃たれた側(入替済みなため)
		extern int iTeppouDefendBushouIDInTeppouDeath; // 撃った側(入替済みなため)
		int i1st = iTeppouDefendBushouIDInTeppouDeath-1;
		int i2nd = iTeppouAttackBushouIDInTeppouDeath-1;

		if ( oCurMsg.isMsg( "む、無念…\x0A" ) || 
			 oCurMsg.isMsg( "ここで死ぬの\x0A" ) ||
			 oCurMsg.isMsg( "これほどの恐怖を\x0A" ) ||
			 oCurMsg.isMsg( "に立ち向かった\x0A" ) ||
			 oCurMsg.isMsg( "次は平和な時代に\x0A" ) ) {

			// ランスロット以外は弾き返さないはずであるが、一応の処理
			if ( isLancelotTartare(i2nd) ) { 
				iTeppouHanekaesiInTeppouDeath = 2;
			} else { 
				iTeppouHanekaesiInTeppouDeath = 0;
			}

			string msg = ％罵倒相手％(i1st ,i2nd) + "め！\x0A鉄砲を刀で弾き返すとは\x0A"+％ははは％(i1st,i2nd,0xFFFF)+"……ぐふ…";
			if (isShuiHidechiyo(i2nd)) {
				msg = ％罵倒相手％(i1st, i2nd) + "め！\x0A鉄砲を棒で弾き返すとは\x0A" + ％ははは％(i1st, i2nd, 0xFFFF) + "……ぐふ…";
			}
			char szmsg[100] = "";
			strcpy(szmsg, msg.c_str() );
			oCurMsg.rwMsg( "む、無念…\x0A",		szmsg);
			oCurMsg.rwMsg( "ここで死ぬの\x0A",		szmsg);
			oCurMsg.rwMsg( "これほどの恐怖を\x0A",		szmsg);
			oCurMsg.rwMsg( "に立ち向かった\x0A",		szmsg);
			oCurMsg.rwMsg( "次は平和な時代に\x0A",		szmsg);
		}
	}

	if ( oCurMsg.isMsg("変えてくれるわ\x0A者ども、続けえ！") || oCurMsg.isMsg("逃げ道はないぞ\x0A攻めかかれえっ！")) {
		if ( nb6fukidashirelatepersons->i1stPersonID <= GAMEDATASTRUCT_BUSHOU_NUM ) {
			int iBushouID = nb6fukidashirelatepersons->i1stPersonID-1;

			if ( rand()%6 == 0 ) {
				// バールゼフォン
				if ( strcmp( nb6bushouname[iBushouID].familyname, "" )==0 && strcmp( nb6bushouname[iBushouID].fastname, "" )==0 ) {
					oCurMsg.rwMsg("変えてくれるわ\x0A者ども、続けえ！", "真の騎士の誇りを\x0A貴様に見せてやろう…\x0Aフレイミングデス！");
					oCurMsg.rwMsg("逃げ道はないぞ\x0A攻めかかれえっ！", "真の騎士の誇りを\x0A貴様に見せてやろう…\x0Aフレイミングデス！");
				}
				// ヴォラック
				else if ( strcmp( nb6bushouname[iBushouID].familyname, "" )==0 && strcmp( nb6bushouname[iBushouID].fastname, "" )==0 ) {
					oCurMsg.rwMsg("変えてくれるわ\x0A者ども、続けえ！", "暗黒騎士団の力を\x0A侮ってもらっては困る\x0Aライアットバーン！");
					oCurMsg.rwMsg("逃げ道はないぞ\x0A攻めかかれえっ！", "暗黒騎士団の力を\x0A侮ってもらっては困る\x0Aライアットバーン！");
				}
				// バルバス
				else if ( strcmp( nb6bushouname[iBushouID].familyname, "" )==0 && strcmp( nb6bushouname[iBushouID].fastname, "" )==0 ) {
					oCurMsg.rwMsg("変えてくれるわ\x0A者ども、続けえ！", "この俺の奥義が見たい\x0Aのか？　いい度胸だ…\x0Aデスアベンジャー！");
					oCurMsg.rwMsg("逃げ道はないぞ\x0A攻めかかれえっ！", "この俺の奥義が見たい\x0Aのか？　いい度胸だ…\x0Aデスアベンジャー！");
				}
				// マルティム
				else if ( strcmp( nb6bushouname[iBushouID].familyname, "㌘㌧" )==0 && strcmp( nb6bushouname[iBushouID].fastname, "㌔㌢㍍" )==0 ) {
					oCurMsg.rwMsg("変えてくれるわ\x0A者ども、続けえ！", "この俺とサシで戦おう\x0Aなんざ10年早ぇんだ！\x0Aフローヴェノム！");
					oCurMsg.rwMsg("逃げ道はないぞ\x0A攻めかかれえっ！", "この俺とサシで戦おう\x0Aなんざ10年早ぇんだ！\x0Aフローヴェノム！");
				}
				// ハボリム
				else if ( strcmp( nb6bushouname[iBushouID].familyname, "" )==0 && strcmp( nb6bushouname[iBushouID].fastname, "" )==0 ) {
				}
				// オズマ
				else if ( strcmp( nb6bushouname[iBushouID].familyname, "ёжз" )==0 && strcmp( nb6bushouname[iBushouID].fastname, "ийк" )==0 ) {
					oCurMsg.rwMsg("変えてくれるわ\x0A者ども、続けえ！", "根性だけは認めるわ…\x0Aでも、そこまでよ…、\x0Aデーモンローズ！");
					oCurMsg.rwMsg("逃げ道はないぞ\x0A攻めかかれえっ！", "根性だけは認めるわ…\x0Aでも、そこまでよ…、\x0Aデーモンローズ！");
				}
				// ベルサリア
				else if ( strcmp( nb6bushouname[iBushouID].familyname, "" )==0 && strcmp( nb6bushouname[iBushouID].fastname, "" )==0 ) {
				}
				// カノープス
				else if ( strcmp( nb6bushouname[iBushouID].familyname, "" )==0 && strcmp( nb6bushouname[iBushouID].fastname, "" )==0 ) {
				}
				// ギルバルド
				else if ( strcmp( nb6bushouname[iBushouID].familyname, "" )==0 && strcmp( nb6bushouname[iBushouID].fastname, "" )==0 ) {
				}
				// ミルディン
				else if ( strcmp( nb6bushouname[iBushouID].familyname, "" )==0 && strcmp( nb6bushouname[iBushouID].fastname, "" )==0 ) {
				}
				// ギルダス
				else if ( strcmp( nb6bushouname[iBushouID].familyname, "" )==0 && strcmp( nb6bushouname[iBushouID].fastname, "шщъ" )==0 ) {
				}
				// デネブ
				else if ( strcmp( nb6bushouname[iBushouID].familyname, "" )==0 && strcmp( nb6bushouname[iBushouID].fastname, "" )==0 ) {
					oCurMsg.rwMsg("変えてくれるわ\x0A者ども、続けえ！", "禁じられし太古の妖炎\x0A呼び戻さん…焼けよ！\x0Aスーパーノヴァ！");
					oCurMsg.rwMsg("逃げ道はないぞ\x0A攻めかかれえっ！", "禁じられし太古の妖炎\x0A呼び戻さん…焼けよ！\x0Aスーパーノヴァ！");
				}
				// ラドラム
				else if ( strcmp( nb6bushouname[iBushouID].familyname, "" )==0 && strcmp( nb6bushouname[iBushouID].fastname, "" )==0 ) {
					oCurMsg.rwMsg("変えてくれるわ\x0A者ども、続けえ！", "天駆ける光竜の御力で\x0A星々を大地に落とさん\x0Aメテオストライク！");
					oCurMsg.rwMsg("逃げ道はないぞ\x0A攻めかかれえっ！", "天駆ける光竜の御力で\x0A星々を大地に落とさん\x0Aメテオストライク！");
				}
				// ウォーレン
				else if (strcmp( nb6bushouname[iBushouID].familyname,"")==0 &&  strcmp( nb6bushouname[iBushouID].fastname, "")==0 ) {
				}

			}
		}
	}

	// 外部メッセージ置き換えをScenarioModにて行う
	if ( p_snOnRewriteFukidashiMessage ) {
		char szModMsgBuf[512] = "";

		// 通常タイプ
		if ( isMessageLOCMode == 4) {

			// 現在のメモリの最新の状態を取り込み
			char szCurMsgBuf[512] = "　　　　"; // 渡した文字がヌルなどの判定に合格しないようにそれなりに
			// メモリから読み込み
			ReadProcessMemory( CommonGlobalHandle::curProcess, (LPCVOID)(CPM.getMemoryAddress(LOC4)), szCurMsgBuf, sizeof(szCurMsgBuf)-1, NULL);
			szCurMsgBuf[511] = NULL; // 安全ブロック

			// 武将名, 現在の書き換え対象となっているメッセージ, 関連人物情報
			// ((PFNSNONREWRITEFUKIDASHIMESSAGE) p_snOnRewriteFukidashiMessage)(szModMsgBuf, name, CPM.getCurVarPtr(LOC4), nb6fukidashirelatepersons);
			((PFNSNONREWRITEFUKIDASHIMESSAGE) p_snOnRewriteFukidashiMessage)(szModMsgBuf, name, szCurMsgBuf, nb6fukidashirelatepersons);
			if ( strlen(szModMsgBuf) > 1 ) {
				// こっちはgetCurVarPtr というように古い状態でもＯＫ
				oCurMsg.rwMsg( (char *)CPM.getCurVarPtr(LOC4), szModMsgBuf, -1 );
				isCurScenarioModReWrite = true;
			}

		// パラメタタイプ(４がパラメタで２に展開された文章がある）
		} else if ( isMessageLOCMode == 2 ) {

			// 現在のメモリの最新の状態を取り込み
			char szCurMsgBuf[512] = "　　　　"; // 渡した文字がヌルなどの判定に合格しないようにそれなりに
			ReadProcessMemory( CommonGlobalHandle::curProcess, (LPCVOID)(CPM.getMemoryAddress(LOC2)), szCurMsgBuf, sizeof(szCurMsgBuf)-1, NULL);
			szCurMsgBuf[511] = NULL; // 安全ブロック

			// ((PFNSNONREWRITEFUKIDASHIMESSAGE) p_snOnRewriteFukidashiMessage)(szModMsgBuf, name, CPM.getCurVarPtr(LOC2), nb6fukidashirelatepersons);
			((PFNSNONREWRITEFUKIDASHIMESSAGE) p_snOnRewriteFukidashiMessage)(szModMsgBuf, name, szCurMsgBuf, nb6fukidashirelatepersons);
			if ( strlen(szModMsgBuf) > 1 ) {
				// こっちはgetCurVarPtr というように古い状態でもＯＫ
				oCurMsg.rwMsg( (char *)CPM.getCurVarPtr(LOC2), szModMsgBuf, -1 );
				isCurScenarioModReWrite = true;
			}

		}
	}

	return true;
}



void checkOnTenshouExeMessageLoad() {
	;
}


/*

0046C18C  |. E8 0C000000    CALL    TENSHOU.0046C19D
0046C191  |. 83C4 04        ADD     ESP, 4
0046C194  |. B8 C0974D00    MOV     EAX, TENSHOU.004D97C0            ;  ASCII "データの読み込み中です
				└ここを JMP TSMod.JmpOnTSExeMessageLoad とするのが目的　そして、元々この行にあった MOV     EAX, TENSHOU.004D97C0 は関数内で処理

0046C199  |. 5E             POP     ESI

0049D144  -E9 8782F2FF               ; ⇒ OnTenshouExeMessageLoadへ
　　　　　　　　└ ここを実行時に書き換えてやる必要がある。
*/
int pTenshouExeJumpFromToOnTenshouExeMessageLoad    =0x46C194; // 関数はこのアドレスから、OnTenshouExeMessageLoadへとジャンプしてくる。
int pTenshouExeReturnLblFromOnTenshouExeMessageLoad =0x46C199; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeMessageLoad() {
	// スタックにためておく
	__asm {
		// 元々TENSHOU.EXE内にあったものをここで
		mov eax, 4D97C0h

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}
	
	checkOnTenshouExeMessageLoad();

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

		jmp pTenshouExeReturnLblFromOnTenshouExeMessageLoad
	}
}

/*
0049D143   90               NOP
0049D144  -E9 8782F2FF      JMP     TSMod.JmpOnTSExeMessageLoad         ; ⇒ OnTenshouExeMessageLoadへ
　　　　　　　　└ ここを実行時に書き換えてやる必要がある。
0049D149   90               NOP
*/
char cmdOnTenshouExeJumpFromMessageLoad[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


void WriteAsmJumperOnTenshouExeMessageLoad() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeMessageLoad;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeMessageLoad + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeMessageLoad  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromMessageLoad+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeMessageLoad), cmdOnTenshouExeJumpFromMessageLoad, 5, NULL); // 5バイトのみ書き込む
}









/*
004ADB28  /$ 8B4424 04      MOV     EAX, DWORD PTR SS:[ESP+4]
004ADB2C  |. 8D4C24 0C      LEA     ECX, DWORD PTR SS:[ESP+C]
004ADB30  |. 51             PUSH    ECX
004ADB31  |. A3 B8674E00    MOV     DWORD PTR DS:[4E67B8], EAX
004ADB36  |. FF7424 0C      PUSH    DWORD PTR SS:[ESP+C]
004ADB3A  |. 68 16DB4A00    PUSH    TENSHOU.004ADB16
004ADB3F  |. E8 E9FCFFFF    CALL    TENSHOU.004AD82D
004ADB44  |. 83C4 0C        ADD     ESP, 0C
004ADB47  |. 8B0D B8674E00  MOV     ECX, DWORD PTR DS:[4E67B8]       ;  TENSHOU.004D9B91
004ADB4D  |. C601 00        MOV     BYTE PTR DS:[ECX], 0
004ADB50  \. C3             RETN


004ADB47  |. 8B0D B8674E00  MOV     ECX, DWORD PTR DS:[4E67B8]       ;  TENSHOU.004D9B91
				└ここを JMP TSMod.OnTSExeMessageCopy とするのが目的　そして、元々この行にあった MOV     EAX, TENSHOU.004D97C0 は関数内で処理

*/
extern void OnTenshouExeEndingConditionYakushokuMsgRewrite();
extern void OnTenshouExeEndingConditionScenarioModMsgRewrite();
// メッセージがコピーされた時。吹き出しや各種ウィンドウに表示される文字があらたに呼ばれたタイミング
void checkOnTenshouExeMessageCopy() {
	// 地方統一モードの場合など、カスタムエンディング判定処理
	if ( TSModCommand::MyselfDaimyo::cmdGameEndConditionIsGettingPosition ) {
		OnTenshouExeEndingConditionYakushokuMsgRewrite();
	}

	OnTenshouExeEndingConditionScenarioModMsgRewrite();
}


int pTenshouExeJumpFromToOnTenshouExeMessageCopy    =0x4ADB47; // 関数はこのアドレスから、OnTenshouExeMessageCopyへとジャンプしてくる。
int pTenshouExeReturnLblFromOnTenshouExeMessageCopy =0x4ADB4D; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeMessageCopy() {
	// スタックにためておく
	__asm {
		// 元々TENSHOU.EXE内にあったものをここで
		mov ecx, dword ptr ds:[4E67B8h]

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}
	
	checkOnTenshouExeMessageCopy();

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

		jmp pTenshouExeReturnLblFromOnTenshouExeMessageCopy
	}
}

/*
0049D143   90               NOP
0049D144  -E9 8782F2FF      JMP     TSMod.OnTSExeMessageCopy         ; ⇒ OnTenshouExeMessageCopyへ
　　　　　　　　└ ここを実行時に書き換えてやる必要がある。
0049D149   90               NOP
*/
char cmdOnTenshouExeJumpFromMessageCopy[7]="\xE9\x90\x90\x90\x90\x90";
// 元の命令が6バイト、以後の関数で生まれる命令が合計５バイトなので、NOPが最後に付け足されている。


void WriteAsmJumperOnTenshouExeMessageCopy() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeMessageCopy;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeMessageCopy + 5 );
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeMessageCopy  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromMessageCopy+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeMessageCopy), cmdOnTenshouExeJumpFromMessageCopy, 6, NULL); // 6バイトのみ書き込む 元の命令が6バイト、新規の命令が５バイトなので、NOPが付け足されている。
}










/*
004AD208  |> 5B             POP     EBX                              ;  TENSHOU.004B5930
				└ここを JMP TSMod.OnTSExeMessageCopyMem とするのが目的　そして、元々この行にあった MOV     EAX, TENSHOU.004D97C0 は関数内で処理
004AD209  |. C601 00        MOV     BYTE PTR DS:[ECX], 0
004AD20C  \. C3             RETN
*/

// メッセージがコピーされた時。吹き出しや各種ウィンドウに表示される文字があらたに呼ばれたタイミング

void checkOnTenshouExeMessageCopyMem() {
	;
}

int pTenshouExeJumpFromToOnTenshouExeMessageCopyMem    =0x4AD208; // 関数はこのアドレスから、OnTenshouExeMessageCopyMemへとジャンプしてくる。
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeMessageCopyMem() {
	// スタックにためておく
	__asm {
		POP     EBX
		MOV     BYTE PTR DS:[ECX], 0
		// 元々TENSHOU.EXE内にあったものをここで

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}
	
	checkOnTenshouExeMessageCopyMem();

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

		// 元々TENSHOU.EXE内にあったものをここで
		RETN
	}
}

/*
0049D143   90               NOP
0049D144  -E9 8782F2FF      JMP     TSMod.OnTSExeMessageCopyMem         ; ⇒ OnTenshouExeMessageCopyMemへ
　　　　　　　　└ ここを実行時に書き換えてやる必要がある。
0049D149   90               NOP
*/
char cmdOnTenshouExeJumpFromMessageCopyMem[6]="\xE9";
// 元の命令が6バイト、以後の関数で生まれる命令が合計５バイトなので、NOPが最後に付け足されている。


void WriteAsmJumperOnTenshouExeMessageCopyMem() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeMessageCopyMem;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeMessageCopyMem + 5 );
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeMessageCopyMem  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromMessageCopyMem+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeMessageCopyMem), cmdOnTenshouExeJumpFromMessageCopyMem, 5, NULL); // 5バイトのみ書き込む
}
