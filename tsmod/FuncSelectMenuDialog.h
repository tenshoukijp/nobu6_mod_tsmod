#pragma once

#pragma pack(1)

#include <string>
#include <vector>

using namespace std;




// 選択タイプのメニューの表示。各メニューの文字列は menu_list に入れておくこと。posXやpoxYは表示する座標。Ｘは右へ、Ｙは下へ 
// 何も選択せず右クリックで閉じた場合は、0xFFFFが値として返ってくる。
// ＸやＹを指定しないときは、真ん中あたりに配置する。
int SelectMenuDialog(vector<string> menu_list, int posX=-1, int posY=-1, int MinMenuWidth=0x70);
