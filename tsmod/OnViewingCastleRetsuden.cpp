#define _CRT_SECURE_NO_WARNINGS

#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "ScenarioMod.h"


short iCastleIDOfCastleMeijoMonogatari = -1;

#define RETSUDEN_BUF_ADDRESS		0x4D97C0

static char szBufMeijoMonogatariTitle [50*1] = ""; // 実際には全角１８字まで多めにとっておく。
static char szBufMeijoMonogatariDetail[50*4] = ""; // 実際には全角１８文字×４行まで。多めにとっておく。

void OverWriteCastleMeijoMonogatariTitle(int iCastleID) {
	// 両方クリアしてから…
	strcpy(szBufMeijoMonogatariTitle, "");
	strcpy(szBufMeijoMonogatariDetail, "");
	if ( p_snOnViewingMeijoMonogatari ) {

		p_snOnViewingMeijoMonogatari(iCastleID, szBufMeijoMonogatariTitle, szBufMeijoMonogatariDetail);

		// 有効な文字が入っていたら、コピー
		if (strlen(szBufMeijoMonogatariTitle) ) {
			char *szTitle = (char *)RETSUDEN_BUF_ADDRESS;
			strcpy(szTitle, szBufMeijoMonogatariTitle);

			// コピーし終わったのでクリアしておく
			strcpy(szBufMeijoMonogatariTitle, "");
		}
	}
}
void OnTenshouExeCastleMeijoMonogatariTitleExecute() {
	int iCastleID = iCastleIDOfCastleMeijoMonogatari-1;
	if ( 0 <= iCastleID && iCastleID < GAMEDATASTRUCT_CASTLE_NUM ) {
		OverWriteCastleMeijoMonogatariTitle(iCastleID);
	}
}

/*
00476F06  |> 66:8B86 C00100>MOV     AX, WORD PTR DS:[ESI+1C0]


00476F69  |. 50             PUSH    EAX                              ; /Arg8 => 00000000
00476F6A  |. 50             PUSH    EAX                              ; |Arg7 => 00000000
00476F6B  |. 50             PUSH    EAX                              ; |Arg6 => 00000000
00476F6C  |. 50             PUSH    EAX                              ; |Arg5 => 00000000
00476F6D  |. 50             PUSH    EAX                              ; |Arg4 => 00000000
00476F6E  |. 50             PUSH    EAX                              ; |Arg3 => 00000000
00476F6F  |. 50             PUSH    EAX                              ; |Arg2 => 00000000
00476F70  |. 53             PUSH    EBX                              ; |Arg1
00476F71  |. E8 EA50FFFF    CALL    TENSHOU.0046C060                 ; \TENSHOU.0046C060
				└ここを JMP TSMod.OnTSExeCastleMeijoMonogatariTitle と書き換えてやる実際の処理
00476F76  |. 83C4 20        ADD     ESP, 20
*/
int pTenshouExeJumpFromToOnTenshouExeCastleMeijoMonogatariTitle	  =0x476F71; // 関数はこのアドレスから、OnTenshouExeCastleMeijoMonogatariTitleへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeCastleMeijoMonogatariTitle =0x46C060; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeCastleMeijoMonogatariTitle  =0x476F76; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w

int iEAXOfCastleMeijoMonogatariTitle = -1; // 一時的なEAX保存先

// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeCastleMeijoMonogatariTitle() {
	// スタックにためておく
	__asm {
		mov iEAXOfCastleMeijoMonogatariTitle, eax

		// 城の番号が入っている
		mov AX, WORD PTR DS:[ESI+0x1C0] // 城の番号

		mov iCastleIDOfCastleMeijoMonogatari, AX // 保存しておく

		mov eax, iEAXOfCastleMeijoMonogatariTitle // EAXの値を戻す

		// 元々の処理をここに
		call pTenshouExeJumpCallFromToOnTenshouExeCastleMeijoMonogatariTitle

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
	OnTenshouExeCastleMeijoMonogatariTitleExecute();

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

		jmp pTenshouExeReturnLblFromOnTenshouExeCastleMeijoMonogatariTitle
	}
}

char cmdOnTenshouExeJumpFromCastleMeijoMonogatariTitle[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeCastleMeijoMonogatariTitle() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeCastleMeijoMonogatariTitle;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeCastleMeijoMonogatariTitle + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeCastleMeijoMonogatariTitle  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromCastleMeijoMonogatariTitle+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeCastleMeijoMonogatariTitle), cmdOnTenshouExeJumpFromCastleMeijoMonogatariTitle, 5, NULL); //5バイトのみ書き込む
}











void OnTenshouExeCastleMeijoMonogatariDetailExecute() {

	// 有効な文字が入っていたら、コピー
	if (strlen(szBufMeijoMonogatariDetail) ) {
		char *szDetail = (char *)RETSUDEN_BUF_ADDRESS;
		strcpy(szDetail, szBufMeijoMonogatariDetail);
		
		// コピーし終わったのでクリアしておく
		strcpy(szBufMeijoMonogatariDetail, "");
	}
}



/*
00476FC1  |. 50             PUSH    EAX                              ; /Arg8 => 00000000
00476FC2  |. 50             PUSH    EAX                              ; |Arg7 => 00000000
00476FC3  |. 50             PUSH    EAX                              ; |Arg6 => 00000000
00476FC4  |. 43             INC     EBX                              ; |
00476FC5  |. 50             PUSH    EAX                              ; |Arg5 => 00000000
00476FC6  |. 50             PUSH    EAX                              ; |Arg4 => 00000000
00476FC7  |. 50             PUSH    EAX                              ; |Arg3 => 00000000
00476FC8  |. 50             PUSH    EAX                              ; |Arg2 => 00000000
00476FC9  |. 53             PUSH    EBX                              ; |Arg1
00476FCA  |. E8 9150FFFF    CALL    TENSHOU.0046C060                 ; \TENSHOU.0046C060
				└ここを JMP TSMod.OnTSExeCastleMeijoMonogatariDetail と書き換えてやる実際の処理
00476FCF  |. 83C4 20        ADD     ESP, 20
*/
int pTenshouExeJumpFromToOnTenshouExeCastleMeijoMonogatariDetail	   = 0x476FCA; // 関数はこのアドレスから、OnTenshouExeCastleMeijoMonogatariDetailへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeCastleMeijoMonogatariDetail = 0x46C060; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeCastleMeijoMonogatariDetail  = 0x476FCF; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeCastleMeijoMonogatariDetail() {
	// スタックにためておく
	__asm {

		// 元々の処理をここに
		call pTenshouExeJumpCallFromToOnTenshouExeCastleMeijoMonogatariDetail

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
	OnTenshouExeCastleMeijoMonogatariDetailExecute();

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

		jmp pTenshouExeReturnLblFromOnTenshouExeCastleMeijoMonogatariDetail
	}
}

char cmdOnTenshouExeJumpFromCastleMeijoMonogatariDetail[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeCastleMeijoMonogatariDetail() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeCastleMeijoMonogatariDetail;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeCastleMeijoMonogatariDetail + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeCastleMeijoMonogatariDetail  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromCastleMeijoMonogatariDetail+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeCastleMeijoMonogatariDetail), cmdOnTenshouExeJumpFromCastleMeijoMonogatariDetail, 5, NULL); //5バイトのみ書き込む
}




