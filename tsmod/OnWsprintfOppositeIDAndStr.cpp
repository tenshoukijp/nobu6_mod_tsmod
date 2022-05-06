#include "WinTarget.h"
#include <fstream>

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"
#include "MessageN6PPatch.h"

using namespace std;



/*
 * 会話中に｢相手の名前｣をIDから格納する必要があるタイミング(Wsprintfへの変数格納)で呼び出される
 */

// 相手の武将の名前が格納されている文字列へのアドレス
int iStrAddressOppositeBushouName = -1;

// 相手の武将のID
int iOppositeBushouID = -1;

void OnTenshouExeWsprintfOppositeIDAndStrExecute() {
	// 特に何もなし。Wsprintfへのパラメタ埋めを直接意識する必要が出れば考える。
}


/*
0046B8F2  |. 50             PUSH    EAX				この段階で、ESIは相手のID, EAXが相手の名前				
0046B8F3  |. FF75 0C        PUSH    DWORD PTR SS:[EBP+C]								
0046B8F6  |. E8 F2180400    CALL    TENSHOU.004AD1ED								
				└ここを JMP TSMod.OnTSExeWsprintfOppositeIDAndStr と書き換えてやる実際の処理
0046B8FB  |. 83C4 08        ADD     ESP, 8								
0046B8FE  |> 33C0           XOR     EAX, EAX								
0046B900  |. EB 0E          JMP     SHORT TENSHOU.0046B910								
 */
int pTenshouExeJumpFromToOnTenshouExeWsprintfOppositeIDAndStr    =0x46B8F6; // 関数はこのアドレスから、OnTenshouExeWsprintfOppositeIDAndStrへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeWsprintfOppositeIDAndStr=0x4AD1ED; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeWsprintfOppositeIDAndStr =0x46B8FB; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeWsprintfOppositeIDAndStr() {
	// スタックにためておく
	__asm {
		// 元々TENSHOU.EXE内にあったものをここで
		call pTenshouExeJumpCallFromToOnTenshouExeWsprintfOppositeIDAndStr

		// 相手の武将のIDをコピー
		mov iOppositeBushouID, esi;

		// 相手の武将の名前が格納されているアドレスをコピー
		mov iStrAddressOppositeBushouName, eax;

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
	OnTenshouExeWsprintfOppositeIDAndStrExecute();

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

		jmp pTenshouExeReturnLblFromOnTenshouExeWsprintfOppositeIDAndStr
	}
}

/*
00444769  |. 50             PUSH    EAX
0044476A  |. E8 3A2C0200    CALL    TENSHOU.004673A9
				└ここを JMP TSMod.OnTSExeWsprintfOppositeIDAndStr と書き換えてやる実際の処理
0044476F  |. 83C4 04        ADD     ESP, 4
*/
char cmdOnTenshouExeJumpFromWsprintfOppositeIDAndStr[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeWsprintfOppositeIDAndStr() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeWsprintfOppositeIDAndStr;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeWsprintfOppositeIDAndStr + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeWsprintfOppositeIDAndStr  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromWsprintfOppositeIDAndStr+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeWsprintfOppositeIDAndStr), cmdOnTenshouExeJumpFromWsprintfOppositeIDAndStr, 5, NULL); //5バイトのみ書き込む
}




