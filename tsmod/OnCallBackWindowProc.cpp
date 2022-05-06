#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"



/*
0044ACD1  /$ 55             PUSH    EBP
0044ACD2  |. 8B4424 0C      MOV     EAX, DWORD PTR SS:[ESP+C]
0044ACD6  |. 8BEC           MOV     EBP, ESP
0044ACD8  |. 83F8 0F        CMP     EAX, 0F
0044ACDB  |. 53             PUSH    EBX
0044ACDC  |. 56             PUSH    ESI
0044ACDD  |. 57             PUSH    EDI
0044ACDE  |. 77 2F          JA      SHORT TENSHOU.0044AD0F
0044ACE0  |. 0F84 4E010000  JE      TENSHOU.0044AE34
0044ACE6  |. 83F8 02        CMP     EAX, 2
0044ACE9  |. 0F84 9E000000  JE      TENSHOU.0044AD8D
0044ACEF  |. 83F8 03        CMP     EAX, 3
0044ACF2  |. 0F84 AD000000  JE      TENSHOU.0044ADA5
0044ACF8  |. 83F8 06        CMP     EAX, 6
0044ACFB  |. 0F84 10010000  JE      TENSHOU.0044AE11
0044AD01  |. 83F8 07        CMP     EAX, 7
0044AD04  |. 0F84 2A010000  JE      TENSHOU.0044AE34
0044AD0A  |. E9 B4040000    JMP     TENSHOU.0044B1C3
0044AD0F  |> 83F8 13        CMP     EAX, 13
0044AD12  |. 77 14          JA      SHORT TENSHOU.0044AD28
0044AD14  |. 0F84 A3010000  JE      TENSHOU.0044AEBD
0044AD1A  |. 83F8 10        CMP     EAX, 10
0044AD1D  |. 0F84 4D010000  JE      TENSHOU.0044AE70
0044AD23  |. E9 9B040000    JMP     TENSHOU.0044B1C3
0044AD28  |> 83F8 1C        CMP     EAX, 1C
0044AD2B  |. 77 1D          JA      SHORT TENSHOU.0044AD4A
0044AD2D  |. 0F84 17020000  JE      TENSHOU.0044AF4A
0044AD33  |. 83F8 14        CMP     EAX, 14
0044AD36  |. 0F84 CC010000  JE      TENSHOU.0044AF08
0044AD3C  |. 83F8 16        CMP     EAX, 16
0044AD3F  |. 0F84 E4010000  JE      TENSHOU.0044AF29
0044AD45  |. E9 79040000    JMP     TENSHOU.0044B1C3
0044AD4A  |> 83F8 24        CMP     EAX, 24
0044AD4D  |. 0F84 8C020000  JE      TENSHOU.0044AFDF
0044AD53  |. 83F8 7E        CMP     EAX, 7E
0044AD56  |. 0F84 C0020000  JE      TENSHOU.0044B01C
0044AD5C  |. 3D 00010000    CMP     EAX, 100
0044AD61  |. 0F84 4A030000  JE      TENSHOU.0044B0B1
0044AD67  |. 3D 02010000    CMP     EAX, 102
0044AD6C  |. 0F84 CB030000  JE      TENSHOU.0044B13D
0044AD72  |. 3D B9030000    CMP     EAX, 3B9				← MM_MCINOTIFY
0044AD77  |. 0F84 D2030000  JE      TENSHOU.0044B14F
0044AD7D  |. 3D E8070000    CMP     EAX, 7E8
*/

/*
 いわゆる LRESULT CALLBACK WindowProc() 関数
 */



int uMsgOfCallBackWindowProcExecute = -1;	// uMsgの値が入っている。WM_**** 計と比較する値
void OnTenshouExeCallBackWindowProcExecute() {

}


/*
0044ACD1  /$ 55             PUSH    EBP
				└ここを JMP TSMod.OnTSExeCallBackWindowProc と書き換えてやる実際の処理
0044ACD2  |. 8B4424 0C      MOV     EAX, DWORD PTR SS:[ESP+C]
0044ACD6  |. 8BEC           MOV     EBP, ESP
0044ACD8  |. 83F8 0F        CMP     EAX, 0F
*/
int pTenshouExeJumpFromToOnTenshouExeCallBackWindowProc		=0x44ACD1; // 関数はこのアドレスから、OnTenshouExeCallBackWindowProcへとジャンプしてくる。
int pTenshouExeReturnLblFromOnTenshouExeCallBackWindowProc	=0x44ACD6; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeCallBackWindowProc() {
	// スタックにためておく
	__asm {
		// 元々TENSHOU.EXE内にあったものをここで
		PUSH EBP
		MOV EAX, DWORD PTR SS:[ESP+0xC]

		// EAXがuMsgの値なので保存
		mov uMsgOfCallBackWindowProcExecute, EAX

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
	OnTenshouExeCallBackWindowProcExecute();

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

		jmp pTenshouExeReturnLblFromOnTenshouExeCallBackWindowProc
	}
}


char cmdOnTenshouExeJumpFromCallBackWindowProc[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeCallBackWindowProc() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeCallBackWindowProc;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeCallBackWindowProc + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeCallBackWindowProc  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromCallBackWindowProc+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeCallBackWindowProc), cmdOnTenshouExeJumpFromCallBackWindowProc, 5, NULL); //5バイトのみ書き込む
}




