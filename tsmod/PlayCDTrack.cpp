#include <windows.h>

#pragma comment(lib, "winmm.lib")

void PlayCDAudio(MCIDEVICEID mciDeviceId, DWORD dwTrackNo, BOOL bEndOfTrack);
BOOL IsExistCDAudio(MCIDEVICEID mciDeviceId);
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hinst, HINSTANCE hinstPrev, LPSTR lpszCmdLine, int nCmdShow)
{
	TCHAR      szAppName[] = TEXT("sample");
	HWND       hwnd;
	MSG        msg;
	WNDCLASSEX wc;

	wc.cbSize        = sizeof(WNDCLASSEX);
	wc.style         = 0;
	wc.lpfnWndProc   = WindowProc;
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = hinst;
	wc.hIcon         = (HICON)LoadImage(NULL, IDI_APPLICATION, IMAGE_ICON, 0, 0, LR_SHARED);
	wc.hCursor       = (HCURSOR)LoadImage(NULL, IDC_ARROW, IMAGE_CURSOR, 0, 0, LR_SHARED);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName  = NULL;
	wc.lpszClassName = szAppName;
	wc.hIconSm       = (HICON)LoadImage(NULL, IDI_APPLICATION, IMAGE_ICON, 0, 0, LR_SHARED);
	
	if (RegisterClassEx(&wc) == 0)
		return 0;

	hwnd = CreateWindowEx(0, szAppName, szAppName, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hinst, NULL);
	if (hwnd == NULL)
		return 0;

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);
	
	while (GetMessage(&msg, NULL, 0, 0) > 0) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static DWORD       dwTrackCount = 0;
	static HWND        hwndListBox = NULL;
	static MCIDEVICEID mciDeviceId = 0;

	switch (uMsg) {

	case WM_CREATE: {
		DWORD            i;
		DWORD            dwLength;
		TCHAR            szBuf[256];
		MCI_OPEN_PARMS   mciOpen;
		MCI_STATUS_PARMS mciStatus;
		
		hwndListBox = CreateWindowEx(0, TEXT("LISTBOX"), NULL, WS_CHILD | WS_VISIBLE | WS_VSCROLL | LBS_NOTIFY, 0, 0, 0, 0, hwnd, (HMENU)1, ((LPCREATESTRUCT)lParam)->hInstance, NULL);

		/*
		CD�������ꍇ�ł��A�ŏ��ɂ��ׂ�������MCI�f�o�C�X�̃I�[�v���ɂȂ�܂��B
		MCI_OPEN_PARMS.lpstrDeviceType�ɂ́A"CDAudio"�܂���MCI_DEVTYPE_CD_AUDIO���w�肷�邱�Ƃ��ł��A ��҂̓^�C�vID�ł��邽�߁AmciSendCommand�̑�3������MCI_OPEN_TYPE_ID���w�肵�܂��B 
		*/
		mciOpen.lpstrDeviceType = (LPCTSTR)MCI_DEVTYPE_CD_AUDIO;
		mciSendCommand(0, MCI_OPEN, MCI_OPEN_TYPE | MCI_OPEN_TYPE_ID, (DWORD_PTR)&mciOpen); // ���I�[�f�B�ICD�f�o�C�X�̃I�[�v��
	
		mciDeviceId = mciOpen.wDeviceID;

		if (!IsExistCDAudio(mciDeviceId))
			return -1;

		/*
		CD�Ɋ܂܂�Ă���g���b�N�̐��́AMCI_STATUS_NUMBER_OF_TRACKS���w�肵��MCI_STATUS�R�}���h�Ŏ擾���邱�Ƃ��ł��܂��B
		�֐����琧�䂪�Ԃ��dwReturn�����o�Ƀg���b�N�����i�[����邽�߁A
		�����dwTrackCount�Ƃ�����p�̕ϐ��ɑ�����A ���̐��������X�g�{�b�N�X�ɍ��ڂ�ǉ����Ă��܂��B 
		*/
		
		mciStatus.dwItem = MCI_STATUS_NUMBER_OF_TRACKS;
		mciSendCommand(mciDeviceId, MCI_STATUS, MCI_STATUS_ITEM, (DWORD_PTR)&mciStatus);

		dwTrackCount = (DWORD)mciStatus.dwReturn;

		/*
		���X�g�{�b�N�X����Đ�����g���b�N��I������ۂɁA
		���̃g���b�N�̍Đ����Ԃ��\������Ă���Ε֗��ł�����A
		MCI_STATUS_LENGTH���w�肵��MCI_STATUS�R�}���h�����s���Ă��܂��B
		
		dwTrack�́A�Đ����Ԃ��擾����g���b�N�̔ԍ��ł���A
		���ꂩ���1����n�܂邱�ƂɂȂ��Ă��邽�߁A+1����悤�ɂ��Ă��܂��B
		
		�擾�����Đ����Ԃ́AMCI_MSF_MINUTE�}�N���Łu���v�̒P�ʂɕϊ����邱�Ƃ��ł��A MCI_MSF_SECOND�}�N���Łu�b�v�̒P�ʂɕϊ����邱�Ƃ��ł��܂��B 
		*/

		for (i = 0; i < dwTrackCount; i++) {
			mciStatus.dwItem  = MCI_STATUS_LENGTH;
			mciStatus.dwTrack = i + 1;
			mciSendCommand(mciDeviceId, MCI_STATUS, MCI_STATUS_ITEM | MCI_TRACK, (DWORD_PTR)&mciStatus);

			dwLength = (DWORD)mciStatus.dwReturn;
			wsprintf(szBuf, TEXT("Track %d  %02d : %02d"), i + 1, MCI_MSF_MINUTE(dwLength), MCI_MSF_SECOND(dwLength));
			SendMessage(hwndListBox, LB_ADDSTRING, 0, (LPARAM)szBuf);
		}
		
		SetTimer(hwnd, 1, 200, NULL);

		return 0;
	}
	
	case WM_TIMER: {
		/*
		�g���b�N�̌��݂̍Đ����Ԃ́AWM_TIMER�Ŏ����I�Ɏ擾����邱�ƂɂȂ�܂��B 

		�܂��AMCI_STATUS_MODE���w�肵��MCI_STATUS�R�}���h�ŁA���݂�MCI�f�o�C�X�̏�Ԃ��擾���܂��B
		MCI_MODE_PLAY�łȂ��ꍇ�́A�Đ����ł͂Ȃ��Ƃ������ƂȂ̂ŁA ���̏ꍇ�͌��݈ʒu���擾����K�v�͂���܂���B
		���݈ʒu��MCI_STATUS_POSITION���w�肵��MCI_STATUS�R�}���h�Ŏ擾�\�ł���A
		MCI_TMSF_XXX�}�N�����g�p���邱�ƂŊe��P�ʂɕϊ��ł���悤�ɂȂ�܂��B 

		���X�g�{�b�N�X�̍��ڂ��I�����ꂽ�ꍇ�AwParam��LBN_SELCHANGE���i�[����WM_COMMAND�������܂��B
		���̎��́ALB_GETCURSEL�őI�����ꂽ���ڂ̃C���f�b�N�X���擾���邱�Ƃ��ł��邽�߁A
		�߂�l��+1���邱�ƂōĐ����ׂ��g���b�N�ԍ������ł��܂��B
		
		PlayCDAudio�Ƃ�������֐��́A��2�����Ŏw�肳�ꂽ�g���b�N�ԍ����Đ����܂��B
		*/


		DWORD            dwLength;
		TCHAR            szBuf[256];
		MCI_STATUS_PARMS mciStatus;
		
		mciStatus.dwItem = MCI_STATUS_MODE;
		mciSendCommand(mciDeviceId, MCI_STATUS, MCI_STATUS_ITEM, (DWORD_PTR)&mciStatus);
		if (mciStatus.dwReturn != MCI_MODE_PLAY)
			return 0;

		mciStatus.dwItem = MCI_STATUS_POSITION;
		mciSendCommand(mciDeviceId, MCI_STATUS, MCI_STATUS_ITEM, (DWORD_PTR)&mciStatus);

		dwLength = (DWORD)mciStatus.dwReturn;

		wsprintf(szBuf, TEXT("Track %d  %02d : %02d"), MCI_TMSF_TRACK(dwLength), MCI_TMSF_MINUTE(dwLength), MCI_TMSF_SECOND(dwLength));
		SetWindowText(hwnd, szBuf);

		return 0;
	}
	
	case WM_COMMAND: {
		DWORD dwTrackNo;

		if (HIWORD(wParam) != LBN_SELCHANGE)
			return 0;

		dwTrackNo = (DWORD)SendMessage(hwndListBox, LB_GETCURSEL, 0, 0) + 1;
		PlayCDAudio(mciDeviceId, dwTrackNo, dwTrackNo == dwTrackCount);

		return 0;
	}

	case WM_SIZE:
		MoveWindow(hwndListBox, 0, 0, LOWORD(lParam), HIWORD(lParam), TRUE);
		return 0;

	case WM_DESTROY:
		KillTimer(hwnd, 1);
		mciSendCommand(mciDeviceId, MCI_STOP, 0, 0);
		mciSendCommand(mciDeviceId, MCI_CLOSE, 0, 0);
		PostQuitMessage(0);
		return 0;

	default:
		break;

	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void PlayCDAudio(MCIDEVICEID mciDeviceId, DWORD dwTrackNo, BOOL bEndOfTrack)
{

	/*
	MCI_PLAY�R�}���h�����s����O�ɁAMCI_SET�R�}���h�Ń^�C���t�H�[�}�b�g�̐ݒ���s���Ă����܂��B
	����́AMCI_PLAY�ŃR�}���h�Đ��ʒu���w�肷��ۂɁA
	�g���b�N�P�ʂł̎w����\�ɂ��邽�߂ł��B
	]
	dwTimeFormat�����o��MCI_FORMAT_TMSF���w�肵�AmciSendCommand�̑�3������MCI_SET_TIME_FORMAT���w�肷�邱�ƂŁA
	�g���b�N�P�ʂł̎w�肪�\�ɂȂ�܂��B
	
	MCI_PLAY_PARMS.dwFrom�ɂ́A�Đ��̊J�n�ʒu���w�肵�܂��B
	
	�^�C���t�H�[�}�b�g�̐ݒ���s���Ă��邽�߁A MCI_MAKE_TMSF�}�N���̑�1�����Ƀg���b�N�ԍ����w�肷�邱�ƂŁA
	�w�肳�ꂽ�g���b�N����Đ������悤�ɂȂ�܂��B
	
	bEndOfTrack�́AdwTrackNo���ŏI�g���b�N�ł��邩�ǂ����������l�ł���A
	���ꂪFALSE�ł���ꍇ�͍Đ��̏I���ʒu���w�肷��K�v������܂��B
	
	�I���ʒu�́AMCI_PLAY_PARMS.dwTo�Ɏw�肷�邱�ƂɂȂ��Ă��邽�߁A
	MCI_MAKE_TMSF�}�N���̑�1�����Ɏ��̃g���b�N�ԍ����w�肷��΁A dwTrackNo�ŕ\�����g���b�N�݂̂��Đ�����邱�ƂɂȂ�܂��B
	
	dwFrom�����o���g�p����ꍇ�́AmciSendCommand�̑�3������MCI_FROM���w�肵�A dwTo�����o���g�p����ꍇ��MCI_TO���w�肵�܂��B
	*/
	
	DWORD          dwParam = MCI_FROM;
	MCI_SET_PARMS  mciSet;
	MCI_PLAY_PARMS mciPlay;
	
	mciSet.dwTimeFormat = MCI_FORMAT_TMSF;
	mciSendCommand(mciDeviceId, MCI_SET, MCI_SET_TIME_FORMAT, (DWORD_PTR)&mciSet);

	mciPlay.dwFrom = MCI_MAKE_TMSF(dwTrackNo, 0, 0, 0);
	if (!bEndOfTrack) {
		mciPlay.dwTo = MCI_MAKE_TMSF(dwTrackNo + 1, 0, 0, 0);
		dwParam |= MCI_TO;
	}

	mciSendCommand(mciDeviceId, MCI_PLAY, dwParam, (DWORD_PTR)&mciPlay);
}

BOOL IsExistCDAudio(MCIDEVICEID mciDeviceId)
{
	/*
	���炩�̏�Ԃ̎擾��m�F�ɂ́AMCI_STATUS�R�}���h���g�p���邱�ƂɂȂ�܂��B
	MCI_STATUS_PARMS.dwItem��MCI_STATUS_MEDIA_PRESENT���w�肷��΁A CD���g���C�ɃZ�b�g����Ă��邩�m�F����A
	dwReturn�����o���炻�̌��ʂ�m�邱�Ƃ��ł��܂��B
	���ꂪ0�ł���ꍇ�́ACD���g���C�ɃZ�b�g����Ă��Ȃ����Ƃ��Ӗ����邽�߁A
	�Z�b�g���邩�ǂ�����MessageBox�ő����悤�ɂ��Ă��܂��B
	
	���������ꍇ�́AMCI_SET�R�}���h��MCI_SET_DOOR_OPEN���w�肵�ăg���C���J���܂��B
	*/

	MCI_STATUS_PARMS mciStatus;
	
	mciStatus.dwItem = MCI_STATUS_MEDIA_PRESENT;
	mciSendCommand(mciDeviceId, MCI_STATUS, MCI_STATUS_ITEM, (DWORD_PTR)&mciStatus);
	if (mciStatus.dwReturn == 0) {
		if (MessageBox(NULL, TEXT("CD���Z�b�g����Ă��܂���BCD���Z�b�g���܂���"), NULL, MB_YESNO) == IDYES) {
			MCI_SET_PARMS mciSet;
			mciSendCommand(mciDeviceId, MCI_SET, MCI_SET_DOOR_OPEN, (DWORD_PTR)&mciSet);
		}
		return FALSE;
	}

	/*
	�����āA���yCD���ǂ������m�F���鏈�������Ă݂܂��B

	���yCD���ǂ����𒲂ׂ�ɂ�dwItem�����o��MCI_CDA_STATUS_TYPE_TRACK���w�肵�AdwTrack�����o��1��ݒ肵�܂��B
	���yCD���f�[�^CD���̔��f�̓f�B�X�N�P�ʂł͂Ȃ��g���b�N�P�ʂōs���܂��B
	
	�Q�[������CD�ł͈ꖇ�̃f�B�X�N�ɁA���y�ƃf�[�^�̃g���b�N�����݂��Ă��邽�߁A
	�g���b�N�P�ʂŒ��ׂ�K�v������̂ł��B
	
	dwTrack�����o���g�p���Ă���̂�mciSendCommand�ł́AMCI_TRACK��Y��Ă͂����܂���B
	���yCD�̏ꍇ��dwReturn�����o��MCI_CDA_TRACK_AUDIO�ɂȂ�A����ȊO�̏ꍇ��MCI_CDA_TRACK_OTHER�ɂȂ�܂��B 
	*/

	mciStatus.dwItem  = MCI_CDA_STATUS_TYPE_TRACK;
	mciStatus.dwTrack = 1;
	mciSendCommand(mciDeviceId, MCI_STATUS, MCI_STATUS_ITEM | MCI_TRACK, (DWORD_PTR)&mciStatus);
	if (mciStatus.dwReturn != MCI_CDA_TRACK_AUDIO) {
		MessageBox(NULL, TEXT("���yCD�ł͂���܂���B"), NULL, MB_ICONWARNING);
		return FALSE;
	}

	return TRUE;
}