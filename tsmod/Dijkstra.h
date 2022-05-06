// メインマップの城の経路をダイクストラ法計算で求めるためのクラス


#pragma once
#include <list>
#include <math.h>
#include <windows.h>

#define Circle_r 1	//描画、検索の基準

struct Node;
struct NodeConnect;

//ノード
struct Node
{
	int iCastleID;
	int GetCastleID() {
		return iCastleID;
	}

	int x;//ノードの座標
	int y;

	std::list<NodeConnect> m_connectNode;	//接続しているノード

	//計算で保持する値-------------------
	int		cost;	//探索に要したコスト。-1の時はそのノードを未探索としています。
	Node*	toGoal;	//ゴールへの最短ルートにつながるノード
	//-----------------------------------


	Node(int iCastleID, int ix, int iy);

	//計算結果をクリアする
	void Reset();

	//特定ノードへの接続を削除
	void RemoveConnect(Node* n);
};

//ノード間のコスト計算(ここでは距離をコストとします)
double QueryCost(Node* n1, Node* n2 );

//他のノードへの接続情報
struct NodeConnect
{
	Node*		node;//移動先ノード
	int			cost;//移動にかかるコスト(ここでは距離)

	NodeConnect( Node*	_node, int _cost )
	{
		node = _node;
		cost = _cost;
	}
};

struct NodePair
{
	Node* n1;
	Node* n2;
};

//ダイクストラ法で最短経路を求めるクラス
class Dijkstra
{
	Node* m_start;
	Node* m_goal;
	std::list<Node*> m_node;

	//ノードがリストの何番目か 0からスタート 無いとき-1
	int QueryIdx( Node* node );
	//インデックス指定でリストからノード取得
	Node* QueryFromIdx(int idx);

public:
	Dijkstra();
	virtual ~Dijkstra();

	//全ノードを削除
	void Clear();

	void AddNode(int iCastleID, int x, int y)
	{
		m_node.push_back( new Node(iCastleID, x,y) );
	}
	
	//指定ノードを削除
	void Remove( Node* n1 );
	//指定ノード接続を削除
	void RemoveConnect( Node* n1, Node* n2 );

	//２点間のルートを作成
	void ConnectNode( Node* n1, Node* n2 );

	//クリック位置のノードを取得
	Node* Query(int x, int y);

	// 城番号からノードを取得
	Node* Query(int iCastleID);

	//クリック位置の線に所属するノード組を取得
	bool QueryLineOnNodePair(NodePair* rslt, int x, int y);

	//各ノードをリセット
	void ResetNode();

	//ルート検索開始
	void SearchRoot();

	//検索スタート点
	void SelctStart(Node* node) {
		m_start	= node;
	}
	void SelctGoal(Node* node) {
		m_goal	= node;
	}

	Node* GetStartNode() {
		return m_start;
	}

	Node* GetGoalNode() {
		return m_goal;
	}

	//描画
	void Draw(HDC hdc);

	//ファイル読み込み、書き込み
	bool Inport();

};
