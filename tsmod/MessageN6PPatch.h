#pragma once

#include <string>


using namespace std;

class HOtherInfo {
	int a;
};



class MessageN6PPatch {

private:
	char m_szBName[13];           // 武将名
	char m_szCurOrigMsg[512];	  // オリジナルメッセージ
	bool m_isCurOrigMsgReWrited;  // 今回の１ファイバーで書き換えたか？
	HOtherInfo* m_hOtherInfo;	  // 他の付属情報
	int m_iReWritePercentForce;	  // 強制書き換え

public:

	static bool isOnTenshouExeMessageCopyUsed;

	MessageN6PPatch( char *szBName, char *szCurOrigMsg, HOtherInfo* hOtherInfo );

	// 現在の焦点の武将名を得る
	char* getBName();

	// 今の焦点の武将名はszTargetNameか？
	bool isBName(char *szTargetName);

	// メッセージがパラメータを含んでいるか？
	bool isIncludeParam(char *szMsg);

	// 該当の文字パターンが一致するかどうか
	BOOL isMsg( char* szMsgReg, Matches* matches=NULL );

	/*

     @szMsgReg:
		該当のメッセージの元のパターン

	 @szCurReWriteMsg
		新たなメッセージ

	 @param:
		iReWritePercent
			元のメッセージと確率で変化させた場合に%で指定する。
			200%の場合は、強制に再度書き換える。
			書き換え済みであっても再度書き換えるといったかなり珍しい処理をする際に 200 入れる。
	 @return:
		今回呼び出しでマッチしたかどうか
	*/
	bool rwMsg( char* szMsgReg, char *szCurReWriteMsg, int iReWritePercent = 100 );

};


//　実際に書き換える
bool ReWritePatchMessageN6P(char *szBName, char *szCurOrigMsg, HOtherInfo* hOtherInfo);

