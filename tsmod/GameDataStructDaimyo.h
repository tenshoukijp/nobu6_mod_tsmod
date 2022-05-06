#pragma once


/*
大名(22*72)

0-1	武将INDEX
2-3	軍団番号
4-5	居城
6-7	隠居リスト先頭
8	家紋
9	朝廷
A	敵対大名
B	友好大名
C
D	商人
E	000A 000B  A評定 B布教
F	役職 0x01 将軍 0x02 管領 0x04 関東管領 0x08 奥州探題 0x10 西国探題 0x20 九州探題 0x40 羽州探題　⇒getNormalizedDaimyoPosition
複数兼職にはならないようだ
*/
struct NB6DAIMYO
{
	WORD attach;		//大名 武将INDEX
	WORD gundan;		//軍団 軍団番号
	WORD castle;		//居城
	WORD retire;		//隠居リスト先頭 // 隠居状態などに変更した場合はリストを整頓しなおす必要がある。
	byte symbol;		//家紋
	byte tyotei;		//朝廷
	byte hostile;		//敵対大名
	byte friendship;	//友好大名
	byte merchantHuman : 4;	//どこの商人か 0:神屋 宗湛 1:島井 宗室 2:津田 宗及 3:今井 宗久 4:呂宋 助左衛門 5:小西 隆佐 6:末吉 孫左衛門 7:茶屋 四郎次郎 8:角倉 了以 9:灰屋 紹由
	byte undef : 4;	//
	byte merchant;		//商人友好度
	byte bPropagate : 1;	//布教 :布教済みか否か
	byte bMerchantAngry : 1; // 商人が怒っている。
	byte _undef1 : 2;		//must 0 布教の残り空ビットに過ぎない。勝手に数値入れないこと
	byte hyotei : 1;		//評定 :評定済みかどうか
	byte _undef2 : 3;		//must 0 評定の残り空ビットに過ぎない。勝手に数値入れないこと
	byte position;		//役職 0x01 将軍 0x02 管領 0x04 関東管領 0x08 奥州探題 0x10 西国探題 0x20 九州探題 0x40 羽州探題　⇒getNormalizedDaimyoPosition
	byte _undef3;
	byte _undef4;
	byte _undef5;
	WORD _undef6;
	byte _undef7;
};

// 全有効大名のリストを返す。ターン大名自身も入ってるので注意。
vector<int> getDaimyoList(BOOL isIncludeTurnDaimyo = TRUE);

// 下のgetDoumeiKankei内部で求まる値は、この配列のINDEXであるため、これを使って実際の値に直す。
int YuukouValueArray[];

// Ａ大名(０始まりでの配列用指定)とＢ大名との友好値
int getYuukouKankei(int iDaimyoA_ID, int iDaimyoB_ID);

// Ａ大名(０始まりでの配列用指定)とＢ大名との友好値をセットする。
void setYuukouKankei(int iDaimyoA_ID, int iDaimyoB_ID, int iValue);

// Ａ大名(０始まりでの配列用指定)とＢ大名との同盟関係 同盟していれば１が返り、していなければ０が返る）
int isDoumeiKankei(int iDaimyoA_ID, int iDaimyoB_ID);

// Ａ大名(０始まりでの配列用指定)とＢ大名との婚姻関係 婚姻していれば２が返り、していなければ０が返る）
int isKoninKankei(int iDaimyoA_ID, int iDaimyoB_ID);

void setDoumeiKankei(int iDaimyoA_ID, int iDaimyoB_ID, BOOL doDoumei = TRUE);
void setKoninKankei(int iDaimyoA_ID, int iDaimyoB_ID, BOOL doKonin = TRUE);


// 御用商人の番号から御用商人の名前を得る。
char *getGoyouSyouninMei(int iGoyousyouninID); // merchantHuman



//最大勢力(最も持ち城数が多い)大名を得る
int GetMaximumInfluenceDaimyo();








