#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "GameDataStruct.h"
#include "TSModCommand.h"

/*
 メイン時・戦闘時かかわらず、熟練度が書き換えられる時
 */



// セーブデータに持たせている、季節の変わり目にフィルターのための累積の熟練度が0になる。
void EXSuperService_ExperienceLimitReset() {

	// 累積の熟練度をリセット。
	for ( int iBushouID=0; iBushouID < GAMEDATASTRUCT_BUSHOU_NUM; iBushouID++ ) {
		//  累積の熟練度をリセット。
		nb6savedata_ex.bushou_ex[iBushouID].ruiseki_exp_gov = 0;
		nb6savedata_ex.bushou_ex[iBushouID].ruiseki_exp_bat = 0;
		nb6savedata_ex.bushou_ex[iBushouID].ruiseki_exp_int = 0;
	}
}



/*
 政治熟練度書き換え時
 */
/*
ESIはそのキャラの原点ポインタそのものなので、0人目の原点である0x4BFD18
を引いて、532で割ると、iBushouIDが出るはず・・・

政治熟練度
004094B6  |. E8 B1390A00    CALL    TENSHOU.004ACE6C
004094BB  |. 66:8946 09     MOV     WORD PTR DS:[ESI+9], AX

この時、返り値(AX)が新しい熟練値
*/
int iPointerOfUpdateExperienceGov = NULL; // 該当武将のポインタの原点
WORD iNewValueOfUpdatedExperienceGov = 0; // 更新された後の熟練値
void OnTenshouExeUpdateExperienceGovExecute() {
	int iBushouID = iPointerOfUpdateExperienceGov - GAMEDATASTRUCT_BUSHOU_ADDRESSS; //
	iBushouID = iBushouID / sizeof(NB6BUSHOU);

	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		if ( CommonGlobalHandle::isBattleMode ) { // 戦争中のみ

			double sub = iNewValueOfUpdatedExperienceGov - nb6bushouref[iBushouID].expgov; // 新値-現在値 = 増加分 
			if ( sub > 1 ) { // マイナスや１の時は処理しない
				nb6savedata_ex.bushou_ex[iBushouID].ruiseki_exp_gov += (int)sub; // 増加分をセーブデータに保持
				if ( nb6savedata_ex.bushou_ex[iBushouID].ruiseki_exp_gov > 2000 ) { // 2000以上は2000で蓋
					nb6savedata_ex.bushou_ex[iBushouID].ruiseki_exp_gov = 2000;
				}
				// 政治については、フィルター処理は行わない。保存処理のみ。
			}
		}
	}
}

/*
004094B6  |. E8 B1390A00    CALL    TENSHOU.004ACE6C
				└ここを JMP TSMod.OnTSExeUpdateExperienceGov と書き換えてやる実際の処理
004094BB  |. 66:8946 09     MOV     WORD PTR DS:[ESI+9], AX
*/
int pTenshouExeJumpFromToOnTenshouExeUpdateExperienceGov	 =0x4094B6; // 関数はこのアドレスから、OnTenshouExeUpdateExperienceGovへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeUpdateExperienceGov =0x4ACE6C; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeUpdateExperienceGov  =0x4094BB; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeUpdateExperienceGov() {
	// スタックにためておく
	__asm {
		// 元々TENSHOU.EXE内にあったものをここで
		call pTenshouExeJumpCallFromToOnTenshouExeUpdateExperienceGov

		mov iPointerOfUpdateExperienceGov, esi // esi が該当武将の原点

		mov iNewValueOfUpdatedExperienceGov, ax // axが更新された値。

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}
	
	// 
	OnTenshouExeUpdateExperienceGovExecute();

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

		mov ax, iNewValueOfUpdatedExperienceGov // 結果を上書きした時用

		jmp pTenshouExeReturnLblFromOnTenshouExeUpdateExperienceGov
	}
}

char cmdOnTenshouExeJumpFromUpdateExperienceGov[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeUpdateExperienceGov() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeUpdateExperienceGov;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeUpdateExperienceGov + 5 );
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeUpdateExperienceGov  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromUpdateExperienceGov+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeUpdateExperienceGov), cmdOnTenshouExeJumpFromUpdateExperienceGov, 5, NULL); //5バイトのみ書き込む
}




//---------------------------------------------------------------------------------------------



/*
 戦闘練度書き換え時
 */
/*
ESIはそのキャラの原点ポインタそのものなので、0人目の原点である0x4BFD18
を引いて、532で割ると、iBushouIDが出るはず・・・

政治熟練度
0040957B  |. E8 EC380A00    CALL    TENSHOU.004ACE6C
00409580  |. 66:8946 0B     MOV     WORD PTR DS:[ESI+B], AX

この時、返り値(AX)が新しい熟練値

*/
int iPointerOfUpdateExperienceBat = NULL;
WORD iNewValueOfUpdatedExperienceBat = 0; // 更新された後の熟練値
void OnTenshouExeUpdateExperienceBatExecute() {
	int iBushouID = iPointerOfUpdateExperienceBat - GAMEDATASTRUCT_BUSHOU_ADDRESSS; //
	iBushouID = iBushouID / sizeof(NB6BUSHOU);

	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		if ( CommonGlobalHandle::isBattleMode ) { // 戦争中のみ

			double sub = iNewValueOfUpdatedExperienceBat - nb6bushouref[iBushouID].expbat; // 新値-現在値 = 増加分 
			
			if ( sub > 1 ) { // マイナスや１の時は処理しない
				nb6savedata_ex.bushou_ex[iBushouID].ruiseki_exp_bat += (int)sub; // 増加分をセーブデータに保持
				if ( nb6savedata_ex.bushou_ex[iBushouID].ruiseki_exp_bat > 2000 ) { // 2000以上は2000で蓋
					nb6savedata_ex.bushou_ex[iBushouID].ruiseki_exp_bat = 2000;
				}
				// 急には成長しないがＯＮ
				if ( TSModCommand::AllBushou::cmdNotGrowUpSuddenly ) {
					double coef = 1 + ((double)nb6savedata_ex.bushou_ex[iBushouID].ruiseki_exp_bat / ((double)180 / (double)TSModCommand::AllBushou::cmdNotGrowUpSuddenly) ); // 1.0～11.0ぐらいまで。現在の累積値が大きいほど、coefが大きい。
					sub = sub / coef; // 累積経験値がすくなければ1に近い値で割る。多い時は11に近い値で割る。
					sub = ceil(sub); // 小数点以下は切り上げる。
					iNewValueOfUpdatedExperienceBat = (int)sub + nb6bushouref[iBushouID].expbat; // この新たな差分と現在値の合計が、制御がかかった新値。
				}
			}
		}
	}
}

/*
0040957B  |. E8 EC380A00    CALL    TENSHOU.004ACE6C
				└ここを JMP TSMod.OnTSExeUpdateExperienceBat と書き換えてやる実際の処理
00409580  |. 66:8946 0B     MOV     WORD PTR DS:[ESI+B], AX
*/
int pTenshouExeJumpFromToOnTenshouExeUpdateExperienceBat	 =0x40957B; // 関数はこのアドレスから、OnTenshouExeUpdateExperienceBatへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeUpdateExperienceBat =0x4ACE6C; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeUpdateExperienceBat  =0x409580; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeUpdateExperienceBat() {
	// スタックにためておく
	__asm {
		// 元々TENSHOU.EXE内にあったものをここで
		call pTenshouExeJumpCallFromToOnTenshouExeUpdateExperienceBat

		mov iPointerOfUpdateExperienceBat, esi // esi が該当武将の原点

		mov iNewValueOfUpdatedExperienceBat, ax // axが更新された値。

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}
	
	// 
	OnTenshouExeUpdateExperienceBatExecute();

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

		mov ax, iNewValueOfUpdatedExperienceBat // 結果を上書きした時用

		jmp pTenshouExeReturnLblFromOnTenshouExeUpdateExperienceBat
	}
}

char cmdOnTenshouExeJumpFromUpdateExperienceBat[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeUpdateExperienceBat() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeUpdateExperienceBat;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeUpdateExperienceBat + 5 );
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeUpdateExperienceBat  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromUpdateExperienceBat+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeUpdateExperienceBat), cmdOnTenshouExeJumpFromUpdateExperienceBat, 5, NULL); //5バイトのみ書き込む
}



//---------------------------------------------------------------------------------------------



/*
 智謀熟練度書き換え時
 */
/*
ESIはそのキャラの原点ポインタそのものなので、0人目の原点である0x4BFD18
を引いて、532で割ると、iBushouIDが出るはず・・・

知力の熟練値の書き込み
00409640  |. E8 27380A00    CALL    TENSHOU.004ACE6C
00409645  |. 66:8946 0D     MOV     WORD PTR DS:[ESI+D], AX

この時、返り値(AX)が新しい熟練値
*/

int iPointerOfUpdateExperienceInt = NULL;
WORD iNewValueOfUpdatedExperienceInt = 0; // 更新された後の熟練値
void OnTenshouExeUpdateExperienceIntExecute() {
	int iBushouID = iPointerOfUpdateExperienceInt - GAMEDATASTRUCT_BUSHOU_ADDRESSS; //
	iBushouID = iBushouID / sizeof(NB6BUSHOU);

	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		if ( CommonGlobalHandle::isBattleMode ) { // 戦争中のみ

			double sub = iNewValueOfUpdatedExperienceInt - nb6bushouref[iBushouID].expint; // 新値-現在値 = 増加分 
			if ( sub > 1 ) { // マイナスや１の時は処理しない
				nb6savedata_ex.bushou_ex[iBushouID].ruiseki_exp_int += (int)sub; // 増加分をセーブデータに保持
				if ( nb6savedata_ex.bushou_ex[iBushouID].ruiseki_exp_int > 2000 ) { // 2000以上は2000で蓋
					nb6savedata_ex.bushou_ex[iBushouID].ruiseki_exp_int = 2000;
				}
				// 急には成長しないがＯＮ
				if ( TSModCommand::AllBushou::cmdNotGrowUpSuddenly ) {
					double coef = 1 + ((double)nb6savedata_ex.bushou_ex[iBushouID].ruiseki_exp_int / ((double)180 / (double)TSModCommand::AllBushou::cmdNotGrowUpSuddenly) ); // 1.0～11.0ぐらいまで。現在の累積値が大きいほど、coefが大きい。
					sub = sub / coef; // 累積経験値がすくなければ1に近い値で割る。多い時は11に近い値で割る。
					sub = ceil(sub); // 小数点以下は切り上げる。
					iNewValueOfUpdatedExperienceInt = (int)sub + nb6bushouref[iBushouID].expint; // この新たな差分と現在値の合計が、制御がかかった新値。
				}
			}
		}
	}
}

/*
00409640  |. E8 27380A00    CALL    TENSHOU.004ACE6C
				└ここを JMP TSMod.OnTSExeUpdateExperienceInt と書き換えてやる実際の処理
00409645  |. 66:8946 0D     MOV     WORD PTR DS:[ESI+D], AX
*/
int pTenshouExeJumpFromToOnTenshouExeUpdateExperienceInt	 =0x409640; // 関数はこのアドレスから、OnTenshouExeUpdateExperienceIntへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeUpdateExperienceInt =0x4ACE6C; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeUpdateExperienceInt  =0x409645; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeUpdateExperienceInt() {
	// スタックにためておく
	__asm {
		// 元々TENSHOU.EXE内にあったものをここで
		call pTenshouExeJumpCallFromToOnTenshouExeUpdateExperienceInt

		mov iPointerOfUpdateExperienceInt, esi // esi が該当武将の原点

		mov iNewValueOfUpdatedExperienceInt, ax // axが更新された値。


		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}
	
	// 
	OnTenshouExeUpdateExperienceIntExecute();

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

		mov ax, iNewValueOfUpdatedExperienceInt // 結果を上書きした時用

		jmp pTenshouExeReturnLblFromOnTenshouExeUpdateExperienceInt
	}
}

char cmdOnTenshouExeJumpFromUpdateExperienceInt[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeUpdateExperienceInt() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeUpdateExperienceInt;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeUpdateExperienceInt + 5 );
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeUpdateExperienceInt  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromUpdateExperienceInt+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeUpdateExperienceInt), cmdOnTenshouExeJumpFromUpdateExperienceInt, 5, NULL); //5バイトのみ書き込む
}



