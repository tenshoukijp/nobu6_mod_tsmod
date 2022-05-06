#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"
#include "TSModCommand.h"



// 戦闘時の出陣リスト選択時に、出陣最大数が決定されるタイミング

int iSubstituteMaxShutujinNum = -1;

void OnTenshouExeSubstituteMaxShutujinNumExecute() {
	// 出陣数を指定している場合。最大数をそれに変える
	if ( TSModCommand::AllGundan::cmdTheNumberOfTheMaxGoToWarFix > 0 ) {
	}
}


/*
004826A9  |. E8 B48FFAFF    |CALL    TENSHOU.0042B662										　　　　←残り出陣数算出関数		
				└ここを JMP TSMod.OnTSExeSubstituteMaxShutujinNum と書き換えてやる実際の処理
004826AE  |. 83C4 04        |ADD     ESP, 4												
004826B1  |. 50             |PUSH    EAX                             ; |Arg2						←残り出陣人数
 */
int pTenshouExeJumpFromToOnTenshouExeSubstituteMaxShutujinNum    =0x4826A9; // 関数はこのアドレスから、OnTenshouExeSubstituteMaxShutujinNumへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeSubstituteMaxShutujinNum=0x42B662; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeSubstituteMaxShutujinNum =0x4826AE; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeSubstituteMaxShutujinNum() {
	// スタックにためておく
	__asm {
		// 元々TENSHOU.EXE内にあったものをここで
		call pTenshouExeJumpCallFromToOnTenshouExeSubstituteMaxShutujinNum

		mov iSubstituteMaxShutujinNum, eax

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
	OnTenshouExeSubstituteMaxShutujinNumExecute();

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

		mov eax, iSubstituteMaxShutujinNum

		jmp pTenshouExeReturnLblFromOnTenshouExeSubstituteMaxShutujinNum
	}
}

/*
004826A9  |. E8 B48FFAFF    |CALL    TENSHOU.0042B662										　　　　←残り出陣数算出関数		
				└ここを JMP TSMod.OnTSExeSubstituteMaxShutujinNum と書き換えてやる実際の処理
004826AE  |. 83C4 04        |ADD     ESP, 4												
004826B1  |. 50             |PUSH    EAX                             ; |Arg2						←残り出陣人数
*/
char cmdOnTenshouExeJumpFromSubstituteMaxShutujinNum[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeSubstituteMaxShutujinNum() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeSubstituteMaxShutujinNum;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeSubstituteMaxShutujinNum + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeSubstituteMaxShutujinNum  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromSubstituteMaxShutujinNum+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeSubstituteMaxShutujinNum), cmdOnTenshouExeJumpFromSubstituteMaxShutujinNum, 5, NULL); //5バイトのみ書き込む
}







//------------------------------------------------------------------------------------------------







// 戦闘時の出陣リスト選択時に、(あと○部隊出陣できますとかいう)メッセージ内の出陣最大数のが決定されるタイミング

int iSubstituteMaxShutujinMsg = -1;

void OnTenshouExeSubstituteMaxShutujinMsgExecute() {
	// 出陣数を指定している場合。最大数をそれに変える
	if ( TSModCommand::AllGundan::cmdTheNumberOfTheMaxGoToWarFix > 0 ) {
		iSubstituteMaxShutujinMsg = 8;
	}
}


/*
0048273F  |> 50             PUSH    EAX
00482740  |. FF75 FE        PUSH    DWORD PTR SS:[EBP-2]
00482743  |. E8 14AFF8FF    CALL    TENSHOU.0040D65C
				└ここを JMP TSMod.OnTSExeSubstituteMaxShutujinMsg と書き換えてやる実際の処理
00482748  |. 83C4 04        ADD     ESP, 4
0048274B  |. 50             PUSH    EAX
 */
int pTenshouExeJumpFromToOnTenshouExeSubstituteMaxShutujinMsg    =0x482743; // 関数はこのアドレスから、OnTenshouExeSubstituteMaxShutujinMsgへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeSubstituteMaxShutujinMsg=0x40D65C; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeSubstituteMaxShutujinMsg =0x482748; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeSubstituteMaxShutujinMsg() {
	// スタックにためておく
	__asm {
		// pushした分はナシ
		pop iSubstituteMaxShutujinMsg // 受け皿変数がないので適当に
		pop iSubstituteMaxShutujinMsg // 受け皿変数がないので適当に

		// eaxをiSubstituteMaxShutujinMsgにコピー
		mov iSubstituteMaxShutujinMsg, eax

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
	OnTenshouExeSubstituteMaxShutujinMsgExecute();

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

		// 上書きしたiSubstituteMaxShutujinMsgをeaxにコピー
		mov eax, iSubstituteMaxShutujinMsg

		// 元々pushすべきものをここで
		PUSH    EAX
		PUSH    DWORD PTR SS:[EBP-0x2]

		// 元々TENSHOU.EXE内にあったものをここで
		call pTenshouExeJumpCallFromToOnTenshouExeSubstituteMaxShutujinMsg

		jmp pTenshouExeReturnLblFromOnTenshouExeSubstituteMaxShutujinMsg
	}
}

/*
0048273F  |> 50             PUSH    EAX
00482740  |. FF75 FE        PUSH    DWORD PTR SS:[EBP-2]
00482743  |. E8 14AFF8FF    CALL    TENSHOU.0040D65C
				└ここを JMP TSMod.OnTSExeSubstituteMaxShutujinMsg と書き換えてやる実際の処理
00482748  |. 83C4 04        ADD     ESP, 4
0048274B  |. 50             PUSH    EAX
*/
char cmdOnTenshouExeJumpFromSubstituteMaxShutujinMsg[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeSubstituteMaxShutujinMsg() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeSubstituteMaxShutujinMsg;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeSubstituteMaxShutujinMsg + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeSubstituteMaxShutujinMsg  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromSubstituteMaxShutujinMsg+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeSubstituteMaxShutujinMsg), cmdOnTenshouExeJumpFromSubstituteMaxShutujinMsg, 5, NULL); //5バイトのみ書き込む
}




//------------------------------------------------------------------------------------------------



// 戦闘時の出陣リスト選択時に、出陣残り数が決定されるタイミング

int iSubstituteMaxShutujinRemain1 = -1;

void OnTenshouExeSubstituteMaxShutujinRemain1Execute() {

	// 出陣数を指定している場合。最大数をそれに変える
	if ( TSModCommand::AllGundan::cmdTheNumberOfTheMaxGoToWarFix > 0 ) {
		iSubstituteMaxShutujinRemain1 = 8;
	}
}


/*
0042B6B5  |. E8 A6FF0100    CALL    TENSHOU.0044B660
				└ここを JMP TSMod.OnTSExeSubstituteMaxShutujinRemain1 と書き換えてやる実際の処理
0042B6BA  |. 83C4 04        ADD     ESP, 4
0042B6BD  |. 2BC7           SUB     EAX, EDI
 */
int pTenshouExeJumpFromToOnTenshouExeSubstituteMaxShutujinRemain1    =0x42B6B5; // 関数はこのアドレスから、OnTenshouExeSubstituteMaxShutujinRemain1へとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeSubstituteMaxShutujinRemain1=0x44B660; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeSubstituteMaxShutujinRemain1 =0x42B6BA; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeSubstituteMaxShutujinRemain1() {
	// スタックにためておく
	__asm {
		// 元々TENSHOU.EXE内にあったものをここで
		call pTenshouExeJumpCallFromToOnTenshouExeSubstituteMaxShutujinRemain1

		mov iSubstituteMaxShutujinRemain1, eax

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
	OnTenshouExeSubstituteMaxShutujinRemain1Execute();

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

		mov eax, iSubstituteMaxShutujinRemain1

		jmp pTenshouExeReturnLblFromOnTenshouExeSubstituteMaxShutujinRemain1
	}
}

/*
004826A9  |. E8 B48FFAFF    |CALL    TENSHOU.0042B662										　　　　←残り出陣数算出関数		
				└ここを JMP TSMod.OnTSExeSubstituteMaxShutujinRemain1 と書き換えてやる実際の処理
004826AE  |. 83C4 04        |ADD     ESP, 4												
004826B1  |. 50             |PUSH    EAX                             ; |Arg2						←残り出陣人数
*/
char cmdOnTenshouExeJumpFromSubstituteMaxShutujinRemain1[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeSubstituteMaxShutujinRemain1() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeSubstituteMaxShutujinRemain1;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeSubstituteMaxShutujinRemain1 + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeSubstituteMaxShutujinRemain1  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromSubstituteMaxShutujinRemain1+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeSubstituteMaxShutujinRemain1), cmdOnTenshouExeJumpFromSubstituteMaxShutujinRemain1, 5, NULL); //5バイトのみ書き込む
}






//---------------------------------------------------------------------




// 戦闘時の出陣リスト選択時に、出陣残り数が決定されるタイミング

int iSubstituteMaxShutujinRemain2 = -1;


void OnTenshouExeSubstituteMaxShutujinRemain2Execute() {

	// 出陣数を指定している場合。最大数をそれに変える
	if ( TSModCommand::AllGundan::cmdTheNumberOfTheMaxGoToWarFix > 0 ) {
		iSubstituteMaxShutujinRemain2 = 8;
	}
}


/*
0042B6C6  |. E8 95FF0100    CALL    TENSHOU.0044B660
				└ここを JMP TSMod.OnTSExeSubstituteMaxShutujinRemain2 と書き換えてやる実際の処理
0042B6CB  |. 83C4 04        ADD     ESP, 4
0042B6CE  |. 2BC7           SUB     EAX, EDI


 */
int pTenshouExeJumpFromToOnTenshouExeSubstituteMaxShutujinRemain2    =0x42B6C6; // 関数はこのアドレスから、OnTenshouExeSubstituteMaxShutujinRemain2へとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeSubstituteMaxShutujinRemain2=0x44B660; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeSubstituteMaxShutujinRemain2 =0x42B6CB; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeSubstituteMaxShutujinRemain2() {
	// スタックにためておく
	__asm {
		// 元々TENSHOU.EXE内にあったものをここで
		call pTenshouExeJumpCallFromToOnTenshouExeSubstituteMaxShutujinRemain2

		mov iSubstituteMaxShutujinRemain2, eax

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
	OnTenshouExeSubstituteMaxShutujinRemain2Execute();

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

		mov eax, iSubstituteMaxShutujinRemain2

		jmp pTenshouExeReturnLblFromOnTenshouExeSubstituteMaxShutujinRemain2
	}
}

/*
004826A9  |. E8 B48FFAFF    |CALL    TENSHOU.0042B662										　　　　←残り出陣数算出関数		
				└ここを JMP TSMod.OnTSExeSubstituteMaxShutujinRemain2 と書き換えてやる実際の処理
004826AE  |. 83C4 04        |ADD     ESP, 4												
004826B1  |. 50             |PUSH    EAX                             ; |Arg2						←残り出陣人数
*/
char cmdOnTenshouExeJumpFromSubstituteMaxShutujinRemain2[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeSubstituteMaxShutujinRemain2() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeSubstituteMaxShutujinRemain2;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeSubstituteMaxShutujinRemain2 + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeSubstituteMaxShutujinRemain2  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromSubstituteMaxShutujinRemain2+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeSubstituteMaxShutujinRemain2), cmdOnTenshouExeJumpFromSubstituteMaxShutujinRemain2, 5, NULL); //5バイトのみ書き込む
}






//---------------------------------------------------------------------

bool bRewrite8WakuShutsuJined = false; // 平野で８部隊いきなり出陣してしまった場合にニーモニックが行かないように。

// ニーモニック書き換え用
// 004826E7     2BC6           SUB     EAX, ESI
// 004826E9     74 05          JE      SHORT TENSHOU.004826F0   ←ここをNopNopにする。
// 004826EB     83F8 08        CMP     EAX, 8
void WriteAsmJumperOnTenshouExeSubstituteMaxShutujinCaseAll() {

	// 出陣数を指定している場合。最大数をそれに変える
	if ( TSModCommand::AllGundan::cmdTheNumberOfTheMaxGoToWarFix > 0 ) {
		if ( !bRewrite8WakuShutsuJined ) {


			char szNopNop[] = "\x90\x90";

			// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
			WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(0x4826E9), szNopNop, 2, NULL); //2バイトのみ書き込む

			bRewrite8WakuShutsuJined = true;
		}
	}
}