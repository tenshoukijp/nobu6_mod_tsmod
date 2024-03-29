#pragma once


namespace 役職 {
	enum { 無し = 0, 征夷大将軍 = 1, 管領 = 2, 関東管領 = 3, 奥州探題 = 4, 西国探題 = 5, 九州探題 = 6, 羽州探題 = 7 };
}

// 上の全部一列ではいった状態から１つ１つを取り出すために必要となる相対アドレス宣言 
int GAMEDATASTRUCT_POSITIONTYPELIST_POINTER[];

// 役職タイプのIDから文字列を得る
char* getPositionTypeName(byte type);


// 正規化された役職番号から大名用の役職番号を得る。
int DeNormalizeDaimyoYakusyokuValue(int iYakusyokuValue);

// 大名IDから正規化された役職IDを得る
int getNormalizedDaimyoPosition(int iDaimyoID);

// 軍団IDから正規化された役職IDを得る
int getNormalizedGundanPosition(int iGundanID);

// 大名の役職の付け替えを試みる
bool tryChangeDaimyoYakusyoku(int iDaimyoID, int iYakusyokuValue);

// 軍団の役職の付け替えを試みる
bool tryChangeGundanYakusyoku(int iGundanID, int iYakusyokuValue);

