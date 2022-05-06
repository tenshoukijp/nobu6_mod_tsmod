#define _CRT_SECURE_NO_WARNINGS

#include "WinTarget.h"
#include "CurProcessMemory.h"
#include "DialogKahou.h"
#include "GameDataStruct.h"
#include "DialogKahouMovie.h"

DialogKahouMode DialogKahou::eModeKahou = KAHOU_MODE_NO;
char DialogKahou:: szMovieKahouTmpName[256] = "";

extern int iKahouShowingMode; // OnShowingKahouDetail.cpp で定義されている。家宝詳細を見ている間は1
extern int iKahouIDOnViewingKahouID; // 家宝IDが入っている。配列用のIDになっている。

#include <fstream>

using namespace std;


/*
0041D002  /$ 55             PUSH    EBP
0041D003  |. 8BEC           MOV     EBP, ESP
0041D005  |. 83EC 30        SUB     ESP, 30
0041D008  |. 53             PUSH    EBX
0041D009  |. 56             PUSH    ESI
0041D00A  |. 57             PUSH    EDI
0041D00B  |. 33FF           XOR     EDI, EDI
0041D00D  |. 57             PUSH    EDI                              ; /hWnd => NULL
0041D00E  |. FF15 08764E00  CALL    DWORD PTR DS:[<&USER32.GetDC>]   ; \GetDC
0041D014  |. 50             PUSH    EAX                              ; /hDC
0041D015  |. 8BF0           MOV     ESI, EAX                         ; |
0041D017  |. FF15 F0734E00  CALL    DWORD PTR DS:[<&GDI32.CreateComp>; \CreateCompatibleDC
0041D01D  |. A3 001A4D00    MOV     DWORD PTR DS:[4D1A00], EAX
0041D022  |. 3BC7           CMP     EAX, EDI
0041D024  |. 75 0F          JNZ     SHORT TENSHOU.0041D035
0041D026  |. 56             PUSH    ESI                              ; /hDC
0041D027  |. 57             PUSH    EDI                              ; |hWnd => NULL
0041D028  |. FF15 0C764E00  CALL    DWORD PTR DS:[<&USER32.ReleaseDC>; \ReleaseDC
0041D02E  |. 33C0           XOR     EAX, EAX
0041D030  |. E9 19010000    JMP     TENSHOU.0041D14E
0041D035  |> 68 84C84B00    PUSH    TENSHOU.004BC884                 ; /FaceName = "真天翔明朝"
0041D03A  |. 33FF           XOR     EDI, EDI                         ; |
0041D03C  |. 57             PUSH    EDI                              ; |PitchAndFamily => DEFAULT_PITCH|FF_DONTCARE
0041D03D  |. 57             PUSH    EDI                              ; |Quality => DEFAULT_QUALITY
0041D03E  |. 57             PUSH    EDI                              ; |ClipPrecision => CLIP_DEFAULT_PRECIS
0041D03F  |. 57             PUSH    EDI                              ; |OutputPrecision => OUT_DEFAULT_PRECIS
0041D040  |. 68 80000000    PUSH    80                               ; |CharSet = 128.
0041D045  |. 57             PUSH    EDI                              ; |StrikeOut => FALSE
0041D046  |. 57             PUSH    EDI                              ; |Underline => FALSE
0041D047  |. 57             PUSH    EDI                              ; |Italic => FALSE
0041D048  |. 57             PUSH    EDI                              ; |Weight => FW_DONTCARE
0041D049  |. 57             PUSH    EDI                              ; |Orientation => 0
0041D04A  |. 57             PUSH    EDI                              ; |Escapement => 0
0041D04B  |. 6A 08          PUSH    8                                ; |Width = 8
0041D04D  |. 6A F0          PUSH    -10                              ; |Height = FFFFFFF0 (-16.)
0041D04F  |. FF15 E4734E00  CALL    DWORD PTR DS:[<&GDI32.CreateFont>; \CreateFontA
0041D055  |. A3 FC194D00    MOV     DWORD PTR DS:[4D19FC], EAX
0041D05A  |. 3BC7           CMP     EAX, EDI
0041D05C  |. 75 1B          JNZ     SHORT TENSHOU.0041D079
0041D05E  |. A1 001A4D00    MOV     EAX, DWORD PTR DS:[4D1A00]
0041D063  |. 50             PUSH    EAX                              ; /hDC => NULL
0041D064  |. FF15 18744E00  CALL    DWORD PTR DS:[<&GDI32.DeleteDC>] ; \DeleteDC
0041D06A  |. 56             PUSH    ESI                              ; /hDC
0041D06B  |. 57             PUSH    EDI                              ; |hWnd => NULL
0041D06C  |. FF15 0C764E00  CALL    DWORD PTR DS:[<&USER32.ReleaseDC>; \ReleaseDC
0041D072  |. 33C0           XOR     EAX, EAX
0041D074  |. E9 D5000000    JMP     TENSHOU.0041D14E
0041D079  |> C745 D0 280000>MOV     DWORD PTR SS:[EBP-30], 28
0041D080  |. C745 D4 100000>MOV     DWORD PTR SS:[EBP-2C], 10
0041D087  |. C745 D8 F0FFFF>MOV     DWORD PTR SS:[EBP-28], -10
0041D08E  |. 66:C745 DC 010>MOV     WORD PTR SS:[EBP-24], 1
0041D094  |. 66:C745 DE 010>MOV     WORD PTR SS:[EBP-22], 1
0041D09A  |. 33DB           XOR     EBX, EBX
0041D09C  |. 895D E0        MOV     DWORD PTR SS:[EBP-20], EBX
0041D09F  |. 895D E4        MOV     DWORD PTR SS:[EBP-1C], EBX
0041D0A2  |. 895D E8        MOV     DWORD PTR SS:[EBP-18], EBX
0041D0A5  |. 895D EC        MOV     DWORD PTR SS:[EBP-14], EBX
0041D0A8  |. 895D F0        MOV     DWORD PTR SS:[EBP-10], EBX
0041D0AB  |. 895D F4        MOV     DWORD PTR SS:[EBP-C], EBX
0041D0AE  |. 885D FA        MOV     BYTE PTR SS:[EBP-6], BL
0041D0B1  |. 885D F8        MOV     BYTE PTR SS:[EBP-8], BL
0041D0B4  |. 53             PUSH    EBX                              ; /Offset => 0
0041D0B5  |. 885D F9        MOV     BYTE PTR SS:[EBP-7], BL          ; |
0041D0B8  |. 53             PUSH    EBX                              ; |hSection => NULL
0041D0B9  |. 8D45 D0        LEA     EAX, DWORD PTR SS:[EBP-30]       ; |
0041D0BC  |. C645 FE FF     MOV     BYTE PTR SS:[EBP-2], 0FF         ; |
0041D0C0  |. C645 FC FF     MOV     BYTE PTR SS:[EBP-4], 0FF         ; |
0041D0C4  |. C645 FD FF     MOV     BYTE PTR SS:[EBP-3], 0FF         ; |
0041D0C8  |. 68 28FD4D00    PUSH    TENSHOU.004DFD28                 ; |ppBits = TENSHOU.004DFD28
0041D0CD  |. 53             PUSH    EBX                              ; |Usage => DIB_RGB_COLORS
0041D0CE  |. 50             PUSH    EAX                              ; |pBitmapInfo
0041D0CF  |. 56             PUSH    ESI                              ; |hDC
0041D0D0  |. FF15 E8734E00  CALL    DWORD PTR DS:[<&GDI32.CreateDIBS>; \CreateDIBSection
0041D0D6  |. A3 F8194D00    MOV     DWORD PTR DS:[4D19F8], EAX
0041D0DB  |. 3BC3           CMP     EAX, EBX
0041D0DD  |. A1 FC194D00    MOV     EAX, DWORD PTR DS:[4D19FC]
0041D0E2  |. 50             PUSH    EAX                              ; /hObject => NULL
0041D0E3  |. 75 1E          JNZ     SHORT TENSHOU.0041D103           ; |
0041D0E5  |. FF15 EC734E00  CALL    DWORD PTR DS:[<&GDI32.DeleteObje>; \DeleteObject
0041D0EB  |. A1 001A4D00    MOV     EAX, DWORD PTR DS:[4D1A00]
0041D0F0  |. 50             PUSH    EAX                              ; /hDC => NULL
0041D0F1  |. FF15 18744E00  CALL    DWORD PTR DS:[<&GDI32.DeleteDC>] ; \DeleteDC
0041D0F7  |. 56             PUSH    ESI                              ; /hDC
0041D0F8  |. 53             PUSH    EBX                              ; |hWnd => NULL
0041D0F9  |. FF15 0C764E00  CALL    DWORD PTR DS:[<&USER32.ReleaseDC>; \ReleaseDC
0041D0FF  |. 33C0           XOR     EAX, EAX
0041D101  |. EB 4B          JMP     SHORT TENSHOU.0041D14E
0041D103  |> FF35 001A4D00  PUSH    DWORD PTR DS:[4D1A00]            ; |hDC = NULL
0041D109  |. FF15 F4734E00  CALL    DWORD PTR DS:[<&GDI32.SelectObje>; \SelectObject
0041D10F  |. FF35 F8194D00  PUSH    DWORD PTR DS:[4D19F8]            ; /hObject = NULL
0041D115  |. FF35 001A4D00  PUSH    DWORD PTR DS:[4D1A00]            ; |hDC = NULL
0041D11B  |. FF15 F4734E00  CALL    DWORD PTR DS:[<&GDI32.SelectObje>; \SelectObject
0041D121  |. 6A 00          PUSH    0                                ; /Color = <BLACK>
0041D123  |. FF35 001A4D00  PUSH    DWORD PTR DS:[4D1A00]            ; |hDC = NULL
0041D129  |. FF15 F8734E00  CALL    DWORD PTR DS:[<&GDI32.SetBkColor>; \SetBkColor
0041D12F  |. 68 FFFFFF00    PUSH    0FFFFFF                          ; /Color = <WHITE>
0041D134  |. FF35 001A4D00  PUSH    DWORD PTR DS:[4D1A00]            ; |hDC = NULL
0041D13A  |. FF15 FC734E00  CALL    DWORD PTR DS:[<&GDI32.SetTextCol>; \SetTextColor
0041D140  |. 56             PUSH    ESI                              ; /hDC
0041D141  |. 6A 00          PUSH    0                                ; |hWnd = NULL
0041D143  |. FF15 0C764E00  CALL    DWORD PTR DS:[<&USER32.ReleaseDC>; \ReleaseDC
0041D149  |. B8 01000000    MOV     EAX, 1
0041D14E  |> 5F             POP     EDI
0041D14F  |. 5E             POP     ESI
0041D150  |. 5B             POP     EBX
0041D151  |. 8BE5           MOV     ESP, EBP
0041D153  |. 5D             POP     EBP
0041D154  \. C3             RETN

*/
extern int iCurKahouGazouWidthOnLayout;
extern int iCurKahouGazouHeightOnLayout;


struct BITMAPHEADER {			// ビットマップ情報とりだし用のヘッダ構造体。
	BITMAPFILEHEADER bfh;
	BITMAPINFOHEADER bih;
};
char bufBitMapData[1024];	   // mapdata.n6pの受け皿
/*
	BITMAPHEADER *BitMapHeader = (BITMAPHEADER *)bufBmpFile;

	//横と縦を指定サイズで上書き
void get

}
*/
// bmpのピクセル数(縦横)を得る
void getBmpPixelSize(char *szFileName, int *witdh, int *height) {
	FILE *fp = fopen( szFileName, "r");
	if ( !fp ) {
		return;
	}
	char szBuf[256];
	fread(szBuf, 256, 1, fp);
	BITMAPHEADER *BitMapHeader = (BITMAPHEADER *)szBuf;
	*witdh = BitMapHeader->bih.biWidth;
	*height = BitMapHeader->bih.biHeight;
	fclose(fp);
}


// 対象の家宝ＩＤのムービー版パスを求める。存在しなければNULL
static char szExistKahouFilePath[512] = "";
char* getExistMovieKahouFilePath(int iKahouID) {
	if ( 0 <= iKahouID && iKahouID < GAMEDATASTRUCT_KAHOU_NUM ) {
		char oMovieFileName[256] = "";
		char oMovieFileNameFullPath[256] = "";
		strcpy( oMovieFileName, nb6kahou[iKahouID].name);

		// 家宝の画像がある場所
		wsprintf(oMovieFileNameFullPath, ".\\kahou_24bit\\EX\\%s.avi", oMovieFileName);
		// 
		struct stat  st;
		// そのムービーがあるのなら、ムービーモードとする。
		if (stat(oMovieFileNameFullPath, &st) == 0) {
			strcpy(szExistKahouFilePath, oMovieFileNameFullPath);
			return szExistKahouFilePath;
		}
		strcpy(szExistKahouFilePath, "");
		return NULL;
	} else {
		strcpy(szExistKahouFilePath, "");
		return NULL;
	}
}

char* getExistPictureKahouFilePath(int iKahouID) {
	if ( 0 <= iKahouID && iKahouID < GAMEDATASTRUCT_KAHOU_NUM ) {
		char oBitMapFileName[256] = "";
		char oBitMapFileFullPath[256] = "";
		strcpy( oBitMapFileName, nb6kahou[iKahouID].name);

		// 家宝の画像がある場所
		wsprintf(oBitMapFileFullPath, ".\\kahou_24bit\\EX\\%s.bmp", oBitMapFileName);
		// 
		struct stat  st;
		// そのkahou_24bit/EXフォルダ内にあったらなら…。
		if (stat(oBitMapFileFullPath, &st) == 0) {
			strcpy(szExistKahouFilePath, oBitMapFileFullPath);
			return szExistKahouFilePath;
		}

		// 家宝の画像がある場所
		wsprintf(oBitMapFileFullPath, ".\\kahou_24bit\\%s.bmp", oBitMapFileName);
		// 
		// そのkahou_24bitフォルダ内にあったらなら…。
		if (stat(oBitMapFileFullPath, &st) == 0) {
			strcpy(szExistKahouFilePath, oBitMapFileFullPath);
			return szExistKahouFilePath;
		}

		strcpy(szExistKahouFilePath, "");
		return NULL;
	} else {
		strcpy(szExistKahouFilePath, "");
		return NULL;
	}
}

void DialogKahou::CheckDialogKahou (
  CONST RECT *lprcSrc1,  // 最初の長方形
  CONST RECT *lprcSrc2   // 2 番目の長方形
)
{

	// 座標情報保存
	setCurSrcRect( lprcSrc1, lprcSrc2 );
	// window_width = lprcSrc1->right - lprcSrc1->left;

	// 以下すべて else if にしないこと!! そのまま流す必要がある!!

	if ( getCurSrc1RectDidth() == 64 && getCurSrc1RectHeight() == 0 && eModeKahou != KAHOU_MODE_GAMEEND) {

		// デバイスコンテキスト作成
		CreateDCs();
		// 家宝を描画すべき場所を決める
		setKahouBMPPosition( lprcSrc1 );

		eModeKahou = KAHOU_MODE1;
	}

	// もし閉じたと思われる時 
	if ( !iKahouShowingMode && eModeKahou >= KAHOU_MODE2 ) {
		eModeKahou = KAHOU_MODE_NO;

		SelectObject( hCompatDC, hPrevBitmap );  // 以前のビットマップに選択を戻す
		DeleteObject( hBitmap );  // ロードしたビットマップを削除する

		DestroyKahouMovieMCI(); // こちらはムービー版の分
		is_movie_mode = false;

		return;
	}

	// 座標情報は保存した。次はフルカラー画像の家宝の名前を突き止める
	if ( eModeKahou == KAHOU_MODE1 && iKahouShowingMode ) {

		if ( ! m_szKahouName[0] ) {

			// まずムービーモードではない。
			is_movie_mode = false;

			// まずムービー方からチェック
			{
				char *p = getExistMovieKahouFilePath( iKahouIDOnViewingKahouID );
				if ( p ) {
					strcpy( szMovieKahouTmpName, p );
					is_movie_mode = true;
					eModeKahou = KAHOU_MODE2;
				}
			}


			// ムービーモードでないならば…
			if ( !is_movie_mode ) {

				char *p = getExistPictureKahouFilePath( iKahouIDOnViewingKahouID );
				if ( p ) {
					hBitmap = (HBITMAP)LoadImage( NULL, p, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE );
				} else {
					hBitmap = NULL;
				}
				// 改めて家宝画像がセットされたら･･･
				if (hBitmap) {
					hPrevBitmap = (HBITMAP)SelectObject( hCompatDC, hBitmap );

					// ビットマップ情報を格納しておく
					GetObject(hBitmap, sizeof(BITMAP), &bmp_info);
					eModeKahou = KAHOU_MODE2;
				}
			}
		}

	}

	// KAHOU_MODE2以上は、BMP表示可能状態
	if (eModeKahou >= KAHOU_MODE2 ) {

		if ( is_movie_mode ) {
			CreateKahouMovieMCI(szMovieKahouTmpName, getKahouBMPPositionX(), getKahouBMPPositionY() ); // この１行はムービー版の分
			MoveKahouMovieMCI( getKahouBMPPositionX(), getKahouBMPPositionY() );
		}

	// この条件を入れると、ウィンドウを動かしている時は、表示しない、除くとウインドウを動かしているときも表示できるが、チラチラする。
	// if ( getCurSrc1RectDidth()==0 && getCurSrc1RectHeight()==0 ) { 

		eModeKahou = KAHOU_MODE4;

		if ( !is_movie_mode ) {
			// ※※※※※※
			// 具体的に家宝の絵を表示
			BeginPaint( CommonGlobalHandle::tenshouWND, &ps );
			BitBlt( hDC, getKahouBMPPositionX() , getKahouBMPPositionY(), bmp_info.bmWidth, bmp_info.bmHeight, hCompatDC, 0, 0, SRCCOPY );
			EndPaint( CommonGlobalHandle::tenshouWND, &ps );
		}
	// }
	}

	if ( eModeKahou == KAHOU_MODE4 && is_movie_mode ) {
		MoveKahouMovieMCI( getKahouBMPPositionX(), getKahouBMPPositionY() );
	}
}



