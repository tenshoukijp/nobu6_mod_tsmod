#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"


/*
00439237   . E8 1B10FDFF    CALL TENSHOU.0040A257
0043923C   . 83C4 04        ADD ESP,4
0043923F   . 50             PUSH EAX
00439240   . E8 6948FDFF    CALL TENSHOU.0040DAAE ←この結果で文字列へのポインタをEAXに得る。この40DAAE自体は汎用の関数なので中身いじらないこと。
													EAX 004D67EC ASCII "  ○"
00439245   . 83C4 08        ADD     ESP, 8



0040DAAE  /$ 56             PUSH ESI                                                 ;  TENSHOU.004E66E8
0040DAAF  |. E8 C5F6FFFF    CALL TENSHOU.0040D179
0040DAB4  |. 8BF0           MOV ESI,EAX
0040DAB6  |. 8B4424 08      MOV EAX,DWORD PTR SS:[ESP+8]
0040DABA  |. 83F8 02        CMP EAX,2
0040DABD  |. 72 07          JB SHORT TENSHOU.0040DAC6
0040DABF  |. B8 4CB94B00    MOV EAX,TENSHOU.004BB94C                                 ;  ASCII "－"
0040DAC4  |. EB 07          JMP SHORT TENSHOU.0040DACD
0040DAC6  |> 8B0485 A8134D0>MOV EAX,DWORD PTR DS:[EAX*4+4D13A8]
0040DACD  |> 50             PUSH EAX
0040DACE  |. FF7424 10      PUSH DWORD PTR SS:[ESP+10]
0040DAD2  |. 68 48B94B00    PUSH TENSHOU.004BB948                                    ;  ASCII "%*s"
0040DAD7  |. 56             PUSH ESI
0040DAD8  |. E8 4B000A00    CALL TENSHOU.004ADB28
0040DADD  |. 83C4 10        ADD ESP,10
0040DAE0  |. 8BC6           MOV EAX,ESI
0040DAE2  |. 5E             POP ESI
0040DAE3  \. C3             RETN
*/

// 特殊部隊名を得る
extern char* getSpecialHeitaiStringOfBushou(int iBushouID);

int iPointerOfViewingBushouListKibaTeppou = NULL; // 特殊部隊文字列へのポインタ
int iBushouIDOfViewingBushouListKibaTeppou = -1; // 対象となる武将のＩＤ。ECXのうち下半分のCXだけ使う

void OnTenshouExeViewingBushouListKibaTeppouExecute() {
	iBushouIDOfViewingBushouListKibaTeppou = (iBushouIDOfViewingBushouListKibaTeppou & 0xFFFF);
	int iBushouID = iBushouIDOfViewingBushouListKibaTeppou-1;

	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
		// 特殊な兵隊名を取得
		char *p = getSpecialHeitaiStringOfBushou(iBushouID);

		iPointerOfViewingBushouListKibaTeppou = (int)p; // そのアドレスを渡す。
	}
}


/*
00439237   . E8 1B10FDFF    CALL TENSHOU.0040A257
0043923C   . 83C4 04        ADD ESP,4
0043923F   . 50             PUSH EAX
00439240   . E8 6948FDFF    CALL TENSHOU.0040DAAE ←この結果で文字列へのポインタをEAXに得る。この40DAAE自体は汎用の関数なので中身いじらないこと。
													EAX 004D67EC ASCII "  ○"
				└ここを JMP TSMod.OnTSExeViewingBushouListKibaTeppou と書き換えてやる実際の処理
00439245   . 83C4 08        ADD     ESP, 8

*/
int pTenshouExeJumpFromToOnTenshouExeViewingBushouListKibaTeppou	 =0x439240; // 関数はこのアドレスから、OnTenshouExeViewingBushouListKibaTeppouへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeViewingBushouListKibaTeppou =0x40DAAE; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeViewingBushouListKibaTeppou	 =0x439245; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeViewingBushouListKibaTeppou() {
	// スタックにためておく
	__asm {
		// 武将ＩＤを確保
		mov iBushouIDOfViewingBushouListKibaTeppou, ecx

			// 元々TENSHOU.EXE内にあったものをここで
		call pTenshouExeJumpCallFromToOnTenshouExeViewingBushouListKibaTeppou

		// 元々のものを保存しておく。
		mov iPointerOfViewingBushouListKibaTeppou, eax

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
	OnTenshouExeViewingBushouListKibaTeppouExecute();

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

		// 騎馬鉄砲の、○とか×を、拡張された文字列で上書きする。
		mov eax, iPointerOfViewingBushouListKibaTeppou

		jmp pTenshouExeReturnLblFromOnTenshouExeViewingBushouListKibaTeppou
	}
}

/*
0049902C  |. 8BCE           MOV     ECX, ESI
0049902E  |. 68 B8334D00    PUSH    TENSHOU.004D33B8                 ;  ASCII "画面サイズを選択してください"
00499033  |. E8 16550100    CALL    TENSHOU.004AE54E
				└ここを JMP TSMod.OnTSExeViewingBushouListKibaTeppou と書き換えてやる実際の処理
00499038  |. 5E             POP     ESI
*/
char cmdOnTenshouExeJumpFromViewingBushouListKibaTeppou[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeViewingBushouListKibaTeppou() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeViewingBushouListKibaTeppou;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeViewingBushouListKibaTeppou + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeViewingBushouListKibaTeppou  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromViewingBushouListKibaTeppou+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeViewingBushouListKibaTeppou), cmdOnTenshouExeJumpFromViewingBushouListKibaTeppou, 5, NULL); //5バイトのみ書き込む
}






