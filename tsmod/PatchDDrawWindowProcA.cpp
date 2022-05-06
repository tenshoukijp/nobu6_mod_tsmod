#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "GameDataStruct.h"
#include "TSMod.h"

/*
 DDraw.dllの中で、リサイズを抑制するためには、WindowProcを制御する必要。

 DDrawは、WindowProcを自前のものに置き換えている。それが以下の記述。
 SetWindowLongに、-4というのが GWL_WNDPROC 新しい値が DDraw.5D762DD0で、WindowProcである。

	5D763E84   68 D02D765D      PUSH    DDraw.5D762DD0					← 新しいWindowsプロシージャ関数のポインタ。
																		DDrawは、Windowプロシージャを自分で定義したものに差し替えている
	5D763E89   A3 043AD65D      MOV     DWORD PTR DS:[5DD63A04], EAX
	5D763E8E   A1 5842D65D      MOV     EAX, DWORD PTR DS:[5DD64258]
	5D763E93   6A FC            PUSH    -4
	5D763E95   50               PUSH    EAX
	5D763E96   FF15 74E1765D    CALL    DWORD PTR DS:[<&USER32.SetWindow>; USER32.SetWindowLongA ← このwin32apiが、Windowプロシージャを自分で定義したものに差し替えるやつ。


	その新しいWindowプロシージャの関数の中身 Caseが以下
	5D762DD0   55               PUSH    EBP
	5D762DD1   8BEC             MOV     EBP, ESP
	5D762DD3   83E4 F8          AND     ESP, FFFFFFF8
	5D762DD6   8B4D 0C          MOV     ECX, DWORD PTR SS:[EBP+C]
	5D762DD9   8B55 14          MOV     EDX, DWORD PTR SS:[EBP+14]
	5D762DDC   83EC 1C          SUB     ESP, 1C
	5D762DDF   53               PUSH    EBX
	5D762DE0   56               PUSH    ESI
	5D762DE1   8B75 10          MOV     ESI, DWORD PTR SS:[EBP+10]
	5D762DE4   57               PUSH    EDI
	5D762DE5   83F9 7E          CMP     ECX, 7E							← ここはWM_DISPLAYCHANGE 
																			ディスプレイモード(256色とフルカラーとか)を変えようとした時に反応する。
	5D762DE8   0F87 37020000    JA      DDraw.5D763025
	5D762DEE   0F84 09020000    JE      DDraw.5D762FFD
	5D762DF4   83F9 1C          CMP     ECX, 1C							← ここはWM_ACTIVATEAPP                  
	5D762DF7   0F84 BF010000    JE      DDraw.5D762FBC
	5D762DFD   83F9 20          CMP     ECX, 20							← ここはWM_SETCURSOR
	5D762E00   0F84 C5000000    JE      DDraw.5D762ECB
	5D762E06   83F9 24          CMP     ECX, 24							← ここはWM_GETMINMAXINFO
	5D762E09   0F85 D1010000    JNZ     DDraw.5D762FE0
	5D762E0F   A1 043BD65D      MOV     EAX, DWORD PTR DS:[5DD63B04]
	5D762E14   3D 80020000      CMP     EAX, 280						← ここがリサイズのためのもの
	5D762E19   0F86 C1010000    JBE     DDraw.5D762FE0
	5D762E1F   8B0D 1C2B7C5D    MOV     ECX, DWORD PTR DS:[5D7C2B1C]
	5D762E25   8B3D 70E1765D    MOV     EDI, DWORD PTR DS:[<&USER32.SetR>; USER32.SetRect


DDrawの先頭アドレス(HINSTANCEのアドレス)から　+2E14のところにある。
/*

/*
 ウィンドウをリサイズが出来なくする

*/
// DDrawオリジナルハンドル
extern HINSTANCE hDDrawMod;

#define DDRAW_DLL_ENTRYPOINT_TO_JE_SETCURSOR					 0x2E00					// DDrawの先頭から、上記の「CMP EAX, 20(WM_SETCURSOR)」の次のジャンプに相当するアドレス(カーソル変更のため)
#define DDRAW_DLL_ENTRYPOINT_TO_COM_EAX_280						 0x2E15					// DDrawの先頭から、上記の「CMP EAX, 280(WM_GETMINMAXINFO)」の280に相当するアドレス(リサイズのため)

extern const char *DDRAW_DLL_FILENAME;

//
// DDrawのリサイズ機能を無くす
void patchDisableDDrawResizeFunction() {

	// カーソルを変更するのをやめる
	int o_JeSetCursor = (int)hDDrawMod + DDRAW_DLL_ENTRYPOINT_TO_JE_SETCURSOR; // DDrawの先頭から、上記の「CMP EAX, 280」の280に相当するアドレス

	char iCaseValueJe[] = "\x90\x90\x90\x90\x90\x90";

	// DDRAWの上記コメントのところを、CMP EAX 0x2020280 とする行為。case文にひっかからないようにする。
	WriteProcessMemory (CommonGlobalHandle::curProcess, (LPVOID)(o_JeSetCursor), &iCaseValueJe, 6, NULL); // 6バイト分かきかえ

	// リサイズをやめる
	int o_CmpEax280 = (int)hDDrawMod + DDRAW_DLL_ENTRYPOINT_TO_COM_EAX_280; // DDrawの先頭から、上記の「CMP EAX, 280」の280に相当するアドレス

	int iCaseValue280 = 0x2020280; // 絶対ヒットしないであろう適当な値。

	// DDRAWの上記コメントのところを、CMP EAX 0x2020280 とする行為。case文にひっかからないようにする。
	WriteProcessMemory (CommonGlobalHandle::curProcess, (LPVOID)(o_CmpEax280), &iCaseValue280, 4, NULL); // 4バイト分かきかえ
}


