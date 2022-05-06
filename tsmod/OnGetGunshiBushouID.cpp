#include "WinTarget.h"
#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "TSModCommand.h"
#include "EXBushou_All.h"
#include "ScenarioMod.h"
#include "HumanRelation.h"


/*
 軍師武将ＩＤが必要とされた時
 */


//---------------------------------------------------------------------------------
// 軍団番号を引数としてもらっているので覚えておく
int iOriginalGundanIDOfOnGunshiBushouIDPrev = -1;

void OnTenshouExeGetGunshiBushouIDPrevExecute() {
	iOriginalGundanIDOfOnGunshiBushouIDPrev = iOriginalGundanIDOfOnGunshiBushouIDPrev & 0xFFFF; // 下半分だけかもしれないのでフィルター

	// 軍団番号の範囲であれば、iGundanIDにしておく。
	if ( 0 <= iOriginalGundanIDOfOnGunshiBushouIDPrev-1 && iOriginalGundanIDOfOnGunshiBushouIDPrev-1 < GAMEDATASTRUCT_GUNDAN_NUM ) {
		iOriginalGundanIDOfOnGunshiBushouIDPrev = iOriginalGundanIDOfOnGunshiBushouIDPrev-1;
	}
}



/*
00454BF1  |. FF7424 14      PUSH    DWORD PTR SS:[ESP+14]
00454BF5  |. E8 21F0FFFF    CALL    TENSHOU.00453C1B
				└ここを JMP TSMod.OnTSExeGetGunshiBushouIDPrev と書き換えてやる実際の処理
00454BFA  |. 66:8BD8        MOV     BX, AX
*/
int pTenshouExeJumpFromToOnTenshouExeGetGunshiBushouIDPrev	   =0x454BF5; // 関数はこのアドレスから、OnTenshouExeGetGunshiBushouIDPrevへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeGetGunshiBushouIDPrev =0x453C1B; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeGetGunshiBushouIDPrev  =0x454BFA; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeGetGunshiBushouIDPrev() {
	// スタックにためておく
	__asm {
		// 軍団番号を控えておく
		mov iOriginalGundanIDOfOnGunshiBushouIDPrev, eax

		// TENSHOU.EXEの元々の処理をここで
		call pTenshouExeJumpCallFromToOnTenshouExeGetGunshiBushouIDPrev

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}

	OnTenshouExeGetGunshiBushouIDPrevExecute();

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

		jmp pTenshouExeReturnLblFromOnTenshouExeGetGunshiBushouIDPrev
	}
}



char cmdOnTenshouExeJumpFromGetGunshiBushouIDPrev[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeGetGunshiBushouIDPrev() {

	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeGetGunshiBushouIDPrev;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeGetGunshiBushouIDPrev + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeGetGunshiBushouIDPrev  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromGetGunshiBushouIDPrev+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeGetGunshiBushouIDPrev), cmdOnTenshouExeJumpFromGetGunshiBushouIDPrev, 5, NULL); //5バイトのみ書き込む
}





















//-----------------------------------------------------------------------------------------------------------

/*
 武将の結果が出たのを格納
 */

/*
00454CBC  |> 66:8BC6        MOV     AX, SI
00454CBF  |. 5D             POP     EBP
00454CC0  |. 5F             POP     EDI
00454CC1  |. 5E             POP     ESI
00454CC2  |. 5B             POP     EBX
00454CC3  \. C3             RETN
*/

WORD iOrgBushouIDOfOnGunshiBushouIDAfter = 0xFFFF;
WORD iNewBushouIDOfOnGunshiBushouIDAfter = 0xFFFF;

void OnTenshouExeGetGunshiBushouIDAfterExecute() {

	// 上書き用。原則的には、常に受け取った値そのまま
	iNewBushouIDOfOnGunshiBushouIDAfter = iOrgBushouIDOfOnGunshiBushouIDAfter; 

	int iBushouID = iOrgBushouIDOfOnGunshiBushouIDAfter-1; // AX部分だけが武将番号

	// 武将でなかったらみ。いなかったら0xFFFFにもどす。
	if ( ! (0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) ) {
		iBushouID = 0xFFFF;
	}

	// 指定の軍団はこれ。
	int iGundanID = iOriginalGundanIDOfOnGunshiBushouIDPrev;

	
	// この２つから能力に関わらず結果を上書きする。
	if ( 0 <= iGundanID && iGundanID < GAMEDATASTRUCT_GUNDAN_NUM ) {

		// ロスローリアン系処理
		// 要求されている軍団のリーダー
		int iLeaderBushouID = nb6gundan[iGundanID].leader - 1;

		// ちゃんと居たら…
		if ( 0 <= iLeaderBushouID && iLeaderBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

			// 指定軍団のリーダーはランスロットタルタロス。
			// バールゼフォンを探す
			if ( isLancelotTartare( iLeaderBushouID ) ) {
				for ( int i=0; i<GAMEDATASTRUCT_BUSHOU_NUM; i++) {
					// バールゼフォンが存在する。
					if ( strcmp( nb6bushouname[i].familyname, "" )==0 && strcmp( nb6bushouname[i].fastname, "" )==0 ) {
						// バールゼフォンとタルタロスが同じ軍団だ
						if ( nb6bushouref[i].gundan == nb6bushouref[iLeaderBushouID].gundan ) {
							iNewBushouIDOfOnGunshiBushouIDAfter = i+1; // バールゼフォンのiBushouID→武将番号へ

							break;
						}
					}
				}
			}

			else {
				// 一般的な軍師アルゴリズムを変更する。
				vector<int> GundanBushouList;
				vector<int> GundanBushouParamList;
				for (int iBushouID=0; iBushouID < GAMEDATASTRUCT_BUSHOU_NUM; iBushouID++ ) {

					// 該当の軍団に所属している。かつ、状態が現役である。(大名や軍団長、隠居や浪人・姫等ではない。)
					if ( nb6bushouref[iBushouID].gundan-1 == iGundanID && nb6bushouname[iBushouID].State == 2 ) {

						// 政治＋智謀が200以上、もしくは、政治+戦闘+智謀の合計値が250以上
						if ( (getBushouGov( iBushouID ) + getBushouInt( iBushouID ))  >= 100 ||
							 (getBushouGov( iBushouID ) + getBushouBat( iBushouID ) + getBushouInt( iBushouID ) ) >= 125 ) {
							// 武将追加
							GundanBushouList.push_back( iBushouID );

							// 能力合計値(一種の評価値)
							int sum_param = getBushouGov( iBushouID ) + getBushouBat( iBushouID ) + getBushouInt( iBushouID );

							// 軍団長と対象武将は血縁である。しかし、子供ではない。
							if ( is自分は相手の血縁(iBushouID, iLeaderBushouID ) && !is自分は相手の子(iBushouID, iLeaderBushouID ) ) {
								sum_param += int(sum_param * 0.2); // 20％評価加算
							}

							// 仕官年数を２で割った値の％を足し込む。
							sum_param += int((sum_param * nb6bushouref[iBushouID].work)/200);

							// 宿老だと20％、家老だと15％、部将だと10％ 侍大将だと5％ の加算。
							sum_param += sum_param * (6-nb6bushouname[iBushouID].position) * 5 / 100;

							// 能力追加
							GundanBushouParamList.push_back( sum_param );
						}
					}
				}

				// 該当者０なら何もしない。本来の処理のままでもＯＫ．
				// 該当者が１人でも居れば…
				if ( GundanBushouList.size() > 0 ) {

					// 誰が一番能力が高いのか捜査
					int iGunshiBushou = 0xFFFF;
					int iMaxSumParam = 0;

					for ( int i=0; i<(int)GundanBushouList.size(); i++ ) {
						// 最大の人
						if ( GundanBushouParamList[i] > iMaxSumParam ) {
							iGunshiBushou = GundanBushouList[i];
							iMaxSumParam = GundanBushouParamList[i];
						}
					}

					iNewBushouIDOfOnGunshiBushouIDAfter = iGunshiBushou+1;
				}

			}

			// ScenarioModのカスタム条件で軍師を設定する
			if ( p_snOnCustomCondition ) {															              //┌軍団番号   ┌現軍師武将番号  
				int isNewGunshiBushouID = ((PFNSNONCUSTOMCONDITION) p_snOnCustomCondition)("AS条件::軍師武将番号", iGundanID+1, iOrgBushouIDOfOnGunshiBushouIDAfter, 0, "", "", 0.0, 0.0);
				// 軍師が変更となっている。
				if ( isNewGunshiBushouID != CUSTOM_CONDITION_NOMODIFY ) {
					iNewBushouIDOfOnGunshiBushouIDAfter = isNewGunshiBushouID;
				}
			}

		}

	}
}



/*
00454CBC  |> 66:8BC6        MOV     AX, SI
				└ここを JMP TSMod.OnTSExeGetGunshiBushouIDAfter と書き換えてやる実際の処理
00454CBF  |. 5D             POP     EBP
00454CC0  |. 5F             POP     EDI
00454CC1  |. 5E             POP     ESI
00454CC2  |. 5B             POP     EBX
00454CC3  \. C3             RETN
*/
int pTenshouExeJumpFromToOnTenshouExeGetGunshiBushouIDAfter	   =0x454CBC; // 関数はこのアドレスから、OnTenshouExeGetGunshiBushouIDAfterへとジャンプしてくる。
int pTenshouExeReturnLblFromOnTenshouExeGetGunshiBushouIDAfter  =0x454CC1; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeGetGunshiBushouIDAfter() {
	// スタックにためておく
	__asm {

		// 元々tenshou.exeにあった処理をここで
		MOV     AX, SI

		mov iOrgBushouIDOfOnGunshiBushouIDAfter, AX

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}

	OnTenshouExeGetGunshiBushouIDAfterExecute();

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

		// 上書きすべきなら上書きする。
		MOV     AX, iNewBushouIDOfOnGunshiBushouIDAfter

		// 元々TENSHOU.EXE内にあったものをここで
		POP     EBP
		POP     EDI

		jmp pTenshouExeReturnLblFromOnTenshouExeGetGunshiBushouIDAfter
	}
}



char cmdOnTenshouExeJumpFromGetGunshiBushouIDAfter[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeGetGunshiBushouIDAfter() {

	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeGetGunshiBushouIDAfter;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeGetGunshiBushouIDAfter + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeGetGunshiBushouIDAfter  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromGetGunshiBushouIDAfter+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeGetGunshiBushouIDAfter), cmdOnTenshouExeJumpFromGetGunshiBushouIDAfter, 5, NULL); //5バイトのみ書き込む
}


