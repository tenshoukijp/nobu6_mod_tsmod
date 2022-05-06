//
#define _CRT_SECURE_NO_WARNINGS
#include "CommonGlobalHandle.h"
#include "GameDataStruct.h"
#include "ls11_mod.h"
#include "CnvHankakuZenkaku.h"
#include "GetOSVersion.h"

std::string HankakuKatakanaToZentakuKatakana(std::string szBufOriginal) {
	std::string szBufCurrent = szBufOriginal;

	Matches matches;

	// ���̓���ȋL���͓V�ċL�ɂ����āA���p�J�^�J�i��S�p�J�^�J�i�ɂ���Ƃ����Ӗ�
	for (int i=0; OnigMatch(szBufCurrent, "^(.*?)\x1b\x4b(.*?)\x1b\x48(.*?)$", &matches) && i<(int)szBufCurrent.size(); i++ ) { 
		// ��`����v�f�𒊏o����B
		if ( !matches[2].empty() ) {
			char szBufZenKatakana[256] = "";
			hankana2zenkana((unsigned char *)matches[2].c_str(), (unsigned char *)szBufZenKatakana);
			szBufZenKatakana[255] = NULL; // ����ȏC�����Ă��\�����Ȃ��悤�O�̂���

			szBufCurrent = matches[1] + szBufZenKatakana + matches[3];
		}
	}

	// �Ō�̕���L�����Ȃ��܂܍s�����}����ꍇ������B
	if ( OnigMatch(szBufCurrent, "^(.*?)\x1b\x4b(.+)$", &matches) ) {

		// ��`����v�f�𒊏o����B
		if ( !matches[2].empty() ) {
			char szBufZenKatakana[256] = "";
			hankana2zenkana((unsigned char *)matches[2].c_str(), (unsigned char *)szBufZenKatakana);
			szBufZenKatakana[255] = NULL; // ����ȏC�����Ă��\�����Ȃ��悤�O�̂���

			szBufCurrent = matches[1] + szBufZenKatakana;
		}
	}

	return szBufCurrent;
}


// bfile.n6p �� ���� NB6BushouExoption�Ɋi�[����B
int setBfileN6PToNB6BUSHOUEXOPTION() {

	// ���傤�� vBufDecodedData[0]=�P�Ԗڂ̕����t�@�C���̃f�[�^��AvBufDecodedData[1]=�Q�Ԗڂ̕����t�@�C���̃f�[�^��A�݂����Ȋ���
	vector<vector<byte>> vBufDecodedData;

	// ���k���ꂽLS11�t�H�[�}�b�g���f�R�[�h����֐�
	ls11_DecodePack("bfile.n6p", NULL, &vBufDecodedData );

	int iRetsudenNo = -1;

	// �f�R�[�h�����f�[�^��(�����I)�t�@�C���ł܂킵�Ă����B0�Ԃ͑Ώۂł͂Ȃ��B�����f�[�^��1�Ԃ���B
	int ivBufDecodedDataSize = vBufDecodedData.size();

	for ( int ifile = 1; ifile < (int)ivBufDecodedDataSize ; ifile++ ) {

		// ���傤�� vSplittedData[0]=�P�Ԗڂ̗v�f�̃f�[�^��AvSplittedData[1]=�Q�Ԗڂ̗v�f�̃f�[�^��A�݂����Ȋ���
		vector<vector<byte>> vSplittedData;

		ls11_SplitData( vBufDecodedData[ifile], &vSplittedData );

		int ivSplittedDataSize = vSplittedData.size();

		// �e�v�f���Ƃɥ�� �܂����O�A�ǂ݉��� ���v�N�̗�
		for ( int ielem=0; ielem < (int)ivSplittedDataSize; ielem++ ) {

			if ( ielem%2==0 ) { iRetsudenNo++; }

			if ( iRetsudenNo < 0 ) { iRetsudenNo = 0; }; // �ň��̂��߂̔ԕ�

			// ������`����Ă����`���𒴂������̂́A����
			if ( iRetsudenNo >= GAMEDATASTRUCT_RETSUDEN_NUM ) { continue; } // 1332�l�𒴂���ƁA������艺�ɂ͂����Ȃ��B�J�E���^�����܂킷�B

			// �������ǂ݉����Ƃ�
			if ( ielem%2==0 ) {

				string szBufCurrent;
				int datasize = vSplittedData[ielem].size();
				try {
					if ( datasize > 0 ) {

						char *bytedata = new char[datasize+1];
						memcpy( bytedata, &vSplittedData[ielem][0], datasize );
						bytedata[datasize] = NULL; // �ԕ�
						szBufCurrent = string(bytedata); 
						delete[] bytedata;
					}
				} catch (exception e) {
				}

				Matches matches;
				// ��`����v�f�𒊏o����B
				if ( OnigMatch(szBufCurrent, "^(.*?) \x1b\x6b(.*?)\x1b\x48 ((.+)�`(.+?))?\x05\x05\x05", &matches) ) {

					/*
					string name;		// ��\��
					string yomigana;	// ��݂���
					string born;	    // �a���N������(????)�Ƃ������邽��
					string death;	    // ���S�N������(????)�Ƃ������邽��
					string retsuden;	// ��`������
					*/

					nb6bushouretsuden[iRetsudenNo].name = matches[1];
					if ( !matches[2].empty() ) {

						// ���p�J�^�J�i��S�p�Ђ炪�Ȃ�
						char szBufZenHiranaga[128] = "";
						hankana2zenhira((unsigned char *)matches[2].c_str(), (unsigned char *)szBufZenHiranaga);
						szBufZenHiranaga[100] = NULL; // ����ȏC�����Ă��\�����Ȃ��悤�O�̂���

						nb6bushouretsuden[iRetsudenNo].yomigana = string(szBufZenHiranaga);
					}
					if ( !matches[4].empty() ) {
						nb6bushouretsuden[iRetsudenNo].born = matches[4];
					}
					if ( !matches[5].empty() ) {
						nb6bushouretsuden[iRetsudenNo].death = matches[5];
					}
				}

			// ��͗�`�B
			} else { 

				string szBufCurrent;

				int datasize = vSplittedData[ielem].size();
				try {
					if ( datasize > 0 ) {

						char *bytedata = new char[datasize+1];
						memcpy( bytedata, &vSplittedData[ielem][0], datasize );
						bytedata[datasize] = NULL; // �ԕ�
						szBufCurrent = string(bytedata); 
						delete[] bytedata;
					}
				} catch (exception e) {
				}

				Matches matches;

				// ���p�J�^�J�i��S�p�J�^�J�i�ɂ���B
				szBufCurrent = HankakuKatakanaToZentakuKatakana(szBufCurrent);

				// �Ō�̏I���L���ȍ~�͏����B
				if ( OnigMatch(szBufCurrent, "^(.+?)\x05\x05\x05", &matches) ) {

					if ( !matches[1].empty() ) {
						// �c��́A���p�J�^�J�i��S�p�Ђ炪�Ȃ�
						char szBufZenHiragana[256] = "";
						hankana2zenhira((unsigned char *)matches[1].c_str(), (unsigned char *)szBufZenHiragana);
						szBufZenHiragana[255] = NULL; // ����ȏC�����Ă��\�����Ȃ��悤�O�̂���
						nb6bushouretsuden[iRetsudenNo].retsuden = string(szBufZenHiragana);

					}
				}

			}

		}
	}


	// ���v��`����Ԃ��B
	return iRetsudenNo+1;
}





// ��`�̐l���̂Ƃ���̃j�[���j�b�N��bfile.n6p�����ɏ��������B�����Jumper�ł͂Ȃ��B
void WriteAsmOnTenshouExeRetsuDenExistNumOfBFileN6P() {
	// TENSHOU.EXE�̗�`�l��������ύX����B�Ó��Ȕ͈͂ɂ��邩�ǂ����`�F�b�N���Ă����B
	if ( 1300 <= iExistRetudenNumOfBFileN6P && iExistRetudenNumOfBFileN6P <= 0xFFFF ) { // 1932���ƃ����@�C�B
		WORD bExistNum = (WORD)iExistRetudenNumOfBFileN6P;

		WriteProcessMemory (CommonGlobalHandle::curProcess, (LPVOID)(GAMEDATASTRUCT_BUSHOU_RETSUDEN_NUM_PATCH_ADDRESS), &bExistNum, 2, NULL);
	}
}