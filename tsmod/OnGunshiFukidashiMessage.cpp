#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "MessageN6PPatch.h"
#include "ScenarioMod.h"

/*
軍師の吹き出しメッセージの直前
004A102C  |. FF75 08        PUSH    DWORD PTR SS:[EBP+8]
004A102F  |. 53             PUSH    EBX
004A1030  |. E8 B8C10000    CALL    TENSHOU.004AD1ED
004A1035  |. 83C4 08        ADD     ESP, 8
*/

int isOnCurGunshiFukidashi = false;
extern bool isCurScenarioModReWrite;
void OnTenshouExeGunshiFukidashiMessageExecute() {
	isOnCurGunshiFukidashi = true;

	// ここで名前を作り出す。
	char name[256] = "";

	if ( nb6fukidashirelatepersons->i1stPersonID <= 532 ) {
		strcat( name, nb6bushouname[nb6fukidashirelatepersons->i1stPersonID-1].familyname);
		strcat( name, nb6bushouname[nb6fukidashirelatepersons->i1stPersonID-1].fastname);
	}

	// 外部メッセージ置き換えをScenarioModにて行う
	if ( p_snOnRewriteFukidashiMessage ) {

		char szModMsgBuf[512] = "";

		// 現在のメモリの最新の状態を取り込み
		char szCurMsgBuf[512] = "　　　　"; // 渡した文字がヌルなどの判定に合格しないようにそれなりに
		// メモリから読み込み
		ReadProcessMemory( CommonGlobalHandle::curProcess, (LPCVOID)(CPM.getMemoryAddress(LOC3)), szCurMsgBuf, sizeof(szCurMsgBuf)-1, NULL);
		szCurMsgBuf[511] = NULL; // 安全ブロック

		((PFNSNONREWRITEFUKIDASHIMESSAGE) p_snOnRewriteFukidashiMessage)(szModMsgBuf, name, szCurMsgBuf, nb6fukidashirelatepersons);
		if ( strlen(szModMsgBuf) > 1 ) {
			// こっちはgetCurVarPtr というように古い状態でもＯＫ
			strcpy((char *)CPM.getMemoryAddress(LOC3), szModMsgBuf);
			isCurScenarioModReWrite = true;
		}
	}
}


/*
004A102C  |. FF75 08        PUSH    DWORD PTR SS:[EBP+8]
004A102F  |. 53             PUSH    EBX
004A1030  |. E8 B8C10000    CALL    TENSHOU.004AD1ED
				└ここを JMP TSMod.OnTSExeGunshiFukidashiMessage と書き換えてやる実際の処理
004A1035  |. 83C4 08        ADD     ESP, 8
 */
int pTenshouExeJumpFromToOnTenshouExeGunshiFukidashiMessage    =0x4A1030; // 関数はこのアドレスから、OnTenshouExeGunshiFukidashiMessageへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeGunshiFukidashiMessage=0x4AD1ED; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeGunshiFukidashiMessage =0x4A1035; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeGunshiFukidashiMessage() {
	// スタックにためておく
	__asm {

		// 元々TENSHOU.EXE内にあったものをここで
		call pTenshouExeJumpCallFromToOnTenshouExeGunshiFukidashiMessage

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
	OnTenshouExeGunshiFukidashiMessageExecute();

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

		jmp pTenshouExeReturnLblFromOnTenshouExeGunshiFukidashiMessage
	}
}

char cmdOnTenshouExeJumpFromGunshiFukidashiMessage[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeGunshiFukidashiMessage() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeGunshiFukidashiMessage;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeGunshiFukidashiMessage + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeGunshiFukidashiMessage  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromGunshiFukidashiMessage+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeGunshiFukidashiMessage), cmdOnTenshouExeJumpFromGunshiFukidashiMessage, 5, NULL); //5バイトのみ書き込む
}

