// AntiDebug.cpp : �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
//

#include "AntiDebug.h"
#include "WinTarget.h"

#define MAX_LOADSTRING 100

// �O���[�o���ϐ�:
HINSTANCE hInst;								// ���݂̃C���^�[�t�F�C�X
TCHAR szTitle[MAX_LOADSTRING];					// �^�C�g�� �o�[�̃e�L�X�g
TCHAR szWindowClass[MAX_LOADSTRING];			// ���C�� �E�B���h�E �N���X��

// ���̃R�[�h ���W���[���Ɋ܂܂��֐��̐錾��]�����܂�:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

typedef int (WINAPI* _NtQueryInformationProcess)(
HANDLE ProcessHandle,
UINT ProcessInformationClass,
PVOID ProcessInformation,
ULONG ProcessInformationLength,
PULONG ReturnLength
);

//����ȏ������s���̂ōœK���𖳌��ɂ���
#pragma optimize("", off)

/*
�f�o�b�O�|�[�g���`�F�b�N����APhantOm plugin�ŉ���\
�P���Ԃ�ƃf�o�b�K����
*/
int AntiDebug1(void)
{
	int i;
	UINT x;
	_NtQueryInformationProcess nqip;

	nqip = (_NtQueryInformationProcess)GetProcAddress(GetModuleHandle("ntdll.dll"), "NtQueryInformationProcess");
	if(!nqip)
	{
		return 1;	
	}

	i = (int)nqip((HANDLE)-1, 7, &x, 4, 0);
	if(i)
	{
		return 1;	
	}

	if(x)
	{
		return 1;	
	}

	return 0;
}

typedef int (WINAPI* _NtSetInformationThread)(
HANDLE ThreadHandle,
int ThreadInformationClass,
PVOID ThreadInformation,
ULONG ThreadInformationLength
);

/*
���X���b�h���f�o�b�O�Ώۂ���O���ăf�o�b�O�ł��Ȃ�������
*/
void AntiDebug2(void)
{
	_NtSetInformationThread nsit;

	nsit = (_NtSetInformationThread)GetProcAddress(GetModuleHandle("ntdll.dll"), "NtSetInformationThread");
	if(!nsit)
	{
		return;	
	}

	nsit((HANDLE)-2, 0x11, 0, 0);
}

/*
CPU�f�o�b�O�p�̗�O���ē��������
�P���Ԃ�ƃf�o�b�K����
*/

int AntiDebug3(void)
{
	int i;
	
	__try
	{
		_asm
		{
			//CPU�f�o�b�O�p�̗�O�𔭍s
			int 0xF1
			//�f�o�b�K������ꍇ�͂��̂܂܏��������s�����
		}
		i = 1;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		//�f�o�b�K�������ꍇ�͂����ŗ�O���󂯎��
		//�f�o�b�K���L���ȏꍇ�͂����ɗ��Ȃ�
		i = 0;
	}

	return i;
}

int AntiDebug4(void)
{
	int i;
		
	i = 1;

	__try
	{
		_asm
		{
			//��O�𔭍s
			int 0x2D
			//nop�͗�O�ŃX�L�b�v�����

			nop
			//�f�o�b�K������ꍇ�͂��̂܂܏��������s�����
		}
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		//�f�o�b�K�������ꍇ�͂����ŗ�O���󂯎��
		//�f�o�b�K���L���ȏꍇ�͂����ɗ��Ȃ�
		i = 0;
	}

	return i;
}


// �^��Ԃ����́A�f�o�b�O�n�b�N�����݂��Ă���Ƃ������ƁB
BOOL AntiDebugALL() {
	int i;
	
	i = AntiDebug1();
	if(i)
	{
		return 1;		
	}

	i = AntiDebug3();
	if(i)
	{
		return 1;		
	}

	i = AntiDebug4();
	if(i)
	{
		return 1;		
	}

	AntiDebug2();

	return 0;
}