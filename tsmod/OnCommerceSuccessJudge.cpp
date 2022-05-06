
#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"

/*
00446B72   E8 14640600      CALL    TENSHOU.004ACF8B	← 商売が成功するのか、横ばいなのか、商人が怒るのか決める関数。このとき、EDIは、交渉に行った武将番号
00446B77   83C4 04          ADD     ESP, 4
00446B7A   85C0             TEST    EAX, EAX
*/


/*
* 商人ならば、結果の成功は約束されている。
*/




int iCurBushouIDInCommerceSuccessJudge = -1;
int nSuccessJudgeInCommerceSuccessJudge = 0;
void OnTenshouExeCommerceSuccessJudgeExecute() {

	// 武将が一定の範囲であること
	if (0 <= iCurBushouIDInCommerceSuccessJudge - 1 && iCurBushouIDInCommerceSuccessJudge - 1 < GAMEDATASTRUCT_BUSHOU_NUM) {

		// 商人であれば… 絶対の成功が約束されている。
		if (nb6bushouref[iCurBushouIDInCommerceSuccessJudge - 1].job == 0x70) {
			nSuccessJudgeInCommerceSuccessJudge = 2; // 大成功

			// OutputDebugString("商売強制での大成功\n");
		}
	}
}


/*
00446B72   E8 14640600      CALL    TENSHOU.004ACF8B	← 商売が成功するのか、横ばいなのか、商人が怒るのか決める関数。このとき、EDIは、交渉に行った武将番号
└ここを JMP TSMod.OnTSExeCommerceSuccessJudge と書き換えてやる実際の処理
00446B77   83C4 04          ADD     ESP, 4
00446B7A   85C0             TEST    EAX, EAX
*/
int pTenshouExeJumpFromToOnTenshouExeCommerceSuccessJudge = 0x446B72; // 関数はこのアドレスから、OnTenshouExeCommerceSuccessJudgeへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeCommerceSuccessJudge = 0x4ACF8B; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeCommerceSuccessJudge = 0x446B77; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec(naked) void WINAPI OnTenshouExeCommerceSuccessJudge() {
	// スタックにためておく
	__asm {

		mov iCurBushouIDInCommerceSuccessJudge, edi

		call pTenshouExeJumpCallFromToOnTenshouExeCommerceSuccessJudge

		mov nSuccessJudgeInCommerceSuccessJudge, eax // 0:交渉してない 1:交渉横ばい 2:交渉成功 4:交渉失敗

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
	OnTenshouExeCommerceSuccessJudgeExecute();

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

		// 商人か否かの結果をもって、0～4を2(=成功)に上書き
		mov eax, nSuccessJudgeInCommerceSuccessJudge

		jmp pTenshouExeReturnLblFromOnTenshouExeCommerceSuccessJudge
	}
}



char cmdOnTenshouExeJumpFromCommerceSuccessJudge[6] = "\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeCommerceSuccessJudge() {

	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeCommerceSuccessJudge;
	int SubAddress = iAddress - (pTenshouExeJumpFromToOnTenshouExeCommerceSuccessJudge + 5);
	// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeCommerceSuccessJudge  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromCommerceSuccessJudge + 1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory(CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeCommerceSuccessJudge), cmdOnTenshouExeJumpFromCommerceSuccessJudge, 5, NULL); //5バイトのみ書き込む
}


























/*
 * 商業時に商人用のメッセージ。
 */


int iCurBushouIDInCommerceMessageJudge = -1;
bool isUseSyoninMsgInCommerceSuccessJudge = false;
void OnTenshouExeCommerceMessageJudgeExecute() {

	// 商売人用のメッセージへと変更すべきかどうかはまずfalse;
	isUseSyoninMsgInCommerceSuccessJudge = false;

	// 武将が一定の範囲であること
	if (0 <= iCurBushouIDInCommerceMessageJudge - 1 && iCurBushouIDInCommerceMessageJudge - 1 < GAMEDATASTRUCT_BUSHOU_NUM) {

		// 商人であれば… 絶対の成功が約束されている。
		if (nb6bushouref[iCurBushouIDInCommerceMessageJudge - 1].job == 0x70) {
			isUseSyoninMsgInCommerceSuccessJudge = true;
		}
	}
}


/*
00446F75  |. 66:A3 907C4D00 MOV     WORD PTR DS:[4D7C90], AX
00446F7B  |. B8 B21F0000    MOV     EAX, 1FB2
└ここを JMP TSMod.OnTSExeCommerceMessageJudge と書き換えてやる実際の処理
00446F80  |. 75 0E          JNZ     SHORT TENSHOU.00446F90
00446F82  |. 83E9 04        SUB     ECX, 4
*/
int pTenshouExeJumpFromToOnTenshouExeCommerceMessageJudge = 0x446F7B; // 関数はこのアドレスから、OnTenshouExeCommerceMessageJudgeへとジャンプしてくる。
int pTenshouExeReturnLblFromOnTenshouExeCommerceMessageJudge = 0x446F80; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec(naked) void WINAPI OnTenshouExeCommerceMessageJudge() {
	// スタックにためておく
	__asm {
		// 元々TENSHOU.EXE内にあったものをここで
		mov eax, 0x1FB2

		// 武将IDを保存
		mov iCurBushouIDInCommerceMessageJudge, edx

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
	OnTenshouExeCommerceMessageJudgeExecute();

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

		test esi, esi

		jmp pTenshouExeReturnLblFromOnTenshouExeCommerceMessageJudge
	}
}




char cmdOnTenshouExeJumpFromCommerceMessageJudge[6] = "\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeCommerceMessageJudge() {

	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeCommerceMessageJudge;
	int SubAddress = iAddress - (pTenshouExeJumpFromToOnTenshouExeCommerceMessageJudge + 5);
	// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeCommerceMessageJudge  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromCommerceMessageJudge + 1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory(CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeCommerceMessageJudge), cmdOnTenshouExeJumpFromCommerceMessageJudge, 5, NULL); //5バイトのみ書き込む
}












//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------











/*
* 商業時に商人ならば、結果の成功は約束されている。
*/

/*
// 武将IDを保存
int iCurBushouIDInCommerceAngryJudge = -1;
bool isAngryMsgForceControlCommerceAngryJudge = false; // 商人の怒りを絶対制御

void OnTenshouExeCommerceAngryJudgeExecuteBefore() {

	// 怒りの直接のコントロール
	isAngryMsgForceControlCommerceAngryJudge = false;

	if (0 <= iCurBushouIDInCommerceAngryJudge - 1 && iCurBushouIDInCommerceAngryJudge - 1 < GAMEDATASTRUCT_BUSHOU_NUM) {

		// 武将が商人
		if (nb6bushouref[iCurBushouIDInCommerceAngryJudge - 1].job == 0x70) {
			// 怒りコントロールモードをONに
			isAngryMsgForceControlCommerceAngryJudge = true;
		}

	}
}

void OnTenshouExeCommerceAngryJudgeExecuteAfter() {

	// 怒りモードがONの時だけ、交渉前の値との比較など、制御をおこなう
	if (isAngryMsgForceControlCommerceAngryJudge) {
		// 武将が商人
		if (nb6bushouref[iCurBushouIDInCommerceAngryJudge - 1].job == 0x70) {

			isAngryMsgForceControlCommerceAngryJudge = true; // これは不要

			// 商人との友好が大きくなっていないなら、元の値より１つ大きくする
			int iDaimyoID = nb6bushouref[iCurBushouIDInCommerceAngryJudge - 1].attach - 1;

			// まず、商人が怒って不在などとなっていたとしたら、それは強制修正
			nb6daimyo[iDaimyoID].bMerchantAngry = 0;
		}
	}
}
*/

/*
004471F6   51               PUSH    ECX
004471F7   FF55 0C          CALL    DWORD PTR SS:[EBP+C]
└ここを JMP TSMod.OnTSExeCommerceAngryJudge と書き換えてやる実際の処理
004471FA   83C4 10          ADD     ESP, 10
004471FD   85C0             TEST    EAX, EAX		EAXが0なら怒る
*/

/*

int pTenshouExeJumpFromToOnTenshouExeCommerceAngryJudge = 0x4471F7; // 関数はこのアドレスから、OnTenshouExeCommerceAngryJudgeへとジャンプしてくる。
int pTenshouExeReturnLblFromOnTenshouExeCommerceAngryJudge = 0x4471FD; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec(naked) void WINAPI OnTenshouExeCommerceAngryJudge() {
	// スタックにためておく
	__asm {

		// 武将IDを保存
		mov iCurBushouIDInCommerceAngryJudge, ecx
			
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
	OnTenshouExeCommerceAngryJudgeExecuteBefore();

	// 商人の怒りを絶対制御
	if (isAngryMsgForceControlCommerceAngryJudge) {
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

			// 元々の実行を呼び出す
			CALL    DWORD PTR SS : [EBP + 0xC]

			ADD     ESP, 0x10

			push eax
			push ebx
			push ecx
			push edx
			push esp
			push ebp
			push esi
			push edi
		}

		OnTenshouExeCommerceAngryJudgeExecuteAfter();
		
		__asm {

			pop edi
			pop esi
			pop ebp
			pop esp
			pop edx
			pop ecx
			pop ebx
			pop eax

			// 商人は決して怒らない
			mov eax, 1

			// このフラグは降ろしておく
			mov isAngryMsgForceControlCommerceAngryJudge, 0

			jmp pTenshouExeReturnLblFromOnTenshouExeCommerceAngryJudge
		}

	} else {

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

			// 元々の実行を呼び出す
			CALL    DWORD PTR SS : [EBP + 0xC]

			ADD     ESP, 0x10

			// このフラグは降ろしておく
			mov isAngryMsgForceControlCommerceAngryJudge, 0

			jmp pTenshouExeReturnLblFromOnTenshouExeCommerceAngryJudge
		}
	}
}

char cmdOnTenshouExeJumpFromCommerceAngryJudge[7] = "\xE9\x90\x90\x90\x90\x90";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeCommerceAngryJudge() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeCommerceAngryJudge;
	int SubAddress = iAddress - (pTenshouExeJumpFromToOnTenshouExeCommerceAngryJudge + 5);
	// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeCommerceAngryJudge  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromCommerceAngryJudge + 1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory(CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeCommerceAngryJudge), cmdOnTenshouExeJumpFromCommerceAngryJudge, 6, NULL); //6バイトのみ書き込む
}
*/




