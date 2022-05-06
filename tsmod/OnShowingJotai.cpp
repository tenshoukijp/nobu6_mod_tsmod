
#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXBushou_All.h"
#include "ScenarioMod.h"
#include "TSModCommand.h"

/*
004C8ED0  94 4E 97 EE 00 00 00 00 88 EA 96 E5 00 00 00 00  年齢....一門....
004C8EE0  8C 52 92 63 92 B7 00 00 90 67 95 AA 00 00 00 00  軍団長..身分....
004C8EF0  91 E5 96 BC 00 00 00 00 95 90 8F AB 96 BC 00 00  大名....武将名..

004D13D0  [F0 8E 4C] 00 [E0 8E 4C] 00 [BC FE 4C] 00 [A0 FF 4C] 00  大名 軍団長 一般 隠居
004D13E0  [B4 FE 4C] 00 [B0 B7 4B] 00 [AC FE 4C] 00 [A4 FE 4C] 00  浪人 姫 待機 死亡


// 状態を要求する関数
// 以下の３つの場所で状態の「文字列ポインタの配列へのポインタ」が利用される。
参照データ ： TENSHOU:.text -> 004D13D0..004D13D0
ｱﾄﾞﾚｽ      ﾃﾞｨｽｱｾﾝﾌﾞﾙ                                ｺﾒﾝﾄ
１カ所目 00438F3B   MOV     EAX, DWORD PTR DS:[EAX*4+4D13D0]  通常の武将リストの時に武将ごとにここを通過する。(基本的に浪人登用の時)
２カ所目 00475A62   PUSH    DWORD PTR DS:[EAX*4+4D13D0]     (城などをクリックした時
３箇所目 00495290   PUSH    DWORD PTR DS:[EAX*4+4D13D0]     (地図による大名情報のところ)
*/





static int GetOverRewriteJotaiAddress(int iBushouID, int iJotai=-1 ) {

	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// デフォルトは現在のポジションが入る
		if ( iJotai==-1) {
			iJotai = nb6bushouname[iBushouID].State;
		}

		// ScenarioModの呼び出し。( return しているのでScenarioModを先に入れる)
		if ( p_snOnViewingBushouStateName ) {
			int iPointerAddress = 0;
			p_snOnViewingBushouStateName( &iPointerAddress, iBushouID+1, iJotai );
			if ( iPointerAddress ) {
				return int(iPointerAddress);
			}
		}

		/*
		if ( TSModCommand::UltraBushou::cmdExists ) { // 以下、ちょっと重たいのでフィルター
			if ( iJotai == 0 ) {
				if ( isLancelotTartare(iBushouID) ) { // ﾗﾝｽﾛｯﾄなら
					static char szJotai[] = "㈱㈲㈹"; // ﾛｽﾛｰﾘｱﾝ
					return int(szJotai);
				}
			}
		}
		*/

		if ( iJotai == 0 ) { // 0大名,1軍長,2現役,3隠居,4浪人,5姫,6,待機,7死亡
			int iDaimyoID = nb6bushouref[iBushouID].attach - 1;
			if ( 0 <= iDaimyoID && iDaimyoID < GAMEDATASTRUCT_DAIMYO_NUM ) {
				int iSimbol = nb6daimyo[iDaimyoID].symbol;
				if ( iSimbol == 41 ) { // 本願寺家紋
					static char szJotai[] = "法主";
					return int(szJotai);
				}
			}

		// 浪人だけど僧侶なら雲水（行脚僧）。
		} else if ( iJotai == 4 && nb6bushouref[iBushouID].job == 0x40 ) {
			static char szJotai[] = "浪人";
			return int(szJotai);
		}

	}


	return NULL;
}





//---------------------------------------------------------------------------------------------------------------------------



/*
 * 身分を表示するときに、武将番号を拾っておく
 * EAX==004D00BDみたいに下半分のビットが武将番号
 */
int iBushouBangouOfShowingJotai1 = -1;
int iJotaiNumberOfShowingJotai1 = -1;
int iEAXPointerOfShowingJotai1 = NULL;
void OnTenshouExeShowingJotai1Execute() {

	int iBushouID = (iBushouBangouOfShowingJotai1 & 0xFFFF) - 1; // 下半分が武将番号

	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
		int ipointer = GetOverRewriteJotaiAddress(iBushouID);

		if ( ipointer ) {
			iEAXPointerOfShowingJotai1 = ipointer;
		}
	}
}



/*
00438F33   > E8 6214FDFF    CALL    TENSHOU.0040A39A
				└ここを JMP TSMod.OnTSExeShowingMibunBefore と書き換えてやる実際の処理
00438F38   . 83C4 04        ADD     ESP, 4
00438F3B   . 8B0485 D0134D0>MOV     EAX, DWORD PTR DS:[EAX*4+4D13D0]
00438F42   > 5E             POP     ESI                              ;  TENSHOU.004E66E8
*/
int pTenshouExeJumpFromToOnTenshouExeShowingJotai1		=0x438F33; // 関数はこのアドレスから、OnTenshouExeShowingJotai1へとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeShowingJotai1  =0x40A39A; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeShowingJotai1	=0x438F42; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeShowingJotai1() {
	// スタックにためておく
	__asm {

		// 武将番号保存
		mov iBushouBangouOfShowingJotai1, ecx

		// 元々TENSHOU.EXE内にあったものをここで
		call pTenshouExeJumpCallFromToOnTenshouExeShowingJotai1

		// 元々TENSHOU.EXE内にあったものをここで
		ADD     ESP, 4

		// 状態番号の保存
		mov iJotaiNumberOfShowingJotai1, eax

		// (右辺値のEAXとは身分の値、「EAX*4+0x4D13B0」が各身分の文字列へのポインタが入っているアドレスですね、左辺値のEAXで受け取っている)
		MOV     EAX, DWORD PTR DS:[EAX*4+0x4D13D0]

		// 元の文字列へのポインタとして保存
		mov iEAXPointerOfShowingJotai1, EAX

		// eaxを元へと戻す
		mov eax, iJotaiNumberOfShowingJotai1

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}
	
	// 
	OnTenshouExeShowingJotai1Execute();

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

		// 文字列のポインタの上書き
		mov EAX, iEAXPointerOfShowingJotai1

		jmp pTenshouExeReturnLblFromOnTenshouExeShowingJotai1
	}
}

char cmdOnTenshouExeJumpFromShowingJotai1[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計８バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeShowingJotai1() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeShowingJotai1;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeShowingJotai1 + 5 );
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeShowingMibunBefore  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromShowingJotai1+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeShowingJotai1), cmdOnTenshouExeJumpFromShowingJotai1, 5, NULL); //5バイトのみ書き込む
}



//---------------------------------------------------------------------------------------------------------------------------



/*
 * 身分を表示するときに、武将番号を拾っておく
 * EAX==004D00BDみたいに下半分のビットが武将番号
 */
int iBushouBangouOfShowingJotai2 = -1;
int iJotaiNumberOfShowingJotai2 = -1;
int iEAXPointerOfShowingJotai2 = NULL;
void OnTenshouExeShowingJotai2Execute() {

	int iBushouID = (iBushouBangouOfShowingJotai2 & 0xFFFF) - 1; // 下半分が武将番号

	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
		int ipointer = GetOverRewriteJotaiAddress(iBushouID);

		if ( ipointer ) {
			iEAXPointerOfShowingJotai2 = ipointer;
		}
	}
}



/*
00475A50  |. FF75 FE                  PUSH    DWORD PTR SS:[EBP-2]
00475A53  |. E8 874CF9FF              CALL    TENSHOU.0040A6DF
				└ここを JMP TSMod.OnTSExeShowingMibunBefore と書き換えてやる実際の処理
00475A58  |. 83C4 04                  ADD     ESP, 4
00475A5B  |. 83F8 01                  CMP     EAX, 1
00475A5E  |. 1BC0                     SBB     EAX, EAX
00475A60  |. F7D8                     NEG     EAX
00475A62  |. FF3485 D0134D00          PUSH    DWORD PTR DS:[EAX*4+4D13D0]
00475A69  |. 68 7CAD4B00              PUSH    TENSHOU.004BAD7C                 ;  ASCII "%s %s"
*/
int pTenshouExeJumpFromToOnTenshouExeShowingJotai2		=0x475A53; // 関数はこのアドレスから、OnTenshouExeShowingJotai2へとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeShowingJotai2  =0x40A6DF; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeShowingJotai2	=0x475A69; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeShowingJotai2() {
	// スタックにためておく
	__asm {

		// EAXを間借りするので、一旦保存
		mov iJotaiNumberOfShowingJotai2, eax

		// 武将の番号
		mov eax,  DWORD PTR SS:[EBP-0x2]

		// 武将の番号の保存
		mov iBushouBangouOfShowingJotai2, eax

		// eaxを元へと戻す
		mov eax, iJotaiNumberOfShowingJotai2

		// 元々TENSHOU.EXE内にあったものをここで
		call pTenshouExeJumpCallFromToOnTenshouExeShowingJotai2

		// 元々TENSHOU.EXE内にあったものをここで
		ADD     ESP, 4
		CMP     EAX, 1
		SBB     EAX, EAX
		NEG     EAX

		// 状態番号の保存
		mov iJotaiNumberOfShowingJotai2, eax

		// EAXを再び間借り
		// (右辺値のEAXとは身分の値、「EAX*4+0x4D13B0」が各身分の文字列へのポインタが入っているアドレスですね、左辺値のEAXで受け取っている)
		MOV     EAX, DWORD PTR DS:[EAX*4+0x4D13D0]

		mov iEAXPointerOfShowingJotai2, EAX

		// eaxを元へと戻す
		mov eax, iJotaiNumberOfShowingJotai2

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}
	
	// 
	OnTenshouExeShowingJotai2Execute();

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

		// 文字列のポインタ
		push iEAXPointerOfShowingJotai2

		// EAXを元の状態番号へと戻す
		mov EAX, iJotaiNumberOfShowingJotai2

		jmp pTenshouExeReturnLblFromOnTenshouExeShowingJotai2
	}
}

char cmdOnTenshouExeJumpFromShowingJotai2[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計８バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeShowingJotai2() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeShowingJotai2;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeShowingJotai2 + 5 );
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeShowingMibunBefore  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromShowingJotai2+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeShowingJotai2), cmdOnTenshouExeJumpFromShowingJotai2, 5, NULL); //5バイトのみ書き込む
}






/*
 * 身分を表示するときに、武将番号を拾っておく
 * EAX==004D00BDみたいに下半分のビットが武将番号
 */
int iBushouBangouOfShowingJotai3 = -1;
int iJotaiNumberOfShowingJotai3 = -1;
int iEAXPointerOfShowingJotai3 = NULL;
void OnTenshouExeShowingJotai3Execute() {

	int iBushouID = (iBushouBangouOfShowingJotai3 & 0xFFFF) - 1; // 下半分が武将番号

	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
		int ipointer = GetOverRewriteJotaiAddress(iBushouID);

		if ( ipointer ) {
			iEAXPointerOfShowingJotai3 = ipointer;
		}
	}
}



/*
00495281  |. FF7424 16      PUSH    DWORD PTR SS:[ESP+16]
00495285  |. E8 1051F7FF    CALL    TENSHOU.0040A39A
				└ここを JMP TSMod.OnTSExeShowingMibunBefore と書き換えてやる実際の処理
0049528A  |. 66:8B0F        MOV     CX, WORD PTR DS:[EDI]
0049528D  |. 83C4 04        ADD     ESP, 4
00495290  |. FF3485 D0134D0>PUSH    DWORD PTR DS:[EAX*4+4D13D0]
00495297  |. 51             PUSH    ECX                              ;  TENSHOU.004D0019

*/
int pTenshouExeJumpFromToOnTenshouExeShowingJotai3		=0x495285; // 関数はこのアドレスから、OnTenshouExeShowingJotai3へとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeShowingJotai3  =0x40A39A; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeShowingJotai3	=0x495297; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeShowingJotai3() {
	// スタックにためておく
	__asm {

		// EAXを間借りするので、一旦保存
		mov iJotaiNumberOfShowingJotai3, eax

		// 武将の番号
		mov eax,  DWORD PTR SS:[ESP]

		// 武将の番号の保存
		mov iBushouBangouOfShowingJotai3, eax

		// eaxを元へと戻す
		mov eax, iJotaiNumberOfShowingJotai3

		// 元々TENSHOU.EXE内にあったものをここで
		call pTenshouExeJumpCallFromToOnTenshouExeShowingJotai3

		// 元々TENSHOU.EXE内にあったものをここで
		MOV     CX, WORD PTR DS:[EDI]
		ADD     ESP, 4

		// 状態番号の保存
		mov iJotaiNumberOfShowingJotai3, eax

		// EAXを再び間借り
		// (右辺値のEAXとは身分の値、「EAX*4+0x4D13B0」が各身分の文字列へのポインタが入っているアドレスですね、左辺値のEAXで受け取っている)
		MOV     EAX, DWORD PTR DS:[EAX*4+0x4D13D0]

		mov iEAXPointerOfShowingJotai3, EAX

		// eaxを元へと戻す
		mov eax, iJotaiNumberOfShowingJotai3

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}
	
	// 
	OnTenshouExeShowingJotai3Execute();

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

		// 文字列のポインタ
		push iEAXPointerOfShowingJotai3

		// EAXを元の状態番号へと戻す
		mov EAX, iJotaiNumberOfShowingJotai3

		jmp pTenshouExeReturnLblFromOnTenshouExeShowingJotai3
	}
}

char cmdOnTenshouExeJumpFromShowingJotai3[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計８バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeShowingJotai3() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeShowingJotai3;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeShowingJotai3 + 5 );
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeShowingMibunBefore  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromShowingJotai3+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeShowingJotai3), cmdOnTenshouExeJumpFromShowingJotai3, 5, NULL); //5バイトのみ書き込む
}

