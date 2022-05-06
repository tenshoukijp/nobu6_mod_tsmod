#define _CRT_SECURE_NO_WARNINGS

#include "WinTarget.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <vfw.h>

#include "CommonGlobalHandle.h"
#include "GameDataStruct.h"

// �ȈՔł̃��[�r�[�w�b�_�\��
struct MY_AVIHEADER {
	int dummy[16];
	int dwWidth;
	int dwHeight;
};

BOOL GetAviHeader(LPTSTR lpszFileName, MainAVIHeader *lpAviHeader, AVIStreamHeader *lpVideoStreamHeader, LPBITMAPINFO lpbm, AVIStreamHeader *lpAudioStreamHeader, LPWAVEFORMAT lpwf);
BOOL GetStreamHeader(HMMIO hmmio, AVIStreamHeader *lpStreamHeader, LPVOID lpFormat);

BOOL GetAviHeader(LPTSTR lpszFileName, MainAVIHeader *lpAviHeader, AVIStreamHeader *lpVideoStreamHeader, LPBITMAPINFO lpbmInfo, AVIStreamHeader *lpAudioStreamHeader, LPWAVEFORMAT lpwf)
{
	HMMIO    hmmio;
	MMCKINFO mmckRiff;
	MMCKINFO mmckList;
	MMCKINFO mmckAvih;
	
	hmmio = mmioOpen(lpszFileName, NULL, MMIO_READ);
	if (hmmio == NULL) {
		// MessageBox(NULL, TEXT("�t�@�C���̃I�[�v���Ɏ��s���܂����B"), NULL, MB_ICONWARNING);
		return FALSE;
	}
	
	mmckRiff.fccType = formtypeAVI;
	if (mmioDescend(hmmio, &mmckRiff, NULL, MMIO_FINDRIFF) != MMSYSERR_NOERROR) {
		// MessageBox(NULL, TEXT("AVI�t�@�C���ł͂���܂���B"), NULL, MB_ICONWARNING);
		mmioClose(hmmio, 0);
		return FALSE;
	}

	mmckList.fccType = listtypeAVIHEADER;
	if (mmioDescend(hmmio, &mmckList, NULL, MMIO_FINDLIST) != MMSYSERR_NOERROR) {
		mmioClose(hmmio, 0);
		return FALSE;
	}

	if (lpAviHeader != NULL) {
		mmckAvih.ckid = ckidAVIMAINHDR;
		if (mmioDescend(hmmio, &mmckAvih, NULL, MMIO_FINDCHUNK) != MMSYSERR_NOERROR) {
			mmioClose(hmmio, 0);
			return FALSE;
		}

		mmioRead(hmmio, (HPSTR)lpAviHeader, mmckAvih.cksize);
		mmioAscend(hmmio, &mmckAvih, 0);
	}

	if (!GetStreamHeader(hmmio, lpVideoStreamHeader, lpbmInfo)) {
		mmioClose(hmmio, 0);
		return FALSE;
	}
	
	if (!GetStreamHeader(hmmio, lpAudioStreamHeader, lpwf)) {
		mmioClose(hmmio, 0);
		return FALSE;
	}
	
	mmioAscend(hmmio, &mmckList, 0);
	mmioAscend(hmmio, &mmckRiff, 0);

	return TRUE;
}

BOOL GetStreamHeader(HMMIO hmmio, AVIStreamHeader *lpStreamHeader, LPVOID lpFormat)
{
	MMCKINFO mmckList;
	MMCKINFO mmckStrh;
	MMCKINFO mmckStrf;

	mmckList.fccType = listtypeSTREAMHEADER;
	if (mmioDescend(hmmio, &mmckList, NULL, MMIO_FINDLIST) != MMSYSERR_NOERROR)
		return FALSE;

	if (lpStreamHeader != NULL) {
		mmckStrh.ckid = ckidSTREAMHEADER;
		if (mmioDescend(hmmio, &mmckStrh, NULL, MMIO_FINDCHUNK) != MMSYSERR_NOERROR) {
			mmioAscend(hmmio, &mmckList, 0);
			return FALSE;
		}

		mmioRead(hmmio, (HPSTR)lpStreamHeader, mmckStrh.cksize);
		mmioAscend(hmmio, &mmckStrh, 0);
	}

	if (lpFormat != NULL) {
		mmckStrf.ckid = ckidSTREAMFORMAT;
		if (mmioDescend(hmmio, &mmckStrf, NULL, MMIO_FINDCHUNK) != MMSYSERR_NOERROR) {
			mmioAscend(hmmio, &mmckList, 0);
			return FALSE;
		}

		mmioRead(hmmio, (HPSTR)lpFormat, mmckStrf.cksize);
		mmioAscend(hmmio, &mmckStrf, 0);
	}

	mmioAscend(hmmio, &mmckList, 0);

	return TRUE;
}




int iMovieMoveWindow_X = 160;		// �ꉞ�ŏ��f�t�H���g��
int iMovieMoveWindow_Y = 120;		// �ꉞ�ŏ��f�t�H���g��
int iMovieMoveWindow_Width  = 320;  // �ꉞ�ŏ��f�t�H���g��
int iMovieMoveWindow_Height = 240;  // �ꉞ�ŏ��f�t�H���g��
HWND *hWndMovieMove = NULL;

int iEBP_OF_MovieMoveWindowExecute; // EBP�̒l�B�����ɁA�`�悷�ׂ����[�r�[�p�E�B���h�E�̂w,�x,�v,�g�̏�񂪋l�܂��Ă���B

void OnTenshouExeMovieMoveWindowExecute() {

	// EBP ���x�[�X�ɁA���[�r�[�̏c�T�C�Y�Ɖ��T�C�Y��A�w�C�x�𓾂�
	int *pEBP = (int *)iEBP_OF_MovieMoveWindowExecute;
	iMovieMoveWindow_Height = *(pEBP-1);
	iMovieMoveWindow_Width  = *(pEBP-2);
	iMovieMoveWindow_Y  = *(pEBP-3);
	iMovieMoveWindow_X  = *(pEBP-4);

	// ���̒l���v�Z����ƁA�N���C�A���g�̕��ƍ��������S�ɓ�����
	int iClientWidth  = iMovieMoveWindow_Width  + iMovieMoveWindow_X * 2;
	int iClientHeight = iMovieMoveWindow_Height + iMovieMoveWindow_Y * 2;

	// ���[�r�[�̃T�C�Y���f�t�H���g�̃T�C�Y�ł���΁A���������s����
	if ( iMovieMoveWindow_Width == 320 && iMovieMoveWindow_Height == 240 ) {

		// ���[�r�[�E�B���h�E�̃E�B���h�E�n���h���𓾂�B
		hWndMovieMove = (HWND *)0x4DFD90;

		// ���[�r�[�E�B���h�E�̃^�C�g���e�L�X�g�𓾂�
		char szTitleName[256] = "";
		GetWindowText( *hWndMovieMove, szTitleName, sizeof(szTitleName) );

		char szFullMovieName[256] = "movie\\";
		strcat_s( szFullMovieName, szTitleName );

		struct stat st;
		// ���̃��[�r�[�̓n�[�h�f�B�X�N�ɑ��݂��邩
		if (stat(szFullMovieName, &st) == 0) {

			FILE *fp = fopen(szFullMovieName, "r");

			if ( fp ) {

				MainAVIHeader   aviHeader;
				AVIStreamHeader videoStreamHeader;
				AVIStreamHeader audioStreamHeader;
				BITMAPINFO      bmInfo;
				WAVEFORMAT      wf;


				// ���[�r�[���o�O���Ă��邱�Ƃ�����̂ŃG���[����
				if (GetAviHeader(szFullMovieName, &aviHeader, &videoStreamHeader, &bmInfo, &audioStreamHeader, &wf) ) {

					// ���[�r�[�̃w�b�_�[��񂩂�A���ƍ����𓾂�
					int iMovieHeight = aviHeader.dwHeight;
					int iMovieWidgh  = aviHeader.dwWidth;

					// X��Y�̈ʒu���v�Z����B
					int iX = (iClientWidth - iMovieWidgh)/2;
					int iY = (iClientHeight- iMovieHeight)/2;

					// ��������X��EBP�̏ꏊ�ւƔ��f���A����̃��[�r�[�T�C�Y�A�Đ��ʒu�Ƃ���B
					*(pEBP-1) = iMovieHeight;
					*(pEBP-2) = iMovieWidgh;
					*(pEBP-3) = iY;
					*(pEBP-4) = iX;
				} else {
					// �G���[�̏ꍇ�́A�V�ċL����̃T�C�Y�Ƃ���
					int iMovieHeight = 240;
					int iMovieWidgh  = 320;

					// X��Y�̈ʒu���v�Z����B
					int iX = (iClientWidth - iMovieWidgh)/2;
					int iY = (iClientHeight- iMovieHeight)/2;

					// ��������X��EBP�̏ꏊ�ւƔ��f���A����̃��[�r�[�T�C�Y�A�Đ��ʒu�Ƃ���B
					*(pEBP-1) = iMovieHeight;
					*(pEBP-2) = iMovieWidgh;
					*(pEBP-3) = iY;
					*(pEBP-4) = iX;
				}

			}

		}
	}
}


/*
00�ϊi  |> 6A 01          PUSH    1                                        ; /Repaint = TRUE
0045320A  |. FF75 FC        PUSH    DWORD PTR SS:[EBP-4]                     ; |Height
0045320D  |. FF75 F8        PUSH    DWORD PTR SS:[EBP-8]                     ; |Width
00453210  |. FF75 F4        PUSH    DWORD PTR SS:[EBP-C]                     ; |Y
00453213  |. FF75 F0        PUSH    DWORD PTR SS:[EBP-10]                    ; |X
00453216  |. FF35 90FD4D00  PUSH    DWORD PTR DS:[4DFD90]                    ; |hWnd = 000420E6 ('SN4_OPENING.avi',class='AVIWnd32',parent=000620DA)
				�������� JMP TSMod.OnTSExeMovieMoveWindow �Ə��������Ă����ۂ̏���
0045321C  |. FF15 A8754E00  CALL    DWORD PTR DS:[<&USER32.MoveWindow>]      ; \MoveWindow
00453222  |. 6A 05          PUSH    5                                        ; /ShowState = SW_SHOW
*/
int pTenshouExeJumpFromToOnTenshouExeMovieMoveWindow	  =0x453208; // �֐��͂��̃A�h���X����AOnTenshouExeMovieMoveWindow�ւƃW�����v���Ă���B
int pTenshouExeReturnLblFromOnTenshouExeMovieMoveWindow	  =0x45321C; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeMovieMoveWindow() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {

		// ���[�r�[�̌��̍Đ��̈ʒu��傫�����R�s�[�B�ʏ��320,240�̒l�A���[�r�[�Q�{�T�C�Y�Đ�����640,480�̒l�ƂȂ�B
		mov iEBP_OF_MovieMoveWindowExecute, EBP;

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}
	
	OnTenshouExeMovieMoveWindowExecute();

	// �X�^�b�N�Ɉ����o��
	__asm {
		pop edi
		pop esi
		pop ebp
		pop esp
		pop edx
		pop ecx
		pop ebx
		pop eax

		
		push    1                                          //; /Repaint = TRUE
		push    DWORD PTR SS:[EBP-0x4]                     //; |Height
		push    DWORD PTR SS:[EBP-0x8]                     //; |Width
		push    DWORD PTR SS:[EBP-0xC]                     //; |Y
		push    DWORD PTR SS:[EBP-0x10]                    //; |X
		push    DWORD PTR DS:[0x4DFD90]                    //; |hWnd = 000420E6 ('********.avi',class='AVIWnd32',parent=000620DA)

		jmp pTenshouExeReturnLblFromOnTenshouExeMovieMoveWindow
	}
}


char cmdOnTenshouExeJumpFromMovieMoveWindow[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeMovieMoveWindow() {

	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeMovieMoveWindow;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeMovieMoveWindow + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeMovieMoveWindow  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromMovieMoveWindow+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeMovieMoveWindow), cmdOnTenshouExeJumpFromMovieMoveWindow, 5, NULL); //5�o�C�g�̂ݏ�������
}




