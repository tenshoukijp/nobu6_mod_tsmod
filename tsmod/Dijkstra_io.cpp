
#include "GameDataStruct.h"
#include "dijkstra.h"
#include "DijkstraCastle.h"



//ファイル読み込み
bool Dijkstra::Inport()
{
	Clear();

	// エッジのスタート地点もしくは、ゴール地点に自身のiCastleIDがある。
	for ( int iCastleID=0; iCastleID<GAMEDATASTRUCT_CASTLE_NUM; iCastleID++ ) {
		// 城の座標をそのままノードの座標として格納。
		AddNode( iCastleID, nb6castlemax[iCastleID].x, nb6castlemax[iCastleID].y );
	}

	// エッジのスタート地点もしくは、ゴール地点に自身のiCastleIDがある。
	for ( int iEdgeID=0; iEdgeID<N6PCASTLE_EDGE_NUM; iEdgeID++ ) {
		Node* n1 = QueryFromIdx(nb6castle_edges[iEdgeID].e1);
		Node* n2 = QueryFromIdx(nb6castle_edges[iEdgeID].e2);
			
		ConnectNode( n1, n2 );
	}

	return true;

}

