#include "GameDataStruct.h"
#include "tmath.h"
#include "GeometryMath.h"
#include <algorithm>
#include <functional>


static D2D_POSITION _StartPosForRoleArrayOfBetweenPosToPos;
static bool _CompareLengthForRoleArrayOfBetweenPosToPos(const HEX_POSITION pos1, const HEX_POSITION pos2 ) {
	// 長さが計算可能な形へ
	D2D_POSITION p1( pos1.x, pos1.y );
	D2D_POSITION p2( pos2.x, pos2.y );

	// 開始点から各々の点へのベクトル
	D2D_VECTOR v1 = p1 - _StartPosForRoleArrayOfBetweenPosToPos;
	D2D_VECTOR v2 = p2 - _StartPosForRoleArrayOfBetweenPosToPos;

	// 昇順
	return v1.length() < v2.length();
}

// ヘックスのＡ点とＢ点の間にあるチップのアレイを返す。
vector<int> RoleArrayOfBetweenPosToPos( HEX_POSITION pos1, HEX_POSITION pos2 ) {

	// 値をグローバル変数にコピー。後で比較関数ように利用するため。
	_StartPosForRoleArrayOfBetweenPosToPos.x = pos1.x;
	_StartPosForRoleArrayOfBetweenPosToPos.y = pos1.y;

	vector<int> arrRoleArray;

	vector< HEX_POSITION > pos_array;

	// ２つの座標から大きな矩形を出す。
	double posy_min = pos1.y <= pos2.y ? pos1.y : pos2.y;
	double posy_max = pos1.y >  pos2.y ? pos1.y : pos2.y;
	double posx_min = pos1.x <= pos2.x ? pos1.x : pos2.x;
	posx_min = posx_min - 2; // 安全のため、１つ左も矩形に入れる。
	double posx_max = pos1.x >  pos2.x ? pos1.x : pos2.x;
	posx_max = posx_max + 2; // 安全のため、1つ右も矩形に入れる。

	for ( double y = posy_min; y <= posy_max; y++ ) {
		for ( double x = posx_min;  x <= posx_max; x++ ) {
			// Yが偶数行である
			y = round_整数化(y);
			x = round_二分の一吸着化(x); // 0.5吸着化

			if ( (int)y % 2 == 0 ) {
				// xが3.5とかなら3.0へと整数化
				x = (int)x;
				pos_array.push_back( HEX_POSITION( x, y ) );

			// Yが奇数である。
			} else {
				// Yが奇数ならXは2.5などでないとダメなのに整数だったら0.5足す。

				// 0.5で割ると偶数になるということは、整数である。0.5を足す。
				if ( int(x / 0.5) % 2 == 0 ) {
					x = x + 0.5;
				}

				pos_array.push_back( HEX_POSITION( x, y ) );
			}
		}
	}

	// ポジションをpos1からの距離でソート
	std::sort(pos_array.begin(),pos_array.end(),_CompareLengthForRoleArrayOfBetweenPosToPos);//比較関数を指定してソート

	// ソートされた配列を走査
	for ( vector<HEX_POSITION>::iterator it = pos_array.begin(); it != pos_array.end(); it++ ) {

		// pos1と同じなら次
		if ( it->x == pos1.x && it->y == pos1.y ) {
			continue;
		}

		// pos2と同じなら次
		if ( it->x == pos2.x && it->y == pos2.y ) {
			continue;
		}

		// 該当座標はちゃんと役割があるかどうか。
		int role = -1;
		if ( Is_FieldWar() ) {
			role = GetFieldHexRole( it->x, it->y );
		} else if ( Is_CastleWar() ) {
			role = GetCastleHexRole( it->x, it->y );
		}

		// その座標は役割がちゃんとあったならば…
		if ( role != -1 ) {
			// 線分と該当チップが交差するならば…
			if ( RectAndLine( it->x-0.6, it->y-0.6, it->x+0.6, it->y+0.6,  pos1.x, pos1.y, pos2.x, pos2.y) ) {
				arrRoleArray.push_back( role );
			}
		}
	}

	// 偶数行か奇数行かで、わけながら、
	// コリジョンとっていく。
	// 偶数行はＸ座標は整数にまとめる。
	// 奇数行は0.5あまってる。

	return arrRoleArray;
}