#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"


// 家宝のIDがEAXでわたってくる。家宝番号。よって配列で使うためには、-1すること。
int iKahouIDOnViewingKahouDetail = -1;
int iKahouIDOnViewingKahouID = 0;
void OnTenshouExeViewingKahouDetailExecute() {
	// 家宝の範囲かどうかをチェックして…
	if ( 0 <= iKahouIDOnViewingKahouDetail-1 && iKahouIDOnViewingKahouDetail-1 < GAMEDATASTRUCT_KAHOU_NUM ) {
		// 番号→配列用のIDにしておく。
		iKahouIDOnViewingKahouID = iKahouIDOnViewingKahouDetail -1;
	}
}


/*
00494ABE  /$ 8B4424 04      MOV     EAX, DWORD PTR SS:[ESP+4]
00494AC2  |. 8B5424 08      MOV     EDX, DWORD PTR SS:[ESP+8]
00494AC6  |. 8941 70        MOV     DWORD PTR DS:[ECX+70], EAX
00494AC9  |. 8951 74        MOV     DWORD PTR DS:[ECX+74], EDX
00494ACC  |. E8 1F15F7FF    CALL    TENSHOU.00405FF0
				└ここを JMP TSMod.OnTSExeViewingKahouDetail と書き換えてやる実際の処理
00494AD1  \. C2 0800        RETN    8
*/
int pTenshouExeJumpFromToOnTenshouExeViewingKahouDetail		=0x494ACC; // 関数はこのアドレスから、OnTenshouExeViewingKahouDetailへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeViewingKahouDetail =0x405FF0; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeViewingKahouDetail	=0x494AD1; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeViewingKahouDetail() {
	// スタックにためておく
	__asm {

		// 見ようとしている家宝番号がEAXに入っている。
		mov iKahouIDOnViewingKahouDetail, eax		

		// 元々TENSHOU.EXE内にあったものをここで
		call pTenshouExeJumpCallFromToOnTenshouExeViewingKahouDetail

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
	OnTenshouExeViewingKahouDetailExecute();

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

		jmp pTenshouExeReturnLblFromOnTenshouExeViewingKahouDetail
	}
}

/*
00494ABE  /$ 8B4424 04      MOV     EAX, DWORD PTR SS:[ESP+4]
00494AC2  |. 8B5424 08      MOV     EDX, DWORD PTR SS:[ESP+8]
00494AC6  |. 8941 70        MOV     DWORD PTR DS:[ECX+70], EAX
00494AC9  |. 8951 74        MOV     DWORD PTR DS:[ECX+74], EDX
00494ACC  |. E8 1F15F7FF    CALL    TENSHOU.00405FF0
				└ここを JMP TSMod.OnTSExeViewingKahouDetail と書き換えてやる実際の処理
00494AD1  \. C2 0800        RETN    8
*/
char cmdOnTenshouExeJumpFromViewingKahouDetail[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeViewingKahouDetail() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeViewingKahouDetail;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeViewingKahouDetail + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeViewingKahouDetail  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromViewingKahouDetail+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeViewingKahouDetail), cmdOnTenshouExeJumpFromViewingKahouDetail, 5, NULL); //5バイトのみ書き込む
}




