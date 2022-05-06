#include "GameDataStruct.h"
#include "dijkstra.h"

/*
 実際のWindows上の描画となる。TSModではこれらの関数を直接これを利用することはない。
 */


//円描画
void DrawCircle(HDC hdc, int x, int y, int r, COLORREF rgb)
{
	HBRUSH brs  = CreateSolidBrush(rgb);
	HBRUSH brsOld = (HBRUSH)SelectObject( hdc , brs );

	Ellipse( hdc, x - r, y - r, x + r, y + r );

	SelectObject( hdc , brsOld );
	DeleteObject(brs);
}

//描画
void Dijkstra::Draw(HDC hdc)
{

	std::list<Node*>::iterator it = m_node.begin();
	Node *node, *sideNode;
	for ( ; it != m_node.end(); it++ ) {
		node = *it;
		
		//点の描画
		DrawCircle(hdc, node->x, node->y, Circle_r, RGB(0,100,0) );

		//ルート描画
		std::list<NodeConnect>::iterator it_cnct = (*it)->m_connectNode.begin();
		for ( ; it_cnct != (*it)->m_connectNode.end(); it_cnct++ ) {
			
			sideNode = it_cnct->node;

			::MoveToEx(hdc, node->x, node->y, NULL );
			::LineTo(hdc,sideNode->x, sideNode->y);
		}
	}

	//経路描画
	if ( m_start && m_start->cost != -1 ) {
		HPEN pen = CreatePen(PS_SOLID,3,RGB(255,0,0));
		
		HPEN oldPen = (HPEN)SelectObject( hdc, pen );
		
		Node* curr = m_start;
		while ( curr->toGoal ) {
			::MoveToEx(hdc, curr->x, curr->y, NULL );
			::LineTo(hdc,curr->toGoal->x, curr->toGoal->y);

			curr = curr->toGoal;
		}

		SelectObject(hdc, oldPen);
		DeleteObject(pen);
	}

	char text[1000];
	ZeroMemory(text, sizeof(text));

	//スタート点ゴール点
	{
	
		int r = Circle_r*1;
		if ( m_start ) {
			DrawCircle(hdc, m_start->x, m_start->y, r, RGB(0,255,0) );
			
			ZeroMemory(text, sizeof(text));
			wsprintf(text, _T("スタート") );
			TextOut(hdc, m_start->x-r, m_start->y+r, text, strlen(text));
		}
		if ( m_goal ) {
			DrawCircle(hdc, m_goal->x, m_goal->y, r, RGB(0,0,255) );

			ZeroMemory(text, sizeof(text));
			wsprintf(text, _T("ゴール") );
			TextOut(hdc, m_goal->x-r, m_goal->y+r, text, strlen(text));
		}
	}

}
