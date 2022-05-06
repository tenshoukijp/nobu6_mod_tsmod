#include <windows.h>

// �w���CD�f�o�C�X�ɉ��yCD�������Ă��邩�ǂ����̃`�F�b�N
BOOL IsExistCDAudio(MCIDEVICEID mciDeviceId)
{
	/*
	���炩�̏�Ԃ̎擾��m�F�ɂ́AMCI_STATUS�R�}���h���g�p���邱�ƂɂȂ�܂��B
	MCI_STATUS_PARMS.dwItem��MCI_STATUS_MEDIA_PRESENT���w�肷��΁ACD���g���C�ɃZ�b�g����Ă��邩�m�F����A
	dwReturn�����o���炻�̌��ʂ�m�邱�Ƃ��ł��܂��B

	���ꂪ0�ł���ꍇ�́ACD���g���C�ɃZ�b�g����Ă��Ȃ����Ƃ��Ӗ����邽�߁A
	�Z�b�g���邩�ǂ�����MessageBox�ő����悤�ɂ��Ă��܂��B

	���������ꍇ�́AMCI_SET�R�}���h��MCI_SET_DOOR_OPEN���w�肵�ăg���C���J���܂��B
	�����āA���yCD���ǂ������m�F���鏈�������Ă݂܂��B 
	*/
	MCI_STATUS_PARMS mciStatus;
	
	mciStatus.dwItem = MCI_STATUS_MEDIA_PRESENT;
	mciSendCommand(mciDeviceId, MCI_STATUS, MCI_STATUS_ITEM, (DWORD_PTR)&mciStatus);
	if (mciStatus.dwReturn == 0) {
		if (MessageBox(NULL, "CD���Z�b�g����Ă��܂���BCD���Z�b�g���܂���", NULL, MB_YESNO) == IDYES) {
			MCI_SET_PARMS mciSet;
			mciSendCommand(mciDeviceId, MCI_SET, MCI_SET_DOOR_OPEN, (DWORD_PTR)&mciSet);
		}
		return FALSE;
	}

	/*
	���yCD���ǂ����𒲂ׂ�ɂ�dwItem�����o��MCI_CDA_STATUS_TYPE_TRACK���w�肵�AdwTrack�����o��1��ݒ肵�܂��B
	���yCD���f�[�^CD���̔��f�̓f�B�X�N�P�ʂł͂Ȃ��g���b�N�P�ʂōs���܂��B
	�Q�[������CD�ł͈ꖇ�̃f�B�X�N�ɁA���y�ƃf�[�^�̃g���b�N�����݂��Ă��邽�߁A�g���b�N�P�ʂŒ��ׂ�K�v������̂ł��B
	dwTrack�����o���g�p���Ă���̂�mciSendCommand�ł́AMCI_TRACK��Y��Ă͂����܂���B
	
	���yCD�̏ꍇ��dwReturn�����o��MCI_CDA_TRACK_AUDIO�ɂȂ�A
	����ȊO�̏ꍇ��MCI_CDA_TRACK_OTHER�ɂȂ�܂��B 
	*/
	mciStatus.dwItem  = MCI_CDA_STATUS_TYPE_TRACK;
	mciStatus.dwTrack = 1;
	mciSendCommand(mciDeviceId, MCI_STATUS, MCI_STATUS_ITEM | MCI_TRACK, (DWORD_PTR)&mciStatus);
	if (mciStatus.dwReturn != MCI_CDA_TRACK_AUDIO) {
		MessageBox(NULL, "���yCD�ł͂���܂���B", NULL, MB_ICONWARNING);
		return FALSE;
	}

	return TRUE;
}



int CDDeviceOpenSample() {
	
	/*
	   CD�������ꍇ�ł��A�ŏ��ɂ��ׂ�������MCI�f�o�C�X�̃I�[�v���ɂȂ�܂��B 
	   MCI_OPEN_PARMS.lpstrDeviceType�ɂ́A"CDAudio"�܂���MCI_DEVTYPE_CD_AUDIO���w�肷�邱�Ƃ��ł��A
	   ��҂̓^�C�vID�ł��邽�߁AmciSendCommand�̑�3������MCI_OPEN_TYPE_ID���w�肵�܂��B
	
	   IsExistCDAudio�Ƃ�������֐��́ACD�g���C��CD���Z�b�g����Ă��邩���m�F����Ƌ��ɁA
	  ���ꂪ���yCD�ł��邩���m�F���܂��B �O�҂̏����́A���̂悤�ɂȂ��Ă��܂��B 
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
	   IsExistCDAudio�ŉ��yCD���Z�b�g����Ă��邩���m�F�����ꍇ�́A ����CD�̏����擾���鏈�������s����܂��B 

	   �����擾����ꍇ��MCI_INFO�R�}���h���g�p���A��4������MCI_INFO_PARMS�\���̂��w�肵�܂��B
	   mciSendCommand�̑�3������MCI_INFO_PRODUCT���w�肵���ꍇ�́A
	   �n�[�h�E�F�A�̐��������o�b�t�@�Ɋi�[����A MCI_INFO_MEDIA_IDENTITY���w�肵���ꍇ�́A
	   ���yCD�Ɋ��蓖�Ă��Ă��鎯�ʎq���i�[����܂��B 
   */

	mciInfo.dwRetSize   = sizeof(szProduct) / sizeof(TCHAR);
	mciInfo.lpstrReturn = szProduct;
	mciSendCommand(mciDeviceId, MCI_INFO, MCI_INFO_PRODUCT, (DWORD_PTR)&mciInfo);

	mciInfo.dwRetSize   = sizeof(szIdentity) / sizeof(TCHAR);
	mciInfo.lpstrReturn = szIdentity;
	mciSendCommand(mciDeviceId, MCI_INFO, MCI_INFO_MEDIA_IDENTITY, (DWORD_PTR)&mciInfo);

	return 0;
}