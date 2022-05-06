#include <windows.h>
#include <stdio.h>

typedef void (WINAPI *PFN_SETTING_MEMORYDECODE) (BYTE *lpSrc, int iSrcLen, int* lpDst, int *iOrgLen, UINT msg, HWND hWnd);
typedef void (WINAPI *PFN_SETTING_FREEHANDLE) (void* lpDst);

static HMODULE hHDSettingDll = NULL;

struct TSETTINGFILEMEMORY {
	BYTE *pEncodedMemoryData;
	int iEncodedMemoryDataLen;

	BYTE *pDecodedMemoryData;
	int *pDecodedMemoryDataLen;
};

TSETTINGFILEMEMORY SettingFileMemory = { NULL, 0, NULL, NULL };

BOOL CopyFileToMemory(char *szFileName) {
	FILE *fp = fopen(szFileName, "rb");
	if (!fp) {
		return FALSE;
	}
	fseek(fp, 0, SEEK_END);
	SettingFileMemory.iEncodedMemoryDataLen = ftell(fp);

	fseek(fp, 0, SEEK_SET);

	SettingFileMemory.pEncodedMemoryData = new BYTE[SettingFileMemory.iEncodedMemoryDataLen];
	fread(SettingFileMemory.pEncodedMemoryData, SettingFileMemory.iEncodedMemoryDataLen, 1, fp);

	SettingFileMemory.pDecodedMemoryDataLen = (int *)(SettingFileMemory.pEncodedMemoryData + 4);
	fclose(fp);

	return TRUE;
}


void MallocLWCData(char *szSrcFileName) {
	// DLLの存在チェック
	if (!hHDSettingDll) {
		hHDSettingDll = LoadLibrary("HD.setting.dll");
	}

	// なければ、エラー
	if (!hHDSettingDll) {
		MessageBox(NULL, "HD.setting.dllが無い", "HD.setting.dllが無い", NULL);
	}

	// ファイルのデコード用途の関数
	PFN_SETTING_MEMORYDECODE pSettingMemoryDecode = (PFN_SETTING_MEMORYDECODE)GetProcAddress(hHDSettingDll, "SettingMemoryDecode");

	// 解放用関数。
	PFN_SETTING_FREEHANDLE   pSettingFreeHandle = (PFN_SETTING_FREEHANDLE)GetProcAddress(hHDSettingDll, "FreeHandle");
	// 名前から引けない感じでテーブルがつぶれてるので、アドレスから直接引く
	if (!pSettingFreeHandle) {
		pSettingFreeHandle = (PFN_SETTING_FREEHANDLE)((int)(hHDSettingDll)+(int)0x2170); // dependancyの情報より
	}
	// デコード用途のやつがなければ、エラー。解放
	if (!pSettingMemoryDecode) {
		FreeLibrary(hHDSettingDll);
		return;
	}

	// デコード用途のやつがなければ、エラー。解放
	if (!pSettingFreeHandle) {
		FreeLibrary(hHDSettingDll);
		return;
	}

	// 対象のファイルをメモリーへとコピー
	CopyFileToMemory(szSrcFileName);

	int iDecodedDataLen = *(SettingFileMemory.pDecodedMemoryDataLen);

	int pOputputDataPointer = 0;
	// メモリにデータ分の領域を確保。
	SettingFileMemory.pDecodedMemoryData = new BYTE[iDecodedDataLen];

	// 中身のクリア
	ZeroMemory(SettingFileMemory.pDecodedMemoryData, iDecodedDataLen);

	// この関数を呼び出すことで、pOputputDataPointerに、データへのポインタへのアドレスが数値で入る
	pSettingMemoryDecode(SettingFileMemory.pEncodedMemoryData, SettingFileMemory.iEncodedMemoryDataLen, &pOputputDataPointer, &iDecodedDataLen, NULL, NULL);

	// バイトポインタとして解釈して
	BYTE *p = (BYTE *)pOputputDataPointer;
	// データの長さの分だけコピー
	memcpy(SettingFileMemory.pDecodedMemoryData, p, iDecodedDataLen);

	pSettingFreeHandle(p);

}

void FreeLWCData() {
	if (SettingFileMemory.pDecodedMemoryData) {
		delete[] SettingFileMemory.pDecodedMemoryData;
	}
}

/*
void test(void) {
	MallocLWCData("systemcfgPK.n6p");

	FILE *fp = fopen("test2.dat", "wb");
	fwrite(SettingFileMemory.pDecodedMemoryData, *(SettingFileMemory.pDecodedMemoryDataLen), 1, fp);
	fclose(fp);

	FreeLWCData();
}
*/
