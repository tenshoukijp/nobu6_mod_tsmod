#pragma once

// 解像度の縦を得る。ウィンドウサイズというよりも、実際の天翔記としてのサイズ(1024x768や1600x768など)
int GetResolutionWidth();
// 解像度の横を得る。ウィンドウサイズというよりも、実際の天翔記としてのサイズ(1024x768や1600x768など)
int GetResolutionHeight();


struct NB6FUKIDASHI_DIALOG_LAYOUT {
	int Lt_x;
	int Lt_y;
	int Rt_x;
	int Rt_y;
};
