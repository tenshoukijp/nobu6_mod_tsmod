
#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"


/*
 * 商業時に弁舌をもつかどうかをジャッジし、商人ならば、弁舌をもたなくても、弁舌をもつことと同じとする。
 */



int iCurBushouIDInCommerceBenzetsuJudge = -1;
int bBenzetsuJudgeInCommerceBenzetsuJudge = 0;
void OnTenshouExeCommerceBenzetsuJudgeExecute() {

	// 武将が一定の範囲であること
	if (0 <= iCurBushouIDInCommerceBenzetsuJudge-1 && iCurBushouIDInCommerceBenzetsuJudge-1 < GAMEDATASTRUCT_BUSHOU_NUM) {

		// 商人であれば… 弁舌を持っていなくても持っていることとなる。
		if ( nb6bushouref[iCurBushouIDInCommerceBenzetsuJudge-1].job == 0x70 ) {
			bBenzetsuJudgeInCommerceBenzetsuJudge = 1;
		}
	}
}


/*
00446A7B  |. 57             PUSH    EDI
00446A7C  |. E8 3159FCFF    CALL    TENSHOU.0040C3B2
				└ここを JMP TSMod.OnTSExeCommerceBenzetsuJudge と書き換えてやる実際の処理
00446A81  |. 83C4 04        ADD     ESP, 4
00446A84  |. 85C0           TEST    EAX, EAX
00446A86  |. 74 45          JE      SHORT TENSHOU.00446ACD
*/
int pTenshouExeJumpFromToOnTenshouExeCommerceBenzetsuJudge		=0x446A7C; // 関数はこのアドレスから、OnTenshouExeCommerceBenzetsuJudgeへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeCommerceBenzetsuJudge  =0x40C3B2; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeCommerceBenzetsuJudge	=0x446A81; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeCommerceBenzetsuJudge() {
	// スタックにためておく
	__asm {
		// 元々TENSHOU.EXE内にあったものをここで
		call pTenshouExeJumpCallFromToOnTenshouExeCommerceBenzetsuJudge

		// 武将IDを保存
		mov iCurBushouIDInCommerceBenzetsuJudge, edi

		// 弁舌をもっているかどうかを保存
		mov bBenzetsuJudgeInCommerceBenzetsuJudge, eax

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
	OnTenshouExeCommerceBenzetsuJudgeExecute();

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

		// 商人か否かの結果をもって、弁舌情報を上書き
		mov eax, bBenzetsuJudgeInCommerceBenzetsuJudge 

		jmp pTenshouExeReturnLblFromOnTenshouExeCommerceBenzetsuJudge
	}
}

/*
00446A7B  |. 57             PUSH    EDI
00446A7C  |. E8 3159FCFF    CALL    TENSHOU.0040C3B2
				└ここを JMP TSMod.OnTSExeCommerceBenzetsuJudge と書き換えてやる実際の処理
00446A81  |. 83C4 04        ADD     ESP, 4
00446A84  |. 85C0           TEST    EAX, EAX
00446A86  |. 74 45          JE      SHORT TENSHOU.00446ACD
*/
char cmdOnTenshouExeJumpFromCommerceBenzetsuJudge[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeCommerceBenzetsuJudge() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeCommerceBenzetsuJudge;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeCommerceBenzetsuJudge + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeCommerceBenzetsuJudge  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromCommerceBenzetsuJudge+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeCommerceBenzetsuJudge), cmdOnTenshouExeJumpFromCommerceBenzetsuJudge, 5, NULL); //5バイトのみ書き込む
}




