// CopyHideName.cpp : �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
//

#include "WinTarget.h"


// ********************************************************* 
// �N���b�v�{�[�h�փe�L�X�g���R�s�[ 
// �߂�l : ���� true, ���s false 
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
