#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"

// 戦争時の徴兵コマンドを選んだ際に、なぜか兵士が100にフィルターされてしまう関数がある。
// それが呼ばれた時の処理


// 兵隊の値。この値が０の時（足軽の時）が徴兵可能となる。この値を条件によって変えるのが目的。
int iConditionedForm = -1;

// 徴兵した対象の武将ID(配列上ではなく、属性の値である(配列より+1多い数))
int iConditionedBushouID = 0;

void OnTenshouExeSoldierDraftConditionInBattleExecute() {

	// 武将が一定の範囲であること
	if (0 <= iConditionedBushouID-1 && iConditionedBushouID-1 < GAMEDATASTRUCT_BUSHOU_NUM) {
		
		// ランスロット・タルタロスである。
		if ( isLancelotTartare(iConditionedBushouID-1) ) {
			// 兵態を0(=足軽)とすることで強制的に徴兵可能となる。
			iConditionedForm = 0;
		}

		// 朱伊である。
		if (isShuiHidechiyo(iConditionedBushouID - 1)) {
			// 兵態を0(=足軽)とすることで強制的に徴兵可能となる。
			iConditionedForm = 0;
		}

	}
}


/*
0041BBE9   . 83C4 04        ADD     ESP, 4										
0041BBEC   . 85C0           TEST    EAX, EAX				このEAXテストが0だと、兵数がMAXであるため、増やす必要がないとでる。EAXを１とすれば良い
0041BBEE   . 57             PUSH    EDI                              ; /Arg2										
0041BBEF   . 75 1E          JNZ     SHORT TENSHOU.0041BC0F           ; |										
0041BBF1   . 68 DC0F0000    PUSH    0FDC                             ; |Arg1 = 00000FDC										
0041BBF6   . E8 5D080500    CALL    TENSHOU.0046C458                 ; \TENSHOU.0046C458										
0041BBFB   . 83C4 08        ADD     ESP, 8										
0041BBFE   . 50             PUSH    EAX										
0041BBFF   . 57             PUSH    EDI										
0041BC00   . E8 42EA0700    CALL    TENSHOU.0049A647										
0041BC05   . 83C4 08        ADD     ESP, 8										
0041BC08   . 33C0           XOR     EAX, EAX										
0041BC0A   . E9 A2000000    JMP     TENSHOU.0041BCB1										
0041BC0F   > E8 75E2FEFF    CALL    TENSHOU.00409E89										
				└ここを JMP TSMod.OnTSExeSoldierDraftConditionInBattle と書き換えてやる実際の処理
0041BC14   . 83C4 04        ADD     ESP, 4										
0041BC17   . 85C0           TEST    EAX, EAX				このEAXテストが「0以外」だと、兵種類が足軽でないと徴兵できない。条件をなくすと、兵種に関わらず、徴兵が可能となる。
0041BC19   . 74 24          JE      SHORT TENSHOU.0041BC3F										
0041BC1B   . 33C0           XOR     EAX, EAX										
*/
int pTenshouExeJumpFromToOnTenshouExeSoldierDraftConditionInBattle		=0x41BC0F; // 関数はこのアドレスから、OnTenshouExeSoldierDraftConditionInBattleへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeSoldierDraftConditionInBattle  =0x409E89; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeSoldierDraftConditionInBattle	=0x41BC17; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeSoldierDraftConditionInBattle() {
	// スタックにためておく
	__asm {
		// 元々TENSHOU.EXE内にあったものをここで
		call pTenshouExeJumpCallFromToOnTenshouExeSoldierDraftConditionInBattle

		// 元々TENSHOU.EXE内にあったものをここで
		add esp, 4

		// 兵態の値が返ってくる。0ならば足軽隊とみなされるので、徴兵可能となる。
		mov iConditionedForm, eax

		// 武将IDをコピー
		mov iConditionedBushouID, edi

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}
	
	// 徴兵条件変更処理をするならば…
	OnTenshouExeSoldierDraftConditionInBattleExecute();

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

		// 置き換えた値でeaxを上書き
		mov eax, iConditionedForm

		jmp pTenshouExeReturnLblFromOnTenshouExeSoldierDraftConditionInBattle
	}
}

/*
0041BC0A   . E9 A2000000    JMP     TENSHOU.0041BCB1										
0041BC0F   > E8 75E2FEFF    CALL    TENSHOU.00409E89										
				└ここを JMP TSMod.OnTSExeSoldierDraftConditionInBattle と書き換えてやる実際の処理
0041BC14   . 83C4 04        ADD     ESP, 4										
*/
char cmdOnTenshouExeJumpFromSoldierDraftConditionInBattle[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeSoldierDraftConditionInBattle() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeSoldierDraftConditionInBattle;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeSoldierDraftConditionInBattle + 5 );
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeSoldierDraftConditionInBattle  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromSoldierDraftConditionInBattle+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeSoldierDraftConditionInBattle), cmdOnTenshouExeJumpFromSoldierDraftConditionInBattle, 5, NULL); //5バイトのみ書き込む
}




