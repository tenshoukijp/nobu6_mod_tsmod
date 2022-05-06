#pragma once

////////////////////////////////////////////////////////////////////////////
// NB6 天翔記
/*
[年号情報(3*1)]

0-1		年数(1454からの経過数)
2		季節(0=春,1=夏,2=秋,3=冬)
*/
struct NB6YEAR
{
	WORD year;			// 年数(1454からの経過数)	 ⇒ ここ直接ではなく、getYearADで得ること
	char season;		// 季節(0=春,1=夏,2=秋,3=冬) ⇒ ここ直接ではなくgetSeasonNameで得ること
};

// 1454年からの差分となっている年を西暦に直す
int getYearAD();

// ゲームの開始年。デフォルトは1454年
int getStartYearAD();

// 年・月・日より元号文字列を得る。
// 年月日を何も指定しなかった場合、ゲーム中の現在の西暦年の年末を指定したものとみなす。
// 年だけを引数として渡した場合、指定年の年末を指定したものとみなす。
// 年・月だけを引数として指定した場合、該当月の31日を指定したものとみなす。(そのような日付が存在しなくとも不都合はない)
//   (天翔記では、「年の途中で元号が変わっている」場合、旧年号ではなく、新元号の方を常に採用するため、100％新年号が返る年末ベースとなっている。)
struct NB6WAREKI {
	char gengou[9]; // 年号
	char kanjiyear[15]; // 和暦の年(漢字)
	int iyear;	// 和暦の年
};
NB6WAREKI getWarekiFromYMD(int iYear = -1, int iMonth = -1, int iDay = -1);

// 季節番号から季節名を得る
char* getSeasonName();

extern int nb6progressseason;

// 選択したシナリオの一番号(上から0番、一番下が5番)
extern int iSelectingScenarioLocation;

// 現在プレイ中のシナリオロケーション番号(0～5, 不明な場合は-1);
int getCurScenarioLocation();

// 対象ＩＤのシナリオのタイトル名を得る
char* getScenarioTitle(int iLocationID);

/*
[ 現在のターンの情報]

W 大名
W 軍団
W 城
*/
struct NB6TURN
{
	WORD daimyo;			// 現在のターンの大名番号
	WORD gundan;			// 現在のターンの軍団番号
	WORD castle;			// 現在のターンの城番号
};


/*
B 米相場
B 鉄砲相場
B 馬相場
B 兵相場
*/
struct NB6SOBA
{
	byte rise;			// 米相場  ･･･　表示されている相場の十倍の数値 １の時、相場0.1となる。
	byte gun;			// 鉄砲相場 ･･･ 画面上でも値の通り。
	byte horse;			// 馬相場 ･･･ 画面上でも値の通り。
	byte soldier;		// 兵相場 ･･･ 画面上でも値の通り。
};

/*
プレイヤーの担当軍団(大名番号ではなく軍団番号)
*/
struct NB6PLAYERS8
{
	WORD gundan;
};

// 対象の軍団IDがいずれかのプレイヤー勢力下の軍団である(最大８人)
bool isPlayerGundan(WORD iTargetGundanID);

// 対象の武将IDがいずれかのプレイヤー勢力下の武将である
bool isPlayerBushou(WORD iTargetBushouID);

// 対象の大名IDがいずれかのプレイヤー担当の大名である
bool isPlayerDaimyo(WORD iTargetDaimyoID);
