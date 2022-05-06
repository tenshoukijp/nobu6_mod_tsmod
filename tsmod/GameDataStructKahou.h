#pragma once

/*
[家宝(19*120)]

0-12	名前
13	分類
0 茶碗     1 茶壺     2 茶入    3 茶釜        4 花入    5 刀剣      6 槍        7 具足   8 馬
9 鞍       A 陣羽織   B 巻子本  C 明朝綴本    D 香木    E 地球儀    F 南蛮時計 10 水墨画
11 十字架  12 聖書    13 玉手箱 14 ビードロ杯 15 遠眼鏡 16 ルソン壺 17 煙草
14	画像
15-16	所持武将
17	0000AAAA A = 上昇値
18	AB000000 A = 消失  B = 未登場
*/
struct NB6KAHOU
{
	char name[13];	//名前
	byte type;		//分類
	byte face;		//画像
	WORD attach;	//所有者  商人なら0xFFFF;
	byte param : 4;	//上昇値
	byte _undef0 : 4;	//0
	byte _undef1 : 6;	//
	byte bMitojo : 1;	//未登場 (未登場=1, 登場=0)
	byte bLost : 1;	//消失(1=消失 0=存在) 
};


// それぞれの家宝分類名へのポインタの配列。24個状態から１つ１つを取り出すために必要となるアドレス宣言 
int GAMEDATASTRUCT_KAHOUTYPELIST_POINTER[];


// 家宝タイプのIDから文字列を得る
char* getKahouTypeName(byte type);


// 家宝種類のカスタム文字列
struct CUSTOM_KAHOU_TYPE_STRING {
	char szKahouTypeString[11]; // 最大全角５文字。家宝種類名
};


// 家宝を武将にアタッチする。元々別武将が持っていた場合は、元の武将から指定した武将へとアタッチが変わる。
// 能力値の変更を伴う。商人に返上する場合は0xFFFFとする。
bool ReAttachKahou(int iKahouID, int iNewBushouID);


/*
家宝列伝関連の情報
*/
struct NB6KAHOURETSUDEN {
	string name;
	string yomigana;	// よみがな
	string retsuden;	// 列伝文字列
};

