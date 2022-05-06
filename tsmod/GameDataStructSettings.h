#pragma once


/*
[ 環境設定]

B 戦争を見る見ない / BGM / SE / ムービー
000ebbcd d=SE c=BGM e=ムービー

B 0?
B メッセージ速度
B 難易度
*/
struct NB6SETTINGS
{
	byte se : 1;				// ＳＥは有効か
	byte bgm : 1;				// ＢＧＭは有効か
	byte other_war : 2;		// 他国の戦争　見る=2 任意=1 見ない=0
	byte movie_is_off : 1;	// ムービーは無効か　(無効なら1で、有効なら0というなんとも分かりにくい…) IF分で使うときに == (イコール判定)を省略すると間違う。ムービー::ＯＮ、ムービー::ＯＦＦのenumを使うこと!!
	byte _undef1 : 3;
	byte _undef2;
	byte messagespd;		// メッセージ速度	0 ～ 4(∞) 表示順通り
	byte difficult;			// 難易度			0 ～ 4 表示順通り
};



// SEが有効か？
bool isSEValid();

// BGMが有効か？
bool isBGMValid();

// Movieが有効か？
bool isMovieValid();

// _INMMモードか？
bool is_InmmMode();

// デモモードか？
bool is_DemoMode();

/*
[ 環境設定拡張 ]

0-1   W 年号表示( 元号=1 西暦=0 )
2-3   W 表示位置記憶(マウス=1 記憶=0 )
4-5   W ?
6-7   W ?
8-9   W 表示切替マップ( 連動=1, 非連動=0 )
10-11 W ?
12-13 W 戦争頻度(好戦=1, 理知=0)
14-15 W 計略頻度(多い=1, 少ない=0)
16-17 W 死亡頻度(多い=1, 少ない=0)
18-19 W ?
20-21 W 自国の戦争(任意=2, 従来=1, 見る=0)
22-23 W 編集機能(有り=1, 無し=0)
24-25 W 情報表示(最小=2, 少ない=1,普通=0 )
26-27 W ?
28-29 W 武将行動志願(有り=1, 無し=0)
30-31 W 姫武将志願(毎回=2, 従来=1, 無し=0 )
*/

struct NB6SETTINGSEX
{
	WORD yeardisp;		// 年号表示( 元号=1 西暦=0 )
	WORD remlocdisp;	// 表示位置記憶(マウス=1 記憶=0 )
	WORD undef1;
	WORD undef2;
	WORD mapdisp;		// 表示切替マップ( 連動=1, 非連動=0 )
	WORD undef3;
	WORD warfreq;		// 戦争頻度(好戦=1, 理知=0)
	WORD keiryakufreq;	// 計略頻度(多い=1, 少ない=0)
	WORD deathfreq;		// 死亡頻度(多い=1, 少ない=0)
	WORD undef4;
	WORD myselfwardisp;	// 自国の戦争(任意=2, 従来=1, 見る=0)
	WORD edit;			// 編集機能(有り=1, 無し=0)
	WORD infodisp;		// 情報表示(最小=2, 少ない=1,普通=0 )
	WORD undef5;
	WORD bushoushigan;	// 武将行動志願(有り=1, 無し=0)
	WORD himeshigan;	// 姫武将志願(毎回=2, 従来=1, 無し=0 )
};



// 鉄砲伝来
bool isTeppouDenrai();

// 鉄砲伝来をやめる
void UnsetTeppouDenrai();

// キリスト教伝来
bool isChristDenrai();

// キリスト教伝来をやめる
void UnsetChristDenrai();

// 安土城が建設済みかどうか
bool isAzuchiCastleComplete();

// 大坂城が建設済みかどうか
bool isOsakaCastleComplete();


