#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"

#include "TSModCommand.h"



/*
00413D9F   > 813D 7C2A4D00 21000000           CMP     DWORD PTR DS:[4D2A7C], 21
00413DA9   . 72 10                            JB      SHORT tenshou.00413DBB
00413DAB   . 6A 01                            PUSH    1
00413DAD   . FF35 C0344D00                    PUSH    DWORD PTR DS:[4D34C0]
*/

/*
0046B561   . 8D4D F8        LEA     ECX, DWORD PTR SS:[EBP-8]
0046B564   . 57             PUSH    EDI
0046B565   . E8 21B2F9FF    CALL    TENSHOU.0040678B
0046B56A   >^E9 3088FAFF    JMP     TENSHOU.00413D9F			← 46B56A
0046B56F     90             NOP
0046B570   > 68 FFFF0000    PUSH    0FFFF
0046B575   .-E9 B6A73600    JMP     TSMOD.JmpOnTSExeNextTurnIsFieldOrCastle
0046B57A   . 83C4 0C        ADD     ESP, 0C
0046B57D   . 85C0           TEST    EAX, EAX

00413D9F   > 813D 7C2A4D00 >CMP     DWORD PTR DS:[4D2A7C], 21	← ここへ来た
00413DA9   . 72 10          JB      SHORT TENSHOU.00413DBB		ここに合格せず 413DBBへ
00413DAB   . 6A 01          PUSH    1
00413DAD   . FF35 C0344D00  PUSH    DWORD PTR DS:[4D34C0]
00413DB3   . E8 31960700    CALL    TENSHOU.0048D3E9
00413DB8   . 83C4 08        ADD     ESP, 8
00413DBB   > E9 2A4A0300    JMP     TENSHOU.004487EA			←



// ここのCMPを修正すればよい
00413D9F     833D 7C2A4D00 >CMP     DWORD PTR DS:[4D2A7C], 21		★ここの数値が21と大きいので曲番号が合格しない。(たぶんだが、神パッチの34以降が戦争の曲なので、33以下だとはじいてると思う。)
00413DA9   . 72 10          JB      SHORT TENSHOU.00413DBB			JB は「未満」
00413DAB   . 6A 01          PUSH    1
00413DAD   . FF35 C0344D00  PUSH    DWORD PTR DS:[4D34C0]
00413DB3   . E8 31960700    CALL    TENSHOU.0048D3E9
00413DB8   . 83C4 08        ADD     ESP, 8
00413DBB   > E9 2A4A0300    JMP     TENSHOU.004487EA
*/


// 戦争時に成らすＢＧＭを、特定の番号以上か以下かでフィルタリングしているアルゴリズム


int iCheckValueOfWarBGMCheckExecute = FALSE;
int iPreviousBGMCheckExecute = -1;

void OnTenshouExeWarBGMCheckExecute() {
	WORD *pBGMID = (WORD *)GAMEDATASTRUCT_PLAYING_BGM_ID_ADDRESS; // 今から再生するべき曲は？

	int iBGMID = *pBGMID;

	iCheckValueOfWarBGMCheckExecute = FALSE;

	// BGMモードが13曲の方だ
	if (TSModCommand::Environment::cmdBGMMode == 0 || TSModCommand::Environment::cmdBGMMode == 1 ||  TSModCommand::Environment::cmdBGMMode == 3 ) {

		if ( iBGMID < 11 ) { // 0オリジンでいう11番未満だけ切り替える。「戦争緊迫の時は、毎ターンやってくるが、そこは切り替えない。戦争夏とも共通であるが、問題ないようである。」

			// 戦争中だよ
			if ( Is_FieldWar() || Is_CastleWar() ) {
				iCheckValueOfWarBGMCheckExecute = TRUE;
			}
		}

	// BGMモードは38曲の方だ。こっちは根本的に何もしなくても良い。
	} else {

		// 
		if ( iBGMID >= 0x21 ) {
			iCheckValueOfWarBGMCheckExecute = TRUE;
		} else {
			iCheckValueOfWarBGMCheckExecute = FALSE;
		}
	
	}
}


/*
00413D9F   > 813D 7C2A4D00 21000000           CMP     DWORD PTR DS:[4D2A7C], 21
00413DA9   . 72 10                            JB      SHORT tenshou.00413DBB		JVB
				└ここを JMP TSMod.OnTSExeWarBGMCheck と書き換えてやる実際の処理
00413DAB   . 6A 01                            PUSH    1
00413DAD   . FF35 C0344D00                    PUSH    DWORD PTR DS:[4D34C0]
 */
int pTenshouExeJumpFromToOnTenshouExeWarBGMCheck    =0x413D9F; // 関数はこのアドレスから、OnTenshouExeWarBGMCheckへとジャンプしてくる。
int pTenshouExeReturnLbl1FromOnTenshouExeWarBGMCheck =0x413DAB; // (38曲の時は)0x21以上の時にいくアドレス (13曲の時は、戦争中かつ次の曲が何かなら認める)
int pTenshouExeReturnLbl2FromOnTenshouExeWarBGMCheck =0x413DBB; // 21未満の時にいくアドレス
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeWarBGMCheck() {
	// スタックにためておく
	__asm {
		// TENSHOU.EXEに元々あった処理をここに

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
	OnTenshouExeWarBGMCheckExecute();

	// 何もしなくても良いならば･･･
	if ( iCheckValueOfWarBGMCheckExecute ) { // 戦争のための曲ならば･･･

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


			jmp pTenshouExeReturnLbl1FromOnTenshouExeWarBGMCheck // 新たなBGMを再生する。Lbl1の方にジャンプ
		}
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


			jmp pTenshouExeReturnLbl2FromOnTenshouExeWarBGMCheck // BGM再生しない。Lbl2の方にジャンプ
		}
	}
}

/*
00444769  |. 50             PUSH    EAX
0044476A  |. E8 3A2C0200    CALL    TENSHOU.004673A9
				└ここを JMP TSMod.OnTSExeWarBGMCheck と書き換えてやる実際の処理
0044476F  |. 83C4 04        ADD     ESP, 4
*/
char cmdOnTenshouExeJumpFromWarBGMCheck[13]="\xE9\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeWarBGMCheck() {

	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeWarBGMCheck;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeWarBGMCheck + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeWarBGMCheck  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromWarBGMCheck+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeWarBGMCheck), cmdOnTenshouExeJumpFromWarBGMCheck, 12, NULL); //12バイトのみ書き込む
}




