#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"



/*
00427648   . E8 89070600    CALL    TENSHOU.00487DD6												
0042764D   . 66:A1 F4E54B00 MOV     AX, WORD PTR DS:[4BE5F4]										←この4BE5F4に２バイト型で武将情報が入っている
00427653   . 83C4 04        ADD     ESP, 4												
00427656   . 56             PUSH    ESI                              ; /Arg4												
00427657   . 6A 00          PUSH    0                                ; |Arg3 = 00000000												
00427659   . 6A 1C          PUSH    1C                               ; |Arg2 = 0000001C												
0042765B   . 50             PUSH    EAX                              ; |Arg1												
0042765C   . E8 66B20200    CALL    TENSHOU.004528C7                 ; \TENSHOU.004528C7												
00427661   . 83C4 10        ADD     ESP, 10												
00427664   . E8 AAF8FFFF    CALL    TENSHOU.00426F13												←この段階で、4CB618に4バイト型でお金が入っている。
00427669   . B9 FFFF0000    MOV     ECX, 0FFFF												
0042766E   . 8D50 FC        LEA     EDX, DWORD PTR DS:[EAX-4]												
*/



// 朝廷工作の際のお金
int iMoneyOnEmperorManeuvers = 0;
WORD iEmperorManeuversBushouID = 0;


#define BUSHOU_ONEMPEROR_MANEUVERS_ADRESSS	0x4BE5F4
#define MONEY_ONEMPEROR_MANEUVERS_ADRESSS	0x4CB618

void OnTenshouExeEmperorManeuversExecute() {

	
	// 武将のIDを得る
	ReadProcessMemory( 
		CommonGlobalHandle::curProcess,
		(LPCVOID)(BUSHOU_ONEMPEROR_MANEUVERS_ADRESSS), 
		&iEmperorManeuversBushouID, 
		2,
		NULL);

	// お金の金額を得る
	ReadProcessMemory( 
		CommonGlobalHandle::curProcess,
		(LPCVOID)(MONEY_ONEMPEROR_MANEUVERS_ADRESSS), 
		&iMoneyOnEmperorManeuvers, 
		4,
		NULL);

	if (0 <= iEmperorManeuversBushouID-1 && iEmperorManeuversBushouID-1 < GAMEDATASTRUCT_BUSHOU_NUM ) { 

		// 高家であるならば…
		if (nb6bushouref[iEmperorManeuversBushouID-1].job == 0x60) {

			// 効果を倍にする
			iMoneyOnEmperorManeuvers = iMoneyOnEmperorManeuvers * 3; // 3倍の結果6x3=18万貢いだだことになってもオーバーフローは心配しなくてもよい
		}

		// 庶民であるならば…
		if (nb6bushouref[iEmperorManeuversBushouID-1].job == 0x80) {

			// 金1の時に、0にしてしまわないように(手土産の欄がなくなってしまうことの回避)
			if ( iMoneyOnEmperorManeuvers >= 3 ) {
				// 効果を半分にする
				iMoneyOnEmperorManeuvers = iMoneyOnEmperorManeuvers / 3;
			}
		}
	}

}


/*
00426FDC  |. 0305 18B64C00  ADD     EAX, DWORD PTR DS:[4CB618]
				└ここを JMP TSMod.OnTSExeEmperorManeuvers と書き換えてやる実際の処理
00426FE2  |. 2BD2           SUB     EDX, EDX
00426FE4  |. F7F1           DIV     ECX
*/
int pTenshouExeJumpFromToOnTenshouExeEmperorManeuvers		=0x426FDC; // 関数はこのアドレスから、OnTenshouExeEmperorManeuversへとジャンプしてくる。
int pTenshouExeReturnLblFromOnTenshouExeEmperorManeuvers	=0x426FE2; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeEmperorManeuvers() {
	// スタックにためておく
	__asm {

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
	OnTenshouExeEmperorManeuversExecute();

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
		add eax, iMoneyOnEmperorManeuvers

		jmp pTenshouExeReturnLblFromOnTenshouExeEmperorManeuvers
	}
}

/*
00426FDC  |. 0305 18B64C00  ADD     EAX, DWORD PTR DS:[4CB618]
				└ここを JMP TSMod.OnTSExeEmperorManeuvers と書き換えてやる実際の処理
00426FE2  |. 2BD2           SUB     EDX, EDX
00426FE4  |. F7F1           DIV     ECX
*/
char cmdOnTenshouExeJumpFromEmperorManeuvers[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeEmperorManeuvers() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeEmperorManeuvers;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeEmperorManeuvers + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeEmperorManeuvers  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromEmperorManeuvers+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeEmperorManeuvers), cmdOnTenshouExeJumpFromEmperorManeuvers, 5, NULL); //5バイトのみ書き込む
}



