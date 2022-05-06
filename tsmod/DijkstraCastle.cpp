#include "GameDataStruct.h"
#include "dijkstra.h"
#include "DijkstraCastle.h"


static Dijkstra  m_dijkstra;  //ダイクストラ法計算クラス
static Node*     m_select;    //選択ノード（接続で使う）



void InitCastleNodeAndEdge() {
	m_select = NULL;

	// ノード位置情報や接続情報をインポート
	m_dijkstra.Inport();
}


void SetStartCastleNode(int iCastleID ) {

	// 城ID範囲チェック
	if (! (0 <= iCastleID && iCastleID < GAMEDATASTRUCT_CASTLE_NUM) ) {
		return;
	}

	Node* n = m_dijkstra.Query(iCastleID);

	if ( n != NULL ) {
		m_dijkstra.SelctStart(n);
	}
}

void SetGoalCastleNode(int iCastleID ) {

	// 城ID範囲チェック
	if (! (0 <= iCastleID && iCastleID < GAMEDATASTRUCT_CASTLE_NUM) ) {
		return;
	}

	Node* n = m_dijkstra.Query(iCastleID);

	if ( n != NULL ) {
		m_dijkstra.SelctGoal(n);
	}
}


void RemoveCastleNode(int iCastleID) {
	// 城ID範囲チェック
	if (! (0 <= iCastleID && iCastleID < GAMEDATASTRUCT_CASTLE_NUM) ) {
		return;
	}

	//ノードやノード接続を削除
	Node* n = m_dijkstra.Query(iCastleID);

	if ( n != NULL ) {
		m_dijkstra.Remove(n);
	}

	NodePair pair;
	if ( m_dijkstra.QueryLineOnNodePair( &pair, n->x, n->y) ) {
		m_dijkstra.RemoveConnect( pair.n1, pair.n2 );
	}
}


vector<int> SearchCastleWayRoot() {
	vector<int> iWayCastleIDList; // 経路上の城ID

	// 経路データ設定
	m_dijkstra.SearchRoot();

	Node* curr = m_dijkstra.GetStartNode();
	iWayCastleIDList.push_back( curr->GetCastleID() ); // スタートのやつは必ず加える

	// ゴールへと繋がる次のノードがあるならば…
	while ( curr->toGoal ) {
		// 次のノードを今のノードとする。
		curr = curr->toGoal;
		iWayCastleIDList.push_back( curr->GetCastleID() ); // 次の番号を加える
	}

	return iWayCastleIDList;
	
}


// 指定の城から道が繋がっている隣接城のリストを得る。
vector<int> getWayConnectCastles(int iCastleID) {
	vector<int> connect_castle_list;

	// エッジのスタート地点もしくは、ゴール地点に自身のiCastleIDがある。
	for ( int iEdgeID=0; iEdgeID<N6PCASTLE_EDGE_NUM; iEdgeID++ ) {
		// 城番号がe1と一致したら、e2が連結先
		if ( iCastleID == nb6castle_edges[iEdgeID].e1 ) {
			connect_castle_list.push_back( nb6castle_edges[iEdgeID].e2 );
		}
		// 逆にe2と一致したら、e1が連結先
		else if ( iCastleID == nb6castle_edges[iEdgeID].e2 ) {
			connect_castle_list.push_back( nb6castle_edges[iEdgeID].e1 );
		}
	}

	return connect_castle_list;
}





