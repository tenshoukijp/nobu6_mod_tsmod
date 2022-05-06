#include "WinTarget.h"
#include <fstream>

#include "CommonGlobalHandle.h"
#include "GameDataStruct.h"
#include "TSModCommand.h"
#include "ScenarioMod.h"

using namespace std;

int iCurBushouSoldier = -1;	// 現在フォーカスとなっている武将の(最大可能)兵数
short * pCurBushouID = 0L;	// 現在フォーカスとなっている武将IDへのポインタ
short iCurBushouID = -1;	// 現在フォーカスとなっている武将IDへのポインタ




NB6SOLDIERMAX_CUSTOM nb6soldiermax_custom[GAMEDATASTRUCT_SOLDIERMAX_NUM] = {
	0,   // 隠居
	100, // 大名
	100, // 宿老
	85,  // 家老
	70,  // 部将
	60,  // 侍大将
	50,  // 足軽頭
};


// 元の各武将の最大兵士数を、忍者・官位・役職などを含めてカスタムした値を返す関数。
// @param:
//		iTargetBushouId:		ターゲット武将のID, 
//		iCurOrgBushouSoldierMax:ターゲット武将の元々の兵士最大数
int getCustomSoldierMax(int iTargetBushouID, int iTargetBushouOrgSoldierMax) {

	int iTargetBushouDstSoldierMax = iTargetBushouOrgSoldierMax;
	int iBackUpBushouDstSoldierMax = iTargetBushouDstSoldierMax;

	// 対象の武将に官位や役職があれば、最大兵士数に影響する
	if (TSModCommand::AllBushou::cmdPositionInfluenceSoldierMax != 0) {

		// 元々の兵力は20下げた状態で始める
		iTargetBushouDstSoldierMax -= 20;
		iBackUpBushouDstSoldierMax = iTargetBushouDstSoldierMax;

		// 該当の武将が官位があるかどうかを検索する
		for ( int iKaniID=0; iKaniID<GAMEDATASTRUCT_KANI_NUM; iKaniID++) {

			// 該当の武将が官位をもっていれば、(官位の中身の数値はプロパティーなので、iTargetBushouID+1)
			if (nb6kani[iKaniID].attach == iTargetBushouID+1 ) {
				// 官位の上昇値と同じ値だけ、最大兵士数が増える
				iTargetBushouDstSoldierMax += nb6kani[iKaniID].param;
				break;
			}
		}
 
		// 該当の大名に役職があるかどうかを検索する
		for ( int iDaimyoID=0; iDaimyoID<GAMEDATASTRUCT_DAIMYO_NUM; iDaimyoID++) {

			// 該当の軍団(長)が役職を持っているならば
			if ( nb6daimyo[iDaimyoID].position ) {

				// 該当の武将が官位をもっていれば、(官位の中身の数値はプロパティーなので、iTargetBushouID+1)
				if (nb6daimyo[iDaimyoID].attach == iTargetBushouID+1 ) {
					// 役職のIDを11から引いた数だけ、最大兵士数が増える
					// 大名IDから正規化された役職IDを得る

					iTargetBushouDstSoldierMax += (11 - getNormalizedDaimyoPosition(iDaimyoID) );
					break;
				}
			}
		}

		// 該当の武将に役職があるかどうかを検索する
		for ( int iGundanID=0; iGundanID<GAMEDATASTRUCT_GUNDAN_NUM; iGundanID++) {

			// 該当の軍団が存在するならば
			if ( nb6gundan[iGundanID].residence != 0xFFFF ) {

				// 該当の武将が官位をもっていれば、(官位の中身の数値はプロパティーなので、iTargetBushouID+1)
				if (nb6gundan[iGundanID].leader == iTargetBushouID+1 ) {

					// 有効な官位を持っていれば…
					if ( int(nb6gundan[iGundanID].position) > 0 ) {
						// 役職のIDを11から引いた数だけ、最大兵士数が増える
						iTargetBushouDstSoldierMax += (11 - getNormalizedGundanPosition(iGundanID) );
						break;
					}
				}
			}
		}

	}

	// 対象の武将が忍者であれば… この項目は割合で下げるので最後に行うこと。
	if (nb6bushouref[iTargetBushouID].job == 0x10) { // 0x10==忍者 GameDataStruct.hを見よ

		// 忍者は影たるものであるべきのコマンドが有効でLv==1ある場合…
		if (TSModCommand::AllBushou::cmdNinjaMustBeAShadow == 1 ) {
			iTargetBushouDstSoldierMax = iTargetBushouDstSoldierMax / 2;  // 兵力は元来の1/2とする。

		// 忍者は影たるものであるべきのコマンドが有効でLv==2ある場合…
		} else if (TSModCommand::AllBushou::cmdNinjaMustBeAShadow == 2 ) {
			iTargetBushouDstSoldierMax = iTargetBushouDstSoldierMax / 3;  // 兵力は元来の1/3とする。

		// 忍者は影たるものであるべきのコマンドが有効でLv==3ある場合…
		} else if (TSModCommand::AllBushou::cmdNinjaMustBeAShadow == 3 ) {
			iTargetBushouDstSoldierMax = iTargetBushouDstSoldierMax / 5;  // 兵力は元来の1/5とする。

			// 忍者は影たるものであるべきのコマンドが有効でLv==4ある場合…
		} else if (TSModCommand::AllBushou::cmdNinjaMustBeAShadow == 4 ) {
			iTargetBushouDstSoldierMax = iTargetBushouDstSoldierMax / 10;  // 兵力は元来の1/10とする。
		}
	}

	// あくまで100をMAXとする。全体として10下がっているのでこれでもまぁまぁ意味が出る。
	if (iTargetBushouDstSoldierMax > int(nb6parammax.soldier)) {
		iTargetBushouDstSoldierMax = int(nb6parammax.soldier);
	}

	// ScenarioModによる再修正
	if (p_snOnRequestSoldierMax) {

		// ScenarioModに再修正を問い合わせ
		int ret = p_snOnRequestSoldierMax(iTargetBushouID, iBackUpBushouDstSoldierMax, iTargetBushouDstSoldierMax);
		if (ret != -1) {
			iTargetBushouDstSoldierMax = ret;
		}
	}

	return iTargetBushouDstSoldierMax;

}

// 外部参照用
int WINAPI Extern_getCustomSoldierMax(int iBushouID) {

	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		//　現在の身分を得る
		int position = nb6bushouname[iBushouID].position;

		//　身分から最大兵数を得る
		int soldiermax = nb6soldiermax[position].soldiermax;

		// 最大兵士数は、忍者だったり官位・役職等によってフィルターがかかる
		soldiermax = getCustomSoldierMax( iBushouID, soldiermax );

		return soldiermax;
	}

	return 0;
}



// (可能最大)兵士数に対して、特殊なフィルターを(その都度)設定する。
void checkEXFilterOfSoldierMax() {
	iCurBushouID =  *(pCurBushouID+4);
	iCurBushouID = iCurBushouID - 1;  // 実際の番号は1引いた値(厳密には、配列で使用するのは-1の値、各種プロパティに入っている値はこのままの値)

	// ここでフィルターをかける
	iCurBushouSoldier = getCustomSoldierMax(iCurBushouID, iCurBushouSoldier);
}

/*
0040BFD8  /$ FF7424 04      PUSH    DWORD PTR SS:[ESP+4]			←この時のESPの先に武将のINDEX(タルタロスなら189=BD)が入っている。
0040BFDC  |. E8 F7E3FFFF    CALL    TENSHOU.0040A3D8
				└ここを JMP TSMod.OnTSExeBushouSoldierMax とするのが目的　そして、元々この行にあった 内容は関数内で処理
0040BFE1  |. 0FB70445 A0B74>MOVZX   EAX, WORD PTR DS:[EAX*2+4BB7A0]	←ここのEAXが最大兵士数
0040BFE9  |. 83C4 04        ADD     ESP, 4
0040BFEC  \. C3             RETN
*/

int pTenshouExeJumpFromToOnTenshouExeBushouSoldierMax    =0x40BFDC; // 関数はこのアドレスから、OnTenshouExeBushouSoldierMaxへとジャンプしてくる。
int pTenshouExeReturnLblFromOnTenshouExeBushouSoldierMax =0x40BFE9; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
int pTenshouExeCallLblFromOnTenshouExeBushouSoldierMax   =0x40A3D8; // TENSHOU.EXE内にある｢身分｣から兵数を得る関数
// この関数はTENSHOU.EXEが武将の兵士数を変更しようとするタイミングで、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeBushouSoldierMax() {
	// スタックにためておく
	__asm {
		// espはポインタ。指すのは武将ID(厳密には武将IDに１足したものの模様)
		mov pCurBushouID, esp

		// tenshou.exeで元からあった処理
		call pTenshouExeCallLblFromOnTenshouExeBushouSoldierMax
		movzx eax, word ptr ds:[eax*2+4BB7A0h]

		// eaxが最大可能兵数となるので、この数値を一端保存する
		mov iCurBushouSoldier, eax

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}
	
	// 処理を行う
	checkEXFilterOfSoldierMax();

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

		mov eax, iCurBushouSoldier

		jmp pTenshouExeReturnLblFromOnTenshouExeBushouSoldierMax
	}
}

/*
0040BFD8  /$ FF7424 04      PUSH    DWORD PTR SS:[ESP+4]     ←この時のESPの先に武将のINDEX(タルタロスなら189=BD)が入っている。
0040BFDC  |. E8 F7E3FFFF    CALL    TENSHOU.0040A3D8
				└ここを JMP TSMod.OnTSExeBushouSoldierMax とするのが目的　そして、元々この行にあった 内容は関数内で処理
0040BFE1  |. 0FB70445 A0B74>MOVZX   EAX, WORD PTR DS:[EAX*2+4BB7A0]	←ここのEAXが最大兵士数
0040BFE9  |. 83C4 04        ADD     ESP, 4
0040BFEC  \. C3             RETN
*/
char cmdOnTenshouExeJumpFromBushouSoldierMax[6]="\xE9";
// 元の命令が6バイト、以後の関数で生まれる命令が合計５バイトなので、NOPが最後に付け足されている。

typedef void (WINAPI *PFNONTENSHOUEXEBUSHOUSOLDIERMAX)();
PFNONTENSHOUEXEBUSHOUSOLDIERMAX iOnTenshouExeBushouSoldierMax;

// 兵士数が変更されようとした時に、呼び出される
void WriteAsmJumperOnTenshouExeBushouSoldierMax() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeBushouSoldierMax;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeBushouSoldierMax + 5 );
		// ５というのは、0040BFDC  -E9 ????????  JMP TSMod.OnTSExeBushouSoldierMax  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromBushouSoldierMax+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeBushouSoldierMax), cmdOnTenshouExeJumpFromBushouSoldierMax, 5, NULL); // 5バイトのみ書き込む 命令が5バイト。
}

