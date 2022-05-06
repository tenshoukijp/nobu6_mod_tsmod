#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "TSModCommand.h"
#include "ScenarioMod.h"

/*
 とあるユニットの移動が終了した時。ＰＣユニット、ＮＰＣユニット共通
 */


/*
00448B5F   E8 8D24FEFF      CALL    TENSHOU.0042AFF1		// ユニットを移動しおわった場所の「ＨＥＸ役割」をEAXに入れている。
00448B64   83C4 04          ADD     ESP, 4
00448B67  |. 83F8 1B        CMP     EAX, 1B					// EAXが城本丸かどうか。
00448B6A  |. 75 2C          JNZ     SHORT TENSHOU.00448B98
00448B6C   56               PUSH    ESI

*/

extern int iCheckRojoBushouNum; // 籠城側の城内に居る人数
extern int iNextTurnCastleID;   // 表示籠城戦中の城番号(+1の方)

int isUnitMoveEndRakujoCondition = 0;	// ショートカット落城条件を満たす。
int iUnitMoveEndBushou = -1;
int iUnitMoveEndNum = -1;
void OnTenshouExeUnitMoveEndExecute() {
	isUnitMoveEndRakujoCondition = 0; // 毎度満たさないで上書きする。
	
	if ( 0 <= iUnitMoveEndBushou-1 && iUnitMoveEndBushou-1 < GAMEDATASTRUCT_BUSHOU_NUM ) {
		iUnitMoveEndBushou = iUnitMoveEndBushou - 1; // iBushouID (-1した武将配列の添え字用の番号としておく)
	}


	// ショートカット落城の値が有効である。
	if ( TSModCommand::AllCastle::cmdShortCutFallCastle ) {
		// 攻城戦中である。(そして、籠城側の人数は０である。← 見ないこととした。)
		if ( Is_CastleWar() /* && iCheckRojoBushouNum==0*/ ) {
			/*
			出陣している、全ての武将を得る関数
			GetSyutujinBushouArray() のリストを
			籠城中の城に所属している武将から引くと、元々出陣していない武将のリストが出る。
			*/
			// 攻められている城
			int iCastleID = iNextTurnCastleID-1;
			int iJokakuValue = 50;
			if ( 0 <= iCastleID && iCastleID < GAMEDATASTRUCT_CASTLE_NUM ) {
				iJokakuValue = nb6castle[iCastleID].build;
			}

			// 攻められている城に所属していて、出陣していない者の武将一覧
			vector<int> vCatstleBushouArray;
			for ( int iBushouID = 0; iBushouID < GAMEDATASTRUCT_BUSHOU_NUM; iBushouID++ ) {
				// 該当の城に所属するならば…
				if ( nb6bushouref[iBushouID].castle-1 == iCastleID ) {
					// マップ全体で出陣している武将
					vector<int> vSyutujinBushouArray = GetSyutujinBushouArray();

					bool isSyutujinZumi = false; // 城に所属しているが現在どこかに出陣しているか？
					for ( vector<int>::iterator it = vSyutujinBushouArray.begin(); it != vSyutujinBushouArray.end(); it++ ) {
						if ( iBushouID == *it ) {
							isSyutujinZumi = true;
						}
					}
					// 出陣済みでない場合だけ加える
					if ( !isSyutujinZumi ) {
						vCatstleBushouArray.push_back(iBushouID);
					}
				}
			}

			// 城にこもっている武将の内情を探る。
			int iSumDefenceSoldier = 0; // 兵数の合計
			for ( vector<int>::iterator it = vCatstleBushouArray.begin(); it != vCatstleBushouArray.end(); it++ ) {
				iSumDefenceSoldier += nb6bushouref[*it].soldier;
			}

	
			// 現在この籠城マップに実際にユニット出陣している武将リスト
			vector<int> vCurMapSyutujinBushouArray = GetCurMapSyutujinBushouArray();

			int iShozokuDaimyo = -1;
			int iSumAttackSoldier = 0; // 兵数の合計
			for ( vector<int>::iterator it = vCurMapSyutujinBushouArray.begin(); it != vCurMapSyutujinBushouArray.end(); it++ ) {
				int iBushouID = *it;

				if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
					// 1発目。最初の人で所属大名を求める。
					if (iShozokuDaimyo == -1) {
						iShozokuDaimyo = nb6bushouref[iBushouID].attach;
					}
					// 他の人全員が最初の人の大名と同じであれば、落城したと見なす。
					// 一致しなければ、落城したとは見なさない。所属大名に-2入れておく。
					else if (iShozokuDaimyo != nb6bushouref[iBushouID].attach ) {
						iShozokuDaimyo = -2;
						break;
					}
					// 武将の兵数の合計を換算しておく
					iSumAttackSoldier += nb6bushouref[iBushouID].soldier;
				}
			}

			// 所属大名が０以上の値として残っていれば、現在表示中の籠城戦マップに居る面子が同じ大名下だということ。
			// よって、落城したと見なす。
			// (攻撃側の兵数はその３倍以上はある。もしくは敵側籠城者０) かつ、攻撃側は城郭値の1/10は最低でも兵力を有していること。
			if ( iShozokuDaimyo >= 0 && ((iSumDefenceSoldier*3 < iSumAttackSoldier) || iCheckRojoBushouNum==0 ) && (iSumAttackSoldier*10 > iJokakuValue) ) {

				int iBushouID = iUnitMoveEndBushou;
				HEX_POSITION my2DPos = GetCastleHexUnitPos(iBushouID); // 自分の位置
				int iRole = GetCastleHexRole(my2DPos); // その部分の役割
				D3D_POSITION my3DPos = CastleHexPos_To_WorldPos(my2DPos); // 自分の位置(3D版)

				// 城内に入っていれば(城内確約は高さが２以上)、もしくは、高さが１以上だが、明らかに城内に居る。(高さが１だけ判定だと、水に入っただけででも高さ１となってしまうため)
				if ( (my3DPos.y >= 2) || (my3DPos.y >= 1 && (iRole==CASTLE_HEX_ROLE::城内整地||iRole==CASTLE_HEX_ROLE::城内平地||iRole==CASTLE_HEX_ROLE::櫓)) ) {
					isUnitMoveEndRakujoCondition = 1;
				}
			}
		}
	}

	if ( iUnitMoveEndNum == CASTLE_HEX_ROLE::本丸 ) { 
		isUnitMoveEndRakujoCondition = 1; // 無条件で満たす。
	}

	/*
	// ScenarioModのカスタム条件で戦争::移動後落城可否を設定する
	if ( Is_CastleWar() ) {
		if ( p_snOnCustomCondition ) {
			int iDefenceCastleID = iNextTurnCastleID;	  // 対象の城の番号(+1の方)
			int iMovedBushouID  = iUnitMoveEndBushou + 1; // 行動をした武将番号(+1の方)

			// 落城条件の成否																							┌実行者	     ┌対象の城	    ┌現在予定されている落城の可否の結果 
			int isCastleFallDownResult = ((PFNSNONCUSTOMCONDITION) p_snOnCustomCondition)("AS条件::戦争::移動後落城可否", iMovedBushouID+1, iDefenceCastleID+1, isUnitMoveEndRakujoCondition, "", "", 0.0, 0.0);
			if ( isCastleFallDownResult != CUSTOM_CONDITION_NOMODIFY ) {

				// 落城条件を上書きする。
				isUnitMoveEndRakujoCondition = isCastleFallDownResult;
			}
		}
	}
	*/
} 
/* 
00448B5F   E8 8D24FEFF      CALL    TENSHOU.0042AFF1		// ユニットを移動しおわった場所の「ＨＥＸ役割」をEAXに入れている。
00448B64   83C4 04          ADD     ESP, 4
00448B67  |. 83F8 1B        CMP     EAX, 1B					// EAXが城本丸かどうか。
00448B6A  |. 75 2C          JNZ     SHORT TENSHOU.00448B98
00448B6C   56               PUSH    ESI
*/ 
int pTenshouExeJumpFromToOnTenshouExeUnitMoveEnd    =0x448B5F; 
int pTenshouExeJumpCallFromToOnTenshouExeUnitMoveEnd=0x42AFF1; 
int pTenshouExeReturnLblFromOnTenshouExeUnitMoveEnd =0x448B6A; 
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeUnitMoveEnd() { 
	__asm { 

		// TENSHOU.EXE内のcall
		call pTenshouExeJumpCallFromToOnTenshouExeUnitMoveEnd

		ADD     ESP, 4

		mov iUnitMoveEndNum, eax  // 移動しをえた場所の役割番号を保存しておく。
		mov iUnitMoveEndBushou, esi // 武将ＩＤを保存しておく。

		push eax 
		push ebx 
		push ecx 
		push edx 
		push esp 
		push ebp 
		push esi 
		push edi 
	} 

	OnTenshouExeUnitMoveEndExecute(); 

	__asm { 
		pop edi 
		pop esi 
		pop ebp 
		pop esp 
		pop edx 
		pop ecx 
		pop ebx 
		pop eax 

		CMP     isUnitMoveEndRakujoCondition, 1			// 比較だけしておいてフラグレジスタに値格納。
		jmp pTenshouExeReturnLblFromOnTenshouExeUnitMoveEnd  // 使わずジャンプ
	} 
} 
char cmdOnTenshouExeJumpFromUnitMoveEnd[6]="\xE9"; 
void WriteAsmJumperOnTenshouExeUnitMoveEnd() { 
	int iAddress = (int)OnTenshouExeUnitMoveEnd; 
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeUnitMoveEnd + 5 ) ; 
	memcpy(cmdOnTenshouExeJumpFromUnitMoveEnd+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。 
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeUnitMoveEnd), cmdOnTenshouExeJumpFromUnitMoveEnd, 5, NULL); // 一応安全のため 
} 


