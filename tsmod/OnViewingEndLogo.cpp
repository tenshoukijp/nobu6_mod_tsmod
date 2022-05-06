#include "WinTarget.h"

#include <sys/types.h>
#include <sys/stat.h>

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "FuncPlayMovie.h"
#include "TSModCommand.h"
#include "GameDataStruct.h"

/*
  最後ロゴを表示する所
 */

/*
00496AC2  |. 50             PUSH EAX                                 ; /Arg5
00496AC3  |. 6A 70          PUSH 70                                  ; |Arg4 = 00000070
00496AC5  |. 68 D0000000    PUSH 0D0                                 ; |Arg3 = 000000D0
00496ACA  |. A1 44A94D00    MOV EAX,DWORD PTR DS:[4DA944]            ; |
00496ACF  |. 83C0 60        ADD EAX,60                               ; |
00496AD2  |. 8BCE           MOV ECX,ESI                              ; |
00496AD4  |. 50             PUSH EAX                                 ; |Arg2
00496AD5  |. A1 40A94D00    MOV EAX,DWORD PTR DS:[4DA940]            ; |
00496ADA  |. 05 D8000000    ADD EAX,0D8                              ; |
00496ADF  |. 50             PUSH EAX                                 ; |Arg1
00496AE0     E8 04760100    CALL TENSHOU.004AE0E9							← LOGO の上部
00496AE5  |. 6A 04          PUSH 4
00496AE7  |. 8BCF           MOV ECX,EDI
00496AE9  |. 53             PUSH EBX
00496AEA  |. 68 F6130000    PUSH 13F6
00496AEF  |. E8 11470100    CALL TENSHOU.004AB205
00496AF4  |. 50             PUSH EAX                                 ; /Arg5
00496AF5  |. 6A 68          PUSH 68                                  ; |Arg4 = 00000068
00496AF7  |. 68 10010000    PUSH 110                                 ; |Arg3 = 00000110
00496AFC  |. A1 44A94D00    MOV EAX,DWORD PTR DS:[4DA944]            ; |
00496B01  |. 05 E0000000    ADD EAX,0E0                              ; |
00496B06  |. 8BCE           MOV ECX,ESI                              ; |
00496B08  |. 50             PUSH EAX                                 ; |Arg2
00496B09  |. A1 40A94D00    MOV EAX,DWORD PTR DS:[4DA940]            ; |
00496B0E  |. 05 B8000000    ADD EAX,0B8                              ; |
00496B13  |. 50             PUSH EAX                                 ; |Arg1
00496B14     E8 D0750100    CALL TENSHOU.004AE0E9							← LOGO の下部
00496B19  |. 68 70304D00    PUSH TENSHOU.004D3070
00496B1E     E8 90C7F8FF    CALL TENSHOU.004232B3
00496B23  |. 83C4 04        ADD ESP,4
00496B26  |. 6A 08          PUSH 8										   ← LOGOの待ち時間
00496B28     E8 C7C5F8FF    CALL TENSHOU.004230F4
00496B2D  |. 83C4 04        ADD ESP,4
00496B30  |. E8 B9C7F8FF    CALL TENSHOU.004232EE
00496B35  |> 5F             POP EDI
00496B36  |. 5E             POP ESI
00496B37  |. 5B             POP EBX
00496B38  |. 8BE5           MOV ESP,EBP
00496B3A  |. 5D             POP EBP
00496B3B  \. C3             RETN
*/



void WritePatchUnviewingEndingLogo5Byte( int iAddress, char *szAsm ) {
	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(iAddress), szAsm, 5, NULL); //call 1つ分の5バイト書き込む
}

void WritePatchUnviewingEndingLogo1Byte( int iAddress, char Value ) {
	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(iAddress), &Value, 1, NULL);
}


// ロゴの上下表示２つと、待ち時間を８→０に
void SetUnviewingEndingLogo() {

	struct stat st;
	if (stat("Movie\\ENDLOGO.avi", &st) == 0) {
		char szNop[] = "\x90\x90\x90\x90\x90";
		WritePatchUnviewingEndingLogo5Byte( 0x496AE0, szNop ); // 00496AE0     E8 04760100    CALL TENSHOU.004AE0E9 ← LOGO の上部
		WritePatchUnviewingEndingLogo5Byte( 0x496B14, szNop ); // 00496B14     E8 D0750100    CALL TENSHOU.004AE0E9 ← LOGO の下部

		int iWaitTime = 0; // 8→0
		WritePatchUnviewingEndingLogo1Byte( 0x496B27, iWaitTime ); // 00496B26  |. 6A 08          PUSH 8   ← LOGOの待ち時間
	}
}



//-----------------------------------------------------------------------------------------


/*
 最後のロゴの表示が終わるのを待っている時
 */

/*
00496B26  |. 6A 08          PUSH 8										   ← LOGOの待ち時間
00496B28     E8 C7C5F8FF    CALL TENSHOU.004230F4
00496B2D  |. 83C4 04        ADD ESP,4
*/


void OnTenshouExeEndLogoWaitingExecute() {

	// メニューやタイトルを隠すを選んでいる場合は、メニューは即座に隠す
	// メニューバーとタイトルバーを隠すモードで、かつ、スクリーンのサイズ選択を終えている(メニューの「終了」がValildになった後
	if ( TSModCommand::Environment::cmdNoTitleBarAndMenuBar && CommonGlobalHandle::IsAfterScreenSizeSelected ) {
		// メニューはすぐ隠す
		SetMenu( CommonGlobalHandle::tenshouWND, NULL );
	}

	// ここに来るたびにエンディングモード終了
	CommonGlobalHandle::isEndingMode = false;

	// プレイヤ担当軍団IDをすべてクリアすることでデモ状態と同様とする
	WORD iGundanID = 0;
	for (int iPG = 0; iPG < GAMEDATASTRUCT_PLAYERS8_NUM; iPG++) {
		// プレイヤーが担当している軍団のＩＤをクリア
		nb6players8[iPG].gundan = 0xFFFF;

	}

	// 次にいけずに糞詰まりになっていることがある。以下のアドレスに0xFF01を入れないとだめな模様
	// エンディング後、メインメニューに戻れないことによる強引なパッチ
	// WORD* nextModeForceAddressAfterEndingMode = (WORD*)0x4D1584;
	// *nextModeForceAddressAfterEndingMode = 0xFF01;


	// そのムービーはハードディスクに存在するか
	struct stat st;
	if (stat("Movie\\ENDLOGO.avi", &st) == 0) {
		FuncPlayMovie( "ENDLOGO", TRUE, TRUE );
	}

}


/*
00496B26  |. 6A 08          PUSH 8										   ← LOGOの待ち時間
00496B28     E8 C7C5F8FF    CALL TENSHOU.004230F4
				└ここを JMP TSMod.OnTSExeEndLogoWaiting と書き換えてやる実際の処理
00496B2D  |. 83C4 04        ADD ESP,4
*/
int pTenshouExeJumpFromToOnTenshouExeEndLogoWaiting		=0x496B28; // 関数はこのアドレスから、OnTenshouExeEndLogoWaitingへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeEndLogoWaiting =0x4230F4; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeEndLogoWaiting	=0x496B2D; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeEndLogoWaiting() {
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
	OnTenshouExeEndLogoWaitingExecute();

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

		// 元々TENSHOU.EXE内にあったものをここで
		call pTenshouExeJumpCallFromToOnTenshouExeEndLogoWaiting

		jmp pTenshouExeReturnLblFromOnTenshouExeEndLogoWaiting
	}
}

/*
*/
char cmdOnTenshouExeJumpFromEndLogoWaiting[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeEndLogoWaiting() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeEndLogoWaiting;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeEndLogoWaiting + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeEndLogoWaiting  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromEndLogoWaiting+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeEndLogoWaiting), cmdOnTenshouExeJumpFromEndLogoWaiting, 5, NULL); //5バイトのみ書き込む
}




