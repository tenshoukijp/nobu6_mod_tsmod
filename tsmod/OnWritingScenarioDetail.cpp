#define _CRT_SECURE_NO_WARNINGS

#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "PluginMod.h"
#include "ScenarioMod.h"

#include <map>

using namespace std;



/*
00422D96  |. 47             INC     EDI
00422D97  |. 53             PUSH    EBX                                          ; /Arg8
00422D98  |. 53             PUSH    EBX                                          ; |Arg7
00422D99  |. 53             PUSH    EBX                                          ; |Arg6
00422D9A  |. 53             PUSH    EBX                                          ; |Arg5
00422D9B  |. 53             PUSH    EBX                                          ; |Arg4
00422D9C  |. 53             PUSH    EBX                                          ; |Arg3
00422D9D  |. 53             PUSH    EBX                                          ; |Arg2
00422D9E  |. 50             PUSH    EAX                                          ; |Arg1
00422D9F  |. E8 BC920400    CALL    TENSHOU.0046C060                             ; \TENSHOU.0046C060  上部
00422DA4  |. 83C4 20        ADD     ESP, 20
00422DA7  |. 8BCE           MOV     ECX, ESI
00422DA9  |. 50             PUSH    EAX
00422DAA  |. E8 9FB70800    CALL    TENSHOU.004AE54E
00422DAF  |. 68 90000000    PUSH    90
00422DB4  |. 8BCE           MOV     ECX, ESI
00422DB6  |. 6A 10          PUSH    10
00422DB8  |. E8 CBE6FDFF    CALL    TENSHOU.00401488
00422DBD  |. 55             PUSH    EBP
00422DBE  |. 53             PUSH    EBX                                          ; /Arg8
00422DBF  |. 53             PUSH    EBX                                          ; |Arg7
00422DC0  |. 53             PUSH    EBX                                          ; |Arg6
00422DC1  |. 53             PUSH    EBX                                          ; |Arg5
00422DC2  |. 53             PUSH    EBX                                          ; |Arg4
00422DC3  |. 53             PUSH    EBX                                          ; |Arg3
00422DC4  |. 53             PUSH    EBX                                          ; |Arg2
00422DC5  |. 57             PUSH    EDI                                          ; |Arg1
00422DC6  |. E8 95920400    CALL    TENSHOU.0046C060                             ; \TENSHOU.0046C060　中部
00422DCB  |. 83C4 20        ADD     ESP, 20
00422DCE  |. 8BCE           MOV     ECX, ESI
00422DD0  |. 50             PUSH    EAX
00422DD1  |. E8 78B70800    CALL    TENSHOU.004AE54E
00422DD6  |. 68 F4000000    PUSH    0F4
00422DDB  |. 8BCE           MOV     ECX, ESI
00422DDD  |. 6A 10          PUSH    10
00422DDF  |. E8 A4E6FDFF    CALL    TENSHOU.00401488
00422DE4  |. 55             PUSH    EBP
00422DE5  |. 53             PUSH    EBX                                          ; /Arg8
00422DE6  |. 53             PUSH    EBX                                          ; |Arg7
00422DE7  |. 47             INC     EDI                                          ; |
00422DE8  |. 53             PUSH    EBX                                          ; |Arg6
00422DE9  |. 53             PUSH    EBX                                          ; |Arg5
00422DEA  |. 53             PUSH    EBX                                          ; |Arg4
00422DEB  |. 53             PUSH    EBX                                          ; |Arg3
00422DEC  |. 53             PUSH    EBX                                          ; |Arg2
00422DED  |. 57             PUSH    EDI                                          ; |Arg1
00422DEE  |. E8 6D920400    CALL    TENSHOU.0046C060                             ; \TENSHOU.0046C060　下部
00422DF3  |. 83C4 20        ADD     ESP, 20
*/


//---------------------------①
/*
 * シナリオ詳細分の「上部」が書かれる時
 */

/*
00422D96  |. 47             INC     EDI
00422D97  |. 53             PUSH    EBX                                          ; /Arg8
00422D98  |. 53             PUSH    EBX                                          ; |Arg7
00422D99  |. 53             PUSH    EBX                                          ; |Arg6
00422D9A  |. 53             PUSH    EBX                                          ; |Arg5
00422D9B  |. 53             PUSH    EBX                                          ; |Arg4
00422D9C  |. 53             PUSH    EBX                                          ; |Arg3
00422D9D  |. 53             PUSH    EBX                                          ; |Arg2
00422D9E  |. 50             PUSH    EAX                                          ; |Arg1
00422D9F  |. E8 BC920400    CALL    TENSHOU.0046C060                             ; \TENSHOU.0046C060
00422DA4  |. 83C4 20        ADD     ESP, 20
*/

// 関数とchar **のセットを覚えておく
map<FARPROC, char **> psnGetProcAdressAndPointerMap;
void ReadFromTaretGeneralModDll(int nSubNo) {

	int nSN = getCurScenarioLocation();

	// PluginMod系の０個以上（複数可能）なＤＬＬのメインゲーム開始時のメソッドを呼び出す。
	// 実行対象となっているDLLを順に捜査して…
	for ( vector<HMODULE>::iterator it = listOfTargetDLL.begin(); it != listOfTargetDLL.end(); it++ ) {

		// 対象のDLLにMainGameStart関数が存在するかをチェック。
		PFNTARGETMODWRITINGSCENARIODETAIL p_OnWritingScenarioDetail = (PFNTARGETMODWRITINGSCENARIODETAIL)GetProcAddress(*it, "OnWritingScenarioDetail");

		if ( !p_OnWritingScenarioDetail ) {
			continue;
		}

		if ( 0 <= nSN && nSN < SCENARIODATASTRUCT_NUM ) {
	
			char **psn = NULL;

			// データベースに未登録であれば…
			if ( psnGetProcAdressAndPointerMap.find((FARPROC)p_OnWritingScenarioDetail) == psnGetProcAdressAndPointerMap.end() ) {
				// 受け取り用に呼び出して
				psn = p_OnWritingScenarioDetail( nSN+1 ); // 1～6に直しておいてあげる。
				// データベースに登録
				psnGetProcAdressAndPointerMap[(FARPROC)p_OnWritingScenarioDetail] = psn;

			// データベースに登録されていれば、流用
			} else {
				psn = psnGetProcAdressAndPointerMap[(FARPROC)p_OnWritingScenarioDetail];
			}

			// TargetのDLLが何か値を渡してきた。
			if ( psn ) {

				if ( strlen(psn[nSubNo]) > 2 ) {
					// 書き換え
					WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(CPM.getMemoryAddress(LOC4)), psn[nSubNo], strlen(psn[nSubNo])+1, NULL);

				}
			}
		}
	}

	// ScenarioModの方
	if ( p_snOnWritingScenarioDetail ) {

		if ( 0 <= nSN && nSN < SCENARIODATASTRUCT_NUM ) {

			char **psn = NULL;

			// データベースに未登録であれば…
			if ( psnGetProcAdressAndPointerMap.find((FARPROC)p_snOnWritingScenarioDetail) == psnGetProcAdressAndPointerMap.end() ) {
				// 受け取り用に呼び出して
				psn = p_snOnWritingScenarioDetail( nSN+1 ); // 1～6に直しておいてあげる。
				// データベースに登録
				psnGetProcAdressAndPointerMap[(FARPROC)p_snOnWritingScenarioDetail] = psn;

			// データベースに登録されていれば、流用
			} else {
				psn = psnGetProcAdressAndPointerMap[(FARPROC)p_snOnWritingScenarioDetail];
			}


			// TargetのDLLが何か値を渡してきた。
			if ( psn ) {

				if ( strlen(psn[nSubNo]) > 2 ) {
					// 書き換え
					WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(CPM.getMemoryAddress(LOC4)), psn[nSubNo], strlen(psn[nSubNo])+1, NULL);

				}
			}
		}
	}

}



void OnTenshouExeWritingScenarioDetail1stExecute() {
	ReadFromTaretGeneralModDll(0); // まずはPluginMod系の一般Modから読む。
}


/*
00422D9E  |. 50             PUSH    EAX                                          ; |Arg1
00422D9F  |. E8 BC920400    CALL    TENSHOU.0046C060                             ; \TENSHOU.0046C060
				└ここを JMP TSMod.OnTSExeWritingScenarioDetail1st と書き換えてやる実際の処理
00422DA4  |. 83C4 20        ADD     ESP, 20
 */
int pTenshouExeJumpFromToOnTenshouExeWritingScenarioDetail1st    =0x422D9F; // 関数はこのアドレスから、OnTenshouExeWritingScenarioDetail1stへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeWritingScenarioDetail1st=0x46C060; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeWritingScenarioDetail1st =0x422DA4; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeWritingScenarioDetail1st() {
	// スタックにためておく
	__asm {
		// 元々TENSHOU.EXE内にあったものをここで
		call pTenshouExeJumpCallFromToOnTenshouExeWritingScenarioDetail1st

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
	OnTenshouExeWritingScenarioDetail1stExecute();

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

		jmp pTenshouExeReturnLblFromOnTenshouExeWritingScenarioDetail1st
	}
}

char cmdOnTenshouExeJumpFromWritingScenarioDetail1st[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeWritingScenarioDetail1st() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeWritingScenarioDetail1st;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeWritingScenarioDetail1st + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeWritingScenarioDetail1st  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromWritingScenarioDetail1st+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeWritingScenarioDetail1st), cmdOnTenshouExeJumpFromWritingScenarioDetail1st, 5, NULL); //5バイトのみ書き込む
}






//---------------------------②
/*
 * シナリオ詳細分の「中部」が書かれる時
 */

/*
00422DBD  |. 55             PUSH    EBP
00422DBE  |. 53             PUSH    EBX                                          ; /Arg8
00422DBF  |. 53             PUSH    EBX                                          ; |Arg7
00422DC0  |. 53             PUSH    EBX                                          ; |Arg6
00422DC1  |. 53             PUSH    EBX                                          ; |Arg5
00422DC2  |. 53             PUSH    EBX                                          ; |Arg4
00422DC3  |. 53             PUSH    EBX                                          ; |Arg3
00422DC4  |. 53             PUSH    EBX                                          ; |Arg2
00422DC5  |. 57             PUSH    EDI                                          ; |Arg1
00422DC6  |. E8 95920400    CALL    TENSHOU.0046C060                             ; \TENSHOU.0046C060　中部
00422DCB  |. 83C4 20        ADD     ESP, 20
*/

void OnTenshouExeWritingScenarioDetail2ndExecute() {
	ReadFromTaretGeneralModDll(1); // まずはPluginMod系の一般Modから読む。
}


/*
00422DC5  |. 57             PUSH    EDI                                          ; |Arg1
00422DC6  |. E8 95920400    CALL    TENSHOU.0046C060                             ; \TENSHOU.0046C060　中部
				└ここを JMP TSMod.OnTSExeWritingScenarioDetail2nd と書き換えてやる実際の処理
00422DCB  |. 83C4 20        ADD     ESP, 20
 */
int pTenshouExeJumpFromToOnTenshouExeWritingScenarioDetail2nd    =0x422DC6; // 関数はこのアドレスから、OnTenshouExeWritingScenarioDetail2ndへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeWritingScenarioDetail2nd=0x46C060; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeWritingScenarioDetail2nd =0x422DCB; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeWritingScenarioDetail2nd() {
	// スタックにためておく
	__asm {
		// 元々TENSHOU.EXE内にあったものをここで
		call pTenshouExeJumpCallFromToOnTenshouExeWritingScenarioDetail2nd

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
	OnTenshouExeWritingScenarioDetail2ndExecute();

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

		jmp pTenshouExeReturnLblFromOnTenshouExeWritingScenarioDetail2nd
	}
}

char cmdOnTenshouExeJumpFromWritingScenarioDetail2nd[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeWritingScenarioDetail2nd() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeWritingScenarioDetail2nd;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeWritingScenarioDetail2nd + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeWritingScenarioDetail2nd  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromWritingScenarioDetail2nd+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeWritingScenarioDetail2nd), cmdOnTenshouExeJumpFromWritingScenarioDetail2nd, 5, NULL); //5バイトのみ書き込む
}






//---------------------------③
/*
 * シナリオ詳細分の「下部」が書かれる時
 */

/*
00422DE5  |. 53             PUSH    EBX                                          ; /Arg8
00422DE6  |. 53             PUSH    EBX                                          ; |Arg7
00422DE7  |. 47             INC     EDI                                          ; |
00422DE8  |. 53             PUSH    EBX                                          ; |Arg6
00422DE9  |. 53             PUSH    EBX                                          ; |Arg5
00422DEA  |. 53             PUSH    EBX                                          ; |Arg4
00422DEB  |. 53             PUSH    EBX                                          ; |Arg3
00422DEC  |. 53             PUSH    EBX                                          ; |Arg2
00422DED  |. 57             PUSH    EDI                                          ; |Arg1
00422DEE  |. E8 6D920400    CALL    TENSHOU.0046C060                             ; \TENSHOU.0046C060　下部
00422DF3  |. 83C4 20        ADD     ESP, 20
*/

void OnTenshouExeWritingScenarioDetail3rdExecute() {
	ReadFromTaretGeneralModDll(2); // まずはPluginMod系の一般Modから読む。
}


/*
00422DED  |. 57             PUSH    EDI                                          ; |Arg1
00422DEE  |. E8 6D920400    CALL    TENSHOU.0046C060                             ; \TENSHOU.0046C060　下部
				└ここを JMP TSMod.OnTSExeWritingScenarioDetail3rd と書き換えてやる実際の処理
00422DF3  |. 83C4 20        ADD     ESP, 20
 */
int pTenshouExeJumpFromToOnTenshouExeWritingScenarioDetail3rd    =0x422DEE; // 関数はこのアドレスから、OnTenshouExeWritingScenarioDetail3rdへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeWritingScenarioDetail3rd=0x46C060; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeWritingScenarioDetail3rd =0x422DF3; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeWritingScenarioDetail3rd() {
	// スタックにためておく
	__asm {
		// 元々TENSHOU.EXE内にあったものをここで
		call pTenshouExeJumpCallFromToOnTenshouExeWritingScenarioDetail3rd

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
	OnTenshouExeWritingScenarioDetail3rdExecute();

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

		jmp pTenshouExeReturnLblFromOnTenshouExeWritingScenarioDetail3rd
	}
}

char cmdOnTenshouExeJumpFromWritingScenarioDetail3rd[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeWritingScenarioDetail3rd() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeWritingScenarioDetail3rd;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeWritingScenarioDetail3rd + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeWritingScenarioDetail3rd  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromWritingScenarioDetail3rd+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeWritingScenarioDetail3rd), cmdOnTenshouExeJumpFromWritingScenarioDetail3rd, 5, NULL); //5バイトのみ書き込む
}

