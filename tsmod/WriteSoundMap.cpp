#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include "WriteSoundMap.h"
#include "SoundServer.h"

char *lpCommonMapAddress;

HANDLE hFMWrite;
extern HANDLE globalSndHANDLE;

int CreateSoundFMap() {
	// すでに存在したら、閉じる
	if (hFMWrite) {
		CloseHandle(hFMWrite);
	}

	// CreateFileMapping関数でファイルマッピングオブジェクトを作成します。 
	hFMWrite = CreateFileMapping( (HANDLE)-1, NULL, PAGE_READWRITE, 0, sizeof(TSoundWaveParam)*SOUND_SERVER_ARRAY_CNT, "TENSHOUKI_SOUND_MAPPING"); // 安全を見て、転送データサイズ*10個まで。

	if (hFMWrite == NULL) {
		return -1;
	}

	// 書き込みモード
	lpCommonMapAddress = (LPSTR)MapViewOfFile(hFMWrite, FILE_MAP_ALL_ACCESS, 0, 0, 0);

	// 失敗した
	if (lpCommonMapAddress == NULL) {
		return -2;
	}

	// ０で埋める
	memset(lpCommonMapAddress, NULL, sizeof(TSoundWaveParam)*SOUND_SERVER_ARRAY_CNT );

	// 書き込みモード解除
	UnmapViewOfFile(lpCommonMapAddress);

	// 無事終了
	return 0;
}

// 共有メモリへの書き込み。
// lpStr
int CleanSoundFMap() {

	if (hFMWrite == NULL)
		return -1;

	// 書き込みモード
	lpCommonMapAddress = (LPSTR)MapViewOfFile(hFMWrite, FILE_MAP_ALL_ACCESS, 0, 0, 0);

	// 失敗した
	if (lpCommonMapAddress == NULL) {
		return -2;
	}

	// ０で埋める
	memset(lpCommonMapAddress, NULL, sizeof(TSoundWaveParam)*SOUND_SERVER_ARRAY_CNT );
	// 書き込みモード解除
	UnmapViewOfFile(lpCommonMapAddress);

	// 無事終了
	return 0;
}


// 共有メモリへの書き込み。
// lpStr
int WriteSoundFMap(TSoundWaveParam *pWriteData) {

	if (hFMWrite == NULL) {
		return -1;
	}

	// 書き込みモード
	lpCommonMapAddress = (LPSTR)MapViewOfFile(hFMWrite, FILE_MAP_ALL_ACCESS, 0, 0, 0);

	// 失敗した
	if (lpCommonMapAddress == NULL) {
		return -2;
	}

	// 共有マップはTSoundWaveParam型の配列(SOUND_SERVER_ARRAY_CNT)とみなして…
	TSoundWaveParam *t = (TSoundWaveParam *)lpCommonMapAddress;

	for ( int i=0; i<SOUND_SERVER_ARRAY_CNT; i++, t++ ) {

		// まだデータが詰まっていない枠であれば、データをそこにコピー
		// t[i]->isEnteringData に 0 をセッティングするのは、受信側
		if ( t && t->isEnteringData == 0 ) {

			t->hSound		   = pWriteData->hSound;
			t->iResID		   = pWriteData->iResID;
			strcpy( t->hModuleName, pWriteData->hModuleName);
			t->px			   = pWriteData->px;
			t->py			   = pWriteData->py;
			t->pz			   = pWriteData->pz;
			t->vx			   = pWriteData->vx;
			t->vy			   = pWriteData->vy;
			t->vz			   = pWriteData->vz;
			t->isLoop		   = pWriteData->isLoop;
			strcpy(t->szCmd     , pWriteData->szCmd );		 
			strcpy(t->szFileName, pWriteData->szFileName );		 

			t->isEnteringData = 1;
			// １つ書いたので終了
			break;
		}
	}

	// 書き込みモード解除
	UnmapViewOfFile(lpCommonMapAddress);

	// 無事終了
	return 0;
}


int CloseSoundFMap() {
	if (hFMWrite == NULL) {
		return -1;
	}

	// 書き込みモード
	lpCommonMapAddress = (LPSTR)MapViewOfFile(hFMWrite, FILE_MAP_ALL_ACCESS, 0, 0, 0);

	if (lpCommonMapAddress == NULL) {
		return -2;
	}

	// ０で埋める
	memset(lpCommonMapAddress, NULL, sizeof(TSoundWaveParam)-1 );

	// 書き込みモード解除
	UnmapViewOfFile(lpCommonMapAddress);

	// 閉じる
	if (hFMWrite) {
		CloseHandle(hFMWrite);
	}

	// 無事終了
	return 0;
}

