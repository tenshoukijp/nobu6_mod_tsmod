#include "WinTarget.h"
#include <stdlib.h>
#include <string.h>

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"



// 大砲もしくは鉄甲船を撃とうとしている武将のID
int iTaihouAndTekkousenAttackTimesRewriteBushouID = -1;

// 大砲もしくは鉄甲船の発射回数
int iTaihouAndTekkousenAttackTimesRewriteCanonFireCnt = 1;

void OnTenshouExeTaihouAndTekkousenAttackTimesRewriteExecute() {

	// iTaihouAndTekkousenAttackTimesRewriteCanonFireCnt == 0 の時は、元々ここにくるはずではなかった。決して処理してはいけない!!
	if ( iTaihouAndTekkousenAttackTimesRewriteCanonFireCnt == 0 ) {
		return;
	}

	// 武将が一定の範囲であること
	if (0 <= iTaihouAndTekkousenAttackTimesRewriteBushouID-1 && iTaihouAndTekkousenAttackTimesRewriteBushouID-1 < GAMEDATASTRUCT_BUSHOU_NUM) {

		// タルタロスであれば、大砲、もしくは、鉄甲船の発射回数を、アイテム如何にかかわらず、２回増やす
		if ( isLancelotTartare(iTaihouAndTekkousenAttackTimesRewriteBushouID-1) ) {
			//
			iTaihouAndTekkousenAttackTimesRewriteCanonFireCnt += 2;
		}

		// 朱伊であれば、大砲、もしくは、鉄甲船の発射回数を、アイテム如何にかかわらず、1回増やす
		else if (isShuiHidechiyo(iTaihouAndTekkousenAttackTimesRewriteBushouID - 1)) {
			//
			iTaihouAndTekkousenAttackTimesRewriteCanonFireCnt += 1;
		}

	}
}


/*
00432B40  |> 85C0           TEST    EAX, EAX
00432B42  |. 0F84 4D010000  JE      TENSHOU.00432C95
00432B48  |. 8945 E0        MOV     DWORD PTR SS:[EBP-20], EAX	       大砲もしくは鉄甲船の回数を代入。ebpアドレスあたりが、この瞬間利用している戦争中の該当武将の一時情報らしい。そのebp-20が発射回数の現在の残り回数の場所。		
				└ここを JMP TSMod.OnTSExeTaihouAndTekkousenAttackTimesRewrite と書き換えてやる実際の処理
00432B4B  |> 8D45 C4        /LEA     EAX, DWORD PTR SS:[EBP-3C]		　 ←ここはループの頭で別の行からジャンプ参照してるところがあるので、関数内にとりこんではいけない。								
 */
int pTenshouExeJumpFromToOnTenshouExeTaihouAndTekkousenAttackTimesRewrite    =0x432B40; // 関数はこのアドレスから、OnTenshouExeTaihouAndTekkousenAttackTimesRewriteへとジャンプしてくる。
int pTenshouExeReturnLblFromOnTenshouExeTaihouAndTekkousenAttackTimesRewrite =0x432B48; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
int pTenshouExeJumpLblFromOnTenshouExeTaihouAndTekkousenAttackTimesNoNeedRewrite =0x432C95; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeTaihouAndTekkousenAttackTimesRewrite() {
	// スタックにためておく
	__asm {
		// 大砲もしくは鉄甲船の打つ回数
		mov iTaihouAndTekkousenAttackTimesRewriteCanonFireCnt, eax;

		// 大砲もしくは鉄甲船をうとうとしている武将のID
		mov iTaihouAndTekkousenAttackTimesRewriteBushouID, esi;

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
	OnTenshouExeTaihouAndTekkousenAttackTimesRewriteExecute();

	// 元来ここに来るべきではなかった場合
	if ( iTaihouAndTekkousenAttackTimesRewriteCanonFireCnt == 0 ) {

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

			// 元々 test eax, eax の結果が0であった場合に飛ぶべきだったはずの場所へと帰る。
			jmp pTenshouExeJumpLblFromOnTenshouExeTaihouAndTekkousenAttackTimesNoNeedRewrite
		}

	}

	if ( iTaihouAndTekkousenAttackTimesRewriteCanonFireCnt != 0 ) {

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
			// 発射回数を計算によるもので上書き。
			mov eax, iTaihouAndTekkousenAttackTimesRewriteCanonFireCnt

			jmp pTenshouExeReturnLblFromOnTenshouExeTaihouAndTekkousenAttackTimesRewrite
		}
	}
}

/*
00432B48  |. 8945 E0        MOV     DWORD PTR SS:[EBP-20], EAX									大砲もしくは鉄甲船の回数を代入。		
				└ここを JMP TSMod.OnTSExeTaihouAndTekkousenAttackTimesRewrite と書き換えてやる実際の処理
00432B4B  |> 8D45 C4        /LEA     EAX, DWORD PTR SS:[EBP-3C]											
*/
char cmdOnTenshouExeJumpFromTaihouAndTekkousenAttackTimesRewrite[9]="\xE9\x90\x90\x90\x90\x90\x90\x90";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeTaihouAndTekkousenAttackTimesRewrite() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeTaihouAndTekkousenAttackTimesRewrite;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeTaihouAndTekkousenAttackTimesRewrite + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeTaihouAndTekkousenAttackTimesRewrite  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromTaihouAndTekkousenAttackTimesRewrite+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeTaihouAndTekkousenAttackTimesRewrite), cmdOnTenshouExeJumpFromTaihouAndTekkousenAttackTimesRewrite, 8, NULL); //11バイトのみ書き込む
}




