#include "WinTarget.h"
#include <stdlib.h>
#include <string.h>

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"




/*
 大砲の攻撃回数を上書き決定するアルゴリズム
 */

// 以下は大砲である。
char szIsTaihouItems[][14] = {
	"カルバリン砲",
	"セーカー砲",
	"ランタカ砲",
	"仏狼機",
	"台車砲",
	"国崩し",
	"火矢筒",
	"臼砲",
	"芝辻砲",
	"雷破山",
};

// 以下は大砲ではない
char szNoTaihouItems[][14] = {
	// 以下は大砲でなく銃の名前
	"仙台筒",
	"備前筒",
	"二連発手中筒",
	"国友筒",
	"堺筒",
	"日野筒",
	"朝鮮短筒",
	"種子島筒",
	"篭筒",
	"籠筒",
	"米沢筒",
	"紀州筒",
	"腰指筒",
	"薩摩筒",
	"阿波筒",
	"馬上筒",

	// 以下は大砲でなく花入れ
	"大内筒",

	// 以下は大砲でなく茶器
	"匹田筒",

	// これは置時計の名前
	"時砲",   
};


int IsItemTaihou(char *szItemName) {

	// 大砲か大砲でないかの辺値結果の格納場所
	int iReturnResult = 0;

	// 全角２文字以上の時、一番最後の全角文字が｢鉄砲｣の字であれば、大砲ではなく鉄砲である
	if ( strlen(szItemName) >= 4) {
		char *pszLast2Character = szItemName + strlen(szItemName) - 4;

		// 一番最後が鉄砲という文字列で終わっていたら大砲ではない
		if ( strcmp(pszLast2Character, "鉄砲")==0 ) {
			return 0;
		}
	}


	// 大砲アイテムをなめて、その文字列が含まれていたら見る
	for ( int ix = 0; ix <sizeof(szIsTaihouItems)/sizeof(szIsTaihouItems[0]); ix++ ) {
		// 大砲アイテムと完全に合致したら、大砲
		if ( strstr(szItemName, szIsTaihouItems[ix] ) ) {
			return 1;
		}
	}

	// 一番最後の全角文字１文字を抜き出す
	char *pszLastCharacter = szItemName + strlen(szItemName) - 2;

	// 最後の文字が｢"砲"」もしくは「"筒"｣なら大砲とみなす。
	if (strcmp("砲", pszLastCharacter) == 0 || strcmp("筒", pszLastCharacter) == 0 ){
		iReturnResult = 1;
	}

	// しかし、そのようなものであっても、大砲ではないアイテムをなめて、その文字列が含まれていたら大砲ではない
	for ( int ix = 0; ix <sizeof(szNoTaihouItems)/sizeof(szNoTaihouItems[0]); ix++ ) {
		// 大砲ではないアイテムと完全に合致したら、大砲ではない
		if ( strstr(szItemName, szNoTaihouItems[ix] ) ) {
			return 0;
		}
	}

	return iReturnResult;
}

// アイテムの名前から大砲のレベルを得る。大砲でなければ０．大砲であれば１、数字付きであれば、２以上の値が返ってくる。
int getItemTaihouCnt(char *szItemName) {
	// 大砲でなければ、カウントは当然0である。
	if ( !IsItemTaihou(szItemName) ) { return 0; }

	char szCanonCount[][3] = {"一", "二", "三", "四", "五", "六", "七", "八", "九"};

	// 何も合致しなければ、通常の大砲であったということで、1相当。これを２連砲として使うのは、大砲部分のOnプログラムにゆだねる。
	int iReturnCanonCnt = 1;

	// 三～九までのどれかの数字が含まれていれば、その値を返す。
	for ( int iCnnCnt=1; iCnnCnt < sizeof(szCanonCount)/sizeof(szCanonCount[0]); iCnnCnt++ ) {

		// 該当の全角漢数字が含まれていたら、その値を返す
		if (strstr(szItemName, szCanonCount[iCnnCnt])) {
			iReturnCanonCnt = iCnnCnt+1;
			break;
		}
	}

	// ｢改｣という名前が付く場合は、さらに＋１カウントされる。
	if ( strstr(szItemName, "改") ) {
		if ( iReturnCanonCnt <= 2 ) { 
			iReturnCanonCnt = 3;
		} else {
			iReturnCanonCnt++;
		}

	}

	// 大砲であれば…
	return iReturnCanonCnt;
}



//----------------------------------------------------------------------------------------------------------

// 該当武将が大砲を持っているかどうか。
int isCurBushouHasTaihou(int iBushouID) {

	int iMaxTaihouLv = 0;

	// 家宝を一通り調べる。大砲を持っているかどうか、持っていたら最大となる大砲を探す。
	for (int iKahouID=0; iKahouID<GAMEDATASTRUCT_KAHOU_NUM; iKahouID++) {

		// 所有者の武将IDが一致する場合のみ判定の対象
		if (nb6kahou[iKahouID].attach-1 == iBushouID) {

			// 該当アイテムの大砲レベルを探る
			int iTaihouLv =  getItemTaihouCnt(nb6kahou[iKahouID].name);

			// それが既存の発見した大砲を上回っていれば、それを新たな所持大砲レベルとする。
			if ( iMaxTaihouLv < iTaihouLv ) { iMaxTaihouLv = iTaihouLv; }
		}
	}
	// 
	return iMaxTaihouLv;
}


// 大砲をうとうとしている武将のID
int iTaihouAttackTimesRewriteBushouID = -1;

// 大砲の発射回数
int iTaihouAttackTimesRewriteCanonFireCnt = 1;

void OnTenshouExeTaihouAttackTimesRewriteExecute() {

	// 大砲の発射回数は毎回1でリセットする。
	iTaihouAttackTimesRewriteCanonFireCnt = 1;

	// 武将が一定の範囲であること
	if (0 <= iTaihouAttackTimesRewriteBushouID-1 && iTaihouAttackTimesRewriteBushouID-1 < GAMEDATASTRUCT_BUSHOU_NUM) {
		
		// 大砲持ちかどうか、また大砲持ちであれば発射回数も
		int iTaihouCnt = isCurBushouHasTaihou( iTaihouAttackTimesRewriteBushouID-1 );

		// 大砲を所持してる武将である場合
		if ( iTaihouCnt >= 1 ) {

			// デフォルトの大砲の場合は、発射回数を２回とする。
			if ( iTaihouCnt == 1 ) { iTaihouCnt = 2; } 

			// アイテムの大砲レベルが既存設定より大きい場合は、それを発射回数とする。
			if ( iTaihouCnt > iTaihouAttackTimesRewriteCanonFireCnt ) {
				iTaihouAttackTimesRewriteCanonFireCnt = iTaihouCnt;
			}
		}
	}

}


/*
00432B3B     B8 01000000    MOV     EAX, 1									大砲の回数		大砲の回数が３にすると３回となる。
				└ここを JMP TSMod.OnTSExeTaihouAttackTimesRewrite と書き換えてやる実際の処理
00432B40  |> 85C0           TEST    EAX, EAX											
 */
int pTenshouExeJumpFromToOnTenshouExeTaihouAttackTimesRewrite    =0x432B3B; // 関数はこのアドレスから、OnTenshouExeTaihouAttackTimesRewriteへとジャンプしてくる。
int pTenshouExeReturnLblFromOnTenshouExeTaihouAttackTimesRewrite =0x432B40; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeTaihouAttackTimesRewrite() {
	// スタックにためておく
	__asm {

		// 大砲をうとうとしている武将のID
		mov iTaihouAttackTimesRewriteBushouID, esi;

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}
	
	// 
	OnTenshouExeTaihouAttackTimesRewriteExecute();

	// スタックに引き出す
	__asm {
		pop edi
		pop esi
		pop ebp
		pop esp
		pop edx
		pop ecx
		pop ebx
		pop eax

		// 元々TENSHOU.EXE内にあったものをここで
		// 発射回数を計算によるもので上書き。
		mov eax, iTaihouAttackTimesRewriteCanonFireCnt

		jmp pTenshouExeReturnLblFromOnTenshouExeTaihouAttackTimesRewrite
	}
}

/*
00432B3B     B8 01000000    MOV     EAX, 1									大砲の回数		大砲の回数が３にすると３回となる。
				└ここを JMP TSMod.OnTSExeTaihouAttackTimesRewrite と書き換えてやる実際の処理
00432B40  |> 85C0           TEST    EAX, EAX											
*/
char cmdOnTenshouExeJumpFromTaihouAttackTimesRewrite[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeTaihouAttackTimesRewrite() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeTaihouAttackTimesRewrite;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeTaihouAttackTimesRewrite + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeTaihouAttackTimesRewrite  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromTaihouAttackTimesRewrite+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeTaihouAttackTimesRewrite), cmdOnTenshouExeJumpFromTaihouAttackTimesRewrite, 5, NULL); //5バイトのみ書き込む
}




