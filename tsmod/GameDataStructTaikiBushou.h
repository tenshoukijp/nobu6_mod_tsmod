#pragma once

/*
struct 待機武将情報１型
{
byte 登場年;				/// <summary>登場年。値としては、1454(天翔記基準年)からの差分の値。
WORD 所属居城【城番号】;	/// <summary>居城</summary>
WORD 所属大名【大名番号】;	/// <summary>大名</summary>
} ×人数
*/
// 待機武将の登場型(ScenarioModと共通)
struct NB6TAIKIBUSHOUAPPEAR {
	byte appear;			/// <summary>登場年。値としては、1454(天翔記基準年)からの差分の値。
	WORD castle;		//居城  所属居城【城番号】;	//居城
	WORD attach;		//大名  所属大名【大名番号】;	//大名
};

struct NB6TAIKIBUSHOU : NB6BUSHOU {
	// 追加フィールドなし、全く同じ
};


// 待機武将戸籍情報型(ScenarioModと共通)
struct NB6TAIKIBUSHOUNAME
{
	char familyname[7];	//名字
	char fastname[7];	//名前
	WORD number;		//武将ナンバー(下位12bitは列伝？)
	byte State : 4;		//0大名,1軍長,2現役,3隠居,4浪人,5姫,6,待機,7死亡
	byte position : 4;	//身分		(隠居=0, 大名=1, 宿老=2, 家老=3, 部将=4, 侍大将=5, 足軽頭=6)
	byte sex : 1;			//性 0-男 1-女
};



// 現在進行形のシナリオの待機武将の登場情報を得る
extern NB6TAIKIBUSHOUAPPEAR* GetTaikiBushouAppearArrayPointer();


// 現在進行形のシナリオの待機武将の戸籍情報を得る
extern NB6TAIKIBUSHOUNAME* GetTaikiBushouNameArrayPointer();


// 現在実行中のゲームで待機から「登場」へと切り替わった武将数
extern int getTouzyouBushouNum();

// 現在実行中のゲーム(=シナリオ)の「待機武将」のデータ数。
// この数はシナリオごとに決まった固定数。一度ゲームが始まると、途中で増減しない。
extern int getTaikiBushouDataNum();

