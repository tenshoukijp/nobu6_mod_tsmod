#pragma once







/*
[官位(18*250)]

0-12	名前
13		位階
14-15	所有武将番号
16		上昇値
17		_undef
*/
struct NB6KANI
{
	char name[13];	//名前
	byte rank;		//官位のランク(正一位～従八位上までを数値にしたもの 一番高いのが0、官位が低いほど、逆に数値は大きい)
	WORD attach;	//所有者
	byte param;		//上昇値(魅力の上昇値)
	byte _undef0;	//
};

// 官位の種類　(正一位～従八位下まで）
char* nb6kanitypelist[];

// 官位の種類のIDから文字列を得る
char* getKaniTypeName(byte type);


// 官位を武将にアタッチする。元々別武将が持っていた場合は、元の武将から指定した武将へとアタッチが変わる。
// 能力値の変更を伴う。朝廷に返上する場合は0xFFFFとする。
bool ReAttachKani(int iKaniID, int iNewBushouID);

