#include "WinTarget.h"
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <time.h>


#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"
#include "MessageInflection.h"
#include "HumanRelation.h"


BOOL ％略記条件１％(int i1st_BushouID, int i2nd_BushouID) {

	// 自分が下、ということ。
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM &&
		0 <= i2nd_BushouID && i2nd_BushouID < GAMEDATASTRUCT_BUSHOU_NUM) {

			// 自身>相身 (自分の身分の方が下ということ)
			if ( nb6bushouname[i1st_BushouID].position > nb6bushouname[i2nd_BushouID].position ){
				return true;
			}
	}
	return false;
}

BOOL ％略記条件２％(int i1st_BushouID, int i2nd_BushouID) {

	// 血縁関係や、同じ釜の飯だが、自分が上、ということ。
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM &&
		0 <= i2nd_BushouID && i2nd_BushouID < GAMEDATASTRUCT_BUSHOU_NUM) {

			// 自相血縁&自齢≧相齢 
			if ( is自分は相手の血縁( i1st_BushouID, i2nd_BushouID ) && getYearsOld(i1st_BushouID) >= getYearsOld(i2nd_BushouID) ) {
				return true;
			}
			// 相手は自分の家臣
			if ( is相手は自分の家臣( i1st_BushouID, i2nd_BushouID ) ) {
				return true;
			}
			// 同じ家に所属している & 自齢≧相齢 & 自身<相身(自分の方が身分が高い)
			if ( is自分と相手は同じ大名家所属( i1st_BushouID, i2nd_BushouID ) &&  getYearsOld(i1st_BushouID) >= getYearsOld(i2nd_BushouID) && 
				nb6bushouname[i1st_BushouID].position < nb6bushouname[i2nd_BushouID].position ) {
					return true;
			}
	}
	return false;
}

int ％略記条件Ｔ％(int i1st_BushouID, int i2nd_BushouID) {
	// 相手の武将がいないのであれば、1となる。
	if ( i2nd_BushouID == 0xFFFF ) {
		return 1;
	}

	// 略記条件２を満たす。(同じ釜の飯でかつ、自分が上)
	else if ( ％略記条件２％( i1st_BushouID, i2nd_BushouID ) ) {
		return 1;
	}
	// 略記条件１を満たす。(自分が下)
	else if ( ％略記条件１％( i1st_BushouID, i2nd_BushouID ) ) {
		return 2;
	}
	// 略記条件１も略記条件２も満たさない(自分の方が身分は上)
	else {
		// それ以外
		return 3;
	}
}



// 〜です 〜だ 〜じゃ
string ％です％(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {

	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// 略記条件Ｔのどれがあてはまるか？ どれにもあてはまらなかったら略記条件Ｙでどちらかに当てはまる。
		int Group = ％略記条件Ｔ％(i1st_BushouID,i2nd_BushouID) ;

		// Group A
		if ( Group == 1 ) {

			// です 自女 
			if ( nb6bushouname[i1st_BushouID].sex == 1 ) {
				return "です";

				// だ 自口=2 || 自口=4
			} else if ( nb6bushouref[i1st_BushouID].tone == 2 || nb6bushouref[i1st_BushouID].tone == 4 ) {
				return "だ";

				// じゃ （他） 
			} else {
				return "じゃ";
			}

			// Group B
		} else {
			// です （他） 
			return "です";
		}
	}

	return "％です％";
}



// 〜ですか 〜でござるか 〜か
string ％ですか％(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {

	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// 略記条件Ｔのどれがあてはまるか？ どれにもあてはまらなかったら略記条件Ｙでどちらかに当てはまる。
		int Group = ％略記条件Ｔ％(i1st_BushouID,i2nd_BushouID) ;

		// Group A
		if ( Group == 1 ) {

			// ですか 自女 
			if ( nb6bushouname[i1st_BushouID].sex == 1 ) {
				return "ですか";

			// か （他） 
			} else {
				return "か";
			}

		// Group B
		} else {
			// ですか 自女 
			if ( nb6bushouname[i1st_BushouID].sex == 1 ) {
				return "ですか";

			// でござるか 自口=5 || 自職=1or3(忍者or剣豪)
			} else if ( nb6bushouref[i1st_BushouID].tone == 5 || nb6bushouref[i1st_BushouID].job == 0x10 || nb6bushouref[i1st_BushouID].job == 0x30 ) {
				return "でござるか";

			// ですか （他） 
			} else {
				return "ですか";
			}
		}
	}

	return "％ですか％";
}



// でしょう 〜じゃろう 〜であろう 〜だろう
string ％でしょう％(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {

	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// 略記条件Ｔのどれがあてはまるか？ どれにもあてはまらなかったら略記条件Ｙでどちらかに当てはまる。
		int Group = ％略記条件Ｔ％(i1st_BushouID,i2nd_BushouID) ;

		// Group A
		if ( Group == 1 ) {

			// でしょう 自女 
			if ( nb6bushouname[i1st_BushouID].sex == 1 ) {
				return "でしょう";

			// であろう 自口=3 || 自口=1 || 自口=5
			} else if ( nb6bushouref[i1st_BushouID].tone == 3 || nb6bushouref[i1st_BushouID].tone == 1 || nb6bushouref[i1st_BushouID].tone == 5 ) {
				return "であろう";

			// だろう 自口=2 || 自口=4
			} else if ( nb6bushouref[i1st_BushouID].tone == 2 || nb6bushouref[i1st_BushouID].tone == 4 ) {
				return "だろう";

			// じゃろう (他)
			} else {
				return "じゃろう";
			}

		// Group B
		} else {
			// でしょう
			return "でしょう";
		}
	}

	return "％でしょう％";
}



// 〜でした 〜じゃった 〜であった 〜だった
string ％でした％(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// 略記条件Ｔのどれがあてはまるか？ どれにもあてはまらなかったら略記条件Ｙでどちらかに当てはまる。
		int Group = ％略記条件Ｔ％(i1st_BushouID,i2nd_BushouID) ;

		// Group A
		if ( Group == 1 ) {

			// でした 自女 
			if ( nb6bushouname[i1st_BushouID].sex == 1 ) {
				return "でした";

			// であった 自口=3 || 自口=1 || 自口=5
			} else if ( nb6bushouref[i1st_BushouID].tone == 3 || nb6bushouref[i1st_BushouID].tone == 1 || nb6bushouref[i1st_BushouID].tone == 5 ) {
				return "であった";

			// だった 自口=2 || 自口=4
			} else if ( nb6bushouref[i1st_BushouID].tone == 2 || nb6bushouref[i1st_BushouID].tone == 4 ) {
				return "だった";

			// じゃった (他)
			} else {
				return "じゃった";
			}

		// Group B
		} else {
			// でした
			return "でした";
		}
	}

	return "％でした％";
}



// 〜あります 〜ございます 〜ござる 〜ある 〜おじゃる
string ％あります％(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// 略記条件Ｔのどれがあてはまるか？ どれにもあてはまらなかったら略記条件Ｙでどちらかに当てはまる。
		int Group = ％略記条件Ｔ％(i1st_BushouID,i2nd_BushouID) ;

		// Group A
		if ( Group == 1 ) {

			// あります 自女 
			if ( nb6bushouname[i1st_BushouID].sex == 1 ) {
				return "あります";

			// おじゃる 自口=3
			} else if ( nb6bushouref[i1st_BushouID].tone == 3 ) {
				return "おじゃる";

			// ある (他)
			} else {
				return "ある";
			}

		// Group B
		} else {
			// あります 自女 
			if ( nb6bushouname[i1st_BushouID].sex == 1 ) {
				return "あります";

			// ござる 自口=5 || 自職=1or3(忍者or剣豪)
			} else if ( nb6bushouref[i1st_BushouID].tone == 5 || nb6bushouref[i1st_BushouID].job == 0x10 || nb6bushouref[i1st_BushouID].job == 0x30 ) {
				return "ござる";

			// あります 自相=同じ大名家所属 
			} else if ( is自分と相手は同じ大名家所属(i1st_BushouID,i2nd_BushouID) ) {
				return "あります";

			// ございます （他） 
			} else {
				return "ございます";

			}
		}
	}

	return "％あります％";
}


// 〜ありません 〜ございません 〜ござらぬ 〜ない 〜おじゃらぬ 〜ありませぬ
string ％ありません％(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// 略記条件Ｔのどれがあてはまるか？ どれにもあてはまらなかったら略記条件Ｙでどちらかに当てはまる。
		int Group = ％略記条件Ｔ％(i1st_BushouID,i2nd_BushouID) ;

		// Group A
		if ( Group == 1 ) {

			// ありませぬ 自女 
			if ( nb6bushouname[i1st_BushouID].sex == 1 ) {
				return "ありませぬ";

			// おじゃらぬ 自口=3
			} else if ( nb6bushouref[i1st_BushouID].tone == 3 ) {
				return "おじゃらぬ";

			// ない (他)
			} else {
				return "ない";
			}

		// Group B
		} else {
			// ありませぬ 自女 
			if ( nb6bushouname[i1st_BushouID].sex == 1 ) {
				return "ありませぬ";

			// ござらぬ 自口=5 || 自職=1or3(忍者or剣豪)
			} else if ( nb6bushouref[i1st_BushouID].tone == 5 || nb6bushouref[i1st_BushouID].job == 0x10 || nb6bushouref[i1st_BushouID].job == 0x30 ) {
				return "ござらぬ";

			// ありません 自相=同じ大名家所属 
			} else if ( is自分と相手は同じ大名家所属(i1st_BushouID,i2nd_BushouID) ) {
				return "あります";

			// ございません （他） 
			} else {
				return "ございます";

			}
		}
	}

	return "％ありません％";
}




// 〜します 〜いたす 〜する
string ％します％(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// 略記条件Ｔのどれがあてはまるか？ どれにもあてはまらなかったら略記条件Ｙでどちらかに当てはまる。
		int Group = ％略記条件Ｔ％(i1st_BushouID,i2nd_BushouID) ;

		// Group A
		if ( Group == 1 ) {

			// します 自女 
			if ( nb6bushouname[i1st_BushouID].sex == 1 ) {
				return "します";

			// する 自口=2 || 自口=4
			} else if ( nb6bushouref[i1st_BushouID].tone == 2 || nb6bushouref[i1st_BushouID].tone == 4 ) {
				return "する";

			// いたす (他)
			} else {
				return "いたす";
			}

		// Group B
		} else {
			// いたす 自口=5 || 自男
			if ( nb6bushouref[i1st_BushouID].tone == 5 || nb6bushouname[i1st_BushouID].sex == 0 ) {
				return "いたす";

			// します (他)
			} else {
				return "します";
			}
		}
	}

	return "％します％";
}

// 〜しません 〜いたさぬ 〜せぬ 〜せん 〜しませぬ
string ％しません％(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// 略記条件Ｔのどれがあてはまるか？ どれにもあてはまらなかったら略記条件Ｙでどちらかに当てはまる。
		int Group = ％略記条件Ｔ％(i1st_BushouID,i2nd_BushouID) ;

		// Group A
		if ( Group == 1 ) {

			// しませぬ 自女 
			if ( nb6bushouname[i1st_BushouID].sex == 1 ) {
				return "しませぬ";

			// せん 自口=2 || 自口=4
			} else if ( nb6bushouref[i1st_BushouID].tone == 2 || nb6bushouref[i1st_BushouID].tone == 4 ) {
				return "せん";

			// せぬ (他)
			} else {
				return "せぬ";
			}

		// Group B
		} else {
			// しませぬ 自口=1 || 自口=3 || 自女
			if ( nb6bushouref[i1st_BushouID].tone == 1 || nb6bushouref[i1st_BushouID].tone == 3 || nb6bushouname[i1st_BushouID].sex == 1 ) {
				return "しませぬ";

			// いたさぬ 自口=5
			} else if ( nb6bushouref[i1st_BushouID].tone == 5 ) {
				return "いたさぬ";

			// しません (他)
			} else {
				return "しません";
			}
		}
	}

	return "％しません％";
}



// 〜しましょう 〜いたそう 〜しよう
string ％しましょう％(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// 略記条件Ｔのどれがあてはまるか？ どれにもあてはまらなかったら略記条件Ｙでどちらかに当てはまる。
		int Group = ％略記条件Ｔ％(i1st_BushouID,i2nd_BushouID) ;

		// Group A
		if ( Group == 1 ) {

			// しましょう 自女 
			if ( nb6bushouname[i1st_BushouID].sex == 1 ) {
				return "しましょう";

			// しよう 自口=2 || 自口=4
			} else if ( nb6bushouref[i1st_BushouID].tone == 2 || nb6bushouref[i1st_BushouID].tone == 4 ) {
				return "しよう";

			// いたそう (他)
			} else {
				return "いたそう";
			}

		// Group B
		} else {
			// いたそう 自口=5 || 自男
			if ( nb6bushouref[i1st_BushouID].tone == 5 || nb6bushouname[i1st_BushouID].sex == 0 ) {
				return "いたそう";

			// しましょう (他)
			} else {
				return "しましょう";
			}
		}
	}

	return "％しましょう％";
}



// 〜いましょう 〜おう  (言いましょうなど)
string ％いましょう％(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// 略記条件Ｔのどれがあてはまるか？ どれにもあてはまらなかったら略記条件Ｙでどちらかに当てはまる。
		int Group = ％略記条件Ｔ％(i1st_BushouID,i2nd_BushouID) ;

		// Group A
		if ( Group == 1 ) {

			// いましょう 自女 
			if ( nb6bushouname[i1st_BushouID].sex == 1 ) {
				return "いましょう";

			// おう (他)
			} else {
				return "おう";
			}

		// Group B
		} else {
			// いましょう (他)
			return "いましょう";
		}
	}

	return "％いましょう％";
}



// 〜きましょう 〜こう  (行きましょうなど)
string ％きましょう％(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// 略記条件Ｔのどれがあてはまるか？ どれにもあてはまらなかったら略記条件Ｙでどちらかに当てはまる。
		int Group = ％略記条件Ｔ％(i1st_BushouID,i2nd_BushouID) ;

		// Group A
		if ( Group == 1 ) {

			// きましょう 自女 
			if ( nb6bushouname[i1st_BushouID].sex == 1 ) {
				return "きましょう";

				// こう (他)
			} else {
				return "こう";
			}

			// Group B
		} else {
			// きましょう (他)
			return "きましょう";
		}
	}

	return "％きましょう％";
}


// 〜しましょう 〜そう  (？… ％しましょう％の方が良いか)
string ％しましょう２％(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// 略記条件Ｔのどれがあてはまるか？ どれにもあてはまらなかったら略記条件Ｙでどちらかに当てはまる。
		int Group = ％略記条件Ｔ％(i1st_BushouID,i2nd_BushouID) ;

		// Group A
		if ( Group == 1 ) {

			// そう (他)
			return "そう";

			// Group B
		} else {
			// しましょう (他)
			return "しましょう";
		}
	}

	return "％しましょう２％";
}



// 〜ちましょう 〜とう  (待ちましょうなど)
string ％ちましょう％(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// 略記条件Ｔのどれがあてはまるか？ どれにもあてはまらなかったら略記条件Ｙでどちらかに当てはまる。
		int Group = ％略記条件Ｔ％(i1st_BushouID,i2nd_BushouID) ;

		// Group A
		if ( Group == 1 ) {

			// とう (他)
			return "とう";

			// Group B
		} else {
			// ちましょう (他)
			return "ちましょう";
		}
	}

	return "％ちましょう％";
}


// 29 〜みましょう 〜もう  (攻め込みましょうなど)
string ％みましょう％(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// 略記条件Ｔのどれがあてはまるか？ どれにもあてはまらなかったら略記条件Ｙでどちらかに当てはまる。
		int Group = ％略記条件Ｔ％(i1st_BushouID,i2nd_BushouID) ;

		// Group A
		if ( Group == 1 ) {

			// もう (他)
			return "もう";

			// Group B
		} else {
			// みましょう (他)
			return "みましょう";
		}
	}

	return "％ちましょう％";
}


// 〜ましょう 〜よう  (その他の一般パターン。立てましょうなど)
string ％ましょう％(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// 略記条件Ｔのどれがあてはまるか？ どれにもあてはまらなかったら略記条件Ｙでどちらかに当てはまる。
		int Group = ％略記条件Ｔ％(i1st_BushouID,i2nd_BushouID) ;

		// Group A
		if ( Group == 1 ) {

			// ましょう 自女 
			if ( nb6bushouname[i1st_BushouID].sex == 1 ) {
				return "ましょう";

				// よう (他)
			} else {
				return "よう";
			}

			// Group B
		} else {
			// ましょう (他)
			return "ましょう";
		}
	}

	return "％ましょう％";
}



// 〜りましょう 〜ろう  (参りましょうなど)
string ％りましょう％(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// 略記条件Ｔのどれがあてはまるか？ どれにもあてはまらなかったら略記条件Ｙでどちらかに当てはまる。
		int Group = ％略記条件Ｔ％(i1st_BushouID,i2nd_BushouID) ;

		// Group A
		if ( Group == 1 ) {

			// りましょう 自女 
			if ( nb6bushouname[i1st_BushouID].sex == 1 ) {
				return "りましょう";

				// ろう (他)
			} else {
				return "ろう";
			}

			// Group B
		} else {
			// りましょう (他)
			return "りましょう";
		}
	}

	return "％りましょう％";
}


// 〜ぎましょう 〜ごう  (急ぎましょうなど)
string ％ぎましょう％(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// 略記条件Ｔのどれがあてはまるか？ どれにもあてはまらなかったら略記条件Ｙでどちらかに当てはまる。
		int Group = ％略記条件Ｔ％(i1st_BushouID,i2nd_BushouID) ;

		// Group A
		if ( Group == 1 ) {

			// ごう (他)
			return "ごう";

		// Group B
		} else {
			// ぎましょう (他)
			return "ぎましょう";
		}
	}

	return "％ぎましょう％";
}


// 〜びましょう 〜ぼう　(遊びましょうなど)
string ％びましょう％(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// 略記条件Ｔのどれがあてはまるか？ どれにもあてはまらなかったら略記条件Ｙでどちらかに当てはまる。
		int Group = ％略記条件Ｔ％(i1st_BushouID,i2nd_BushouID) ;

		// Group A
		if ( Group == 1 ) {

			// ぼう (他)
			return "ぼう";

		// Group B
		} else {
			// びましょう (他)
			return "びましょう";
		}
	}

	return "％びましょう％";
}



// 35 〜います 〜ござる 〜おる 〜いる 〜おじゃる 〜おります
string ％います％(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// 略記条件Ｔのどれがあてはまるか？ どれにもあてはまらなかったら略記条件Ｙでどちらかに当てはまる。
		int Group = ％略記条件Ｔ％(i1st_BushouID,i2nd_BushouID) ;

		// Group A
		if ( Group == 1 ) {

			// おります 自女 
			if ( nb6bushouname[i1st_BushouID].sex == 1 ) {
				return "おります";

			// おじゃらぬ 自口=3
			} else if ( nb6bushouref[i1st_BushouID].tone == 3 ) {
				return "おじゃる";

			// いる 自口=2 || 自口=4
			} else if ( nb6bushouref[i1st_BushouID].tone == 2 || nb6bushouref[i1st_BushouID].tone == 4 ) {
				return "いる";

			// おる (他)
			} else {
				return "おる";
			}

		// Group B
		} else {
			// おります 自口=1 or 自口=3 or 自女 
			if ( nb6bushouref[i1st_BushouID].tone == 1 || nb6bushouref[i1st_BushouID].tone == 3 || nb6bushouname[i1st_BushouID].sex == 1 ) {
				return "おります";

			// ござる 自口=5 || 自職=1or3(忍者or剣豪)
			} else if ( nb6bushouref[i1st_BushouID].tone == 5 || nb6bushouref[i1st_BushouID].job == 0x10 || nb6bushouref[i1st_BushouID].job == 0x30 ) {
				return "ござる";

			// います (他） 
			} else {
				return "います";

			}
		}
	}

	return "％います％";
}



// 37 〜いません 〜ござらぬ 〜おらぬ 〜おらん 〜おじゃらぬ 〜おりませぬ
string ％いません％(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// 略記条件Ｔのどれがあてはまるか？ どれにもあてはまらなかったら略記条件Ｙでどちらかに当てはまる。
		int Group = ％略記条件Ｔ％(i1st_BushouID,i2nd_BushouID) ;

		// Group A
		if ( Group == 1 ) {

			// おりませぬ 自女 
			if ( nb6bushouname[i1st_BushouID].sex == 1 ) {
				return "おりませぬ";

			// おじゃらぬ 自口=3
			} else if ( nb6bushouref[i1st_BushouID].tone == 3 ) {
				return "おじゃらぬ";

			// おらん 自口=2 || 自口=4
			} else if ( nb6bushouref[i1st_BushouID].tone == 2 || nb6bushouref[i1st_BushouID].tone == 4 ) {
				return "おらん";

			// おらぬ (他)
			} else {
				return "おらぬ";
			}

		// Group B
		} else {
			// おりませぬ 自口=1 or 自口=3 or 自女 
			if ( nb6bushouref[i1st_BushouID].tone == 1 || nb6bushouref[i1st_BushouID].tone == 3 || nb6bushouname[i1st_BushouID].sex == 1 ) {
				return "おりませぬ";

			// ござらぬ 自口=5 || 自職=1or3(忍者or剣豪)
			} else if ( nb6bushouref[i1st_BushouID].tone == 5 || nb6bushouref[i1st_BushouID].job == 0x10 || nb6bushouref[i1st_BushouID].job == 0x30 ) {
				return "ござらぬ";

			// いません (他） 
			} else {
				return "いません";

			}
		}
	}

	return "％いません％";
}



// 39 〜いました 〜ござった 〜おった 〜いた 〜おじゃった 〜おりました
string ％いました％(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// 略記条件Ｔのどれがあてはまるか？ どれにもあてはまらなかったら略記条件Ｙでどちらかに当てはまる。
		int Group = ％略記条件Ｔ％(i1st_BushouID,i2nd_BushouID) ;

		// Group A
		if ( Group == 1 ) {

			// おりました 自女 
			if ( nb6bushouname[i1st_BushouID].sex == 1 ) {
				return "おりました";

			// おじゃった 自口=3
			} else if ( nb6bushouref[i1st_BushouID].tone == 3 ) {
				return "おじゃった";

			// いた 自口=2 || 自口=4
			} else if ( nb6bushouref[i1st_BushouID].tone == 2 || nb6bushouref[i1st_BushouID].tone == 4 ) {
				return "いた";

			// おった (他)
			} else {
				return "おった";
			}

		// Group B
		} else {
			// おりました 自口=1 or 自口=3 or 自女 
			if ( nb6bushouref[i1st_BushouID].tone == 1 || nb6bushouref[i1st_BushouID].tone == 3 || nb6bushouname[i1st_BushouID].sex == 1 ) {
				return "おりました";

			// ござった 自口=5 || 自職=1or3(忍者or剣豪)
			} else if ( nb6bushouref[i1st_BushouID].tone == 5 || nb6bushouref[i1st_BushouID].job == 0x10 || nb6bushouref[i1st_BushouID].job == 0x30 ) {
				return "ござった";

			// いました (他） 
			} else {
				return "いました";

			}
		}
	}

	return "％いました％";
}



// 〜ます 〜る 〜ります
string ％ます％(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// 略記条件Ｔのどれがあてはまるか？ どれにもあてはまらなかったら略記条件Ｙでどちらかに当てはまる。
		int Group = ％略記条件Ｔ％(i1st_BushouID,i2nd_BushouID) ;

		// Group A
		if ( Group == 1 ) {

			// ます 自女 
			if ( nb6bushouname[i1st_BushouID].sex == 1 ) {
				return "ます";

			// る (他)
			} else {
				return "る";
			}

		// Group B
		} else {

			// ます 自相=同じ大名家所属 
			if ( is自分と相手は同じ大名家所属(i1st_BushouID,i2nd_BushouID) ) {
				return "ます";

			// ります（他） 
			} else {
				return "ります";

			}
		}
	}

	return "％ます％";
}



// 〜りません 〜らぬ
string ％りません％(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// 略記条件Ｔのどれがあてはまるか？ どれにもあてはまらなかったら略記条件Ｙでどちらかに当てはまる。
		int Group = ％略記条件Ｔ％(i1st_BushouID,i2nd_BushouID) ;

		// Group A
		if ( Group == 1 ) {

			// りません 自女 
			if ( nb6bushouname[i1st_BushouID].sex == 1 ) {
				return "りません";

			// らぬ (他)
			} else {
				return "らぬ";
			}

		// Group B
		} else {

			// りません（他） 
			return "りません";
		}
	}

	return "％りません％";
}


// 〜ません 〜ぬ 〜ねえ 〜ませぬ
string ％ません％(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// 略記条件Ｔのどれがあてはまるか？ どれにもあてはまらなかったら略記条件Ｙでどちらかに当てはまる。
		int Group = ％略記条件Ｔ％(i1st_BushouID,i2nd_BushouID) ;

		// Group A
		if ( Group == 1 ) {

			// ませぬ 自女 
			if ( nb6bushouname[i1st_BushouID].sex == 1 ) {
				return "ませぬ";

			// ねえ 自口=2 && 自身<=3
			} else if ( nb6bushouref[i1st_BushouID].tone == 2 && nb6bushouname[i2nd_BushouID].position <= 3 ) {
				return "ねえ";

			// ぬ (他)
			} else {
				return "ぬ";
			}

		// Group B
		} else {
			// ませぬ 自口=1 || 自口=5
			if ( nb6bushouref[i1st_BushouID].tone == 1 || nb6bushouref[i1st_BushouID].tone == 5 ) {
				return "ませぬ";

			// ません (他） 
			} else {
				return "ません";

			}
		}
	}

	return "％ません％";
}



// 〜ました 〜た
string ％ました％(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// 略記条件Ｔのどれがあてはまるか？ どれにもあてはまらなかったら略記条件Ｙでどちらかに当てはまる。
		int Group = ％略記条件Ｔ％(i1st_BushouID,i2nd_BushouID) ;

		// Group A
		if ( Group == 1 ) {

			// ました 自女 
			if ( nb6bushouname[i1st_BushouID].sex == 1 ) {
				return "ました";

			// た (他)
			} else {
				return "た";
			}

		// Group B
		} else {

			// ました (他） 
			return "ました";

		}
	}

	return "％ました％";
}



// 〜りました 〜った
string ％りました％(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// 略記条件Ｔのどれがあてはまるか？ どれにもあてはまらなかったら略記条件Ｙでどちらかに当てはまる。
		int Group = ％略記条件Ｔ％(i1st_BushouID,i2nd_BushouID) ;

		// Group A
		if ( Group == 1 ) {

			// りました 自女 
			if ( nb6bushouname[i1st_BushouID].sex == 1 ) {
				return "りました";

			// った (他)
			} else {
				return "った";
			}

		// Group B
		} else {

			// りました (他） 
			return "りました";

		}
	}

	return "％りました％";
}



// 〜ませんでした 〜なかった
string ％ませんでした％(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// 略記条件Ｔのどれがあてはまるか？ どれにもあてはまらなかったら略記条件Ｙでどちらかに当てはまる。
		int Group = ％略記条件Ｔ％(i1st_BushouID,i2nd_BushouID) ;

		// Group A
		if ( Group == 1 ) {

			// ませんでした 自女 
			if ( nb6bushouname[i1st_BushouID].sex == 1 ) {
				return "ませんでした";

			// なかった (他)
			} else {
				return "なかった";
			}

		// Group B
		} else {

			// ませんでした (他） 
			return "ませんでした";

		}
	}

	return "％ませんでした％";
}



// 〜思います 〜存じます 〜思う
string ％思います％(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// 略記条件Ｔのどれがあてはまるか？ どれにもあてはまらなかったら略記条件Ｙでどちらかに当てはまる。
		int Group = ％略記条件Ｔ％(i1st_BushouID,i2nd_BushouID) ;

		// Group A
		if ( Group == 1 ) {

			// 思います 自女 
			if ( nb6bushouname[i1st_BushouID].sex == 1 ) {
				return "思います";

			// 思う (他)
			} else {
				return "思う";
			}

		// Group B
		} else {

			// 存じます 自口=1 or 自口=3 or 自女
			if ( nb6bushouref[i1st_BushouID].tone == 1 || nb6bushouref[i1st_BushouID].tone == 3 || nb6bushouname[i1st_BushouID].sex == 1 ) {
				return "存じます";

			// 思います (他)
			} else {
				return "思います";
			}

		}
	}

	return "％思います％";
}


// 〜参りました 〜参上しました 〜参った
string ％参りました％(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// 略記条件Ｔのどれがあてはまるか？ どれにもあてはまらなかったら略記条件Ｙでどちらかに当てはまる。
		int Group = ％略記条件Ｔ％(i1st_BushouID,i2nd_BushouID) ;

		// Group A
		if ( Group == 1 ) {

			// 参りました 自女 
			if ( nb6bushouname[i1st_BushouID].sex == 1 ) {
				return "参りました";

			// 参った (他)
			} else {
				return "参った";
			}

		// Group B
		} else {

			// 参りました 自女
			if ( nb6bushouname[i1st_BushouID].sex == 1 ) {
				return "参りました";

			// 参上しました 自口=1 or 自口=5
			} else if ( nb6bushouref[i1st_BushouID].tone == 1 || nb6bushouref[i1st_BushouID].tone == 5 ) {
				return "参上しました";

			// 参りました (他)
			} else {
				return "参りました";
			}

		}
	}

	return "％参りました％";
}



// 〜くだされ 〜下さい 〜くれ
string ％くだされ％(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// 略記条件Ｔのどれがあてはまるか？ どれにもあてはまらなかったら略記条件Ｙでどちらかに当てはまる。
		int Group = ％略記条件Ｔ％(i1st_BushouID,i2nd_BushouID) ;

		// Group A
		if ( Group == 1 ) {

			// 下さい 自女 
			if ( nb6bushouname[i1st_BushouID].sex == 1 ) {
				return "下さい";

			// くれ (他)
			} else {
				return "くれ";
			}

		// Group B
		} else {

			// 下さい 自女 
			if ( nb6bushouname[i1st_BushouID].sex == 1 ) {
				return "下さい";

			// くだされ (他)
			} else {
				return "くだされ";
			}

		}
	}

	return "％くだされ％";
}



// 〜すみません 〜申し訳ない 〜すまぬ 〜すまん 〜ごめんなさい
string ％すみません％(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// 略記条件Ｔのどれがあてはまるか？ どれにもあてはまらなかったら略記条件Ｙでどちらかに当てはまる。
		int Group = ％略記条件Ｔ％(i1st_BushouID,i2nd_BushouID) ;

		// Group A
		if ( Group == 1 ) {

			// ごめんなさい 自女 
			if ( nb6bushouname[i1st_BushouID].sex == 1 ) {
				return "ごめんなさい";

			// すまん 自口=2 || 自口=4
			} else if ( nb6bushouref[i1st_BushouID].tone == 2 || nb6bushouref[i1st_BushouID].tone == 4 ) {
				return "すまん";

			// すまぬ (他)
			} else {
				return "すまぬ";
			}

		// Group B
		} else {
			// 申し訳ない 自口=5 or 自男 
			if ( nb6bushouref[i1st_BushouID].tone == 5 || nb6bushouname[i1st_BushouID].sex == 0 ) {
				return "申し訳ない";

			// すみません (他） 
			} else {
				return "すみません";

			}
		}
	}

	return "％すみません％";
}

// 〜願います 〜お願いいたす 〜願おう 〜願う 〜お願いします
string ％願います％(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// 略記条件Ｔのどれがあてはまるか？ どれにもあてはまらなかったら略記条件Ｙでどちらかに当てはまる。
		int Group = ％略記条件Ｔ％(i1st_BushouID,i2nd_BushouID) ;

		// Group A
		if ( Group == 1 ) {

			// 願います 自女 
			if ( nb6bushouname[i1st_BushouID].sex == 1 ) {
				return "願います";

			// 願う 自口=2 || 自口=4
			} else if ( nb6bushouref[i1st_BushouID].tone == 2 || nb6bushouref[i1st_BushouID].tone == 4 ) {
				return "願う";

			// 願おう (他)
			} else {
				return "願おう";
			}

		// Group B
		} else {
			// お願します 自口=1 or 自口=3 or 自女 
			if ( nb6bushouref[i1st_BushouID].tone == 1 || nb6bushouref[i1st_BushouID].tone == 3 || nb6bushouname[i1st_BushouID].sex == 1 ) {
				return "お願いします";

			// お願いいたす 自口=5
			} else if ( nb6bushouref[i1st_BushouID].tone == 5 ) {
				return "ござった";

			// 願います (他） 
			} else {
				return "願います";

			}
		}
	}

	return "％願います％";
}



// 〜よい 〜よろしい
string ％よい％(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// 略記条件Ｔのどれがあてはまるか？ どれにもあてはまらなかったら略記条件Ｙでどちらかに当てはまる。
		int Group = ％略記条件Ｔ％(i1st_BushouID,i2nd_BushouID) ;

		// Group A
		if ( Group == 1 ) {

			// よろしい 自女 
			if ( nb6bushouname[i1st_BushouID].sex == 1 ) {
				return "よろしい";

			// よい (他)
			} else {
				return "よい";
			}

		// Group B
		} else {

			// よろしい 自口=1 or 自口=3 or 自女
			if ( nb6bushouref[i1st_BushouID].tone == 1 || nb6bushouref[i1st_BushouID].tone == 3 || nb6bushouname[i1st_BushouID].sex == 1 ) {
				return "よろしい";

			// よい (他)
			} else {
				return "よい";
			}

		}
	}

	return "％よい％";
}
