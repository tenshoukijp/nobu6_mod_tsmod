#include "GameDataStruct.h"
#include "dijkstra.h"

double DistPoints(double lx, double ly, double rx, double ry )
{
	double len = ( rx - lx )*( rx - lx ) + ( ry - ly )*( ry - ly );
	
	return pow( len , 0.5 );
}

//外積　直線と点の距離を出すときに使う
double CrossProduct(double lx, double ly, double rx, double ry ) { return (lx * ry) - (ly * rx); }

//内積　ベクトル向きのチェックにつかう
double DotProduct(double lx, double ly, double rx, double ry ) { return (lx * rx) + (ly * ry); }

//線分上に点があるか
bool IntersectPointLine(
	int x, int y,		//点
	int lx1, int ly1,	//線分始点
	int lx2, int ly2,	//線分終点
	double limit )		//誤差
{
	double dist;
	
	do {

		double lineLen = DistPoints(lx1, ly1, lx2, ly2);
		if ( lineLen == 0.0 ) {
			dist = DistPoints( x, y, lx1, ly1 );
			break;
		}
		
		if (DotProduct(lx2 - lx1, ly2 - ly1, x - lx1, y- ly1) <= 0) {
			dist = DistPoints( x, y, lx1, ly1 );
			break;
        }

		if (DotProduct(lx1 - lx2, ly1 - ly2, x - lx2, y- ly2) <= 0) {
			dist = DistPoints( x, y, lx2, ly2 );
            break;
        }
		
		dist = CrossProduct(lx2 - lx1, ly2 - ly1, x - lx1, y- ly1) / lineLen;
		dist = abs(dist);

	} while(0);
	
	if ( dist < limit) {
		return true;
	}
	return false;
}


//ノード間のコスト(距離)計算
double QueryCost(Node* n1, Node* n2 )
{
	return DistPoints(n1->x,n1->y, n2->x, n2->y);
}

Dijkstra::Dijkstra()
{
	m_start = NULL;
	m_goal = NULL;
}
Dijkstra::~Dijkstra()
{
	Clear();
}

//指定ノードを削除
void Dijkstra::Remove( Node* n1 )
{
	std::list<NodeConnect>::iterator it = n1->m_connectNode.begin();
	for ( ; it != n1->m_connectNode.end(); it++ ) {
		//接続先のノードから自身の情報を除外
		it->node->RemoveConnect(n1);
	}

	//リストから自信を除外して解放。
	m_node.remove(n1);
	delete n1;
}

//ノード接続を削除
void Dijkstra::RemoveConnect( Node* n1, Node* n2 )
{
	//n1のノードからn2を除外
	n1->RemoveConnect(n2);
	n2->RemoveConnect(n1);
}

//全ノードを削除
void Dijkstra::Clear()
{
	std::list<Node*>::iterator it = m_node.begin();
	for ( ; it != m_node.end(); it++ ) {
		delete (*it);
	}
	
	m_node.clear();
	m_start = NULL;
	m_goal  = NULL;
}

//ノードがリストの何番目か 0からスタート 無いとき-1
int Dijkstra::QueryIdx( Node* node )
{
	int ret = 0;
	std::list<Node*>::iterator it = m_node.begin();
	for ( ; it != m_node.end(); it++, ret++ ) {
		if ( *it == node ) {
			return ret;
		}
	}
	return -1;
}

//インデックス指定でリストからノード取得
Node* Dijkstra::QueryFromIdx(int idx)
{
	int ret = 0;
	std::list<Node*>::iterator it = m_node.begin();
	for ( ; it != m_node.end(); it++, ret++ ) {
		if ( ret == idx ) {
			return (*it);
		}
	}
	return NULL;
}

//２点間のルートを作成
void Dijkstra::ConnectNode( Node* n1, Node* n2 )
{
	//登録済みでないか
	std::list<NodeConnect>::iterator it = n1->m_connectNode.begin();
	for ( ; it != n1->m_connectNode.end(); it++ ) {

		if ( it->node == n2 ) {
			return;
		}
	}
	
	//コスト=距離計算
	double cost = QueryCost(n1, n2);

	//接続
	n1->m_connectNode.push_back( NodeConnect(n2,(int)cost) );
	n2->m_connectNode.push_back( NodeConnect(n1,(int)cost) );
}

//クリック位置のノードを取得
Node* Dijkstra::Query(int x, int y)
{
	std::list<Node*>::iterator it = m_node.begin();
	for ( ; it != m_node.end(); it++ ) {
		
		if ( (*it)->x == x && (*it)->y == y ) {
			return (*it);	
		}
	
	}
	return NULL;
}

// 城番号からノードを取得
Node* Dijkstra::Query(int iCastleID)
{
	std::list<Node*>::iterator it = m_node.begin();
	for ( ; it != m_node.end(); it++ ) {
		
		if ( (*it)->GetCastleID() == iCastleID ) {
			return (*it);	
		}
	
	}
	return NULL;
}

//クリック位置の線に所属するノード組を取得
bool Dijkstra::QueryLineOnNodePair( NodePair* rslt, int x, int y )
{
	int search_leng = Circle_r * 1;

	std::list<Node*>::iterator it = m_node.begin();
	Node *node, *sideNode;
	for ( ; it != m_node.end(); it++ ) {
		node = *it;
		
		std::list<NodeConnect>::iterator it_cnct = (*it)->m_connectNode.begin();
		for ( ; it_cnct != (*it)->m_connectNode.end(); it_cnct++ ) {
			
			sideNode = it_cnct->node;
		
			//点と線の交差チェック
			if ( IntersectPointLine( x, y, node->x, node->y, sideNode->x, sideNode->y, search_leng ) ) {
				rslt->n1 = node;
				rslt->n2 = sideNode;
				return true;
			}
		}
	}

	return false;
}

//各ノードをリセット
void Dijkstra::ResetNode() {
	
	std::list<Node*>::iterator it = m_node.begin();
	for ( ; it != m_node.end(); it++ ) {
		(*it)->Reset();
	}
}

//ルート検索開始
void Dijkstra::SearchRoot() {

	//全ノードの計算結果をリセット
	ResetNode();

	if ( !m_start || !m_goal ) { return; }

	std::list<Node*> work1;
	std::list<Node*> work2;

	std::list<Node*>* currLevel = &work1;
	std::list<Node*>* nextLevel = &work2;
	std::list<Node*>* for_swap;//currLevelとnextLevel入れ替えに使う

    //ゴールから計算します。
	m_goal->cost = 0;//ゴールにコスト0をセットして計算済みとする
	currLevel->push_back(m_goal);//検索第一階層のノードセット(ゴールノードをセットする)

	std::list<Node*>::iterator it;
	int nodeCost;
	while ( currLevel->size() ) {

		for( it = currLevel->begin(); it != currLevel->end(); it++ ) {
			
			//次のノード階層へ進めるなら、nextレベルに格納
			std::list<NodeConnect>::iterator it_cnct = (*it)->m_connectNode.begin();
			for( ; it_cnct != (*it)->m_connectNode.end(); it_cnct++ ) {
				
				nodeCost = (*it)->cost + it_cnct->cost;

				if ( ( it_cnct->node->cost == -1 ) || ( nodeCost < it_cnct->node->cost ) ) {
					//未探索ノードあるいは最短ルートを更新できる場合

					//経路コストとゴールへ向かうためのノードをセット
					it_cnct->node->cost = nodeCost;
					it_cnct->node->toGoal = (*it);

				} else {
					continue;
				}
				
				//次に検索する階層のリストに登録
				nextLevel->push_back( it_cnct->node );
			}
		
		}
		
		//リストを入れ替えて次の階層を検索する
		for_swap = currLevel;
		currLevel = nextLevel;
		nextLevel = for_swap;
		nextLevel->clear();//クリアする
	}
		
}