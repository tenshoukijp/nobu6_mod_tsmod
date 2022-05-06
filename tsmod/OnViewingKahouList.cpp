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
 * 家宝リストを見た時
 */

// 家宝番号
int iKahouIDOnViewingKahouList = -1;

// 家宝の種類名が入っている文字列のポインタ
int iKahouTypePointerOnViewingKahouList = 0;

void OnTenshouExeViewingKahouListExecute() {

	iKahouIDOnViewingKahouList = iKahouIDOnViewingKahouList - 1;

	iKahouTypePointerOnViewingKahouList = NULL; // 毎度NULLで初期化

	// 家宝の範囲かどうかをチェックして…
	if ( 0 <= iKahouIDOnViewingKahouList && iKahouIDOnViewingKahouList < GAMEDATASTRUCT_KAHOU_NUM ) {

		char *pszCustomKahouType = GetOverWriteKahouTypeMeiPointer(iKahouIDOnViewingKahouList);

		// 家宝種類名の特殊な書き換えが返って切れいれば…
		if ( pszCustomKahouType ) {
			// 配列に家宝名を入れる。
			strcpy( nb6kahou_custom_type_string[iKahouIDOnViewingKahouList].szKahouTypeString, pszCustomKahouType);

			// 配列に入れた場所を、表示すべきポインタとして渡す
			iKahouTypePointerOnViewingKahouList = (int)nb6kahou_custom_type_string[iKahouIDOnViewingKahouList].szKahouTypeString;
		}

		// ScenarioModで独特の家宝種類名変化があるかどうか。
		if ( p_snOnViewingKahouTypeName ) {
			// 安全を見て大き目に
			char szTempName[20] = "";
			bool isReWrite = ((PFNSNONVIEWINGKAHOUTYPENAME)p_snOnViewingKahouTypeName)( szTempName, iKahouIDOnViewingKahouList+1 );
			if ( isReWrite ) {
				szTempName[10] = NULL; // 全角５文字まで 

				// 配列に家宝名を入れる。
				strcpy( nb6kahou_custom_type_string[iKahouIDOnViewingKahouList].szKahouTypeString, szTempName);

				// 配列に入れた場所を、表示すべきポインタとして渡す
				iKahouTypePointerOnViewingKahouList = (int)nb6kahou_custom_type_string[iKahouIDOnViewingKahouList].szKahouTypeString;
			}
		}

	}
}


/*
004839EF   . FF7424 04      PUSH    DWORD PTR SS:[ESP+4]
004839F3   . E8 79C0FCFF    CALL    TENSHOU.0044FA71
				└ここを JMP TSMod.OnTSExeViewingKahouList と書き換えてやる実際の処理
004839F8   . 83C4 04        ADD     ESP, 4
004839FB   . 8B0485 20144D0>MOV     EAX, DWORD PTR DS:[EAX*4+4D1420] ;  TENSHOU.004CFE64
00483A02   . C3             RETN
*/
int pTenshouExeJumpFromToOnTenshouExeViewingKahouList		=0x4839F3; // 関数はこのアドレスから、OnTenshouExeViewingKahouListへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeViewingKahouList   =0x44FA71; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeViewingKahouList	=0x483A02; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
int pTenshouExeReturnLblFromOnTenshouExeViewingKahouListNoCustomCase	=0x4839F8; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeViewingKahouList() {
	// スタックにためておく
	__asm {

		// 見ようとしている家宝番号がECXに入っている。
		mov iKahouIDOnViewingKahouList, ecx		

		// 元々TENSHOU.EXE内にあったものをここで
		call pTenshouExeJumpCallFromToOnTenshouExeViewingKahouList

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
	OnTenshouExeViewingKahouListExecute();

	// カスタム文字列があった場合
	if ( iKahouTypePointerOnViewingKahouList ) {

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

			// 元々の処理をここで
			add esp, 4

			// 
			MOV EAX, DWORD PTR DS:[iKahouTypePointerOnViewingKahouList]

			jmp pTenshouExeReturnLblFromOnTenshouExeViewingKahouList
		}

	// カスタム文字列が無い場合
	} else {
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

			jmp pTenshouExeReturnLblFromOnTenshouExeViewingKahouListNoCustomCase
		}
	}
}


char cmdOnTenshouExeJumpFromViewingKahouList[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeViewingKahouList() {

	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeViewingKahouList;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeViewingKahouList + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeViewingKahouList  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromViewingKahouList+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeViewingKahouList), cmdOnTenshouExeJumpFromViewingKahouList, 5, NULL); //5バイトのみ書き込む
}




