#pragma once

#include <string>
using namespace std;

// 0F 〜です 〜だ じゃ
string ％です％			(int i1st_BushouID, int i2nd_BushouID = 0xFFFF	, int i3rd_BushouID = 0xFFFF);

// 11 〜ですか 〜でござるか 〜か
string ％ですか％		(int i1st_BushouID, int i2nd_BushouID = 0xFFFF	, int i3rd_BushouID = 0xFFFF);

// 13 〜でしょう 〜じゃろう 〜であろう 〜だろう
string ％でしょう％		(int i1st_BushouID, int i2nd_BushouID = 0xFFFF	, int i3rd_BushouID = 0xFFFF);

// 15 〜でした 〜じゃった 〜であった 〜だった
string ％でした％		(int i1st_BushouID, int i2nd_BushouID = 0xFFFF	, int i3rd_BushouID = 0xFFFF);

// 17 〜あります 〜ございます 〜ござる 〜ある 〜おじゃる
string ％あります％		(int i1st_BushouID, int i2nd_BushouID = 0xFFFF	, int i3rd_BushouID = 0xFFFF);

// 19 〜ありません 〜ございません 〜ござらぬ 〜ない 〜おじゃらぬ 〜ありませぬ
string ％ありません％	(int i1st_BushouID, int i2nd_BushouID = 0xFFFF	, int i3rd_BushouID = 0xFFFF);

// 1B 〜します 〜いたす 〜する
string ％します％		(int i1st_BushouID, int i2nd_BushouID = 0xFFFF	, int i3rd_BushouID = 0xFFFF);

// 1D 〜しません 〜いたさぬ 〜せぬ 〜せん 〜しませぬ
string ％しません％		(int i1st_BushouID, int i2nd_BushouID = 0xFFFF	, int i3rd_BushouID = 0xFFFF);

// 1F 〜しましょう 〜いたそう 〜しよう
string ％しましょう％	(int i1st_BushouID, int i2nd_BushouID = 0xFFFF	, int i3rd_BushouID = 0xFFFF);


// 21 〜いましょう 〜おう  (言いましょうなど)
string ％いましょう％	(int i1st_BushouID, int i2nd_BushouID = 0xFFFF	, int i3rd_BushouID = 0xFFFF);

// 23 〜きましょう 〜こう  (行きましょうなど)
string ％きましょう％	(int i1st_BushouID, int i2nd_BushouID = 0xFFFF	, int i3rd_BushouID = 0xFFFF);

// 25 〜しましょう 〜そう  (？… ％しましょう％の方が良いか)
string ％しましょう２％	(int i1st_BushouID, int i2nd_BushouID = 0xFFFF	, int i3rd_BushouID = 0xFFFF);

// 27 〜ちましょう 〜とう  (待ちましょうなど)
string ％ちましょう％	(int i1st_BushouID, int i2nd_BushouID = 0xFFFF	, int i3rd_BushouID = 0xFFFF);

// 29 〜みましょう 〜もう  (攻め込みましょうなど)
string ％みましょう％	(int i1st_BushouID, int i2nd_BushouID = 0xFFFF	, int i3rd_BushouID = 0xFFFF);

// 2B 〜ましょう 〜よう  (その他の一般パターン。立てましょうなど)
string ％ましょう％		(int i1st_BushouID, int i2nd_BushouID = 0xFFFF	, int i3rd_BushouID = 0xFFFF);

// 2D 〜りましょう 〜ろう  (参りましょうなど)
string ％りましょう％	(int i1st_BushouID, int i2nd_BushouID = 0xFFFF	, int i3rd_BushouID = 0xFFFF);

// 31 〜ぎましょう 〜ごう  (急ぎましょうなど)
string ％ぎましょう％	(int i1st_BushouID, int i2nd_BushouID = 0xFFFF	, int i3rd_BushouID = 0xFFFF);

// 33 〜びましょう 〜ぼう　(遊びましょうなど)
string ％びましょう％	(int i1st_BushouID, int i2nd_BushouID = 0xFFFF	, int i3rd_BushouID = 0xFFFF);


// 35 〜います 〜ござる 〜おる 〜いる 〜おじゃる 〜おります
string ％います％		(int i1st_BushouID, int i2nd_BushouID = 0xFFFF	, int i3rd_BushouID = 0xFFFF);

// 37 〜いません 〜ござらぬ 〜おらぬ 〜おらん 〜おじゃらぬ 〜おりませぬ
string ％いません％		(int i1st_BushouID, int i2nd_BushouID = 0xFFFF	, int i3rd_BushouID = 0xFFFF);

// 39 〜いました 〜ござった 〜おった 〜いた 〜おじゃった 〜おりました
string ％いました％		(int i1st_BushouID, int i2nd_BushouID = 0xFFFF	, int i3rd_BushouID = 0xFFFF);

// 3B 〜ます 〜る 〜ります
string ％ます％			(int i1st_BushouID, int i2nd_BushouID = 0xFFFF	, int i3rd_BushouID = 0xFFFF);

// 3D 〜りません 〜らぬ
string ％りません％		(int i1st_BushouID, int i2nd_BushouID = 0xFFFF	, int i3rd_BushouID = 0xFFFF);

// 3F 〜ません 〜ぬ 〜ねえ 〜ませぬ
string ％ません％		(int i1st_BushouID, int i2nd_BushouID = 0xFFFF	, int i3rd_BushouID = 0xFFFF);

// 41 〜ました 〜た
string ％ました％		(int i1st_BushouID, int i2nd_BushouID = 0xFFFF	, int i3rd_BushouID = 0xFFFF);

// 43 〜りました 〜った
string ％りました％		(int i1st_BushouID, int i2nd_BushouID = 0xFFFF	, int i3rd_BushouID = 0xFFFF);

// 45 〜ませんでした 〜なかった
string ％ませんでした％	(int i1st_BushouID, int i2nd_BushouID = 0xFFFF	, int i3rd_BushouID = 0xFFFF);

// 47 〜思います 〜存じます 〜思う
string ％思います％		(int i1st_BushouID, int i2nd_BushouID = 0xFFFF	, int i3rd_BushouID = 0xFFFF);

// 49 〜参りました 〜参上しました 〜参った
string ％参りました％	(int i1st_BushouID, int i2nd_BushouID = 0xFFFF	, int i3rd_BushouID = 0xFFFF);

// 4B 〜くだされ 〜下さい 〜くれ
string ％くだされ％		(int i1st_BushouID, int i2nd_BushouID = 0xFFFF	, int i3rd_BushouID = 0xFFFF);

// 4D 〜すみません 〜申し訳ない 〜すまぬ 〜すまん 〜ごめんなさい
string ％すみません％	(int i1st_BushouID, int i2nd_BushouID = 0xFFFF	, int i3rd_BushouID = 0xFFFF);

// 4F 〜願います 〜お願いいたす 〜願おう 〜願う 〜お願いします
string ％願います％		(int i1st_BushouID, int i2nd_BushouID = 0xFFFF	, int i3rd_BushouID = 0xFFFF);

// 51 〜よい 〜よろしい
string ％よい％			(int i1st_BushouID, int i2nd_BushouID = 0xFFFF	, int i3rd_BushouID = 0xFFFF);