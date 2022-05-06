#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS

// �u�Ȃ��UNICODE��˂�I�v�Ƃ����˂����݈ȊO�͂Ƃ��ɖ��Ȃ��Ǝv���܂�(^^;�@
#define UNICODE
#define _UNICODE

#include "WinTarget.h"
#include <tlhelp32.h>
#include <tchar.h>

// tlhelp32.h�̓v���Z�X�̌������s�����߂Ɏg���֐��̒�`�Ȃ̂ł����A�ڂ����͂̂��قǐ������܂��B
// TARGET_EXE_NAME��DLL�𒍓�����v���Z�X�̖��O�ADLL_FILE_NAME�͒�������DLL�̃t�@�C�����ł��BDLL��exe�t�@�C���Ɠ����t�H���_�ɂ���Ȃ���΂Ȃ�܂���B
#define TARGET_EXE_NAME  "IEXPLORE.EXE"
#define DLL_FILE_NAME    "ClipBoard.dll"

/*
int APIENTRY _tWinMain(HINSTANCE hInstance,
                       HINSTANCE hPrevInstance,
                       LPTSTR    lpCmdLine,
                       int       nCmdShow)
*/
int InputDllToOtherProcess()
{
	// ���������v���Z�X�̌������s���܂��B
	// CreateToolhelp32Snapshot�̓v���Z�X�̗񋓂������Ȃ��^�X�N�}�l�[�W���[�I�ȃv���O���������ۂɗ��p�����v���O�����ŁA
	// �����Ă��̏ꍇProcess32First��Process32Next�ƃZ�b�g�ɂ��Ďg���APROCESSENTRY32�\���̂�pe�Ƀv���Z�X�̏�񂪓���܂��B
    HANDLE hSnap;
    if((hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0)) == INVALID_HANDLE_VALUE){
        MessageBox(NULL, _TEXT("CreateToolhelp32Snapshot"), _TEXT("Error"), MB_OK);
        return -1;
    }

    PROCESSENTRY32 pe;
    pe.dwSize = sizeof(pe);

    DWORD dwProcessId = 0;
    BOOL bResult = Process32First(hSnap, &pe);
    while(bResult){
        if(!lstrcmp(pe.szExeFile, _TEXT(TARGET_EXE_NAME))){
            dwProcessId = pe.th32ProcessID;
            break;
        }
        bResult = Process32Next(hSnap, &pe);
    }
    CloseHandle(hSnap);

    if(dwProcessId == 0){
        MessageBox(NULL, _TEXT("Process not found."), _TEXT("ERROR"), MB_OK);
        return -1;
    }

	// ����̓v���Z�X���J���֐��ł��i�݂�΂킩��܂����j�B
	// �v���Z�XID��n���΂��̃v���Z�X���J���Ă����̂ŁA
	// ������Process32Next�Ȃǂ��g���Č������Ă���TARGET_EXE_NAME�̃v���Z�XID�i�܂�DLL�𒍓�����^�[�Q�b�g�̃v���Z�XID�j��n���Ă��܂��B
	// �߂�l�͂��̃v���Z�X�̃n���h���ł��B
    HANDLE hProcess;
    hProcess = OpenProcess(
         PROCESS_QUERY_INFORMATION |
         PROCESS_CREATE_THREAD     |
         PROCESS_VM_OPERATION      |
         PROCESS_VM_WRITE,
         FALSE, dwProcessId);

    if(hProcess == NULL){
        MessageBox(NULL, _TEXT("OpenProcess"), _TEXT("ERROR"), MB_OK);
        return -1;
    }

	// DLL�̃p�X���쐬���Ă��܂��B
	// �܂�GetModuleFileName�Ŏ������g�i�܂�exe�t�@�C���j�̐�΃p�X���擾���āA
	//���̕�����̌�납��T���čŏ��Ɂu\�v���������ꂽ�Ƃ��납��Ȍ�DLL�t�@�C���̖��O�ɒu�������Ă��܂��B
	// �܂莩���Ɠ����t�H���_�ɂ���DLL�̐�΃p�X���쐬�����킯�ł��B
    TCHAR szLibFile[256];
    GetModuleFileName(NULL, szLibFile, sizeof(szLibFile));
    _tcscpy(_tcsrchr(szLibFile, _TEXT('\\')) + 1, _TEXT(DLL_FILE_NAME));

    int szLibFileLen;
    szLibFileLen = lstrlen(szLibFile) + 1;
    szLibFileLen = szLibFileLen * sizeof(TCHAR);

	// �܂�VirtualAllocEx�Ń^�[�Q�b�g�v���Z�X�Ƀ�������Ԃ��쐬���Ă��܂��B
	// �����DLL�̐�΃p�X�����郁������Ԃł��B
	// ������WriteProcessMemory��DLL�̐�΃p�X����������ł��܂��B
	// �����VirtualAllocEx�̖߂�l��DLL�̐�΃p�X�̈ʒu�i�|�C���^�j�Ƃ������ƂɂȂ�̂ŁA�����CreateRemoteThread�̈����Ƃ��ēn���܂��B
	// �v����ɕʃv���Z�X���Ƀ��������m�ۂ��āA�m�ۂ����擪�A�h���X��Ԃ�
    PWSTR RemoteProcessMemory;
    RemoteProcessMemory = (PWSTR)VirtualAllocEx(
        hProcess, NULL, szLibFileLen, MEM_COMMIT, PAGE_READWRITE);
    if(RemoteProcessMemory == NULL){
        MessageBox(NULL, _TEXT("VirtualAllocEx"), _TEXT("ERROR"), MB_OK);
        return -1;
    }

	// �����ɁADLL�̃t���l�[������������
    if(WriteProcessMemory(hProcess, RemoteProcessMemory,
        (PVOID)szLibFile, szLibFileLen, NULL) == 0){
            MessageBox(NULL, _TEXT("WriteProcessMemory"), _TEXT("ERROR"), MB_OK);
            return -1;
    }

	// GetProcAddress��Kernel32.dll����LoadLibraryW�iUNICODE�p�j��T���Ă��Ă��̃A�h���X��Ԃ��Ă��܂��B
	// �����UNICODE�p�̃\�[�X�R�[�h�ł��̂�LoadLibraryW�ł����AASCII�Ȃ�LoadLibraryA�𗘗p���Ă��������B
	// Windows2000/XP��LoadLibraryA���g���Ă������I��UNICODE�ɕϊ�����Č���LoadLibraryW���Ă�ł���悤�Ȃ̂ŁA
	// UNICODE��ASCII���œ��ɋC�ɂ���K�v�͂Ȃ��ł��傤�B
    PTHREAD_START_ROUTINE pfnThreadRtn;
    pfnThreadRtn = (PTHREAD_START_ROUTINE)GetProcAddress(
        GetModuleHandle(_TEXT("Kernel32")), "LoadLibraryW");
    if (pfnThreadRtn == NULL){
        MessageBox(NULL, _TEXT("GetProcAddress"), _TEXT("ERROR"), MB_OK);
        return -1;
    }


	// ����CreateRemoteThread���Ă΂�܂����B
	// ��������OpenProcess�̖߂�l�ł���v���Z�X�̃n���h���ł��B
	// ��l������LoadLibraryW�̃A�h���X�B��܈�����DLL��΃p�X�̃A�h���X�B
	// �����CreateRemoteThread���Ăяo���΃^�[�Q�b�g�̃v���Z�X��LoadLibraryW�X���b�h����������A
	// �����A�C�ӂ�DLL���^�[�Q�b�g�v���Z�X�Ƀ}�b�s���O����邱�ƂɂȂ�܂��B
	// �����āA���̃v���O�����iin_dll.exe�j�͏I�����܂��B���̃v���O������DLL���}�b�s���O����Ă���Ȃ�΁A
	// �I���Ɠ�����DLL���̂��I�����܂��B�������ʂ̃v���Z�X�Ƀ}�b�s���O�������Ȃ�Ύ������g���I�����Ă�DLL�̏����͐��������܂��BIEXPLORE.EXE�Ȃ��IE���I������܂Ő��������܂����Aexplorer.exe�Ƃ�����Windows�̓���ɕs���ȃv���Z�X�ȂǂɃ}�b�s���O�������Ȃ��Windows�̏I���܂Ő��������邱�ƂɂȂ�܂��B
    HANDLE hThread;
    hThread = CreateRemoteThread(hProcess, NULL, 0, 
        pfnThreadRtn, RemoteProcessMemory, 0, NULL);
    if (hThread == NULL){
        MessageBox(NULL, _TEXT("CreateRemoteThread"), _TEXT("ERROR"), MB_OK);
        return -1;
    }

    Sleep(5000);
    return 0;
}
