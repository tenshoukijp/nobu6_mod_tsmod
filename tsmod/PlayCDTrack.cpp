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
		CDを扱う場合でも、最初にすべき処理はMCIデバイスのオープンになります。
		MCI_OPEN_PARMS.lpstrDeviceTypeには、"CDAudio"またはMCI_DEVTYPE_CD_AUDIOを指定することができ、 後者はタイプIDであるため、mciSendCommandの第3引数にMCI_OPEN_TYPE_IDを指定します。 
		*/
		mciOpen.lpstrDeviceType = (LPCTSTR)MCI_DEVTYPE_CD_AUDIO;
		mciSendCommand(0, MCI_OPEN, MCI_OPEN_TYPE | MCI_OPEN_TYPE_ID, (DWORD_PTR)&mciOpen); // ★オーディオCDデバイスのオープン
	
		mciDeviceId = mciOpen.wDeviceID;

		if (!IsExistCDAudio(mciDeviceId))
			return -1;

		/*
		CDに含まれているトラックの数は、MCI_STATUS_NUMBER_OF_TRACKSを指定したMCI_STATUSコマンドで取得することができます。
		関数から制御が返るとdwReturnメンバにトラック数が格納されるため、
		これをdwTrackCountという専用の変数に代入し、 この数だけリストボックスに項目を追加しています。 
		*/
		
		mciStatus.dwItem = MCI_STATUS_NUMBER_OF_TRACKS;
		mciSendCommand(mciDeviceId, MCI_STATUS, MCI_STATUS_ITEM, (DWORD_PTR)&mciStatus);

		dwTrackCount = (DWORD)mciStatus.dwReturn;

		/*
		リストボックスから再生するトラックを選択する際に、
		そのトラックの再生時間が表示されていれば便利ですから、
		MCI_STATUS_LENGTHを指定したMCI_STATUSコマンドを実行しています。
		
		dwTrackは、再生時間を取得するトラックの番号であり、
		これからは1から始まることになっているため、+1するようにしています。
		
		取得した再生時間は、MCI_MSF_MINUTEマクロで「分」の単位に変換することができ、 MCI_MSF_SECONDマクロで「秒」の単位に変換することができます。 
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
		トラックの現在の再生時間は、WM_TIMERで周期的に取得されることになります。 

		まず、MCI_STATUS_MODEを指定したMCI_STATUSコマンドで、現在のMCIデバイスの状態を取得します。
		MCI_MODE_PLAYでない場合は、再生中ではないということなので、 この場合は現在位置を取得する必要はありません。
		現在位置はMCI_STATUS_POSITIONを指定したMCI_STATUSコマンドで取得可能であり、
		MCI_TMSF_XXXマクロを使用することで各種単位に変換できるようになります。 

		リストボックスの項目が選択された場合、wParamにLBN_SELCHANGEを格納したWM_COMMANDが送られます。
		この時は、LB_GETCURSELで選択された項目のインデックスを取得することができるため、
		戻り値に+1することで再生すべきトラック番号を特定できます。
		
		PlayCDAudioという自作関数は、第2引数で指定されたトラック番号を再生します。
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
	MCI_PLAYコマンドを実行する前に、MCI_SETコマンドでタイムフォーマットの設定を行っておきます。
	これは、MCI_PLAYでコマンド再生位置を指定する際に、
	トラック単位での指定を可能にするためです。
	]
	dwTimeFormatメンバにMCI_FORMAT_TMSFを指定し、mciSendCommandの第3引数にMCI_SET_TIME_FORMATを指定することで、
	トラック単位での指定が可能になります。
	
	MCI_PLAY_PARMS.dwFromには、再生の開始位置を指定します。
	
	タイムフォーマットの設定を行っているため、 MCI_MAKE_TMSFマクロの第1引数にトラック番号を指定することで、
	指定されたトラックから再生されるようになります。
	
	bEndOfTrackは、dwTrackNoが最終トラックであるかどうかを示す値であり、
	これがFALSEである場合は再生の終了位置を指定する必要があります。
	
	終了位置は、MCI_PLAY_PARMS.dwToに指定することになっているため、
	MCI_MAKE_TMSFマクロの第1引数に次のトラック番号を指定すれば、 dwTrackNoで表されるトラックのみが再生されることになります。
	
	dwFromメンバを使用する場合は、mciSendCommandの第3引数にMCI_FROMを指定し、 dwToメンバを使用する場合はMCI_TOを指定します。
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
	何らかの状態の取得や確認には、MCI_STATUSコマンドを使用することになります。
	MCI_STATUS_PARMS.dwItemにMCI_STATUS_MEDIA_PRESENTを指定すれば、 CDがトレイにセットされているか確認され、
	dwReturnメンバからその結果を知ることができます。
	これが0である場合は、CDがトレイにセットされていないことを意味するため、
	セットするかどうかをMessageBoxで促すようにしています。
	
	応答した場合は、MCI_SETコマンドにMCI_SET_DOOR_OPENを指定してトレイを開きます。
	*/

	MCI_STATUS_PARMS mciStatus;
	
	mciStatus.dwItem = MCI_STATUS_MEDIA_PRESENT;
	mciSendCommand(mciDeviceId, MCI_STATUS, MCI_STATUS_ITEM, (DWORD_PTR)&mciStatus);
	if (mciStatus.dwReturn == 0) {
		if (MessageBox(NULL, TEXT("CDがセットされていません。CDをセットしますか"), NULL, MB_YESNO) == IDYES) {
			MCI_SET_PARMS mciSet;
			mciSendCommand(mciDeviceId, MCI_SET, MCI_SET_DOOR_OPEN, (DWORD_PTR)&mciSet);
		}
		return FALSE;
	}

	/*
	続いて、音楽CDかどうかを確認する処理を見てみます。

	音楽CDかどうかを調べるにはdwItemメンバにMCI_CDA_STATUS_TYPE_TRACKを指定し、dwTrackメンバに1を設定します。
	音楽CDかデータCDかの判断はディスク単位ではなくトラック単位で行います。
	
	ゲーム等のCDでは一枚のディスクに、音楽とデータのトラックが混在しているため、
	トラック単位で調べる必要があるのです。
	
	dwTrackメンバを使用しているのでmciSendCommandでは、MCI_TRACKを忘れてはいけません。
	音楽CDの場合はdwReturnメンバがMCI_CDA_TRACK_AUDIOになり、それ以外の場合はMCI_CDA_TRACK_OTHERになります。 
	*/

	mciStatus.dwItem  = MCI_CDA_STATUS_TYPE_TRACK;
	mciStatus.dwTrack = 1;
	mciSendCommand(mciDeviceId, MCI_STATUS, MCI_STATUS_ITEM | MCI_TRACK, (DWORD_PTR)&mciStatus);
	if (mciStatus.dwReturn != MCI_CDA_TRACK_AUDIO) {
		MessageBox(NULL, TEXT("音楽CDではありません。"), NULL, MB_ICONWARNING);
		return FALSE;
	}

	return TRUE;
}