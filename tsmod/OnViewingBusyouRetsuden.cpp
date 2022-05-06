#define _CRT_SECURE_NO_WARNINGS

#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "ScenarioMod.h"






int iBushouIDOfBushouRetsudenTitlePrev = -1;

void OnTenshouExeBushouRetsudenTitlePrevExecute() {
}

/*
00493E82  |. 6A 01          PUSH    1
00493E84  |. E8 9486FDFF    CALL    TENSHOU.0046C51D
				└ここを JMP TSMod.OnTSExeBushouRetsudenTitlePrev と書き換えてやる実際の処理
00493E89  |. 83C4 04        ADD     ESP, 4
*/
int pTenshouExeJumpFromToOnTenshouExeBushouRetsudenTitlePrev	 =0x493E84; // 関数はこのアドレスから、OnTenshouExeBushouRetsudenTitlePrevへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeBushouRetsudenTitlePrev =0x46C51D; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeBushouRetsudenTitlePrev	 =0x493E89; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeBushouRetsudenTitlePrev() {
	// スタックにためておく
	__asm {
		// 武将番号を控えておく
		mov iBushouIDOfBushouRetsudenTitlePrev, EAX

		// 元々あった処理をこちらに
		call pTenshouExeJumpCallFromToOnTenshouExeBushouRetsudenTitlePrev

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
	OnTenshouExeBushouRetsudenTitlePrevExecute();

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

		jmp pTenshouExeReturnLblFromOnTenshouExeBushouRetsudenTitlePrev
	}
}

char cmdOnTenshouExeJumpFromBushouRetsudenTitlePrev[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeBushouRetsudenTitlePrev() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeBushouRetsudenTitlePrev;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeBushouRetsudenTitlePrev + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeBushouRetsudenTitlePrev  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromBushouRetsudenTitlePrev+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeBushouRetsudenTitlePrev), cmdOnTenshouExeJumpFromBushouRetsudenTitlePrev, 5, NULL); //5バイトのみ書き込む
}














int iFaceIDOfBushouRetsudenTitle = -1; // 要求された顔番号

#define RETSUDEN_BUF_ADDRESS		0x4D97C0

static char szBufRestudenTitle [50*1] = ""; // 実際には全角１８字まで多めにとっておく。
static char szBufRestudenDetail[50*4] = ""; // 実際には全角１８文字×４行まで。多めにとっておく。

void OverWriteBushouRetsudenTitle(int iBushouID, int iFaceID) {
	// 両方クリアしてから…
	strcpy(szBufRestudenTitle, "");
	strcpy(szBufRestudenDetail, "");

	if ( p_snOnViewingRetsuden ) {

		p_snOnViewingRetsuden(iBushouID, iFaceID, szBufRestudenTitle, szBufRestudenDetail);

		// 有効な文字が入っていたら、コピー
		if (strlen(szBufRestudenTitle) ) {
			char *szTitle = (char *)RETSUDEN_BUF_ADDRESS;
			strcpy(szTitle, szBufRestudenTitle);

			// コピーし終わったのでクリアしておく
			strcpy(szBufRestudenTitle, "");
		}
	}
}
void OnTenshouExeBushouRetsudenTitleExecute() {
	int iBushouID = iBushouIDOfBushouRetsudenTitlePrev-1;
	int iFaceID = iFaceIDOfBushouRetsudenTitle;
	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
		OverWriteBushouRetsudenTitle(iBushouID, iFaceID);
	}
}

/*
00493EE8  |. 50             PUSH    EAX                              ; /Arg8 => 00000000
00493EE9  |. 50             PUSH    EAX                              ; |Arg7 => 00000000
00493EEA  |. 50             PUSH    EAX                              ; |Arg6 => 00000000
00493EEB  |. 50             PUSH    EAX                              ; |Arg5 => 00000000
00493EEC  |. 50             PUSH    EAX                              ; |Arg4 => 00000000
00493EED  |. 50             PUSH    EAX                              ; |Arg3 => 00000000
00493EEE  |. 50             PUSH    EAX                              ; |Arg2 => 00000000
00493EEF  |. 57             PUSH    EDI                              ; |Arg1
00493EF0  |. E8 6B81FDFF    CALL    TENSHOU.0046C060                 ; \TENSHOU.0046C060
				└ここを JMP TSMod.OnTSExeBushouRetsudenTitle と書き換えてやる実際の処理
00493EF5  |. 83C4 20        ADD     ESP, 20
*/
int pTenshouExeJumpFromToOnTenshouExeBushouRetsudenTitle	 =0x493EF0; // 関数はこのアドレスから、OnTenshouExeBushouRetsudenTitleへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeBushouRetsudenTitle =0x46C060; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeBushouRetsudenTitle	 =0x493EF5; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeBushouRetsudenTitle() {
	// スタックにためておく
	__asm {

		mov iFaceIDOfBushouRetsudenTitle, ESI // 顔番号

		// 元々の処理をここに
		call pTenshouExeJumpCallFromToOnTenshouExeBushouRetsudenTitle

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
	OnTenshouExeBushouRetsudenTitleExecute();

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

		jmp pTenshouExeReturnLblFromOnTenshouExeBushouRetsudenTitle
	}
}

char cmdOnTenshouExeJumpFromBushouRetsudenTitle[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeBushouRetsudenTitle() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeBushouRetsudenTitle;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeBushouRetsudenTitle + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeBushouRetsudenTitle  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromBushouRetsudenTitle+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeBushouRetsudenTitle), cmdOnTenshouExeJumpFromBushouRetsudenTitle, 5, NULL); //5バイトのみ書き込む
}











void OnTenshouExeBushouRetsudenDetailExecute() {

	// 有効な文字が入っていたら、コピー
	if (strlen(szBufRestudenDetail) ) {
		char *szDetail = (char *)RETSUDEN_BUF_ADDRESS;
		strcpy(szDetail, szBufRestudenDetail);
		
		// コピーし終わったのでクリアしておく
		strcpy(szBufRestudenDetail, "");
	}
}



/*
00493F3D  |. 55             PUSH    EBP                              ; |Arg7
00493F3E  |. 55             PUSH    EBP                              ; |Arg6
00493F3F  |. 47             INC     EDI                              ; |
00493F40  |. 55             PUSH    EBP                              ; |Arg5
00493F41  |. 55             PUSH    EBP                              ; |Arg4
00493F42  |. 55             PUSH    EBP                              ; |Arg3
00493F43  |. 55             PUSH    EBP                              ; |Arg2
00493F44  |. 57             PUSH    EDI                              ; |Arg1
00493F45  |. E8 1681FDFF    CALL    TENSHOU.0046C060                 ; \TENSHOU.0046C060
				└ここを JMP TSMod.OnTSExeBushouRetsudenDetail と書き換えてやる実際の処理
00493F4A  |. 83C4 20        ADD     ESP, 20
*/
int pTenshouExeJumpFromToOnTenshouExeBushouRetsudenDetail	  = 0x493F45; // 関数はこのアドレスから、OnTenshouExeBushouRetsudenDetailへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeBushouRetsudenDetail = 0x46C060; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeBushouRetsudenDetail  = 0x493F4A; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeBushouRetsudenDetail() {
	// スタックにためておく
	__asm {

		// 元々の処理をここに
		call pTenshouExeJumpCallFromToOnTenshouExeBushouRetsudenDetail

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
	OnTenshouExeBushouRetsudenDetailExecute();

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

		jmp pTenshouExeReturnLblFromOnTenshouExeBushouRetsudenDetail
	}
}

char cmdOnTenshouExeJumpFromBushouRetsudenDetail[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeBushouRetsudenDetail() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeBushouRetsudenDetail;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeBushouRetsudenDetail + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeBushouRetsudenDetail  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromBushouRetsudenDetail+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeBushouRetsudenDetail), cmdOnTenshouExeJumpFromBushouRetsudenDetail, 5, NULL); //5バイトのみ書き込む
}




