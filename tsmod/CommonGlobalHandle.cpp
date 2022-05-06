#include "CommonGlobalHandle.h"

HANDLE CommonGlobalHandle::hTenshouExe = NULL;
char CommonGlobalHandle::szTenshouExeFullPath[MAX_PATH] = "";
HANDLE CommonGlobalHandle::hDLLModule = NULL;
HWND CommonGlobalHandle::tenshouWND = NULL;
HANDLE CommonGlobalHandle::curProcess = NULL;
DWORD CommonGlobalHandle::tenshouThreadId = 0;
HDC CommonGlobalHandle::hDC = NULL;
BOOL CommonGlobalHandle::isNobunaganoYabouGameStart = false;
BOOL CommonGlobalHandle::isMainGameStart = false;
BOOL CommonGlobalHandle::isModeScenario = false;
HMODULE CommonGlobalHandle::hResourceWavMod = NULL;
HMODULE CommonGlobalHandle::hResourceWavMod2 = NULL;
HMODULE CommonGlobalHandle::hResourceWavMod3 = NULL;
BOOL CommonGlobalHandle::isNobunaganoYabouGameEnd = false;
int CommonGlobalHandle::isBattleMode = 0;
int CommonGlobalHandle::isPlayerMainScreenInputWait = 0; // ���C����ʂŃv���C������E�B���h�E�ƌR�c�E�B���h�E���o���āA�R�}���h���͉\�ȏ�ԁB
HMENU CommonGlobalHandle::hMenu = NULL;
HMENU CommonGlobalHandle::hScenarioModPopMenu = NULL;
BOOL CommonGlobalHandle::isShiftKey = false;
BOOL CommonGlobalHandle::isNoDiskMode = false;
BOOL CommonGlobalHandle::isExistDisk = true;
BOOL CommonGlobalHandle::isEndingMode = false;
BOOL CommonGlobalHandle::isEndingModeAfter = false;
HDC CommonGlobalHandle::curHDC = NULL;
BOOL CommonGlobalHandle::IsAfterScreenSizeSelected = FALSE;
HWND CommonGlobalHandle::hWndTaskBar = NULL;


// �N�����̃I�v�V�����\����
RECT CommonGlobalHandle::latestCenterWindowRect = {0};
/*
typedef struct _STARTUPINFO {
    DWORD  cb;              // �\���̃T�C�Y
    LPTSTR  lpReserved;     // �\��(0)
    LPTSTR  lpDesktop;      // �f�X�N�g�b�v
    LPTSTR  lpTitle;        // �E�B���h�E�^�C�g��
    DWORD  dwX;             // x�ʒu
    DWORD  dwY;             // y�ʒu
    DWORD  dwXSize;         // x�T�C�Y
    DWORD  dwYSize;         // y�T�C�Y
    DWORD  dwXCountChars;   // ������
    DWORD  dwYCountChars;   // ������
    DWORD  dwFillAttribute; // �����F�E�w�i�F
    DWORD  dwFlags;         // �t���O
    WORD   wShowWindow;     // �E�B���h�E�\���`�� WinMain�֐��̈����ł���unCmdShow�v�Ɠ����l
    WORD   cbReserved2;     // �\��(0)
    LPBYTE lpReserved2;     // �\��(0)
    HANDLE hStdInput;       // �W������
    HANDLE hStdOutput;      // �W���o��
    HANDLE hStdError;       // �W���G���[�o��
} STARTUPINFO, *LPSTARTUPINFO;
*/

int WINAPI Extern_IsBattleMode() {
	return CommonGlobalHandle::isBattleMode;
}