#pragma once

#include <string>


using namespace std;

class HOtherInfo {
	int a;
};



class MessageN6PPatch {

private:
	char m_szBName[13];           // ������
	char m_szCurOrigMsg[512];	  // �I���W�i�����b�Z�[�W
	bool m_isCurOrigMsgReWrited;  // ����̂P�t�@�C�o�[�ŏ������������H
	HOtherInfo* m_hOtherInfo;	  // ���̕t�����
	int m_iReWritePercentForce;	  // ������������

public:

	static bool isOnTenshouExeMessageCopyUsed;

	MessageN6PPatch( char *szBName, char *szCurOrigMsg, HOtherInfo* hOtherInfo );

	// ���݂̏œ_�̕������𓾂�
	char* getBName();

	// ���̏œ_�̕�������szTargetName���H
	bool isBName(char *szTargetName);

	// ���b�Z�[�W���p�����[�^���܂�ł��邩�H
	bool isIncludeParam(char *szMsg);

	// �Y���̕����p�^�[������v���邩�ǂ���
	BOOL isMsg( char* szMsgReg, Matches* matches=NULL );

	/*

     @szMsgReg:
		�Y���̃��b�Z�[�W�̌��̃p�^�[��

	 @szCurReWriteMsg
		�V���ȃ��b�Z�[�W

	 @param:
		iReWritePercent
			���̃��b�Z�[�W�Ɗm���ŕω��������ꍇ��%�Ŏw�肷��B
			200%�̏ꍇ�́A�����ɍēx����������B
			���������ς݂ł����Ă��ēx����������Ƃ��������Ȃ蒿��������������ۂ� 200 �����B
	 @return:
		����Ăяo���Ń}�b�`�������ǂ���
	*/
	bool rwMsg( char* szMsgReg, char *szCurReWriteMsg, int iReWritePercent = 100 );

};


//�@���ۂɏ���������
bool ReWritePatchMessageN6P(char *szBName, char *szCurOrigMsg, HOtherInfo* hOtherInfo);

