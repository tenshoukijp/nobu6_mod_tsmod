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
	// DLL�̑��݃`�F�b�N
	if (!hHDSettingDll) {
		hHDSettingDll = LoadLibrary("HD.setting.dll");
	}

	// �Ȃ���΁A�G���[
	if (!hHDSettingDll) {
		MessageBox(NULL, "HD.setting.dll������", "HD.setting.dll������", NULL);
	}

	// �t�@�C���̃f�R�[�h�p�r�̊֐�
	PFN_SETTING_MEMORYDECODE pSettingMemoryDecode = (PFN_SETTING_MEMORYDECODE)GetProcAddress(hHDSettingDll, "SettingMemoryDecode");

	// ����p�֐��B
	PFN_SETTING_FREEHANDLE   pSettingFreeHandle = (PFN_SETTING_FREEHANDLE)GetProcAddress(hHDSettingDll, "FreeHandle");
	// ���O��������Ȃ������Ńe�[�u�����Ԃ�Ă�̂ŁA�A�h���X���璼�ڈ���
	if (!pSettingFreeHandle) {
		pSettingFreeHandle = (PFN_SETTING_FREEHANDLE)((int)(hHDSettingDll)+(int)0x2170); // dependancy�̏����
	}
	// �f�R�[�h�p�r�̂���Ȃ���΁A�G���[�B���
	if (!pSettingMemoryDecode) {
		FreeLibrary(hHDSettingDll);
		return;
	}

	// �f�R�[�h�p�r�̂���Ȃ���΁A�G���[�B���
	if (!pSettingFreeHandle) {
		FreeLibrary(hHDSettingDll);
		return;
	}

	// �Ώۂ̃t�@�C�����������[�ւƃR�s�[
	CopyFileToMemory(szSrcFileName);

	int iDecodedDataLen = *(SettingFileMemory.pDecodedMemoryDataLen);

	int pOputputDataPointer = 0;
	// �������Ƀf�[�^���̗̈���m�ہB
	SettingFileMemory.pDecodedMemoryData = new BYTE[iDecodedDataLen];

	// ���g�̃N���A
	ZeroMemory(SettingFileMemory.pDecodedMemoryData, iDecodedDataLen);

	// ���̊֐����Ăяo�����ƂŁApOputputDataPointer�ɁA�f�[�^�ւ̃|�C���^�ւ̃A�h���X�����l�œ���
	pSettingMemoryDecode(SettingFileMemory.pEncodedMemoryData, SettingFileMemory.iEncodedMemoryDataLen, &pOputputDataPointer, &iDecodedDataLen, NULL, NULL);

	// �o�C�g�|�C���^�Ƃ��ĉ��߂���
	BYTE *p = (BYTE *)pOputputDataPointer;
	// �f�[�^�̒����̕������R�s�[
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
