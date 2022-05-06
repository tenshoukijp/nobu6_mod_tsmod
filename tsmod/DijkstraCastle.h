#include "GameDataStruct.h"
#include "dijkstra.h"


#define N6PCASTLE_EDGE_NUM	306


struct N6PCASTLE_EDGE {
	WORD e1;
	WORD e2;
};

// iCastleID同士の連結情報。
extern N6PCASTLE_EDGE nb6castle_edges[N6PCASTLE_EDGE_NUM];


// 最初のノードやエッジの初期化。経路関係を使う前に実行しておく必要がある。
void InitCastleNodeAndEdge();

// スタートノードの設定
void SetStartCastleNode(int iCastleID );

// ゴールノードの設定
void SetGoalCastleNode(int iCastleID );

// 特定の城ノードの削除(その城にくっついているエッジも削除される)
void RemoveCastleNode(int iCastleID );

// スタート→ゴールへのルートを計算し、経路となった城IDのリストを返す。
vector<int> SearchCastleWayRoot();