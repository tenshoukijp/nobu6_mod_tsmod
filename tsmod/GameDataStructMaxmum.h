#pragma once

/*
様々なパラメータの最大値
*/
struct NB6PARAMMAX
{
	DWORD maxgov;		//政才		110
	DWORD maxbat;		//戦才		110
	DWORD maxint;		//智才		110
	DWORD expgov;		//政熟		2000
	DWORD expbat;		//戦熟		2000
	DWORD expint;		//智熟		2000
	DWORD charm;		//魅力		110
	DWORD ambition;		//野望		110
	DWORD loyal;		//忠誠		100
	DWORD merits;		//勲功		999
	DWORD blood;		//血筋		255
	DWORD attach;		//大名番号	72
	DWORD gundan;		//軍団		214
	DWORD castle;		//居城		214
	DWORD soldier;		//兵数		100
	DWORD training;		//訓練		200
	DWORD solloyal;		//兵忠		100
};



struct NB6PARAMMAX_MAIN
{
	DWORD castle;		//城数		214
	DWORD build;		//城城郭(防御度)	250
	DWORD grain;		//石高		2500
	DWORD town;			//商業		200
	DWORD population;	//人口		9999
	DWORD loyal;		//民忠		100
	DWORD soldier;		//徴兵可	999
	DWORD quality;		//兵質		2
	DWORD chimei;		//地名		74
	DWORD _kani;		//官位？(不明)	250
	DWORD _undef2;		//？？			200
	DWORD _form;		//兵態？(不明)    3
};

