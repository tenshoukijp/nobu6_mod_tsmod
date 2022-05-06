#include <windows.h>

// 指定のCDデバイスに音楽CDが入っているかどうかのチェック
BOOL IsExistCDAudio(MCIDEVICEID mciDeviceId)
{
	/*
	何らかの状態の取得や確認には、MCI_STATUSコマンドを使用することになります。
	MCI_STATUS_PARMS.dwItemにMCI_STATUS_MEDIA_PRESENTを指定すれば、CDがトレイにセットされているか確認され、
	dwReturnメンバからその結果を知ることができます。

	これが0である場合は、CDがトレイにセットされていないことを意味するため、
	セットするかどうかをMessageBoxで促すようにしています。

	応答した場合は、MCI_SETコマンドにMCI_SET_DOOR_OPENを指定してトレイを開きます。
	続いて、音楽CDかどうかを確認する処理を見てみます。 
	*/
	MCI_STATUS_PARMS mciStatus;
	
	mciStatus.dwItem = MCI_STATUS_MEDIA_PRESENT;
	mciSendCommand(mciDeviceId, MCI_STATUS, MCI_STATUS_ITEM, (DWORD_PTR)&mciStatus);
	if (mciStatus.dwReturn == 0) {
		if (MessageBox(NULL, "CDがセットされていません。CDをセットしますか", NULL, MB_YESNO) == IDYES) {
			MCI_SET_PARMS mciSet;
			mciSendCommand(mciDeviceId, MCI_SET, MCI_SET_DOOR_OPEN, (DWORD_PTR)&mciSet);
		}
		return FALSE;
	}

	/*
	音楽CDかどうかを調べるにはdwItemメンバにMCI_CDA_STATUS_TYPE_TRACKを指定し、dwTrackメンバに1を設定します。
	音楽CDかデータCDかの判断はディスク単位ではなくトラック単位で行います。
	ゲーム等のCDでは一枚のディスクに、音楽とデータのトラックが混在しているため、トラック単位で調べる必要があるのです。
	dwTrackメンバを使用しているのでmciSendCommandでは、MCI_TRACKを忘れてはいけません。
	
	音楽CDの場合はdwReturnメンバがMCI_CDA_TRACK_AUDIOになり、
	それ以外の場合はMCI_CDA_TRACK_OTHERになります。 
	*/
	mciStatus.dwItem  = MCI_CDA_STATUS_TYPE_TRACK;
	mciStatus.dwTrack = 1;
	mciSendCommand(mciDeviceId, MCI_STATUS, MCI_STATUS_ITEM | MCI_TRACK, (DWORD_PTR)&mciStatus);
	if (mciStatus.dwReturn != MCI_CDA_TRACK_AUDIO) {
		MessageBox(NULL, "音楽CDではありません。", NULL, MB_ICONWARNING);
		return FALSE;
	}

	return TRUE;
}



int CDDeviceOpenSample() {
	
	/*
	   CDを扱う場合でも、最初にすべき処理はMCIデバイスのオープンになります。 
	   MCI_OPEN_PARMS.lpstrDeviceTypeには、"CDAudio"またはMCI_DEVTYPE_CD_AUDIOを指定することができ、
	   後者はタイプIDであるため、mciSendCommandの第3引数にMCI_OPEN_TYPE_IDを指定します。
	
	   IsExistCDAudioという自作関数は、CDトレイにCDがセットされているかを確認すると共に、
	  それが音楽CDであるかも確認します。 前者の処理は、次のようになっています。 
    */

	MCIDEVICEID    mciDeviceId;
	MCI_OPEN_PARMS mciOpen;
	MCI_INFO_PARMS mciInfo;
	TCHAR          szProduct[256];
	TCHAR          szIdentity[256];
	
	mciOpen.lpstrDeviceType = (LPCTSTR)MCI_DEVTYPE_CD_AUDIO;
	mciSendCommand(0, MCI_OPEN, MCI_OPEN_TYPE | MCI_OPEN_TYPE_ID, (DWORD_PTR)&mciOpen);
	
	mciDeviceId = mciOpen.wDeviceID;

	if (!IsExistCDAudio(mciDeviceId)) {
		mciSendCommand(mciDeviceId, MCI_CLOSE, 0, 0);
		return 0;
	}

	/*
	   IsExistCDAudioで音楽CDがセットされているかを確認した場合は、 そのCDの情報を取得する処理が実行されます。 

	   情報を取得する場合はMCI_INFOコマンドを使用し、第4引数にMCI_INFO_PARMS構造体を指定します。
	   mciSendCommandの第3引数にMCI_INFO_PRODUCTを指定した場合は、
	   ハードウェアの説明文がバッファに格納され、 MCI_INFO_MEDIA_IDENTITYを指定した場合は、
	   音楽CDに割り当てられている識別子が格納されます。 
   */

	mciInfo.dwRetSize   = sizeof(szProduct) / sizeof(TCHAR);
	mciInfo.lpstrReturn = szProduct;
	mciSendCommand(mciDeviceId, MCI_INFO, MCI_INFO_PRODUCT, (DWORD_PTR)&mciInfo);

	mciInfo.dwRetSize   = sizeof(szIdentity) / sizeof(TCHAR);
	mciInfo.lpstrReturn = szIdentity;
	mciSendCommand(mciDeviceId, MCI_INFO, MCI_INFO_MEDIA_IDENTITY, (DWORD_PTR)&mciInfo);

	return 0;
}