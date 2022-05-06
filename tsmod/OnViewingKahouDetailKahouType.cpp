#define _CRT_SECURE_NO_WARNINGS

#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"
#include "ScenarioMod.h"

#include "KahouType.h"


/*
 * 家宝詳細画面を開く際に、「家宝種類の値」が呼び出される時。
 */


// 家宝のIDがEAXでわたってくる。家宝番号。よって配列で使うためには、-1すること。
int iKahouIDOnViewingKahouDetailKahouType = -1;

// 家宝の種類名が入っている文字列のポインタ
int iKahouTypePointerOnViewingKahouDetailKahouType = 0;

// この関数は一瞬で２回呼び出される。
void OnTenshouExeViewingKahouDetailKahouTypeExecute() {
	// esiの値から、家宝が入っている場所を突き止め、その値をiKahouIDOnViewingKahouDetailKahouTypeに改めて入れる。
	// MOV     EBP, DWORD PTR DS:[ESI+70] と同じことをやろうというのだ。
	char *p = (char *)iKahouIDOnViewingKahouDetailKahouType + 0x70;
	iKahouIDOnViewingKahouDetailKahouType = *p;
	iKahouIDOnViewingKahouDetailKahouType--; // 番号→ＩＤへ

	if ( 0 <= iKahouIDOnViewingKahouDetailKahouType && iKahouIDOnViewingKahouDetailKahouType < GAMEDATASTRUCT_KAHOU_NUM ) {

		char *pszCustomKahouType = GetOverWriteKahouTypeMeiPointer(iKahouIDOnViewingKahouDetailKahouType);

		// 家宝種類名の特殊な書き換えが返って切れいれば…
		if ( pszCustomKahouType ) {
			// 配列に家宝名を入れる。
			strcpy( nb6kahou_custom_type_string[iKahouIDOnViewingKahouDetailKahouType].szKahouTypeString, pszCustomKahouType);

			// 配列に入れた場所を、表示すべきポインタとして渡す
			iKahouTypePointerOnViewingKahouDetailKahouType = (int)nb6kahou_custom_type_string[iKahouIDOnViewingKahouDetailKahouType].szKahouTypeString;
		}

		// ScenarioModで独特の家宝種類名変化があるかどうか。
		if ( p_snOnViewingKahouTypeName ) {
			// 安全を見て大き目に
			char szTempName[20] = "";
			bool isReWrite = ((PFNSNONVIEWINGKAHOUTYPENAME)p_snOnViewingKahouTypeName)( szTempName, iKahouIDOnViewingKahouDetailKahouType+1 );
			if ( isReWrite ) {
				szTempName[10] = NULL; // 全角５文字まで 

				// 配列に家宝名を入れる。
				strcpy( nb6kahou_custom_type_string[iKahouIDOnViewingKahouDetailKahouType].szKahouTypeString, szTempName);

				// 配列に入れた場所を、表示すべきポインタとして渡す
				iKahouTypePointerOnViewingKahouDetailKahouType = (int)nb6kahou_custom_type_string[iKahouIDOnViewingKahouDetailKahouType].szKahouTypeString;
			}
		}
	}
}


/*
0049493D  |. FF76 70        PUSH    DWORD PTR DS:[ESI+70]
00494940  |. E8 3DB1FBFF    CALL    TENSHOU.0044FA82
				└ここを JMP TSMod.OnTSExeViewingKahouDetailKahouType と書き換えてやる実際の処理
00494945  |. 83C4 04        ADD     ESP, 4
*/
int pTenshouExeJumpFromToOnTenshouExeViewingKahouDetailKahouType	 =0x494940; // 関数はこのアドレスから、OnTenshouExeViewingKahouDetailKahouTypeへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeViewingKahouDetailKahouType =0x44FA82; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeViewingKahouDetailKahouType	 =0x494945; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeViewingKahouDetailKahouType() {
	// スタックにためておく
	__asm {

		// 。
		mov iKahouIDOnViewingKahouDetailKahouType, esi // 家宝ＩＤ関連アドレスをコピー

		// 元々TENSHOU.EXE内にあったものをここで
		call pTenshouExeJumpCallFromToOnTenshouExeViewingKahouDetailKahouType

		// この結果得られるEAXが、家宝のタイプの文字列ポインタそのもの。
		mov iKahouTypePointerOnViewingKahouDetailKahouType, eax;

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
	OnTenshouExeViewingKahouDetailKahouTypeExecute();

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

		// ポインタを上書きする
		mov eax, iKahouTypePointerOnViewingKahouDetailKahouType

		jmp pTenshouExeReturnLblFromOnTenshouExeViewingKahouDetailKahouType
	}
}


char cmdOnTenshouExeJumpFromViewingKahouDetailKahouType[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeViewingKahouDetailKahouType() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeViewingKahouDetailKahouType;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeViewingKahouDetailKahouType + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeViewingKahouDetailKahouType  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromViewingKahouDetailKahouType+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeViewingKahouDetailKahouType), cmdOnTenshouExeJumpFromViewingKahouDetailKahouType, 5, NULL); //5バイトのみ書き込む
}




