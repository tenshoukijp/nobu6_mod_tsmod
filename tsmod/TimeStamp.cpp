#include "WinTarget.h"

#include <stdio.h>
#include <sys/types.h> 
#include <sys/stat.h>

#include "OutputDebugStream.h"

#include "TimeStamp.h"

// �^�C���X�^���v����ԃV���[�Y�̂��̂��ǂ����B
int isTimeStampTeibanSeries = TRUE;

bool CompareTimeStampTeibanSeiries(SYSTEMTIME systime);

void CheckTimeStampMyself() {
	// ����(tenshou.exe)�̖��O�𓾂�
	char szFilename[1024];
	GetModuleFileName(NULL,szFilename,sizeof(szFilename));

	HANDLE file;
	FILETIME file_time,local_file_time;

	SYSTEMTIME system_time;

	//�t�@�C�����J���i���݂���̂Ƃ��̂݁j
	file = CreateFile(szFilename,GENERIC_READ,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if(file == INVALID_HANDLE_VALUE){

		// OutputDebugStream("�t�@�C�����J���܂���B\n");
		return;

	}

	//-- ���݂̍X�V����
	//�X�V���Ԃ̎擾�i�K�v�Ȃ�����NULL�łn�j�j
	GetFileTime(file,NULL,NULL,&file_time);
	//���[�J���̎��Ԃ֕ϊ�
	FileTimeToLocalFileTime(&file_time,&local_file_time);//�f�[�^�̌^�͕ς���ĂȂ�
	//SYSTEMTIME�\���̂֕ϊ�
	FileTimeToSystemTime(&local_file_time,&system_time);//�f�[�^�̌^�̕ϊ�
	//�擾���̕\��

	CompareTimeStampTeibanSeiries(system_time);

	//�t�@�C�������
	CloseHandle(file);

	return;
}

bool CompareTimeStampTeibanSeiries(SYSTEMTIME systime) {

	// 2005/2/20 11:24:26 �Ɣ�r

	if ( systime.wYear  == 2005 &&
		 systime.wMonth ==    2 &&
		 systime.wDay   ==   20 &&
		 systime.wHour  ==   11 &&
		 systime.wMinute==   24 &&
		 systime.wSecond==   26 ) {
			 return true; // ��ԃV���[�Y�ƈ�v����
	} else {
		isTimeStampTeibanSeries = FALSE;
		return false; // ��ԃV���[�Y�ƈ�v��v���Ȃ��B
	}
	/*
	OutputDebugStream("%d/%d/%d %d:%d:%d:%d\n",systime.wYear

		,systime.wMonth
		,systime.wDay
		,systime.wHour
		,systime.wMinute
		,systime.wSecond
		,systime.wMilliseconds
		);
	*/
}
