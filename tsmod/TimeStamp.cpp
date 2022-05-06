#include "WinTarget.h"

#include <stdio.h>
#include <sys/types.h> 
#include <sys/stat.h>

#include "OutputDebugStream.h"

#include "TimeStamp.h"

// タイムスタンプが定番シリーズのものかどうか。
int isTimeStampTeibanSeries = TRUE;

bool CompareTimeStampTeibanSeiries(SYSTEMTIME systime);

void CheckTimeStampMyself() {
	// 自分(tenshou.exe)の名前を得る
	char szFilename[1024];
	GetModuleFileName(NULL,szFilename,sizeof(szFilename));

	HANDLE file;
	FILETIME file_time,local_file_time;

	SYSTEMTIME system_time;

	//ファイルを開く（存在するのときのみ）
	file = CreateFile(szFilename,GENERIC_READ,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if(file == INVALID_HANDLE_VALUE){

		// OutputDebugStream("ファイルが開けません。\n");
		return;

	}

	//-- 現在の更新時間
	//更新時間の取得（必要ない情報はNULLでＯＫ）
	GetFileTime(file,NULL,NULL,&file_time);
	//ローカルの時間へ変換
	FileTimeToLocalFileTime(&file_time,&local_file_time);//データの型は変わってない
	//SYSTEMTIME構造体へ変換
	FileTimeToSystemTime(&local_file_time,&system_time);//データの型の変換
	//取得情報の表示

	CompareTimeStampTeibanSeiries(system_time);

	//ファイルを閉じる
	CloseHandle(file);

	return;
}

bool CompareTimeStampTeibanSeiries(SYSTEMTIME systime) {

	// 2005/2/20 11:24:26 と比較

	if ( systime.wYear  == 2005 &&
		 systime.wMonth ==    2 &&
		 systime.wDay   ==   20 &&
		 systime.wHour  ==   11 &&
		 systime.wMinute==   24 &&
		 systime.wSecond==   26 ) {
			 return true; // 定番シリーズと一致する
	} else {
		isTimeStampTeibanSeries = FALSE;
		return false; // 定番シリーズと一致一致しない。
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
