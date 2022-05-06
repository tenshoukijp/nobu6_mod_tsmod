#pragma once

#include<windows.h>


// �V�ċL����V�����ւƓn�����́A�������̌^�Bdummy�͗\��
struct TenShougiBushouInfo {
	char szUserName[20];
	char B_or_W;	 // ��肩��肩�B'+' �� '-';
	char szMsg[256]; // �V�ċL������n���Ă��鎟�Ɍ������b�Z�[�W
	int dummy[10];
};


// �V�����J�n�B����iBushouID�ƌ���iBushouID��n���B
void TenShougiModBgn(int iBBushouID = 0xFFFF, int iWBushouID = 0xFFFF);

// �V�����I��
void TenShougiModEnd();


// �V�����̃t�@�C���Q�����݂��Ă��邩�H
bool IsTenShougiModFilesExists();


// �Ώۂ̊��m�̊��͂�Ԃ��B
int GetShogiPower(int iBushouID);


// �V�����̃����������X�V����B�������ȗ������ۂɂ́A�����Ƃ����������n���ꂽ���̂��̂����̂܂ܗ��p����B
void Update_TenShougiMemoryInfo(int iBBushouID = -1, int iWBushouID = -1);



 // �w��̒i�ʂ𓾂�B
char *getSpecialDaniStringOfBushou(int iBushouID);

typedef void (WINAPI *PFNTENSHOUGIMODBGN)();
extern PFNTENSHOUGIMODBGN p_sgTenShougiModBgn;

typedef void (WINAPI *PFNTENSHOUGIMODEND)();
extern PFNTENSHOUGIMODEND p_sgTenShougiModEnd;


// �t�H���g����]���B
typedef void (WINAPI *PFNSETTENSHOUGIFONTNAME)(char *pszFontName);
extern PFNSETTENSHOUGIFONTNAME p_sgSetTenShougiFontNam;

// ��������]��
typedef void (WINAPI *PFNSETTENSHOUGIBUSHOUINFO)(NB6BUSHOU *pB, NB6BUSHOU *pW, NB6BUSHOUNAME *pBn,  NB6BUSHOUNAME *pWn);
extern PFNSETTENSHOUGIBUSHOUINFO p_sgSetTenShougiBushouInfo;

// �����̊��͂�]��
// �����̊��͂Ɋ֐����p�����[�^��]������B
struct TBUshouDaniPowerInfo {
	int iPower;
	char szDaniName[10];
	int iBushouID;
	int iDummy; // �\��
};

typedef void (WINAPI *PFNSETTENSHOUGIBUSHOUPOWER)(void* iBPowerInfo, void* iWPowerInfo);
extern PFNSETTENSHOUGIBUSHOUPOWER p_sgSetTenShougiBushouPower;

// �V�����E�B���h�E��\�����Ă邩�ǂ����̏���V��������TSMod���ւƓ���
typedef BOOL (WINAPI *PFNISTENSHOUGIFORMSHOWN)();
extern PFNISTENSHOUGIFORMSHOWN p_sgIsTenShougiFormShown;

// �V�����ɃR�}���h�𑗂�B
typedef void (WINAPI *PFNSENDCOMMAND)(char *szCommand);
extern PFNSENDCOMMAND p_sgSendCommand;
