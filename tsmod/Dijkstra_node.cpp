#include "dijkstra.h"

Node::Node(int iCastleID, int ix, int iy)
{
	this->iCastleID = iCastleID;

	x = ix;
	y = iy;
	
	Reset();
}

//計算結果をクリアする
void Node::Reset()
{
	cost = -1;
	toGoal = NULL;
}

//特定ノードへの接続を削除
void Node::RemoveConnect(Node* n)
{
	std::list<NodeConnect>::iterator it = m_connectNode.begin();
	for ( ; it != m_connectNode.end(); ) {
		
		if ( it->node == n ) {
			it = m_connectNode.erase(it);
			break;
		}
		it++;
	}
}

