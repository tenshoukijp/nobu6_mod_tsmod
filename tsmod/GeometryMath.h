#pragma once

#pragma pack(1)

#include "3DPosition.h"
#include "2DPosition.h"


/*-----------------------------------------------------------------
// 線分A1x, A1y → A2x, A2y  と  線分B1x, B1y → B2x, B2y  とが 衝突するかどうか。
/*-----------------------------------------------------------------*/
bool LineAndLine( double A1x, double A1y, double A2x, double A2y, double B1x, double B1y, double B2x, double B2y );


/*-----------------------------------------------------------------
// 四角形と線分との衝突の判定
/-----------------------------------------------------------------*/
bool RectAndLine( double left, double top, double right, double bottom, double x1, double y1, double x2, double y2 );


/*-----------------------------------------------------------------
	座標pos に最も近い 線分ab上の座標 を計算
  -----------------------------------------------------------------*/
D3D_POSITION GetClosePosToLine( D3D_POSITION pos, D3D_POSITION a, D3D_POSITION b);


/*-----------------------------------------------------------------
	座標pos と 線分ab との距離を計算
  -----------------------------------------------------------------*/
double GetDistanceToLine( D3D_POSITION pos, D3D_POSITION a, D3D_POSITION b);


