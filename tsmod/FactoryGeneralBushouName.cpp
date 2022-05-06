#define _CRT_SECURE_NO_WARNINGS

#include "GameDataStruct.h"

// とある文字列が全部シフトＪＩＳ通常漢字かどうか
int isAllKanji (char *fullname) {
	// 全角１文字以上
	if ( strlen(fullname) >= 2 ) {
		// ２バイトに１つ頭の１個検査。
		for ( int i=0; i<(int)strlen(fullname)/2; i++) {
			if ( 0x88 <= fullname[i*2] && fullname[i*2] <= 0x9F || 0xE0 <= fullname[i*2] && fullname[i*2] <= 0xEE ) {
			} else {
				// ひとつでも違ったらfalse
				return false;
			}
		}
		return true;
	}
	return false;
}

// 名前と名字を列伝1300個を元に自動生成する。名字用と名前用の２つのバッファーを渡すこと。
void getFactoryGeneralName(char *bufFamilyName, char *bufFirstName) { // ベースとなる武将ID。-1ならベースなし。

	string familyname;
	string firstname;

	vector<string> name1; // 名字の１文字目の配列
	vector<string> name2; // 名字の２文字目の配列
	vector<string> name3; // 名前の１文字目の配列
	vector<string> name4; // 名前の２文字目の配列

	// 列伝の名前を探す。
	for ( int iRetsudenID = 0; iRetsudenID < GAMEDATASTRUCT_REGISTED_BUSHOU_NUM; iRetsudenID++ ) {
		if ( nb6bushouretsuden[iRetsudenID].name.size() == 8 ) { // 基本的に名字＋名前が全角４文字の武将から取ってくる。
			// フルネーム
			char *fullname = (char *)nb6bushouretsuden[iRetsudenID].name.c_str();
			// 全部通常漢字であれば(改造対策)
			//if ( isAllKanji(fullname) ) {
				char zen[3] = "";

				strncpy( zen, fullname, 2); // 最初の全角１文字をコピー
				zen[2] = NULL;
				name1.push_back(string(zen));

				strncpy( zen, fullname+2, 2); // ２文字をコピー
				zen[2] = NULL;
				name2.push_back(string(zen));

				strncpy( zen, fullname+4, 2); // ３文字をコピー
				zen[2] = NULL;
				name3.push_back(string(zen));

				strncpy( zen, fullname+6, 2); // ４文字をコピー
				zen[2] = NULL;
				name4.push_back(string(zen));
			//}
		}
	}

	// ためた名前からランダムで名字と名前を生成する
	// 名字の２文字を作成
	if ( name1.size() > 0 && name2.size() > 0 && name3.size() > 0 && name4.size() > 0 ) {
		int i = rand() % name1.size();
		familyname = name1[i];
		i = rand() % name2.size();
		// もし１文字目と同じならもっぺんサイコロ振り直し(伊伊とかにならないように)
		if ( familyname == name2[i] ) {
			i = rand() % name2.size();
		}
		familyname += name2[i];

		// 名前の２文字を作成
		i = rand() % name3.size();
		firstname = name3[i];
		i = rand() % name4.size();
		// もし１文字目と同じならもっぺんサイコロ振り直し(信信とかにならないように)
		if ( firstname == name4[i] ) {
			i = rand() % name4.size();
		}
		firstname += name4[i];
	}

	strcpy( bufFamilyName, familyname.c_str() );
	strcpy( bufFirstName, firstname.c_str() );
}


// ゲームで元々用意されているランダムな姫名を得る。
// 
struct _HIME_NAME {
	char name[7];
};
char *getRandomHimeName() {
	// 姫ポインタの位置
	_HIME_NAME *pHName = (_HIME_NAME *)GAMEDATASTRUCT_HIMENAME_ARRAY;
	int iRnd = rand() % 100; // 100人用意されている。
	return pHName[iRnd].name;
}