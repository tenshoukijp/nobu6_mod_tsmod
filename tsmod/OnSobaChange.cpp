#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"
#include "ScenarioMod.h"
#include "PluginMod.h"

/*
0044476F  |. 83C4 04        ADD     ESP, 4
00444772  |. E8 03230200    CALL    TENSHOU.00466A7A
00444777  |. 8BF0           MOV     ESI, EAX
00444779  |.-E9 3C47EBFF    JMP     TSMod.JmpOnTSExeSobaChange  米相場
0044477E  |. B9 03000000    MOV     ECX, 3
00444783  |. 03C6           ADD     EAX, ESI
00444785  |. 2BD2           SUB     EDX, EDX
00444787  |. F7F1           DIV     ECX
00444789  |. 8BF2           MOV     ESI, EDX
0044478B  |. E8 472C0200    CALL    TENSHOU.004673D7		─兵相場読み込み
00444790  |. 50             PUSH    EAX                              ; /Arg4
00444791     6A 00          PUSH    0
00444793     6A 00          PUSH    0
00444795  |. 56             PUSH    ESI                              ; |Arg1
00444796  |. E8 3B010000    CALL    TENSHOU.004448D6                 ; \TENSHOU.004448D6
0044479B  |. 83C4 10        ADD     ESP, 10
0044479E  |. 50             PUSH    EAX
0044479F  |. E8 3B2C0200    CALL    TENSHOU.004673DF　　　　─兵相場書き込み
004447A4  |. 83C4 04        ADD     ESP, 4
004447A7  |. E8 192C0200    CALL    TENSHOU.004673C5　　　　─馬相場読み込み
004447AC  |. 50             PUSH    EAX                              ; /Arg4
004447AD     6A 00          PUSH    0
004447AF     6A 00          PUSH    0
004447B1  |. 56             PUSH    ESI                              ; |Arg1
004447B2  |. E8 1F010000    CALL    TENSHOU.004448D6                 ; \TENSHOU.004448D6
004447B7  |. 83C4 10        ADD     ESP, 10
004447BA  |. 50             PUSH    EAX
004447BB  |. E8 0D2C0200    CALL    TENSHOU.004673CD		─馬相場書き込み
004447C0  |. 83C4 04        ADD     ESP, 4
004447C3  |. E8 EB2B0200    CALL    TENSHOU.004673B3		─鉄砲相場読み込み
004447C8  |. 50             PUSH    EAX                              ; /Arg4
004447C9     6A 00          PUSH    0
004447CB     6A 00          PUSH    0
004447CD  |. 56             PUSH    ESI                              ; |Arg1
004447CE  |. E8 03010000    CALL    TENSHOU.004448D6                 ; \TENSHOU.004448D6
004447D3  |. 83C4 10        ADD     ESP, 10
004447D6  |. 50             PUSH    EAX
004447D7  |. E8 DF2B0200    CALL    TENSHOU.004673BB		─鉄砲相場書き込み
004447DC  |. 83C4 04        ADD     ESP, 4

00444803  |. 83C4 0C        ADD     ESP, 0C					─本当の兵相場書き込み
00444806  |. 50             PUSH    EAX
00444807  |. E8 D32B0200    CALL    TENSHOU.004673DF



0044482A  |. 50             PUSH    EAX						─本当の馬相場書き込み
0044482B  |. E8 9D2B0200    CALL    TENSHOU.004673CD
00444830  |. 83C4 04        ADD     ESP, 4


00444846  |. E8 E0000000    CALL    TENSHOU.0044492B                 ; \TENSHOU.0044492B
0044484B  |. 83C4 0C        ADD     ESP, 0C
0044484E  |. 50             PUSH    EAX
0044484F  |. E8 672B0200    CALL    TENSHOU.004673BB        ─本当の鉄砲相場書き込み
00444854  |. 83C4 04        ADD     ESP, 4


*/


// 相場が意図的に修正された。
NB6SOBA modify_soba;

void OnTenshouExeSobaChangeExecute() {

	// 毎季、各相場が変更されたかチェック。変更されたフラグをリセット
	modify_soba.rise    = 0;
	modify_soba.gun     = 0;
	modify_soba.horse   = 0;
	modify_soba.soldier = 0;

	// 相場変更変更時に行うサービス
	EXSuperService_OnSobaChange();

	// PluginMod系の０個以上（複数可能）なＤＬＬのメインゲーム開始時のメソッドを呼び出す。
	// 実行対象となっているDLLを順に捜査して…
	for ( vector<HMODULE>::iterator it = listOfTargetDLL.begin(); it != listOfTargetDLL.end(); it++ ) {

		// 対象のDLLにMainGameStart関数が存在するかをチェック。
		PFNTARGETMODCHANGESOBA p_ChangeSoba = (PFNTARGETMODCHANGESOBA)GetProcAddress(*it, "OnSobaChange");

		if ( !p_ChangeSoba ) {
			continue;
		}

		((PFNTARGETMODCHANGESOBA) p_ChangeSoba)();
	}

	// SenarioModで相場を強制上書きする。
	if ( p_snOnChangeSoba ) {
		((PFNSNONCHANGESOBA) p_snOnChangeSoba)(&modify_soba);
	}

	// 米の相場を強制変更しているならば、それに基づいて上書き。
	if ( modify_soba.rise ) {
		nb6soba.rise = modify_soba.rise;
	}
}


/*
0044476F  |. 83C4 04        ADD     ESP, 4
00444772  |. E8 03230200    CALL    TENSHOU.00466A7A
00444777  |. 8BF0           MOV     ESI, EAX
00444779  |. E8 6B2C0200    CALL    TENSHOU.004673E9
				└ここを JMP TSMod.OnTSExeSobaChange と書き換えてやる実際の処理
0044477E  |. B9 03000000    MOV     ECX, 3

 */
int pTenshouExeJumpFromToOnTenshouExeSobaChange    =0x444779; // 関数はこのアドレスから、OnTenshouExeSobaChangeへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeSobaChange=0x4673E9; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeSobaChange =0x44477E; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeSobaChange() {
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
	OnTenshouExeSobaChangeExecute();

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
		call pTenshouExeJumpCallFromToOnTenshouExeSobaChange

		jmp pTenshouExeReturnLblFromOnTenshouExeSobaChange
	}
}

/*
00444769  |. 50             PUSH    EAX
0044476A  |. E8 3A2C0200    CALL    TENSHOU.004673A9
				└ここを JMP TSMod.OnTSExeSobaChange と書き換えてやる実際の処理
0044476F  |. 83C4 04        ADD     ESP, 4
*/
char cmdOnTenshouExeJumpFromSobaChange[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeSobaChange() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeSobaChange;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeSobaChange + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeSobaChange  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromSobaChange+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeSobaChange), cmdOnTenshouExeJumpFromSobaChange, 5, NULL); //5バイトのみ書き込む
}




//------------------------------------------------------------------------------------------


int SobaSoldierChange = 0;
void OnTenshouExeSobaSoldierChangeExecute() {
	// 相場を上書きしていれば…
	if ( modify_soba.soldier ) {
		SobaSoldierChange = modify_soba.soldier;
		nb6soba.soldier = modify_soba.soldier;
	}
}


/*
00444803  |. 83C4 0C        ADD     ESP, 0C					─本当の兵相場書き込み
00444806  |. 50             PUSH    EAX
00444807  |. E8 D32B0200    CALL    TENSHOU.004673DF
└ここを JMP TSMod.OnTSExeSobaSoldierChange と書き換えてやる実際の処理
0044480C  |. 83C4 04        ADD     ESP, 4

 */
int pTenshouExeJumpFromToOnTenshouExeSobaSoldierChange    =0x444807; // 関数はこのアドレスから、OnTenshouExeSobaSoldierChangeへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeSobaSoldierChange=0x4673DF; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeSobaSoldierChange =0x44480C; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeSobaSoldierChange() {
	// スタックにためておく
	__asm {
		// 兵士の相場を保存
		mov SobaSoldierChange, eax;

		// プッシュしたのはなかったことにする
		pop eax;

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}
	
	OnTenshouExeSobaSoldierChangeExecute();

	// 兵士の相場がプログラムで変更された。
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

		// 値を上書きしているのを反映
		mov eax, SobaSoldierChange

		push eax; // 元々TENSHOU.EXEにあったpush命令

		// 元々TENSHOU.EXE内にあったものをここで
		call pTenshouExeJumpCallFromToOnTenshouExeSobaSoldierChange

		jmp pTenshouExeReturnLblFromOnTenshouExeSobaSoldierChange
	}
}

/*
0044479B  |. 83C4 10        ADD     ESP, 10
0044479E  |. 50             PUSH    EAX
0044479F  |. E8 3B2C0200    CALL    TENSHOU.004673DF
				└ここを JMP TSMod.OnTSExeSobaSoldierChange と書き換えてやる実際の処理
004447A4  |. 83C4 04        ADD     ESP, 4

 */
char cmdOnTenshouExeJumpFromSobaSoldierChange[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeSobaSoldierChange() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeSobaSoldierChange;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeSobaSoldierChange + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeSobaSoldierChange  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromSobaSoldierChange+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeSobaSoldierChange), cmdOnTenshouExeJumpFromSobaSoldierChange, 5, NULL); //5バイトのみ書き込む
}






//------------------------------------------------------------------------------------------




int SobaHorseChange = 0;
void OnTenshouExeSobaHorseChangeExecute() {
	// 相場を上書きしていれば…
	if ( modify_soba.horse ) {
		SobaHorseChange = modify_soba.horse;
		nb6soba.horse = modify_soba.horse;
	}
}


/*
0044482A  |. 50             PUSH    EAX						─本当の馬相場書き込み
0044482B  |. E8 9D2B0200    CALL    TENSHOU.004673CD
				└ここを JMP TSMod.OnTSExeSobaHorseChange と書き換えてやる実際の処理
00444830  |. 83C4 04        ADD     ESP, 4

 */
int pTenshouExeJumpFromToOnTenshouExeSobaHorseChange    =0x44482B; // 関数はこのアドレスから、OnTenshouExeSobaHorseChangeへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeSobaHorseChange=0x4673CD; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeSobaHorseChange =0x444830; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeSobaHorseChange() {
	// スタックにためておく
	__asm {
		// 兵士の相場を保存
		mov SobaHorseChange, eax;

		// プッシュしたのはなかったことにする
		pop eax;

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}
	
	OnTenshouExeSobaHorseChangeExecute();

	// 兵士の相場がプログラムで変更された。
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

		// 値を上書きしているのを反映
		mov eax, SobaHorseChange

		push eax; // 元々TENSHOU.EXEにあったpush命令

		// 元々TENSHOU.EXE内にあったものをここで
		call pTenshouExeJumpCallFromToOnTenshouExeSobaHorseChange

		jmp pTenshouExeReturnLblFromOnTenshouExeSobaHorseChange
	}
}

/*
0044479B  |. 83C4 10        ADD     ESP, 10
0044479E  |. 50             PUSH    EAX
0044479F  |. E8 3B2C0200    CALL    TENSHOU.004673DF
				└ここを JMP TSMod.OnTSExeSobaHorseChange と書き換えてやる実際の処理
004447A4  |. 83C4 04        ADD     ESP, 4

 */
char cmdOnTenshouExeJumpFromSobaHorseChange[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeSobaHorseChange() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeSobaHorseChange;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeSobaHorseChange + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeSobaHorseChange  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromSobaHorseChange+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeSobaHorseChange), cmdOnTenshouExeJumpFromSobaHorseChange, 5, NULL); //5バイトのみ書き込む
}



//------------------------------------------------------------------------------------------

int SobaGunChange = 0;
void OnTenshouExeSobaGunChangeExecute() {
	// 相場を上書きしていれば…
	if ( modify_soba.gun ) {
		SobaGunChange = modify_soba.gun;
		nb6soba.gun = modify_soba.gun;
	}
}


/*
0044484E  |. 50             PUSH    EAX
0044484F  |. E8 672B0200    CALL    TENSHOU.004673BB        ─本当の鉄砲相場書き込み
				└ここを JMP TSMod.OnTSExeSobaGunChange と書き換えてやる実際の処理
00444854  |. 83C4 04        ADD     ESP, 4

 */
int pTenshouExeJumpFromToOnTenshouExeSobaGunChange    =0x44484F; // 関数はこのアドレスから、OnTenshouExeSobaGunChangeへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeSobaGunChange=0x4673BB; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeSobaGunChange =0x444854; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeSobaGunChange() {
	// スタックにためておく
	__asm {
		// 兵士の相場を保存
		mov SobaGunChange, eax;

		// プッシュしたのはなかったことにする
		pop eax;

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}
	
	OnTenshouExeSobaGunChangeExecute();

	// 兵士の相場がプログラムで変更された。
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

		// 値を上書きしているのを反映
		mov eax, SobaGunChange

		push eax; // 元々TENSHOU.EXEにあったpush命令

		// 元々TENSHOU.EXE内にあったものをここで
		call pTenshouExeJumpCallFromToOnTenshouExeSobaGunChange

		jmp pTenshouExeReturnLblFromOnTenshouExeSobaGunChange
	}
}

/*
0044479B  |. 83C4 10        ADD     ESP, 10
0044479E  |. 50             PUSH    EAX
0044479F  |. E8 3B2C0200    CALL    TENSHOU.004673DF
				└ここを JMP TSMod.OnTSExeSobaGunChange と書き換えてやる実際の処理
004447A4  |. 83C4 04        ADD     ESP, 4

 */
char cmdOnTenshouExeJumpFromSobaGunChange[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeSobaGunChange() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeSobaGunChange;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeSobaGunChange + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeSobaGunChange  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromSobaGunChange+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeSobaGunChange), cmdOnTenshouExeJumpFromSobaGunChange, 5, NULL); //5バイトのみ書き込む
}


