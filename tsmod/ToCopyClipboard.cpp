// CopyHideName.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "WinTarget.h"


// ********************************************************* 
// クリップボードへテキストをコピー 
// 戻り値 : 成功 true, 失敗 false 
// ********************************************************* 
BOOL ToCopyClipboard( LPTSTR szData ) 
{ 

	HGLOBAL hGlobal; 
	LPTSTR pMem; 

	hGlobal = GlobalAlloc(GHND, lstrlen( szData ) + 128 ); 
	if ( hGlobal == NULL ) { 
		return false; 
	} 

	pMem = (LPTSTR)GlobalLock( hGlobal ); 
	if ( pMem == NULL ) { 
		GlobalFree( hGlobal ); 
		return false; 
	} 

	lstrcpy( pMem, szData ); 
	GlobalUnlock( hGlobal ); 
	OpenClipboard( NULL ); 
	EmptyClipboard(); 
	SetClipboardData(CF_TEXT, hGlobal); 
	CloseClipboard(); 

	return true; 
}
