#define _CRT_SECURE_NO_WARNINGS

#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"

#define GAMEDATASTRUCT_CASTLE_RETSUDEN_LAYOUT_X_ADDRESS_FROM_EBP -0x1E4	// 城列伝のダイアログのLEFT(X)座標がはいった場所（EBPの値のアドレスからのアドレス差分)
#define GAMEDATASTRUCT_CASTLE_RETSUDEN_LAYOUT_Y_ADDRESS_FROM_EBP -0x1E0	// 城列伝のダイアログのTOP(Y)座標がはいった場所（EBPの値のアドレスからのアドレス差分)

extern BOOL isMainWindowCastleButtonMode;

/*
 * 列伝画面を開く前後のフック
 */ 


/*
0045376B  |. E8 94350200    CALL    TENSHOU.00476D04                 ; \TENSHOU.00476D04
00453770  |. E8 62810200    CALL    TENSHOU.0047B8D7
00453775  |. 8D8D F4FDFFFF  LEA     ECX, DWORD PTR SS:[EBP-20C]
*/

// 城の番号へのポインタ
static int pEBPOFViewingCastleRetsuden = NULL;
static int iCastleIDOFViewingCastleRetsudenFunc = -1;

static HMODULE hCASTLEMAPINRETSUDENDLL = NULL;

typedef int (WINAPI *PFNCASTLEMAPINRETSUDENONBEGINPAINT)(int iCastleID, char *szCastleName, int x, int y, int iCastleRank);
static PFNCASTLEMAPINRETSUDENONBEGINPAINT pBeginPaint = NULL;

typedef int (WINAPI *PFNCASTLEMAPINRETSUDENONUPDATEPAINT)(int x, int y);
static PFNCASTLEMAPINRETSUDENONUPDATEPAINT pUpdatePaint = NULL;

typedef int (WINAPI *PFNCASTLEMAPINRETSUDENONENDPAINT)();
static PFNCASTLEMAPINRETSUDENONENDPAINT pEndPaint = NULL;

// 城列伝で、城マップを描画すべきだ
static bool isRetsudenEditorExtRepaint = false;

// 城列伝が、表示された。
static bool isRetsudenEditorExtViewing = false;

static int iPrevWindowX = 0;
static int iPrevWindowY = 0;
static const int iDeltaInRepaintterval = 5; // 毎度描画しているとウィンドウの移動に遅延がでるので、城内マップの方をあえて５チックおくらせることで、ウィンドウの移動を円滑にする。
static int iDeltaTickCnt = 0;

void RetsudenEditorExtRepaintCheck(CONST RECT *lprcSrc1, CONST RECT *lprcSrc2) {
	if (!isRetsudenEditorExtRepaint) { return; }

	// CastleMapInRetsudenMod.dllを読みにかかる。基本的には無いだろう。
	if (!hCASTLEMAPINRETSUDENDLL) {
		hCASTLEMAPINRETSUDENDLL = LoadLibrary( "CastleMapInRetsudenMod.dll" );
		if (!hCASTLEMAPINRETSUDENDLL) {
			isRetsudenEditorExtRepaint = false;
			return;
		}
		pBeginPaint = (PFNCASTLEMAPINRETSUDENONBEGINPAINT)GetProcAddress( hCASTLEMAPINRETSUDENDLL, "OnBeginPaint" );
		if (!pBeginPaint) {
			isRetsudenEditorExtRepaint = false;
			return;
		}

		pUpdatePaint = (PFNCASTLEMAPINRETSUDENONUPDATEPAINT)GetProcAddress( hCASTLEMAPINRETSUDENDLL, "OnUpdatePaint" );
		if (!pUpdatePaint) {
			isRetsudenEditorExtRepaint = false;
			return;
		}

		pEndPaint = (PFNCASTLEMAPINRETSUDENONENDPAINT)GetProcAddress( hCASTLEMAPINRETSUDENDLL, "OnEndPaint" );
		if (!pEndPaint) {
			isRetsudenEditorExtRepaint = false;
			return;
		}

	}

	if ( pBeginPaint && pUpdatePaint && pEndPaint ) {
	
		int x = lprcSrc1->left;
		int y = lprcSrc1->top;
		int w = lprcSrc1->right-lprcSrc1->left;
		int h = lprcSrc1->bottom-lprcSrc1->top;

		if (isRetsudenEditorExtViewing) {
			// 今のダイアログの座標
			int *pX = (int *)(pEBPOFViewingCastleRetsuden + GAMEDATASTRUCT_CASTLE_RETSUDEN_LAYOUT_X_ADDRESS_FROM_EBP);
			int *pY = (int *)(pEBPOFViewingCastleRetsuden + GAMEDATASTRUCT_CASTLE_RETSUDEN_LAYOUT_Y_ADDRESS_FROM_EBP);

			if ( iDeltaTickCnt >= iDeltaInRepaintterval ) {
				iDeltaTickCnt = 0;
				// 描画更新
				pUpdatePaint(*pX, *pY);
			}

			iPrevWindowX = *pX;
			iPrevWindowY = *pY;

			// マウス操作などで、ウィンドウを動かしたら、10フレームは描画しない。
			if (*pX != iPrevWindowX || *pY != iPrevWindowY) {
				iDeltaTickCnt = 0;
			} else {
				iDeltaTickCnt++;
			}


		// 最初きたら、とりあえずデルタカウントを増やす
		} else if (isRetsudenEditorExtViewing == false && iDeltaTickCnt < 1 ) {
			iDeltaTickCnt = 1;

		// デルタで１フレーム経過したら、最初の実行
		} else if (isRetsudenEditorExtViewing == false && iDeltaTickCnt >= 1 ) {
			isRetsudenEditorExtViewing = true;
			iDeltaTickCnt = iDeltaInRepaintterval;

			// 今のダイアログの座標
			int *pX = (int *)(pEBPOFViewingCastleRetsuden + GAMEDATASTRUCT_CASTLE_RETSUDEN_LAYOUT_X_ADDRESS_FROM_EBP);
			int *pY = (int *)(pEBPOFViewingCastleRetsuden + GAMEDATASTRUCT_CASTLE_RETSUDEN_LAYOUT_Y_ADDRESS_FROM_EBP);

			// 範囲チェック
			if ( 0 <= iCastleIDOFViewingCastleRetsudenFunc && iCastleIDOFViewingCastleRetsudenFunc < GAMEDATASTRUCT_CASTLE_NUM) {

				// 城ID
				int iCastleID = iCastleIDOFViewingCastleRetsudenFunc;

				// 城のフル名称
				string castlefullname = nb6castle[iCastleID].name;
				castlefullname = castlefullname + getCastleTypeName(nb6castle[iCastleID].title);

				// 城のランクは、通常は0と1だが…
				int iCastleRank = nb6castle[iCastleID].bLarge;

				// 安土城と大阪城に関しては、
				const int kCastleOosaka = 167;
				const int kCastleAzuchi = 136;

				// 大坂城で、完成している、超巨城なのでランクを２に
				if ( iCastleID == kCastleOosaka && isOsakaCastleComplete() ) {
					iCastleRank = 2;
				}
				// 安土城で、完成している。超巨城なのでランクを２に
				else if ( iCastleID == kCastleAzuchi && isAzuchiCastleComplete() ) {
					iCastleRank = 2;
				}

				// 最初のペイント。
				pBeginPaint(iCastleIDOFViewingCastleRetsudenFunc, (char *)castlefullname.c_str(), *pX, *pY, iCastleRank);
			}
		}
	}

}

void OnTenshouExeViewingCastleRetsudenFuncExecutePrev() {

	// メインウィンドウの「城」ボタンから入ってきているかどうか
	if (!isMainWindowCastleButtonMode) {
		return;
	}

	// 城IDをスタックから取得
	WORD iCastleID = *((WORD *)(pEBPOFViewingCastleRetsuden + 0x8)) - 1;

	if ( 0 <= iCastleID && iCastleID < GAMEDATASTRUCT_CASTLE_NUM ) {
		iCastleIDOFViewingCastleRetsudenFunc = iCastleID;
		iDeltaTickCnt = 0;
		isRetsudenEditorExtRepaint=true;
	} else {
		isRetsudenEditorExtRepaint=false;
	}
}

void OnTenshouExeViewingCastleRetsudenFuncExecutePost() {

	// メインウィンドウの「城」ボタンから入ってきているかどうか
	if (!isMainWindowCastleButtonMode) {
		return;
	}

	if (hCASTLEMAPINRETSUDENDLL && pEndPaint) {
		pEndPaint();
	}

	isRetsudenEditorExtRepaint=false;
	isRetsudenEditorExtViewing=false;
	iDeltaTickCnt = 0;
}


/*
0045376B  |. E8 94350200    CALL    TENSHOU.00476D04                 ; \TENSHOU.00476D04
00453770  |. E8 62810200    CALL    TENSHOU.0047B8D7
00453775  |. 8D8D F4FDFFFF  LEA     ECX, DWORD PTR SS:[EBP-20C]
*/
int pTenshouExeJumpFromToOnTenshouExeViewingCastleRetsudenFunc	   = 0x453770; // 関数はこのアドレスから、OnTenshouExeViewingCastleRetsudenFuncへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeViewingCastleRetsudenFunc = 0x47B8D7; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeViewingCastleRetsudenFunc  = 0x453775; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeViewingCastleRetsudenFunc() {
	// スタックにためておく
	__asm {
		mov pEBPOFViewingCastleRetsuden, EBP // EBP+0x8のアドレスに、城番号(+1ベース)が入っている。
		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}
	
	OnTenshouExeViewingCastleRetsudenFuncExecutePrev();

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

		// 元の処理をここに記載
		call pTenshouExeJumpCallFromToOnTenshouExeViewingCastleRetsudenFunc
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


	OnTenshouExeViewingCastleRetsudenFuncExecutePost();

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

		jmp pTenshouExeReturnLblFromOnTenshouExeViewingCastleRetsudenFunc
	}

}




char cmdOnTenshouExeJumpFromViewingCastleRetsudenFunc[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計5バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeViewingCastleRetsudenFunc() {

	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeViewingCastleRetsudenFunc;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeViewingCastleRetsudenFunc + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeViewingCastleRetsudenFunc  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromViewingCastleRetsudenFunc+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeViewingCastleRetsudenFunc), cmdOnTenshouExeJumpFromViewingCastleRetsudenFunc, 5, NULL); //5バイトのみ書き込む
}




