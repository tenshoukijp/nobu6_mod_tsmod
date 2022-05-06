#include "WinTarget.h"

#include "TSModCommand.h"
#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"
#include "ScenarioMod.h"

// エンディングテスト用のフラグ
int iEndingConditionTest = -1;
// エンディングを迎えた大名ＩＤ
int iEndingConditionDaimyoID = -1;

// エンディングを迎えた大名ＩＤ
char szEndingConditionRewriteMsg[100] = "";			// ダイアログの中身
char szEndingConditionRewriteYakushokuMsg[20] = "祝！役職就任";
char szEndingConditionRewriteDoumeiAllMsg[20] = "祝！同盟統一";
char szEndingConditionRewriteScenarioModAllMsg[20] = "";
int isMustRewriteEndingConditionRewriteMsg = -1;  // 1の時は、役職就任。2の時は、同盟統一

void OnTenshouExeEndingConditionYakushokuMsgRewrite() {
	// エンディング置き換え用文字列に有効な値が入っている。
	if ( isMustRewriteEndingConditionRewriteMsg == 1) {
		// 全国統一のメッセージであれば…
		if ( strstr ( (char *)CPM.getMemoryAddress(LOC1), "は全国の統一を達成した" ) ) {
			// 該当のメッセージで置き換える
			WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(CPM.getMemoryAddress(LOC1)), szEndingConditionRewriteMsg, strlen(szEndingConditionRewriteMsg)+1, NULL);
		}
	}
}

void OnTenshouExeEndingConditionScenarioModMsgRewrite() {
	// ScenarioMod用
	if ( isMustRewriteEndingConditionRewriteMsg == 3) {
		// 全国統一のメッセージであれば…
		if ( strstr ( (char *)CPM.getMemoryAddress(LOC1), "は全国の統一を達成した" ) ) {
			// 該当のメッセージで置き換える
			WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(CPM.getMemoryAddress(LOC1)), szEndingConditionRewriteMsg, strlen(szEndingConditionRewriteMsg)+1, NULL);
		}
	}
}

#pragma warning( push )
#pragma warning( disable: 4996 )


// ScenarioModと共有のエンディングダイアログの型
struct TEndingDialogInfo {
	int iDaimyoNo; // +1の方
	char szEndingTitle[20];
	char szEndingConditionRewriteMsg[100];
	int isNeverEnding;
	char szEndingMovieName[50];
};

extern void TenshouExeEndingTitlePushAddressClear();
void OnTenshouExeEndingConditionTestExecute() {

	// もうエンディング突入なら何もしない
	if ( CommonGlobalHandle::isEndingMode ) {
		return;
	}

	// エンディングのパラメタクリア後、トップメニューに戻る前にここにまた来た。
	if (CommonGlobalHandle::isEndingModeAfter) {
		TenshouExeEndingTitlePushAddressClear();
		return;
	}

	isMustRewriteEndingConditionRewriteMsg = 0;

	if ( p_snOnEndingCondition ) {
		// エンディングの該当者が居ない場合のみ、この特別判定。

		TEndingDialogInfo pEndingDialogInfo;
		// 特殊なエンディングの成否
		BOOL isReWrite = ((PFNSNONENDINGCONDITION) p_snOnEndingCondition)(&pEndingDialogInfo, nb6turn.daimyo, -1);
		if ( isReWrite ) {

			// エンディング無視フラグが立っていれば
			if (pEndingDialogInfo.isNeverEnding ) {
				// コンディションは無かったことになる。(1ならば統一者は居ない)
				iEndingConditionTest = 1;

				return;
			}

			// エンディングの該当者が居ない場合のみ、この特別判定処理を行う。
			// 強制的になんらかのエンディングにするという意思
			int iDaimyoID = nb6turn.daimyo-1;

			if ( 0 <= iDaimyoID && iDaimyoID < GAMEDATASTRUCT_DAIMYO_NUM ) {

				iEndingConditionTest = 0; // 誰かが統一(というかエンディング)へと向かう

				// 統一相当大名としてＩＤを入れる
				iEndingConditionDaimyoID = iDaimyoID + 1;

				// エンディングのタイトルをあとで使う領域にコピーしておく
				strcpy( szEndingConditionRewriteScenarioModAllMsg, pEndingDialogInfo.szEndingTitle );

				// エンディングのメッセージをコピーする。
				strcpy( szEndingConditionRewriteMsg, pEndingDialogInfo.szEndingConditionRewriteMsg );

				// ScenarioMod用に置き換え必要があるというフラグをONとする。
				isMustRewriteEndingConditionRewriteMsg = 3;

				return;
			}
		}


		// 書き換えていない、もしくは、有効でないならば、CUSTOM_CONDITION_NOMODIFY なら他のエンディング条件へと流れていく
	}

	if ( TSModCommand::MyselfDaimyo::cmdGameEndConditionIsGettingPosition == 1) { 
		// エンディングの該当者が居ない場合のみ、この地方処理判定を行う。
		if ( iEndingConditionDaimyoID == 0xFFFF && iEndingConditionTest == 1 ) {
			for ( int iDaimyoID=0; iDaimyoID < GAMEDATASTRUCT_DAIMYO_NUM; iDaimyoID++ ) {
				// 該当の大名がプレイヤー担当の大名であれば…
				if ( isPlayerBushou( nb6daimyo[iDaimyoID].attach-1 )) {
					int iYakushokuID = getNormalizedDaimyoPosition( iDaimyoID );
					// その大名が役職を持っていれば…
					if ( iYakushokuID > 0 ) {
						// 統一相当大名としてＩＤを入れる
						iEndingConditionDaimyoID = iDaimyoID+1;
						// 誰かが統一したこととする。、
						iEndingConditionTest = 0;

						// エンディングのメッセージを作成する。
						strcpy( szEndingConditionRewriteMsg, "");
						char szWsprintfEndingConditionRewriteMsg[100] = "";
						wsprintf( szWsprintfEndingConditionRewriteMsg, "%s%sは幕府の役職、%sに就任した！\x0",
						nb6bushouname[nb6daimyo[iDaimyoID].attach-1].familyname,
						nb6bushouname[nb6daimyo[iDaimyoID].attach-1].fastname,
						getPositionTypeName(iYakushokuID));
						strcpy( szEndingConditionRewriteMsg, szWsprintfEndingConditionRewriteMsg );

						// 置き換え必要があるというフラグをONとする。
						isMustRewriteEndingConditionRewriteMsg = 1;

					}
				}
			}
		}
	}
	// 戦国時代がエンドレスという設定がなされているならば…
	else if ( TSModCommand::AllDaimyo::cmdTheSengokouPeriodIsEndless ) {
		// コンディションは無かったことになる。(1ならば統一者は居ない)
		iEndingConditionTest = 1;

	// 同盟型エンディングの実装
	}
	else {
		// エンディングの該当者が居ない場合のみ、この同盟型エンディング処理判定を行う。
		if ( iEndingConditionDaimyoID == 0xFFFF && iEndingConditionTest == 1 ) {

			for ( int iDaimyoID=0; iDaimyoID < GAMEDATASTRUCT_DAIMYO_NUM; iDaimyoID++ ) {

				// 該当IDの大名が居なければ、次の人
				if (nb6daimyo[iDaimyoID].attach == 0xFFFF) {
					continue;
				}

				int iMyBushouID = nb6daimyo[iDaimyoID].attach-1;

				// 該当の大名がプレイヤー担当の大名であれば…
				if ( isPlayerBushou( iMyBushouID )) {

					if ( iDaimyoID == GetMaximumInfluenceDaimyo() ) {

						// 全ての生存大名と同盟状態であるかどうかをチェック
						bool isAllDoumeiStatus = true;

						for ( int iTargetDaimyoID=0; iTargetDaimyoID < GAMEDATASTRUCT_DAIMYO_NUM; iTargetDaimyoID++ ) {

							// 該当IDの大名が自分なら、次の大名
							if (iDaimyoID == iTargetDaimyoID) {
								continue;
							}

							// 該当IDの大名が存在しなければ、次の大名
							if (nb6daimyo[iTargetDaimyoID].attach == 0xFFFF) {
								continue;
							}

							if ( !isDoumeiKankei(iDaimyoID, iTargetDaimyoID) ) {
								isAllDoumeiStatus = false;
							}
						}

						if ( isAllDoumeiStatus ) {
							// 誰かが統一したこととする。、
							iEndingConditionTest = 0;

							// エンディングを迎えた大名は該当大名
							iEndingConditionDaimyoID = iDaimyoID+1;

							// 置き換え必要があるというフラグをONとする。
							isMustRewriteEndingConditionRewriteMsg = 2;


						}
					}
				}
			}
		}
	}
}

#pragma warning( pop )


/*
統一達成判定のフラグは以下のEAXに依存する。
EAXが0ならば統一。
1ならばまだ統一していない。

この時のESIが統一した大名番号(１始まり)。統一した大名が居ない時には0xFFFFとなっている。

0047EC3F  |. E8 4A700200    CALL    TENSHOU.004A5C8E
				└ここを JMP TSMod.OnTSExeEndingConditionTest と書き換えてやる実際の処理
0047EC44  |. 83C4 04        ADD     ESP, 4
0047EC47  |. 85C0           TEST    EAX, EAX
0047EC49  |. 75 34          JNZ     SHORT TENSHOU.0047EC7F
0047EC4B  |. 68 00400000    PUSH    4000
0047EC50  |. E8 5E83FEFF    CALL    TENSHOU.00466FB3
 */
int pTenshouExeJumpFromToOnTenshouExeEndingConditionTest    =0x47EC3F; // 関数はこのアドレスから、OnTenshouExeEndingConditionTestへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeEndingConditionTest=0x4A5C8E; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeEndingConditionTest =0x47EC44; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w

// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeEndingConditionTest() {
	// スタックにためておく
	__asm {
		// 元々TENSHOU.EXE内にあったものをここで
		call pTenshouExeJumpCallFromToOnTenshouExeEndingConditionTest

		// エンディングを迎えたやつがいない(0=統一者が居る。1ならば統一者はいない)
		mov iEndingConditionTest, EAX

		// エンディングを迎えた大名のＩＤ(0xFFFFならば該当者は居ない)
		mov iEndingConditionDaimyoID, ESI

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}
	
	// エンディングコンディションへ
	OnTenshouExeEndingConditionTestExecute();

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

		// 統一したやつがいるかどうかを上書き
		mov eax, iEndingConditionTest
		// 統一した大名情報を上書き
		mov esi, iEndingConditionDaimyoID

		jmp pTenshouExeReturnLblFromOnTenshouExeEndingConditionTest
	}
}

/*
00444769  |. 50             PUSH    EAX
0044476A  |. E8 3A2C0200    CALL    TENSHOU.004673A9
				└ここを JMP TSMod.OnTSExeEndingConditionTest と書き換えてやる実際の処理
0044476F  |. 83C4 04        ADD     ESP, 4
*/
char cmdOnTenshouExeJumpFromEndingConditionTest[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeEndingConditionTest() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeEndingConditionTest;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeEndingConditionTest + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeEndingConditionTest  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromEndingConditionTest+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeEndingConditionTest), cmdOnTenshouExeJumpFromEndingConditionTest, 5, NULL); //5バイトのみ書き込む
}






//---------------------------------------------------------------------------------------------






//-------------------------------------------------------------------


int EndingTitlePushAddress = -1;

// ゲームが終わったので、Endingがらみの文字列上書き等で変更したものをもとへと戻す
void TenshouExeEndingTitlePushAddressClear() {

	EndingTitlePushAddress = 0x4D1978; // デフォルトの状態に一応戻しておく

	iEndingConditionTest = -1;
	// エンディングテスト用のフラグ
	iEndingConditionDaimyoID = -1;
	// エンディングを迎えた大名ＩＤ

	ZeroMemory(szEndingConditionRewriteMsg, sizeof(szEndingConditionRewriteMsg)); // ダイアログの中身クリア。これは武将名一覧などとメモリアドレスが共有なのでクリアしておく
	ZeroMemory(szEndingConditionRewriteScenarioModAllMsg, sizeof(szEndingConditionRewriteScenarioModAllMsg)); // ScenarioModからの受け口。

	WriteProcessMemory(CommonGlobalHandle::curProcess, (LPVOID)(CPM.getMemoryAddress(LOC1)), szEndingConditionRewriteMsg, sizeof(szEndingConditionRewriteMsg), NULL);

	// プレイしている人をなくすことで強制的に２重目のループを解消できるのではないか。
	if (CommonGlobalHandle::isEndingModeAfter) {
		for (int iPG = 0; iPG < GAMEDATASTRUCT_PLAYERS8_NUM; iPG++) {
			// プレイヤーが担当している軍団のＩＤを確保
			nb6players8[iPG].gundan = 0xFFFF;
		}
	}
}

void OnTenshouExeEndingTitlePushExecute() {

	// 一応念のためデフォルト（祝！全国統一の文字列へのアドレス）
	EndingTitlePushAddress = 0x4D1978;

	if ( TSModCommand::MyselfDaimyo::cmdGameEndConditionIsGettingPosition ) { 

		// 祝！全国統一 の文字を（祝！役職就任）に変更するかどうか
		if ( isMustRewriteEndingConditionRewriteMsg == 1) {
			EndingTitlePushAddress = (int)szEndingConditionRewriteYakushokuMsg;

		} else {
			// これがデフォルト。
			EndingTitlePushAddress = 0x4D1978;
		}

	// 祝！全国統一 の文字を（祝！同盟統一）に変更するかどうか
	} else if (isMustRewriteEndingConditionRewriteMsg == 2 ) {
		EndingTitlePushAddress = (int)szEndingConditionRewriteDoumeiAllMsg;
	}

	// ScenarioModからの書き換え
	if (isMustRewriteEndingConditionRewriteMsg == 3 ) {

		EndingTitlePushAddress = (int)szEndingConditionRewriteScenarioModAllMsg;
	}
}


/*
0047EC04  |. 50             PUSH    EAX                              ; /Arg8 => 0000FFFF
0047EC05  |. 50             PUSH    EAX                              ; |Arg7 => 0000FFFF
0047EC06  |. 50             PUSH    EAX                              ; |Arg6 => 0000FFFF
0047EC07  |. 6A 0F          PUSH    0F                               ; |Arg5 = 0000000F
0047EC09  |. 6A 00          PUSH    0                                ; |Arg4 = 00000000
0047EC0B  |. 6A 01          PUSH    1                                ; |Arg3 = 00000001
0047EC0D  |. 68 78194D00    PUSH    TENSHOU.004D1978                 ; |Arg2 = 004D1978
				└ここを JMP TSMod.OnTSExeEndingTitlePush と書き換えてやる実際の処理
0047EC12  |. 56             PUSH    ESI                              ; |Arg1
0047EC13  |. BE 38914D00    MOV     ESI, TENSHOU.004D9138            ; |ASCII "XWK"
0047EC18  |. 8BCE           MOV     ECX, ESI                         ; |
0047EC1A  |. E8 4A230200    CALL    TENSHOU.004A0F69                 ; \TENSHOU.004A0F69
 */
int pTenshouExeJumpFromToOnTenshouExeEndingTitlePush    =0x47EC0D; // 関数はこのアドレスから、OnTenshouExeEndingTitlePushへとジャンプしてくる。
int pTenshouExeReturnLblFromOnTenshouExeEndingTitlePush =0x47EC12; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeEndingTitlePush() {
	// スタックにためておく
	__asm {

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
	OnTenshouExeEndingTitlePushExecute();

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

		// 書き換えたタイトルをプッシュする。
		push EndingTitlePushAddress

		jmp pTenshouExeReturnLblFromOnTenshouExeEndingTitlePush
	}
}

/*
00444769  |. 50             PUSH    EAX
0044476A  |. E8 3A2C0200    CALL    TENSHOU.004673A9
				└ここを JMP TSMod.OnTSExeEndingTitlePush と書き換えてやる実際の処理
0044476F  |. 83C4 04        ADD     ESP, 4
*/
char cmdOnTenshouExeJumpFromEndingTitlePush[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeEndingTitlePush() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeEndingTitlePush;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeEndingTitlePush + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeEndingTitlePush  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromEndingTitlePush+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeEndingTitlePush), cmdOnTenshouExeJumpFromEndingTitlePush, 5, NULL); //5バイトのみ書き込む
}
