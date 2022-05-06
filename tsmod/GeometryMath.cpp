#pragma pack(1)

#include "GeometryMath.h"





// 線分A1x, A1y → A2x, A2y  と  線分B1x, B1y → B2x, B2y  とが 衝突するかどうか。
bool LineAndLine( double A1x, double A1y, double A2x, double A2y, double B1x, double B1y, double B2x, double B2y ){
	{
		const double baseX = B2x - B1x;
		const double baseY = B2y - B1y;
		const double sub1X = A1x - B1x;
		const double sub1Y = A1y - B1y;
		const double sub2X = A2x - B1x;
		const double sub2Y = A2y - B1y;

		const double bs1 = baseX * sub1Y - baseY * sub1X;
		const double bs2 = baseX * sub2Y - baseY * sub2X;
		const double re = bs1 * bs2;
		if( re > 0 ){
			return false;
		}
	}
	{
		const double baseX = A2x - A1x;
		const double baseY = A2y - A1y;
		const double sub1X = B1x - A1x;
		const double sub1Y = B1y - A1y;
		const double sub2X = B2x - A1x;
		const double sub2Y = B2y - A1y;

		const double bs1 = baseX * sub1Y - baseY * sub1X;
		const double bs2 = baseX * sub2Y - baseY * sub2X;
		const double re = bs1 * bs2;
		if( re > 0 ){
			return false;
		}
	}
	return true;
}

// 四角形と線分との衝突の判定
bool RectAndLine( double left, double top, double right, double bottom, double x1, double y1, double x2, double y2 ){
	if( LineAndLine( left, top, right, top, x1, y1, x2, y2 ) ){ return true; 	}
	if( LineAndLine( right, top, right, bottom, x1, y1, x2, y2 ) ){ return true; }
	if( LineAndLine( right, bottom, left, bottom, x1, y1, x2, y2 ) ){ return true; }
	if( LineAndLine( left, bottom, left, top, x1, y1, x2, y2 ) ){ return true; }

	return false;
}



/*-----------------------------------------------------------------
	座標pos に最も近い 線分ab上の座標 を計算
  -----------------------------------------------------------------*/
D3D_POSITION GetClosePosToLine( D3D_POSITION pos, D3D_POSITION a, D3D_POSITION b) {

	if ( a == b ) { // a==b。すなわち線分を構築していない。
		return a;
	}
	// ここから以降は b-a には長さがあることが保障されている。

	D3D_VECTOR lineVec = D3D_VECTOR( b - a );
	lineVec.normalize(); // 単位ベクトル化

	double dotA = lineVec * (pos-a); // 内積
	if ( dotA < 0 ) {
		return a;
	}

	double dotB = (lineVec * -1) * (pos-b); // 内積
	if ( dotB < 0 ) {
		return b;
	}

	D3D_VECTOR ret = D3D_VECTOR(a) + D3D_VECTOR(lineVec * dotA);
	return D3D_POSITION(ret.x, ret.y, ret.z);
}


/*-----------------------------------------------------------------
	座標pos と 線分ab との距離を計算
  -----------------------------------------------------------------*/
double GetDistanceToLine( D3D_POSITION pos, D3D_POSITION a, D3D_POSITION b) {
	D3D_POSITION close_pos = GetClosePosToLine(pos, a, b);
	D3D_VECTOR vclose_pos(close_pos-pos);
	return vclose_pos.length();
}



