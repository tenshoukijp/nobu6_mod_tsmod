#define _CRT_SECURE_NO_WARNINGS

#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"

#include "SetOverWriteButaiMei.h"


/*
 * メイン画面の武将リストを開こうとした時
 */ 


/*
0043919C   . E8 E80CFDFF    CALL    TENSHOU.00409E89
004391A1   . 83C4 04        ADD     ESP, 4
004391A4   . 8B0485 80134D0>MOV     EAX, DWORD PTR DS:[EAX*4+4D1380] ;  TENSHOU.004BBC2C
004391AB   . EB 0A          JMP     SHORT TENSHOU.004391B7
*/

// 表示しようとしている武将番号
int iBushouIDOnTenshouExeViewingMainBushouListButaimeiExecute = -1;
extern int pOrgButaiMeiArrayOnExecute; // 元々TENSHOU.EXE内にあったCALL先

extern int pNewButaiMeiArrayOnExecute;
extern int pOrgButaiFormNo;

void OnTenshouExeViewingMainBushouListButaimeiExecute() {
	// ESIのうち、下のSIの部分だけが武将番号に相当する
	iBushouIDOnTenshouExeViewingMainBushouListButaimeiExecute = iBushouIDOnTenshouExeViewingMainBushouListButaimeiExecute & 0xFFFF;

	int iBushouID = iBushouIDOnTenshouExeViewingMainBushouListButaimeiExecute - 1;

	pOrgButaiMeiArrayOnExecute = 0x4D1380;

	// 部隊名配列ポインタを上書きする。
	int p = GetOverWriteButaiMeiArrayPointer(iBushouID);
	if ( p ) {
		pNewButaiMeiArrayOnExecute = p + pOrgButaiFormNo*4;

	} else {
		// 部隊名ラベルは必ずオリジナルで上書きされる。
		pNewButaiMeiArrayOnExecute = 0x4D1380 + pOrgButaiFormNo*4;
	}
}



/*
// この段階でESIに武将番号が入っているが、実際の武将番号はSI部分(下位ビット)
0043919C   . E8 E80CFDFF    CALL    TENSHOU.00409E89
				└ここを JMP TSMod.OnTSExeViewingMainBushouListButaimei と書き換えてやる実際の処理
004391A1   . 83C4 04        ADD     ESP, 4
004391A4   . 8B0485 80134D0>MOV     EAX, DWORD PTR DS:[EAX*4+4D1380] ;  TENSHOU.004BBC2C
004391AB   . EB 0A          JMP     SHORT TENSHOU.004391B7
*/
int pTenshouExeJumpFromToOnTenshouExeViewingMainBushouListButaimei	   = 0x43919C; // 関数はこのアドレスから、OnTenshouExeViewingMainBushouListButaimeiへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeViewingMainBushouListButaimei = 0x409E89; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeViewingMainBushouListButaimei  = 0x4391AB; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeViewingMainBushouListButaimei() {
	// スタックにためておく
	__asm {
		// 元の処理をここに記載
		call pTenshouExeJumpCallFromToOnTenshouExeViewingMainBushouListButaimei

		// 元の処理
		ADD     ESP, 4

		// nb6bushou[iBushouID].form 相当を保存
		mov pOrgButaiFormNo, eax

		// 武将番号の保存
		mov iBushouIDOnTenshouExeViewingMainBushouListButaimeiExecute, esi

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
	OnTenshouExeViewingMainBushouListButaimeiExecute();

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

		// pNewButaiMeiArrayOnExecuteは少なくともオリジナルか、上書きされた、char *[4]の配列。
		mov eax, pNewButaiMeiArrayOnExecute
		mov eax, [eax]

		jmp pTenshouExeReturnLblFromOnTenshouExeViewingMainBushouListButaimei
	}
}




char cmdOnTenshouExeJumpFromViewingMainBushouListButaimei[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計5バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeViewingMainBushouListButaimei() {

	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeViewingMainBushouListButaimei;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeViewingMainBushouListButaimei + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeViewingMainBushouListButaimei  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromViewingMainBushouListButaimei+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeViewingMainBushouListButaimei), cmdOnTenshouExeJumpFromViewingMainBushouListButaimei, 5, NULL); //5バイトのみ書き込む
}




