#define _CRT_SECURE_NO_WARNINGS

#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "ScenarioMod.h"



int iKahouIDOfKahouMeibutsuJiten = -1;

#define RETSUDEN_BUF_ADDRESS		0x4D97C0

static char szBufMeibutsuJitenTitle [50*1] = ""; // 実際には全角１８字まで多めにとっておく。
static char szBufMeibutsuJitenDetail[50*4] = ""; // 実際には全角１８文字×４行まで。多めにとっておく。

void OverWriteKahouMeibutsuJitenTitle(int iKahouID) {
	// 両方クリアしてから…
	strcpy(szBufMeibutsuJitenTitle, "");
	strcpy(szBufMeibutsuJitenDetail, "");

	if ( p_snOnViewingMeibutsuJiten ) {
		p_snOnViewingMeibutsuJiten(iKahouID, szBufMeibutsuJitenTitle, szBufMeibutsuJitenDetail);

		// 有効な文字が入っていたら、コピー
		if (strlen(szBufMeibutsuJitenTitle) ) {
			char *szTitle = (char *)RETSUDEN_BUF_ADDRESS;
			strcpy(szTitle, szBufMeibutsuJitenTitle);

			// コピーし終わったのでクリアしておく
			strcpy(szBufMeibutsuJitenTitle, "");
		}
	}
}
void OnTenshouExeKahouMeibutsuJitenTitleExecute() {
	int iKahouID = iKahouIDOfKahouMeibutsuJiten;
	if ( 0 <= iKahouID && iKahouID < GAMEDATASTRUCT_KAHOU_NUM ) {
		OverWriteKahouMeibutsuJitenTitle(iKahouID);
	}
}

/*
00494A4B  |. 6A 00          PUSH    0                                                                                       ; /Arg8 = 00000000
00494A4D  |. 6A 00          PUSH    0                                                                                       ; |Arg7 = 00000000
00494A4F  |. 6A 00          PUSH    0                                                                                       ; |Arg6 = 00000000
00494A51  |. 6A 00          PUSH    0                                                                                       ; |Arg5 = 00000000
00494A53  |. 6A 00          PUSH    0                                                                                       ; |Arg4 = 00000000
00494A55  |. 6A 00          PUSH    0                                                                                       ; |Arg3 = 00000000
00494A57  |. 6A 00          PUSH    0                                                                                       ; |Arg2 = 00000000
00494A59  |. 57             PUSH    EDI                                                                                     ; |Arg1
00494A5A  |. E8 0176FDFF    CALL    TENSHOU.0046C060                                                                        ; \TENSHOU.0046C060
				└ここを JMP TSMod.OnTSExeKahouMeibutsuJitenTitle と書き換えてやる実際の処理
00494A5F  |. 83C4 20        ADD     ESP, 20
*/
int pTenshouExeJumpFromToOnTenshouExeKahouMeibutsuJitenTitle	  =0x494A5A; // 関数はこのアドレスから、OnTenshouExeKahouMeibutsuJitenTitleへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeKahouMeibutsuJitenTitle =0x46C060; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeKahouMeibutsuJitenTitle  =0x494A5F; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeKahouMeibutsuJitenTitle() {
	// スタックにためておく
	__asm {

		mov iKahouIDOfKahouMeibutsuJiten, EBP // 家宝番号配列用(-1の方)が入ってくる。

		// 元々の処理をここに
		call pTenshouExeJumpCallFromToOnTenshouExeKahouMeibutsuJitenTitle

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
	OnTenshouExeKahouMeibutsuJitenTitleExecute();

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

		jmp pTenshouExeReturnLblFromOnTenshouExeKahouMeibutsuJitenTitle
	}
}

char cmdOnTenshouExeJumpFromKahouMeibutsuJitenTitle[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeKahouMeibutsuJitenTitle() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeKahouMeibutsuJitenTitle;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeKahouMeibutsuJitenTitle + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeKahouMeibutsuJitenTitle  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromKahouMeibutsuJitenTitle+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeKahouMeibutsuJitenTitle), cmdOnTenshouExeJumpFromKahouMeibutsuJitenTitle, 5, NULL); //5バイトのみ書き込む
}











void OnTenshouExeKahouMeibutsuJitenDetailExecute() {

	// 有効な文字が入っていたら、コピー
	if (strlen(szBufMeibutsuJitenDetail) ) {
		char *szDetail = (char *)RETSUDEN_BUF_ADDRESS;
		strcpy(szDetail, szBufMeibutsuJitenDetail);
		
		// コピーし終わったのでクリアしておく
		strcpy(szBufMeibutsuJitenDetail, "");
	}
}



/*
00494A93  |. 50             PUSH    EAX                                                                                     ; /Arg8 => 00000000
00494A94  |. 50             PUSH    EAX                                                                                     ; |Arg7 => 00000000
00494A95  |. 50             PUSH    EAX                                                                                     ; |Arg6 => 00000000
00494A96  |. 47             INC     EDI                                                                                     ; |
00494A97  |. 50             PUSH    EAX                                                                                     ; |Arg5 => 00000000
00494A98  |. 50             PUSH    EAX                                                                                     ; |Arg4 => 00000000
00494A99  |. 50             PUSH    EAX                                                                                     ; |Arg3 => 00000000
00494A9A  |. 50             PUSH    EAX                                                                                     ; |Arg2 => 00000000
00494A9B  |. 57             PUSH    EDI                                                                                     ; |Arg1
00494A9C  |. E8 BF75FDFF    CALL    TENSHOU.0046C060                                                                        ; \TENSHOU.0046C060
				└ここを JMP TSMod.OnTSExeKahouMeibutsuJitenDetail と書き換えてやる実際の処理
00494AA1  |. 83C4 20        ADD     ESP, 20
*/
int pTenshouExeJumpFromToOnTenshouExeKahouMeibutsuJitenDetail	   = 0x494A9C; // 関数はこのアドレスから、OnTenshouExeKahouMeibutsuJitenDetailへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeKahouMeibutsuJitenDetail = 0x46C060; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeKahouMeibutsuJitenDetail  = 0x494AA1; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeKahouMeibutsuJitenDetail() {
	// スタックにためておく
	__asm {

		// 元々の処理をここに
		call pTenshouExeJumpCallFromToOnTenshouExeKahouMeibutsuJitenDetail

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
	OnTenshouExeKahouMeibutsuJitenDetailExecute();

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

		jmp pTenshouExeReturnLblFromOnTenshouExeKahouMeibutsuJitenDetail
	}
}

char cmdOnTenshouExeJumpFromKahouMeibutsuJitenDetail[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeKahouMeibutsuJitenDetail() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeKahouMeibutsuJitenDetail;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeKahouMeibutsuJitenDetail + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeKahouMeibutsuJitenDetail  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromKahouMeibutsuJitenDetail+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeKahouMeibutsuJitenDetail), cmdOnTenshouExeJumpFromKahouMeibutsuJitenDetail, 5, NULL); //5バイトのみ書き込む
}




