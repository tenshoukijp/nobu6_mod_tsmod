#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"



int iAddressLastestWord = 0;

char szMessageBuildLastestWordName[13] = "";


void OnTenshouExeMessageBuildLastestWordExecute() {
#pragma warning(push)
#pragma warning(disable:4996)
	// 名前をコピー
	strncpy( szMessageBuildLastestWordName, (char *)iAddressLastestWord, 12);
#pragma warning(pop)
	// 最悪の時のためのガード
	szMessageBuildLastestWordName[12]='\0';
}


/*
/*
004AD7E1  |. 50             PUSH    EAX
004AD7E2  |. FF7424 18      PUSH    DWORD PTR SS:[ESP+18]
004AD7E6  |. 55             PUSH    EBP
004AD7E7  |. E8 9BFDFFFF    CALL    TENSHOU.004AD587
				└ここを JMP TSMod.OnTSExeMessageBuildLastestWord と書き換えてやる実際の処理
004AD7EC  |. 83C4 0C        ADD     ESP, 0C

 */

int pTenshouExeJumpFromToOnTenshouExeMessageBuildLastestWord    =0x4AD7E7; // 関数はこのアドレスから、OnTenshouExeMessageBuildLastestWordへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeMessageBuildLastestWord=0x4AD587; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeMessageBuildLastestWord =0x4AD7EC; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeMessageBuildLastestWord() {
	// スタックにためておく
	__asm {
	
		// 最後の単語が格納されているアドレス
		mov iAddressLastestWord, esi;

		// 元々TENSHOU.EXE内にあったものをここで
		call pTenshouExeJumpCallFromToOnTenshouExeMessageBuildLastestWord

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
	OnTenshouExeMessageBuildLastestWordExecute();

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

		jmp pTenshouExeReturnLblFromOnTenshouExeMessageBuildLastestWord
	}
}

/*
00444769  |. 50             PUSH    EAX
0044476A  |. E8 3A2C0200    CALL    TENSHOU.004673A9
				└ここを JMP TSMod.OnTSExeMessageBuildLastestWord と書き換えてやる実際の処理
0044476F  |. 83C4 04        ADD     ESP, 4
*/
char cmdOnTenshouExeJumpFromMessageBuildLastestWord[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeMessageBuildLastestWord() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeMessageBuildLastestWord;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeMessageBuildLastestWord + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeMessageBuildLastestWord  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromMessageBuildLastestWord+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeMessageBuildLastestWord), cmdOnTenshouExeJumpFromMessageBuildLastestWord, 5, NULL); //5バイトのみ書き込む
}




