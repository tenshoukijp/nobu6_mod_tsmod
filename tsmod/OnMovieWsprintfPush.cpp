#include "WinTarget.h"

#include <sys/types.h>
#include <sys/stat.h>

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"

/*
使用方法は、
004B4F73   . 6A 01          PUSH    1
004B4F75   . 6A 21          PUSH    21
004B4F77   . E8 E87DF8FF    CALL    TENSHOU.0043CD64

みたいな感じのようだ。


ムービーの再生関数は、以下がスタートと思われる。
0043CD64  /$ 55             PUSH    EBP
0043CD65  |. 33C0           XOR     EAX, EAX
0043CD67  |. 8BEC           MOV     EBP, ESP
0043CD69  |. 81EC 18010000  SUB     ESP, 118
0043CD6F  |. 53             PUSH    EBX
0043CD70  |. 8945 EC        MOV     DWORD PTR SS:[EBP-14], EAX
0043CD73  |. 56             PUSH    ESI
0043CD74  |. 8945 F0        MOV     DWORD PTR SS:[EBP-10], EAX
0043CD77  |. C745 F4 400100>MOV     DWORD PTR SS:[EBP-C], 140
0043CD7E  |. C745 F8 F00000>MOV     DWORD PTR SS:[EBP-8], 0F0
0043CD85  |. 57             PUSH    EDI
0043CD86  |. 8B7D 08        MOV     EDI, DWORD PTR SS:[EBP+8]
0043CD89  |. 3BF8           CMP     EDI, EAX
0043CD8B  |. 0F8E A1010000  JLE     TENSHOU.0043CF32
0043CD91  |. 83FF 24        CMP     EDI, 24
0043CD94  |. 0F8D 98010000  JGE     TENSHOU.0043CF32
0043CD9A  |. 83FF 01        CMP     EDI, 1
0043CD9D  |. 7C 05          JL      SHORT TENSHOU.0043CDA4
0043CD9F  |. 83FF 03        CMP     EDI, 3
0043CDA2  |. 7E 21          JLE     SHORT TENSHOU.0043CDC5
0043CDA4  |> 833D 38C24B00 >CMP     DWORD PTR DS:[4BC238], 0
0043CDAB  |. 0F84 81010000  JE      TENSHOU.0043CF32
0043CDB1  |. E8 41A20200    CALL    TENSHOU.00466FF7
0043CDB6  |. 85C0           TEST    EAX, EAX
0043CDB8  |. 0F85 74010000  JNZ     TENSHOU.0043CF32
0043CDBE  |. BE 01000000    MOV     ESI, 1
0043CDC3  |. EB 02          JMP     SHORT TENSHOU.0043CDC7
0043CDC5  |> 33F6           XOR     ESI, ESI
0043CDC7  |> BB 80954C00    MOV     EBX, TENSHOU.004C9580            ;  ASCII "T:\"
0043CDCC  |> 6A 01          /PUSH    1                               ; /ErrorMode = SEM_FAILCRITICALERRORS
0043CDCE  |. FF15 18754E00  |CALL    DWORD PTR DS:[<&KERNEL32.SetErr>; \SetErrorMode
0043CDD4  |. 53             |PUSH    EBX                             ; /Arg1
0043CDD5  |. 8945 FC        |MOV     DWORD PTR SS:[EBP-4], EAX       ; |
0043CDD8  |. E8 1CCD0200    |CALL    TENSHOU.00469AF9                ; \TENSHOU.00469AF9
0043CDDD  |. 83C4 04        |ADD     ESP, 4
0043CDE0  |. 85C0           |TEST    EAX, EAX
0043CDE2  |. 74 30          |JE      SHORT TENSHOU.0043CE14
0043CDE4  |. FF34BD 88954C0>|PUSH    DWORD PTR DS:[EDI*4+4C9588]     ; /<%s>
0043CDEB  |. 53             |PUSH    EBX                             ; |<%s>
0043CDEC  |. 68 17F4C001    |PUSH    1C0F417                         ; |Format = 01C0F417 ???




*/

char szExistDirName[5]= ".\\";

// ターゲットムービーのポインタが入っているベースの場所(0x4C9588からの差分)
int iTargetMoviePointerIX = 0;
// ターゲットムービーへのポインタの値が入っているアドレス
int *pTargetMovieAddress = NULL;
char *pszTargetMovieName = NULL;

char *pszOriginalDirName = NULL;

void OnTenshouExeMovieWsprintfPushExecute() {

	struct stat  st;

	// 0043CDE4  |. FF34BD 88954C00     |PUSH    DWORD PTR DS:[EDI*4+4C9588]     ; /<%s>
	// 対象となるムービーの名前へのポインタが入っているアドレスを計算して得る。
	// 上のアセンブラのうち、EDI*4+4C9588　という[ ] を抜いた状態の値
	pTargetMovieAddress = (int *)(iTargetMoviePointerIX*4 + 0x4C9588);

	// そのアドレスにある中身をchar *として評価したものが、該当のムービーの場所である。
	// すなわち[EDI*4+4C9588]という[ ]を付けたものと等しくする。
	pszTargetMovieName = (char *)(*pTargetMovieAddress);

	// ディレクトリ付きの相対のフルネーム化
	char szMovieName[256] = "";
	wsprintf(szMovieName, "MOVIE\\%s.avi", pszTargetMovieName);

	// そのムービーはハードディスクに存在するか
	if (stat(szMovieName, &st) == 0) {
		// 存在する
		// 存在した場合は、CDからではなく、相対からに変更する。
		pszOriginalDirName = szExistDirName;

		// endlogoもある。
		if ( stat("MOVIE\\ENDLOGO.avi", &st) == 0 ) {
			AddModuleItem( (HMODULE)OnTenshouExeMovieWsprintfPushExecute+5, "movie && logo"); //OnTenshouExeMovieWsprintfPushExecute+5という数値に意味はない。ユニークにするためだけ。
		// endlogoはない。
		} else {
			AddModuleItem( (HMODULE)OnTenshouExeMovieWsprintfPushExecute+5, "movie"); //OnTenshouExeMovieWsprintfPushExecute+5という数値に意味はない。ユニークにするためだけ。
		}
	} else {
		// 存在しない
	}
}

/*
0043CDE4  |. FF34BD 88954C00     |PUSH    DWORD PTR DS:[EDI*4+4C9588]     ; /<%s>
0043CDEB     53                  PUSH    EBX
0043CDEC  |. 68 18964C00         |PUSH    TENSHOU.004C9618                ; |Format = ".\Movie\%s.avi"
				└ここを JMP TSMod.OnTSExeMovieWsprintfPush と書き換えてやる実際の処理
0043CDF1  |. 8D8D E8FEFFFF       |LEA     ECX, DWORD PTR SS:[EBP-118]     ; |
0043CDF7  |. 51                  |PUSH    ECX                             ; |s
0043CDF8  |. FF15 F0754E00       |CALL    DWORD PTR DS:[<&USER32.wsprintf>; \wsprintfA
*/
int pTenshouExeJumpFromToOnTenshouExeMovieWsprintfPush		=0x43CDEC; // 関数はこのアドレスから、OnTenshouExeMovieWsprintfPushへとジャンプしてくる。
int pTenshouExeJumpPushFromToOnTenshouExeMovieWsprintfPush  =0x4C9618; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeMovieWsprintfPush	=0x43CDF1; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeMovieWsprintfPush() {
	// スタックにためておく
	__asm {

		mov iTargetMoviePointerIX, edi

		// ディレクトリの名前の文字列へのポインタ
		mov pszOriginalDirName, ebx

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
	OnTenshouExeMovieWsprintfPushExecute();

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

		// １回TENSHOU.EXE内でPushした分はやめる
		pop ebx

		// 替わりに状況をみて計算したディレクトリ名を入れる(元のままなのか、｢.\｣に置き換えるのか)
		push pszOriginalDirName

		// 元々TENSHOU.EXE内にあったものをここで
		push pTenshouExeJumpPushFromToOnTenshouExeMovieWsprintfPush

		jmp pTenshouExeReturnLblFromOnTenshouExeMovieWsprintfPush
	}
}

/*
0043CDE4  |. FF34BD 88954C00     |PUSH    DWORD PTR DS:[EDI*4+4C9588]     ; /<%s>
0043CDEB     53                  PUSH    EBX
0043CDEC  |. 68 18964C00         |PUSH    TENSHOU.004C9618                ; |Format = ".\Movie\%s.avi"
				└ここを JMP TSMod.OnTSExeMovieWsprintfPush と書き換えてやる実際の処理
0043CDF1  |. 8D8D E8FEFFFF       |LEA     ECX, DWORD PTR SS:[EBP-118]     ; |
0043CDF7  |. 51                  |PUSH    ECX                             ; |s
0043CDF8  |. FF15 F0754E00       |CALL    DWORD PTR DS:[<&USER32.wsprintf>; \wsprintfA
*/
char cmdOnTenshouExeJumpFromMovieWsprintfPush[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeMovieWsprintfPush() {

	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeMovieWsprintfPush;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeMovieWsprintfPush + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeMovieWsprintfPush  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromMovieWsprintfPush+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeMovieWsprintfPush), cmdOnTenshouExeJumpFromMovieWsprintfPush, 5, NULL); //5バイトのみ書き込む
}




