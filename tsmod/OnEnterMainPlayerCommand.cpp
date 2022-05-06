#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "ScenarioMod.h"

/*

0041050B  |> 6A 00          /PUSH    0
0041050D  |. E8 C4780700    |CALL    TENSHOU.00487DD6
00410512  |. 83C4 04        |ADD     ESP, 4
00410515  |. E8 BDB30600    |CALL    TENSHOU.0047B8D7				← プレイヤがメイン画面で特に何も入力しておらず、控えている状態（メインコマンド名控え直前を作る!!)
0041051A  |. 8BD8           |MOV     EBX, EAX
0041051C  |. 85DB           |TEST    EBX, EBX
0041051E  |. 74 2E          |JE      SHORT TENSHOU.0041054E


004105EB  |. 897D F8        |MOV     DWORD PTR SS:[EBP-8], EDI
004105EE  |. E8 196D0600    |CALL    TENSHOU.0047730C
004105F3  |. E8 A6000000    |CALL    TENSHOU.0041069E				← 「静観」コマンド。ここは特殊
004105F8  |. 85C0           |TEST    EAX, EAX
004105FA  |. 74 63          |JE      SHORT TENSHOU.0041065F
004105FC  |. E8 28040000    |CALL    TENSHOU.00410A29
00410601  |. EB 7D          |JMP     SHORT TENSHOU.00410680
00410603  |> B9 64000000    |MOV     ECX, 64
00410608  |. 8BC3           |MOV     EAX, EBX
0041060A  |. 2BD2           |SUB     EDX, EDX
0041060C  |. F7F1           |DIV     ECX
0041060E  |. 83F8 03        |CMP     EAX, 3
00410611  |. 75 1A          |JNZ     SHORT TENSHOU.0041062D
00410613  |. 8D45 F4        |LEA     EAX, DWORD PTR SS:[EBP-C]
00410616  |. 8D4D F8        |LEA     ECX, DWORD PTR SS:[EBP-8]
00410619  |. 50             |PUSH    EAX                             ; /Arg3
0041061A  |. 51             |PUSH    ECX                             ; |Arg2
0041061B  |. 53             |PUSH    EBX                             ; |Arg1
0041061C  |. E8 D3010000    |CALL    TENSHOU.004107F4                ; \TENSHOU.004107F4	←コマンド群。メインカテゴリ
00410621  |. 83C4 0C        |ADD     ESP, 0C
00410624  |. 8BD8           |MOV     EBX, EAX
00410626  |. 83FB 01        |CMP     EBX, 1
00410629  |. 77 61          |JA      SHORT TENSHOU.0041068C
0041062B  |. EB 1D          |JMP     SHORT TENSHOU.0041064A
0041062D  |> 83F8 04        |CMP     EAX, 4
00410630  |. 75 2D          |JNZ     SHORT TENSHOU.0041065F
00410632  |. 8D45 F4        |LEA     EAX, DWORD PTR SS:[EBP-C]
00410635  |. 8D4D F8        |LEA     ECX, DWORD PTR SS:[EBP-8]
00410638  |. 50             |PUSH    EAX
00410639  |. 51             |PUSH    ECX
0041063A  |. 53             |PUSH    EBX
0041063B  |. E8 56010000    |CALL    TENSHOU.00410796										← コマンド郡。サブカテゴリ
00410640  |. 83C4 0C        |ADD     ESP, 0C

*/

#include "OnEnterMainPlayerCommand.h"

void OnEnterMainPlayerCommand(int iSelectedCommandID, int iSubCommandID) {
	if ( iSelectedCommandID == 0 ) {
		return;
	}

	// ScenarioModの呼出し
	if ( p_snOnEnterMainPlayerCommand ) {
		p_snOnEnterMainPlayerCommand( iSelectedCommandID, iSubCommandID );
	}
	// ★何かのコマンドに入った際にはここで判定可能
	// OutputDebugStream << "Enter" << iSelectedCommandID << "," << iSubCommand << endl;
}



int iEnterOfClickMainScreenMainCommand = 0; // コマンドに侵入したかどうか。メインコマンド名とサブコマンド名で侵入のタイミングを分ける。
int iCurrentCommandOfClickMainScreenMainCommand = -1; // 選択されたコマンド。
void OnTenshouExeClickMainScreenMainCommandExecute() {

	switch (iCurrentCommandOfClickMainScreenMainCommand ) {
		case メインコマンド名::本城:
			{
				iEnterOfClickMainScreenMainCommand = メインコマンド名::本城;
				break;
			}
		case メインコマンド名::支城:
			{
				iEnterOfClickMainScreenMainCommand = メインコマンド名::支城;
				break;
			}
		case メインコマンド名::城:
			{
				iEnterOfClickMainScreenMainCommand = メインコマンド名::城;
				break;
			}
		case メインコマンド名::地図:
			{
				iEnterOfClickMainScreenMainCommand = メインコマンド名::地図;
				break;
			}
		case メインコマンド名::戦争:
			{
				iEnterOfClickMainScreenMainCommand = メインコマンド名::戦争;
				break;
			}
		case メインコマンド名::評定:
			{
				iEnterOfClickMainScreenMainCommand = メインコマンド名::評定;
				break;
			}
		case メインコマンド名::会見:
			{
				iEnterOfClickMainScreenMainCommand = メインコマンド名::会見;
				break;
			}
		default:
			{
				iEnterOfClickMainScreenMainCommand = 0; // まだ侵入していない。サブコマンド名を選ぶ必要があるため。
			}
	}

	OnEnterMainPlayerCommand(iEnterOfClickMainScreenMainCommand, -1);
}


/*
00410613  |. 8D45 F4        |LEA     EAX, DWORD PTR SS:[EBP-C]
00410616  |. 8D4D F8        |LEA     ECX, DWORD PTR SS:[EBP-8]
00410619  |. 50             |PUSH    EAX                             ; /Arg3
0041061A  |. 51             |PUSH    ECX                             ; |Arg2
0041061B  |. 53             |PUSH    EBX                             ; |Arg1	← これが選択のコマンド。enum値と一致
0041061C  |. E8 D3010000    |CALL    TENSHOU.004107F4                ; \TENSHOU.004107F4	←コマンド群。メインカテゴリ
				└ここを JMP TSMod.OnTSExeClickMainScreenMainCommand と書き換えてやる実際の処理
00410621  |. 83C4 0C        |ADD     ESP, 0C
*/
int pTenshouExeJumpFromToOnTenshouExeClickMainScreenMainCommand		 =0x41061C; // 関数はこのアドレスから、OnTenshouExeClickMainScreenMainCommandへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeClickMainScreenMainCommand  =0x4107F4; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeClickMainScreenMainCommand	 =0x410621; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeClickMainScreenMainCommand() {
	// スタックにためておく
	__asm {

		mov iCurrentCommandOfClickMainScreenMainCommand, ebx // 選択(クリック)コマンドの値を保存

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
	OnTenshouExeClickMainScreenMainCommandExecute();

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
		call pTenshouExeJumpCallFromToOnTenshouExeClickMainScreenMainCommand

		jmp pTenshouExeReturnLblFromOnTenshouExeClickMainScreenMainCommand
	}
}

char cmdOnTenshouExeJumpFromClickMainScreenMainCommand[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeClickMainScreenMainCommand() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeClickMainScreenMainCommand;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeClickMainScreenMainCommand + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeClickMainScreenMainCommand  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromClickMainScreenMainCommand+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeClickMainScreenMainCommand), cmdOnTenshouExeJumpFromClickMainScreenMainCommand, 5, NULL); //5バイトのみ書き込む
}











//----------------------------------------------------------------------サブコマンド名



int iCurrentCommandOfClickMainScreenSubCommand = -1; // 選択されたコマンド。
int iPointerOfMainCommandValueOfClickMainScreenSubCommand = NULL; // 選択したサブコマンド名のメインコマンド名のID(ただし、純粋な値ではなく、引き算されている)
void OnTenshouExeClickMainScreenSubCommandExecute() {

	int *iMainCommandValud =  (int *)iPointerOfMainCommandValueOfClickMainScreenSubCommand;
	int iMainCommandValue = メインコマンド名::本城 + (*iMainCommandValud); // アドレスの中身が親のコマンド番号値。
	iEnterOfClickMainScreenMainCommand = iMainCommandValue;

	// サブコマンドの一番左を押したら0x190、２番目なら0x191なので、これを引いて、代わりにメインコマンドの1000倍を足す。
	// これできれいな一意の番号とする。
	OnEnterMainPlayerCommand(iMainCommandValue, iMainCommandValue*1000 + iCurrentCommandOfClickMainScreenSubCommand-0x190);
}


/*
00410632  |. 8D45 F4        |LEA     EAX, DWORD PTR SS:[EBP-C]
00410635  |. 8D4D F8        |LEA     ECX, DWORD PTR SS:[EBP-8]
00410638  |. 50             |PUSH    EAX													
00410639  |. 51             |PUSH    ECX													← この時にECXアドレスにある値がメインコマンド名の値と関係している。
0041063A  |. 53             |PUSH    EBX
0041063B  |. E8 56010000    |CALL    TENSHOU.00410796										← コマンド郡。サブカテゴリ
				└ここを JMP TSMod.OnTSExeClickMainScreenSubCommand と書き換えてやる実際の処理
00410640  |. 83C4 0C        |ADD     ESP, 0C
*/
int pTenshouExeJumpFromToOnTenshouExeClickMainScreenSubCommand		 =0x41063B; // 関数はこのアドレスから、OnTenshouExeClickMainScreenSubCommandへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeClickMainScreenSubCommand   =0x410796; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeClickMainScreenSubCommand	 =0x410640; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeClickMainScreenSubCommand() {
	// スタックにためておく
	__asm {

		mov iCurrentCommandOfClickMainScreenSubCommand, ebx // 選択(クリック)コマンドの値を保存

		mov iPointerOfMainCommandValueOfClickMainScreenSubCommand, ecx // メインコマンド名の値が入っているアドレスを保存

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
	OnTenshouExeClickMainScreenSubCommandExecute();

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
		call pTenshouExeJumpCallFromToOnTenshouExeClickMainScreenSubCommand

		jmp pTenshouExeReturnLblFromOnTenshouExeClickMainScreenSubCommand
	}
}

char cmdOnTenshouExeJumpFromClickMainScreenSubCommand[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeClickMainScreenSubCommand() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeClickMainScreenSubCommand;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeClickMainScreenSubCommand + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeClickMainScreenSubCommand  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromClickMainScreenSubCommand+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeClickMainScreenSubCommand), cmdOnTenshouExeJumpFromClickMainScreenSubCommand, 5, NULL); //5バイトのみ書き込む
}





//----------------------------------------------------------------------静観コマンド

extern void OnLeaveMainPlayerCommand();

void OnTenshouExeClickMainScreenSeikanCommandExecute() {

	// 静観に入った
	iEnterOfClickMainScreenMainCommand = メインコマンド名::静観;

	OnEnterMainPlayerCommand(iEnterOfClickMainScreenMainCommand, -1);
}

int isZikkouOfMainScreenSeikanCommand = -1; // 静観コマンドを取消せずに実行した
void OnTenshouExeClickMainScreenSeikanCommandExecute2() {

	// 静観コマンドが実行されてしまうと、OnLeaveが実行されなくなるので、ここで明示的に呼び出す
	if ( isZikkouOfMainScreenSeikanCommand ) {
		OnLeaveMainPlayerCommand();
	}
}

/*
004105EE  |. E8 196D0600    |CALL    TENSHOU.0047730C
004105F3  |. E8 A6000000    |CALL    TENSHOU.0041069E				← 「静観」コマンド。ここは特殊
				└ここを JMP TSMod.OnTSExeClickMainScreenSeikanCommand と書き換えてやる実際の処理
004105F8  |. 85C0           |TEST    EAX, EAX
*/
int pTenshouExeJumpFromToOnTenshouExeClickMainScreenSeikanCommand		 =0x4105F3; // 関数はこのアドレスから、OnTenshouExeClickMainScreenSeikanCommandへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeClickMainScreenSeikanCommand    =0x41069E; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeClickMainScreenSeikanCommand	 =0x4105F8; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeClickMainScreenSeikanCommand() {
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
	OnTenshouExeClickMainScreenSeikanCommandExecute();

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
		call pTenshouExeJumpCallFromToOnTenshouExeClickMainScreenSeikanCommand

		mov isZikkouOfMainScreenSeikanCommand, eax // 静観コマンドの「実行」「取消」の結果保存
	}

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
	OnTenshouExeClickMainScreenSeikanCommandExecute2();

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

		jmp pTenshouExeReturnLblFromOnTenshouExeClickMainScreenSeikanCommand
	}
}

char cmdOnTenshouExeJumpFromClickMainScreenSeikanCommand[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeClickMainScreenSeikanCommand() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeClickMainScreenSeikanCommand;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeClickMainScreenSeikanCommand + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeClickMainScreenSeikanCommand  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromClickMainScreenSeikanCommand+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeClickMainScreenSeikanCommand), cmdOnTenshouExeJumpFromClickMainScreenSeikanCommand, 5, NULL); //5バイトのみ書き込む
}


