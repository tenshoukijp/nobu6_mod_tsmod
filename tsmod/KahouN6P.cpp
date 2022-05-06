//
#define _CRT_SECURE_NO_WARNINGS
#include "CommonGlobalHandle.h"
#include "GameDataStruct.h"
#include "ls11_mod.h"
#include "CnvHankakuZenkaku.h"
#include "GetOSVersion.h"


extern std::string HankakuKatakanaToZentakuKatakana(std::string szBufOriginal);

// message.n6p �� ���̂����A�ƕ󖼂�ƕ��`��NB6KahouExoption�Ɋi�[����B
int setBKahouN6PToNB6KAHOUEXOPTION() {

	// ���傤�� vBufDecodedData[0]=�P�Ԗڂ̕����t�@�C���̃f�[�^��AvBufDecodedData[1]=�Q�Ԗڂ̕����t�@�C���̃f�[�^��A�݂����Ȋ���
	vector<vector<byte>> vBufDecodedData;

	// ���k���ꂽLS11�t�H�[�}�b�g���f�R�[�h����֐�
	ls11_DecodePack("message.n6p", NULL, &vBufDecodedData );


	int iKahouNo = -1;

	int vBufDecodedDataSize = (int)vBufDecodedData.size();

	// �f�R�[�h�����f�[�^��(�����I)�t�@�C���ł܂킵�Ă����B0�Ԃ͑Ώۂł͂Ȃ��B // 15�`17���ƕ�f�[�^
	for ( int ifile = 15; ifile < vBufDecodedDataSize && ifile <= 17 ; ifile++ ) {

		// ���傤�� vSplittedData[0]=�P�Ԗڂ̗v�f�̃f�[�^��AvSplittedData[1]=�Q�Ԗڂ̗v�f�̃f�[�^��A�݂����Ȋ���
		vector<vector<byte>> vSplittedData;

		// ����ɗv�f�����B
		ls11_SplitData( vBufDecodedData[ifile], &vSplittedData );

		int ivSplittedDataSize = (int)vSplittedData.size();

		continue;

		// �e�v�f���Ƃɥ�� �܂����O�A�ǂ݉���
		for ( int ielem=0; ielem < ivSplittedDataSize; ielem++ ) {

			if ( ielem%2==0 ) { iKahouNo++; }

			if ( iKahouNo < 0 ) { iKahouNo = 0; }; // �ň��̂��߂̔ԕ�

			// ������`����Ă����`���𒴂������̂́A����
			if ( iKahouNo >= GAMEDATASTRUCT_KAHOU_NUM ) { break; }

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
				// �ƕ��`����v�f�𒊏o����B
				if ( OnigMatch(szBufCurrent, "^\x1b\x6b(.*?)\x1b\x48(.*?)\n?\x05\x05\x05$", &matches) ) {

					/*
					string name;		// ��\��
					string yomigana;	// ��݂���
					string retsuden;	// ��`������
					*/
					nb6kahouretsuden[iKahouNo].name = matches[2];
					if ( !matches[1].empty() ) {

						// ���p�J�^�J�i��S�p�Ђ炪�Ȃ�
						char szBufZenHiranaga[128] = "";
						hankana2zenhira((unsigned char *)matches[1].c_str(), (unsigned char *)szBufZenHiranaga);
						szBufZenHiranaga[100] = NULL; // ����ȏC�����Ă��\�����Ȃ��悤�O�̂���

						nb6kahouretsuden[iKahouNo].yomigana = string(szBufZenHiranaga);
					}
				}

			// ��͉ƕ��`�B
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


				// ���̓���ȋL���͓V�ċL�ɂ����āA���p�J�^�J�i��S�p�J�^�J�i�ɂ���Ƃ����Ӗ�
				szBufCurrent = HankakuKatakanaToZentakuKatakana(szBufCurrent);

				Matches matches;
				// �Ō�̏I���L���ȍ~�͏����B
				if ( OnigMatch(szBufCurrent, "^(.*?)\x05\x05\x05$", &matches) ) {

					if ( !matches[1].empty() ) {
						// �c��́A���p�J�^�J�i��S�p�Ђ炪�Ȃ�
						char szBufZenHiragana[256] = "";
						hankana2zenhira((unsigned char *)matches[1].c_str(), (unsigned char *)szBufZenHiragana);
						szBufZenHiragana[255] = NULL; // ����ȏC�����Ă��\�����Ȃ��悤�O�̂���
						nb6kahouretsuden[iKahouNo].retsuden = string(szBufZenHiragana);
					}
				}

			}
		}
	}
	return true; //
}
