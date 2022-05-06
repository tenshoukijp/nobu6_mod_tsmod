#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"

/*
リスト選択する際に、4DBD40に武将のＩＤを展開してるっぽい。以下01EFはおそらくバールゼフォン
どこに大名自身を足しても良い。その後、能力準でソートされるから。
004DBD40  [EF 01] [F4 01] F0 01 F3 01 F2 01 8B 00 F1 01 BB 00 					
004DBD50  [FF FF]=終了フラグ 8B 00 D3 00 FF FF C8 00 FF FF 
004DBD60  74 01 76 01 7B 01 7C 01 73 01 50 01 7E 01 FF FF  
004DBD70  C2 00 BE 00 D1 00 D0 00 D2 00 D3 00 D4 00 C7 00  
004DBD80  C9 00 FF FF 00 00 00 00 00 00 00 00 00 00 00 00  
004DBD90  00

この当たりは披露の時にしか流れてこない
00419DE1   . 68 40BD4D00    PUSH    TENSHOU.004DBD40
00419DE6   . 6A 26          PUSH    26
00419DE8   . E8 83D20400    CALL    TENSHOU.00467070
00419DED   . 50             PUSH    EAX                              ; |Arg1
00419DEE   . E8 B843FFFF    CALL    TENSHOU.0040E1AB                 ; \TENSHOU.0040E1AB  ここのCallで披露で選択武将リスト一覧になりえる武将リストが、4DBD40に作られている

00419DF3   . 83C4 18        ADD     ESP, 18
00419DF6   . 83F8 07        CMP     EAX, 7
00419DF9   . 73 23          JNB     SHORT TENSHOU.00419E1E
00419DFB   . 57             PUSH    EDI
00419DFC   . 57             PUSH    EDI

*/

/*
 * 披露時に大名自身を選択対象と出切るように
 */
void OnTenshouExeHirouSelectingBushouListExecute() {

	// 選択武将の武将ＩＤのリストがメモリに展開されている場所
	WORD *pBushouListAddress = NULL;
	pBushouListAddress = (WORD *)GAMEDATASTRUCT_SELECTING_BUSHOU_LIST;

	// 現在のターン情報の、現在の大名（ターン大名）の番号を得る。
	WORD iCurDaimyo = nb6turn.daimyo;
	int iCurDaimyoBushouID = nb6daimyo[iCurDaimyo-1].attach;

	// 無限にいかないように一応武将の最大人数でポインタ増加への安全装置
	for ( int ix=0; ix < GAMEDATASTRUCT_BUSHOU_NUM; ix++ ) {

		// リストの一番最後であれば、大名を追加する
		if (*(pBushouListAddress+ix) == 0xFFFF) {
			// リストの一番最後なら、大名に置き換える。
			*(pBushouListAddress+ix) = iCurDaimyoBushouID;
			// その次を最後とする。
			*(pBushouListAddress+ix+1) = 0xFFFF;
			break;

		// 大名自身を見つけた場合は何もしない。
		} else if ( *(pBushouListAddress+ix) == iCurDaimyoBushouID ) {
			break;
		}	
	}
}


/*
00419DED   . 50             PUSH    EAX                              ; |Arg1
00419DEE   . E8 B843FFFF    CALL    TENSHOU.0040E1AB                 ; \TENSHOU.0040E1AB  ここのCallで披露で選択武将リスト一覧になりえる武将リストが、4DBD40に作られている
				└ここを JMP TSMod.OnTSExeHirouSelectingBushouList と書き換えてやる実際の処理
00419DF3   . 83C4 18        ADD     ESP, 18
*/
int pTenshouExeJumpFromToOnTenshouExeHirouSelectingBushouList		=0x419DEE; // 関数はこのアドレスから、OnTenshouExeHirouSelectingBushouListへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeHirouSelectingBushouList   =0x40E1AB; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeHirouSelectingBushouList	=0x419DF3; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeHirouSelectingBushouList() {
	// スタックにためておく
	__asm {
		// 元々TENSHOU.EXE内にあったものをここで
		call pTenshouExeJumpCallFromToOnTenshouExeHirouSelectingBushouList

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
	OnTenshouExeHirouSelectingBushouListExecute();

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

		jmp pTenshouExeReturnLblFromOnTenshouExeHirouSelectingBushouList
	}
}

/*
00419DED   . 50             PUSH    EAX                              ; |Arg1
00419DEE   . E8 B843FFFF    CALL    TENSHOU.0040E1AB                 ; \TENSHOU.0040E1AB  ここのCallで披露で選択武将リスト一覧になりえる武将リストが、4DBD40に作られている
				└ここを JMP TSMod.OnTSExeHirouSelectingBushouList と書き換えてやる実際の処理
00419DF3   . 83C4 18        ADD     ESP, 18
*/
char cmdOnTenshouExeJumpFromHirouSelectingBushouList[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeHirouSelectingBushouList() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeHirouSelectingBushouList;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeHirouSelectingBushouList + 5 );
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeHirouSelectingBushouList  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromHirouSelectingBushouList+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeHirouSelectingBushouList), cmdOnTenshouExeJumpFromHirouSelectingBushouList, 5, NULL); //5バイトのみ書き込む
}




