#include "WinTarget.h"

#include "CommonGlobalHandle.h"

/*
 * ディスクの空き容量がチェックされる時

/*
00479CCC  |. E8 D7000000    CALL    TENSHOU.00479DA8
00479CD1     3D 00A00A00    CMP     EAX, 0AA000
00479CD6     7D 28          JGE     SHORT TENSHOU.00479D00
*/

/*
BOOL GetDiskFreeSpaceEx(
  LPCTSTR lpDirectoryName,                 // ディレクトリ名
  PULARGE_INTEGER lpFreeBytesAvailable,    // 呼び出し側が利用できるバイト数
  PULARGE_INTEGER lpTotalNumberOfBytes,    // ディスク全体のバイト数
  PULARGE_INTEGER lpTotalNumberOfFreeBytes // ディスク全体の空きバイト数
);
*/
int iEAXOfCheckDiskSpaceExecute = 0; // ディスク空き容量値を入れる。但し、最大でも32bitのsigned int maxの 0x7FFFFFFF を超えない。

void OnTenshouExeCheckDiskSpaceExecute() {
    ULARGE_INTEGER i64UseBytes;   // 呼び出し側が利用できるバイト数
    ULARGE_INTEGER i64TotalBytes; // ディスク全体のバイト数
    ULARGE_INTEGER i64FreeBytes;  // ディスク全体の空きバイト数
	GetDiskFreeSpaceEx( NULL, &i64UseBytes, &i64TotalBytes, &i64FreeBytes);

	ULONGLONG ifreebyte = i64TotalBytes.QuadPart; //ULARGE_INTEGERは特殊な共用体で、全体はULONGLONG型のQuadPart。

	// 32bitの符号付intをオーバーしていたら、符号付INTのMAXとする。
	if (ifreebyte > 0x7FFFFFFF ) {
		ifreebyte = 0x7FFFFFFF;
	}

	iEAXOfCheckDiskSpaceExecute = (int)ifreebyte;
	
}

/*
00479CCC  |. E8 D7000000    CALL    TENSHOU.00479DA8
00479CD1     3D 00A00A00    CMP     EAX, 0AA000
00479CD6     7D 28          JGE     SHORT TENSHOU.00479D00
*/
int pTenshouExeJumpFromToOnTenshouExeCheckDiskSpace		=0x479CCC; // 関数はこのアドレスから、OnTenshouExeCheckDiskSpaceへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeCheckDiskSpace =0x479DA8; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeCheckDiskSpace	=0x479CD1; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeCheckDiskSpace() {
	// スタックにためておく
	__asm {

		// 元々の処理
		CALL pTenshouExeJumpCallFromToOnTenshouExeCheckDiskSpace

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
	OnTenshouExeCheckDiskSpaceExecute();

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

		mov eax, iEAXOfCheckDiskSpaceExecute // EAXの値は空き容量。現代の関数で空き容量を計算して再代入。最大値は32bit signed intの0x7FFFFFFFまでに抑えてある。

		jmp pTenshouExeReturnLblFromOnTenshouExeCheckDiskSpace
	}
}


char cmdOnTenshouExeJumpFromCheckDiskSpace[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeCheckDiskSpace() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeCheckDiskSpace;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeCheckDiskSpace + 5 );
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeCheckDiskSpace  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromCheckDiskSpace+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeCheckDiskSpace), cmdOnTenshouExeJumpFromCheckDiskSpace, 5, NULL); //5バイトのみ書き込む
}




