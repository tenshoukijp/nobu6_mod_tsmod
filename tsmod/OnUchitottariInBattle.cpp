#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "ScenarioMod.h"
#include "SoundServer.h"


/*
004316CA  |. 66:8B75 08     MOV     SI, WORD PTR SS:[EBP+8]
004316CE  |. 57             PUSH    EDI                              ; /Arg3
004316CF  |. 56             PUSH    ESI                              ; |Arg2
004316D0  |. 68 62100000    PUSH    1062                             ; |Arg1 = 00001062
004316D5  |. E8 A5AD0300    CALL    TENSHOU.0046C47F                 ; \TENSHOU.0046C47F
004316DA  |. 83C4 0C        ADD     ESP, 0C
004316DD  |. 50             PUSH    EAX
004316DE  |. 56             PUSH    ESI
004316DF  |. E8 638F0600    CALL    TENSHOU.0049A647
004316E4  |. 83C4 08        ADD     ESP, 8
004316E7  |. 56             PUSH    ESI                              ; /Arg3
004316E8  |. 57             PUSH    EDI                              ; |Arg2
004316E9  |. 68 68100000    PUSH    1068                             ; |Arg1 = 00001068
004316EE  |. E8 8CAD0300    CALL    TENSHOU.0046C47F                 ; \TENSHOU.0046C47F
004316F3  |. 83C4 0C        ADD     ESP, 0C
004316F6  |. 50             PUSH    EAX
004316F7  |. 57             PUSH    EDI
004316F8  |. E8 4A8F0600    CALL    TENSHOU.0049A647
004316FD  |. 83C4 08        ADD     ESP, 8


00431747  |. E8 068CFDFF    CALL    TENSHOU.0040A352
0043174C  |. 83C4 08        ADD     ESP, 8
0043174F  |. 5F             POP     EDI
00431750  |. 5E             POP     ESI
*/



/*
 */

int iUchitottariNoudouInBattleBushouID = -1; // (生け取った側)の武将ID
int iUchitottariJyudouInBattleBushouID = -1; // (生け取られた側)の武将ID

void OnTenshouExeUchitottariInBattleExecute() { 
	int iBushouID = iUchitottariNoudouInBattleBushouID-1; 
	int iTargetID = iUchitottariJyudouInBattleBushouID-1; 

	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM && 0 <= iTargetID && iTargetID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// position 	(隠居=0, 大名=1, 宿老=2, 家老=3, 部将=4, 侍大将=5, 足軽頭=6)
		// State		0大名,1軍長,2現役,3隠居,4浪人,5姫,6?,7死亡
		// 相手が、大名、宿老、家老、もしくは、軍団長
		// あるいは一流の戦場武将
		if ( nb6bushouname[iTargetID].position <= 3 || nb6bushouname[iTargetID].State == 0 || nb6bushouname[iTargetID].State == 1 ||
			( (nb6bushouref[iTargetID-1].maxbat + nb6bushouref[iTargetID-1].maxint) >= 160 )
		 ) {

			// 自制力だけの面子をあつめて、その全ての位置の中心点を取る。
			// さらにはやられた側の武将の位置も集める
			// その中心点から、実際のiBushouIDの武将への方向ベクトルを得て
			// ３～５ｍほどの距離へと収束させて音を鳴らす。

			// 現在戦場にいる面子
			vector<int> curMembers = GetCurMapSyutujinBushouArray();
			vector<int> posMembers;
			for ( vector<int>::iterator it = curMembers.begin(); it != curMembers.end(); it++ ) {
				// 自勢力の面子のみのリストとする。
				if ( nb6bushouref[*it].attach == nb6bushouref[iBushouID].attach ) {
					posMembers.push_back(*it);
				}
			}
			// iBushouIDをサイズ分だけ、いれておくことで、戦場に何人いようとも、該当キャラの位置にフォーカスが当たる。
			int size = posMembers.size();
			for ( int i=0; i<size; i++ ) {
				posMembers.push_back(iBushouID);
			}

			// 野戦
			if ( Is_FieldWar() ) {
				//
				HEX_POSITION my2DPos = GetFieldHexUnitPos(iBushouID); // 自分の位置
				D3D_POSITION my3DPos = FieldHexPos_To_WorldPos(my2DPos); // 自分の位置

				D3D_POSITION cnt3DPos(0,0,0); // (posMembers)全員の中心用
				
				// 全員の中心を足して
				for ( vector<int>::iterator it = posMembers.begin(); it != posMembers.end(); it++ ) {
					HEX_POSITION it2DPos = GetFieldHexUnitPos(*it);
					D3D_POSITION it3DPos = FieldHexPos_To_WorldPos(it2DPos);
					cnt3DPos.x += it3DPos.x;
					cnt3DPos.y += it3DPos.y;
					cnt3DPos.z += it3DPos.z;
				}
				// 人数で割ることで、中心点となる。
				if ( posMembers.size() > 0 ) {
					cnt3DPos.x /= posMembers.size();
					cnt3DPos.y /= posMembers.size();
					cnt3DPos.z /= posMembers.size();
				}

				D3D_VECTOR vDir = my3DPos-cnt3DPos; // 中心→自分へのベクトルを求める。
				vDir /= 3;
				if ( vDir.length() >= 3 ) {
					vDir.normalize(); // 単位ベクトル化して、長さ３までに縮める
					vDir *= 3;
				}

				// ３種類用意しているのでランダムで。言う確率も1/2
				int irand = rand() % 6;
				if ( irand == 1 ) {
					// 「敵将、打ち取ったり」の効果音を空間再生。
					PlaySound3DResID( 751, CommonGlobalHandle::hResourceWavMod, false , vDir.x, vDir.y, vDir.z,  0, 0, 0 );
				} else if ( irand == 2 ) {
					// 「敵将、打ち取ったり」の効果音を空間再生。
					PlaySound3DResID( 751, CommonGlobalHandle::hResourceWavMod2, false , vDir.x, vDir.y, vDir.z,  0, 0, 0 );
				} else if ( irand == 3 ) {
					// 「敵将、打ち取ったり」の効果音を空間再生。
					PlaySound3DResID( 751, CommonGlobalHandle::hResourceWavMod3, false , vDir.x, vDir.y, vDir.z,  0, 0, 0 );
				}

			// 攻城戦(完全に同じアルゴリズム。場所がCastle用関数なだけ。)
			} else if ( Is_CastleWar() ) {
				//
				HEX_POSITION my2DPos = GetCastleHexUnitPos(iBushouID); // 自分の位置
				D3D_POSITION my3DPos = CastleHexPos_To_WorldPos(my2DPos); // 自分の位置

				D3D_POSITION cnt3DPos(0,0,0); // (posMembers)全員の中心用
				
				// 全員の中心を足して
				for ( vector<int>::iterator it = posMembers.begin(); it != posMembers.end(); it++ ) {
					HEX_POSITION it2DPos = GetCastleHexUnitPos(*it);
					D3D_POSITION it3DPos = CastleHexPos_To_WorldPos(it2DPos);
					cnt3DPos.x += it3DPos.x;
					cnt3DPos.y += it3DPos.y;
					cnt3DPos.z += it3DPos.z;
				}

				if ( posMembers.size() > 0 ) {
					// 人数で割ることで、中心点となる。
					cnt3DPos.x /= posMembers.size();
					cnt3DPos.y /= posMembers.size();
					cnt3DPos.z /= posMembers.size();
				}
				D3D_VECTOR vDir = my3DPos-cnt3DPos; // 中心→自分へのベクトルを求める。
				vDir /= 3;
				if ( vDir.length() >= 3 ) {
					vDir.normalize(); // 単位ベクトル化して、長さ３までに縮める
					vDir *= 3;
				}

				// ３種類用意しているのでランダムで。言う確率も1/2
				int irand = rand() % 6;
				if ( irand == 1 ) {
					// 「敵将、打ち取ったり」の効果音を空間再生。
					PlaySound3DResID( 751, CommonGlobalHandle::hResourceWavMod, false , vDir.x, vDir.y, vDir.z,  0, 0, 0 );
				} else if ( irand == 2 ) {
					// 「敵将、打ち取ったり」の効果音を空間再生。
					PlaySound3DResID( 751, CommonGlobalHandle::hResourceWavMod2, false , vDir.x, vDir.y, vDir.z,  0, 0, 0 );
				} else if ( irand == 3 ) {
					// 「敵将、打ち取ったり」の効果音を空間再生。
					PlaySound3DResID( 751, CommonGlobalHandle::hResourceWavMod3, false , vDir.x, vDir.y, vDir.z,  0, 0, 0 );
				}


			}
		} 
	}

} 
/* 
004316E7  |. 56             PUSH    ESI                              ; /Arg3	← 攻撃された側(打ち取られた側)
004316E8  |. 57             PUSH    EDI                              ; |Arg2	← 攻撃した側
004316E9  |. 68 68100000    PUSH    1068                             ; |Arg1 = 00001068
004316EE  |. E8 8CAD0300    CALL    TENSHOU.0046C47F                 ; \TENSHOU.0046C47F
004316F3  |. 83C4 0C        ADD     ESP, 0C

004316F6  |. 50             PUSH    EAX
004316F7  |. 57             PUSH    EDI
004316F8  |. E8 4A8F0600    CALL    TENSHOU.0049A647
004316FD  |. 83C4 08        ADD     ESP, 8

*/ 
int pTenshouExeJumpFromToOnTenshouExeUchitottariInBattle    =0x4316F8; 
int pTenshouExeJumpCallFromToOnTenshouExeUchitottariInBattle=0x49A647; 
int pTenshouExeReturnLblFromOnTenshouExeUchitottariInBattle =0x4316FD; 
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeUchitottariInBattle() { 
	__asm { 
		mov iUchitottariNoudouInBattleBushouID, edi
		mov iUchitottariJyudouInBattleBushouID, esi

		// TENSHOU.EXE内のcall
		call pTenshouExeJumpCallFromToOnTenshouExeUchitottariInBattle 

		push eax 
		push ebx 
		push ecx 
		push edx 
		push esp 
		push ebp 
		push esi 
		push edi 
	} 

	OnTenshouExeUchitottariInBattleExecute(); 

	__asm { 
		pop edi 
		pop esi 
		pop ebp 
		pop esp 
		pop edx 
		pop ecx 
		pop ebx 
		pop eax 

		jmp pTenshouExeReturnLblFromOnTenshouExeUchitottariInBattle 
	} 
} 
char cmdOnTenshouExeJumpFromUchitottariInBattle[6]="\xE9"; 
void WriteAsmJumperOnTenshouExeUchitottariInBattle() { 
	int iAddress = (int)OnTenshouExeUchitottariInBattle; 
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeUchitottariInBattle + 5 ) ; 
	memcpy(cmdOnTenshouExeJumpFromUchitottariInBattle+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。 
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeUchitottariInBattle), cmdOnTenshouExeJumpFromUchitottariInBattle, 5, NULL); // 一応安全のため 
} 


