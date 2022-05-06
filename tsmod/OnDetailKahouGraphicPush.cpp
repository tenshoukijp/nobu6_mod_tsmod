
#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"



/*
家宝詳細画面のレイアウト
004949E8  |. 6A 30          PUSH    30
004949EA  |. E8 16680100    CALL    TENSHOU.004AB205
004949EF  |. 50             PUSH    EAX                              ; /Arg5
004949F0  |. 8BCB           MOV     ECX, EBX                         ; |
004949F2  |. 6A 40          PUSH    40                               ; |Arg4 = 00000040   ← 64ピクセル
004949F4  |. 6A 40          PUSH    40                               ; |Arg3 = 00000040   ← 64ピクセル
004949F6  |. 6A 24          PUSH    24                               ; |Arg2 = 00000024   ← x座標
004949F8  |. 6A 10          PUSH    10                               ; |Arg1 = 00000010   ← y座標
004949FA  |. E8 EA960100    CALL    TENSHOU.004AE0E9                 ; \TENSHOU.004AE0E9  ← ここが家宝アイコンプッシュ
*/

/*
まず、家宝の番号とアイコンがプッシュされる。
この時、リアル画像家宝があれば、40,40→0,0にするのが良い
004949EF  |. 50             PUSH    EAX                              ; /Arg5
004949F0  |. 8BCB           MOV     ECX, EBX                         ; |
004949F2  |. 6A 40          PUSH    40                               ; |Arg4 = 00000040
004949F4  |. 6A 40          PUSH    40                               ; |Arg3 = 00000040
004949F6  |. 6A 24          PUSH    24                               ; |Arg2 = 00000024
004949F8  |. 6A 10          PUSH    10                               ; |Arg1 = 00000010
004949FA  |. E8 EA960100    CALL    TENSHOU.004AE0E9                 ; \TENSHOU.004AE0E9


次に、かならず、どこに描画をするのかの情報となる。これにより、ダイアログの位置がわかる。
00449906  |. 68 50FD4D00    PUSH    TENSHOU.004DFD50                 ; /pRect2 = 004DFD50 {208.,136.,528.,336.} ← こっちはよくわからん。描画範囲か？
0044990B  |. 894D F4        MOV     DWORD PTR SS:[EBP-C], ECX        ; |
0044990E  |. 034D 14        ADD     ECX, DWORD PTR SS:[EBP+14]       ; |
00449911  |. BB 08FF4D00    MOV     EBX, TENSHOU.004DFF08            ; |
00449916  |. 8945 F8        MOV     DWORD PTR SS:[EBP-8], EAX        ; |
00449919  |. 894D FC        MOV     DWORD PTR SS:[EBP-4], ECX        ; |
0044991C  |. 8D45 F0        LEA     EAX, DWORD PTR SS:[EBP-10]       ; |
0044991F  |. 50             PUSH    EAX                              ; |pRect1 = 0018FAA8 {224.,172.,288.,236.} ← こちらが家宝の描画すべき座標
00449920  |. 53             PUSH    EBX                              ; |pRectDst => TENSHOU.004DFF08
00449921  |. FF15 F4754E00  CALL    DWORD PTR DS:[<&USER32.Intersect>; \IntersectRect
*/

extern char* getExistMovieKahouFilePath( int iKahouID );
extern char* getExistPictureKahouFilePath( int iKahouID );
extern void getAviPixelSize( char *szKahouName, int *width, int *height );
extern void getBmpPixelSize( char *szKahouName, int *width, int *height );


/*
 * 家宝のグラフィックをプッシュする時
 */

int iCurKahouIDOnGraphicPush = 0L;
BOOL iCurKahouIs24bitGraphic = FALSE;
void OnTenshouExeDetailKahouGraphicPushExecute() {

	iCurKahouIs24bitGraphic = FALSE;

	// esiの値から、家宝が入っている場所を突き止め、その値をiCurKahouIDOnGraphicPushに改めて入れる。
	// MOV     EBP, DWORD PTR DS:[ESI+70] と同じことをやろうというのだ。
	char *p = (char *)iCurKahouIDOnGraphicPush + 0x70;
	iCurKahouIDOnGraphicPush = *p;
	iCurKahouIDOnGraphicPush--; // 番号→ＩＤへ

	if ( 0 <= iCurKahouIDOnGraphicPush && iCurKahouIDOnGraphicPush < GAMEDATASTRUCT_KAHOU_NUM ) {
		// 対象となる家宝がムービーかどうかチェック
		char *p = getExistMovieKahouFilePath( iCurKahouIDOnGraphicPush );
		// それはムービーだ。
		if ( p ) {
			iCurKahouIs24bitGraphic = TRUE;
		}

		p = getExistPictureKahouFilePath( iCurKahouIDOnGraphicPush );
		// それは画像だ。
		if ( p ) {
			iCurKahouIs24bitGraphic = TRUE;
		}
	}
}


/*
004949EA  |. E8 16680100    CALL    TENSHOU.004AB205
				└ここを JMP TSMod.OnTSExeDetailKahouGraphicPush と書き換えてやる実際の処理
004949EF  |. 50             PUSH    EAX                              ; /Arg5
004949F0  |. 8BCB           MOV     ECX, EBX                         ; |
004949F2  |. 6A 40          PUSH    40                               ; |Arg4 = 00000040   ← 64ピクセル
004949F4  |. 6A 40          PUSH    40                               ; |Arg3 = 00000040   ← 64ピクセル
004949F6  |. 6A 24          PUSH    24                               ; |Arg2 = 00000024   ← x座標
*/
int pTenshouExeJumpFromToOnTenshouExeDetailKahouGraphicPush		=0x4949EA; // 関数はこのアドレスから、OnTenshouExeDetailKahouGraphicPushへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeDetailKahouGraphicPush =0x4AB205; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeDetailKahouGraphicPush	=0x4949F6; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeDetailKahouGraphicPush() {
	// スタックにためておく
	__asm {
		mov iCurKahouIDOnGraphicPush, esi // 家宝ＩＤ関連アドレスをコピー

		// 元々TENSHOU.EXE内にあったものをここで
		call pTenshouExeJumpCallFromToOnTenshouExeDetailKahouGraphicPush

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
	OnTenshouExeDetailKahouGraphicPushExecute();

	if ( iCurKahouIs24bitGraphic ) {

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

			// tenshou.exeにあった処理。24bit家宝を表示する際は、元の画像のheightを0とする。事実上これで見えない
			push eax
			mov ecx, ebx
			push 0x0				// height を0にする。
			push 0x40				// width は64pxのまま

			jmp pTenshouExeReturnLblFromOnTenshouExeDetailKahouGraphicPush
		}
	}

	else {
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

			// tenshou.exeにあった処理。
			push eax
			mov ecx, ebx
			push 0x40				// height を0にする。
			push 0x40				// width は64pxのまま

			jmp pTenshouExeReturnLblFromOnTenshouExeDetailKahouGraphicPush
		}
	}
}

/*
004538DA  |. FF75 08        PUSH    DWORD PTR SS:[EBP+8]
004538DD  |. E8 DC110400    CALL    TENSHOU.00494ABE
004538E2  |. E8 F07F0200    CALL    TENSHOU.0047B8D7
				└ここを JMP TSMod.OnTSExeDetailKahouGraphicPush と書き換えてやる実際の処理
004538E7  |. 8D8D 7CFFFFFF  LEA     ECX, DWORD PTR SS:[EBP-84]
004538ED  |. 8935 80154D00  MOV     DWORD PTR DS:[4D1580], ESI
*/
char cmdOnTenshouExeJumpFromDetailKahouGraphicPush[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeDetailKahouGraphicPush() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeDetailKahouGraphicPush;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeDetailKahouGraphicPush + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeDetailKahouGraphicPush  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromDetailKahouGraphicPush+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeDetailKahouGraphicPush), cmdOnTenshouExeJumpFromDetailKahouGraphicPush, 5, NULL); //5バイトのみ書き込む
}




