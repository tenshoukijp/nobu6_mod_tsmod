

#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"












/*
 * セーブやロード時に、スロット番号を選択した時
 */



int iSelectSaveOrLoadNo = -1;

// セーブロードダイアログで選択している時だけ使える。
int getSelectSaveOrLoadNo() {
	return iSelectSaveOrLoadNo;
}


void OnTenshouExeSelectSaveOrLoadNoExecute() {
//	OutputDebugStream << iSelectSaveOrLoadNo << endl;
}


/*
004798D9  |. 43             INC     EBX
004798DA  |. 53             PUSH    EBX									← このEBXがスロットの番号(上から1で下が8)
004798DB  |. BE 789B4D00    MOV     ESI, TENSHOU.004D9B78            ;  ASCII "1:1560年 春  桶狭間の合戦 中級   1回起動"
004798E0  |. 68 B4F04C00    PUSH    TENSHOU.004CF0B4                 ;  ASCII "%d番目%sます"
004798E5  |. 56             PUSH    ESI
004798E6  |. E8 3D420300    CALL    TENSHOU.004ADB28
				└ここを JMP TSMod.OnTSExeSelectSaveOrLoadNo と書き換えてやる実際の処理
004798EB  |. 83C4 10        ADD     ESP, 10

*/
int pTenshouExeJumpFromToOnTenshouExeSelectSaveOrLoadNo		=0x4798E6; // 関数はこのアドレスから、OnTenshouExeSelectSaveOrLoadNoへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeSelectSaveOrLoadNo  =0x4ADB28; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeSelectSaveOrLoadNo	=0x4798EB; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeSelectSaveOrLoadNo() {
	// スタックにためておく
	__asm {
		mov iSelectSaveOrLoadNo, ebx

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
	OnTenshouExeSelectSaveOrLoadNoExecute();

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
		call pTenshouExeJumpCallFromToOnTenshouExeSelectSaveOrLoadNo

		jmp pTenshouExeReturnLblFromOnTenshouExeSelectSaveOrLoadNo
	}
}

/*
00446A7B  |. 57             PUSH    EDI
00446A7C  |. E8 3159FCFF    CALL    TENSHOU.0040C3B2
				└ここを JMP TSMod.OnTSExeSelectSaveOrLoadNo と書き換えてやる実際の処理
00446A81  |. 83C4 04        ADD     ESP, 4
00446A84  |. 85C0           TEST    EAX, EAX
00446A86  |. 74 45          JE      SHORT TENSHOU.00446ACD
*/
char cmdOnTenshouExeJumpFromSelectSaveOrLoadNo[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeSelectSaveOrLoadNo() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeSelectSaveOrLoadNo;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeSelectSaveOrLoadNo + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeSelectSaveOrLoadNo  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromSelectSaveOrLoadNo+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeSelectSaveOrLoadNo), cmdOnTenshouExeJumpFromSelectSaveOrLoadNo, 5, NULL); //5バイトのみ書き込む
}




