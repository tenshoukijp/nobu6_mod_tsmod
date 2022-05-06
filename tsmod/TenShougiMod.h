#pragma once

#include<windows.h>


// 天翔記から天将棋へと渡す時の、武将情報の型。dummyは予備
struct TenShougiBushouInfo {
	char szUserName[20];
	char B_or_W;	 // 先手か後手か。'+' か '-';
	char szMsg[256]; // 天翔記側から渡ってくる次に言うメッセージ
	int dummy[10];
};


// 天将棋開始。先手のiBushouIDと後手のiBushouIDを渡す。
void TenShougiModBgn(int iBBushouID = 0xFFFF, int iWBushouID = 0xFFFF);

// 天将棋終了
void TenShougiModEnd();


// 天将棋のファイル群が存在しているか？
bool IsTenShougiModFilesExists();


// 対象の棋士の棋力を返す。
int GetShogiPower(int iBushouID);


// 天将棋のメモリ情報を更新する。引数を省略した際には、ちゃんとした引数が渡された時のものをそのまま利用する。
void Update_TenShougiMemoryInfo(int iBBushouID = -1, int iWBushouID = -1);



 // 指定の段位を得る。
char *getSpecialDaniStringOfBushou(int iBushouID);

typedef void (WINAPI *PFNTENSHOUGIMODBGN)();
extern PFNTENSHOUGIMODBGN p_sgTenShougiModBgn;

typedef void (WINAPI *PFNTENSHOUGIMODEND)();
extern PFNTENSHOUGIMODEND p_sgTenShougiModEnd;


// フォント名を転送。
typedef void (WINAPI *PFNSETTENSHOUGIFONTNAME)(char *pszFontName);
extern PFNSETTENSHOUGIFONTNAME p_sgSetTenShougiFontNam;

// 武将情報を転送
typedef void (WINAPI *PFNSETTENSHOUGIBUSHOUINFO)(NB6BUSHOU *pB, NB6BUSHOU *pW, NB6BUSHOUNAME *pBn,  NB6BUSHOUNAME *pWn);
extern PFNSETTENSHOUGIBUSHOUINFO p_sgSetTenShougiBushouInfo;

// 武将の棋力を転送
// 武将の棋力に関数ｒパラメータを転送する。
struct TBUshouDaniPowerInfo {
	int iPower;
	char szDaniName[10];
	int iBushouID;
	int iDummy; // 予備
};

typedef void (WINAPI *PFNSETTENSHOUGIBUSHOUPOWER)(void* iBPowerInfo, void* iWPowerInfo);
extern PFNSETTENSHOUGIBUSHOUPOWER p_sgSetTenShougiBushouPower;

// 天将棋ウィンドウを表示してるかどうかの情報を天将棋側→TSMod側へと得る
typedef BOOL (WINAPI *PFNISTENSHOUGIFORMSHOWN)();
extern PFNISTENSHOUGIFORMSHOWN p_sgIsTenShougiFormShown;

// 天将棋にコマンドを送る。
typedef void (WINAPI *PFNSENDCOMMAND)(char *szCommand);
extern PFNSENDCOMMAND p_sgSendCommand;
