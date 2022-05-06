#include "WinTarget.h"

#include "TSModCommand.h"
#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXBushou_All.h"

/*
 * 大砲の距離（実際には適正値）適正をセットする。
 * 弓隊で下げてしまった基本距離をここであげなおすのだ。
 */


extern int isBushouIDJudgeRemoteAttack; // リモート攻撃可能かどうか判定対象となる武将番号

int iCanTaihouAndTekkousenDistance = -1;
int iCanTaihouAndTekkousenDistanceCompareVariable = -1;
void OnTenshouExeSetCanTaihouAndTekkousenDistanceExecute() {
	// 鉄砲適正と大砲マス距離の関係
	/*
	S 5
	A 4
	B 3
	C 3
	D 2
	E 2
	*/
	int TaihouDistanceArray[] = {2,2,3,3,4,5,5,6,6,7,7}; // 鉄砲適正と大距離枡目の関係配列
	int iBushouID = (isBushouIDJudgeRemoteAttack & 0xFFFF)-1;

	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
		// 鉄砲の適正を得る
		int iTeppouAptitude = getBushouAptitude( iBushouID, APTITUDE_TEPPOU );

		// 野戦の場合
		if ( Is_FieldWar() ) {

			// 対象武将が(城に居れば)位置を得る。
			HEX_POSITION p = GetFieldHexUnitPos( iBushouID );
			// 武将の位置の役割を得る
			int iHexRole = GetFieldHexRole( p );

			// 大砲を持っているか… もしくは、鉄甲船を持ってて海の上
			if ( (nb6bushouref[iBushouID].biggun) || (nb6bushouref[iBushouID].steelship && (iHexRole==FIELD_HEX_ROLE::海||iHexRole==FIELD_HEX_ROLE::湖) ) ) {

				if ( nb6bushouref[iBushouID].form == 0 && nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi >= 3) { // 足軽で弓がＢ以上であれば…
					// 鉄砲適正から、大砲距離マス目を取得
					iCanTaihouAndTekkousenDistance = TaihouDistanceArray[iTeppouAptitude]; // 
				}

				// 弓騎馬の時
				else if ( nb6bushouref[iBushouID].form == 1 && nb6bushouref[iBushouID].aptitudekib >= 3 && nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi >= 3) {
					// 鉄砲適正から、大砲距離マス目を取得
					iCanTaihouAndTekkousenDistance = TaihouDistanceArray[iTeppouAptitude]; // 
				}

				// 海賊で、今海ならば距離を加算…
				if ( nb6bushouref[iBushouID].job == 0x50 && (iHexRole==FIELD_HEX_ROLE::海||iHexRole==FIELD_HEX_ROLE::湖) ) {
					iCanTaihouAndTekkousenDistance += (getBushouAptitude(iBushouID, APTITUDE_SUIGUN)+1)/2; // 適正に応じて加算する。
				}

				// タルタロスが海上で鉄甲船状態ならば、そもそも値を変更する。
				if ( isLancelotTartare(iBushouID) && (nb6bushouref[iBushouID].steelship && (iHexRole==FIELD_HEX_ROLE::海||iHexRole==FIELD_HEX_ROLE::湖) ) ) {
					iCanTaihouAndTekkousenDistance = 2 + getBushouAptitude(iBushouID, APTITUDE_SUIGUN)*2 + getBushouAptitude(iBushouID, APTITUDE_TEPPOU)*2;
				}

				// 朱伊が海上で鉄甲船状態ならば、そもそも値を変更する。
				if (isShuiHidechiyo(iBushouID) && (nb6bushouref[iBushouID].steelship && (iHexRole == FIELD_HEX_ROLE::海 || iHexRole == FIELD_HEX_ROLE::湖))) {
					iCanTaihouAndTekkousenDistance = 2 + getBushouAptitude(iBushouID, APTITUDE_SUIGUN) * 2 + getBushouAptitude(iBushouID, APTITUDE_TEPPOU) * 2;
				}

			}

			// 攻城戦の場合
		} else if ( Is_CastleWar() ) {

			// 対象武将が(城に居れば)位置を得る。
			HEX_POSITION p = GetCastleHexUnitPos( iBushouID );
			// 武将の位置の役割を得る
			int iHexRole = GetCastleHexRole( p );

			if ( nb6bushouref[iBushouID].biggun ) { // 大砲を持っているか、もしくは、(今鉄甲船=鉄鋼船を持っていて、海の上)ならば…

				// 配列にアクセスできそうであれば…
				if ( iTeppouAptitude < sizeof(TaihouDistanceArray)/sizeof(TaihouDistanceArray[0]) ) {

					if ( nb6bushouref[iBushouID].form == 0 && nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi >= 3) { // 足軽で弓がＢ以上であれば…
						// 鉄砲適正から、大砲距離マス目を取得
						iCanTaihouAndTekkousenDistance = TaihouDistanceArray[iTeppouAptitude]; // 

						if ( iHexRole == CASTLE_HEX_ROLE::櫓 || iHexRole == CASTLE_HEX_ROLE::本丸 ) {
							iCanTaihouAndTekkousenDistance++;
						}
					}

					// 弓騎馬の時
					else if ( nb6bushouref[iBushouID].form == 1 && nb6bushouref[iBushouID].aptitudekib >= 3 && nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi >= 3) {
						// 鉄砲適正から、大砲距離マス目を取得
						iCanTaihouAndTekkousenDistance = TaihouDistanceArray[iTeppouAptitude]; // 

						if ( iHexRole == CASTLE_HEX_ROLE::櫓 || iHexRole == CASTLE_HEX_ROLE::本丸 ) {
							iCanTaihouAndTekkousenDistance++;
						}
					}
				}
			}
		}

		if ( iCanTaihouAndTekkousenDistance < 9 ) {
			iCanTaihouAndTekkousenDistance++; // インクリメント 元のTENSHOU.EXEと同じことをやっておく。
		}

	}
}

/*
大砲を持っていたら、射程を１増やすという話なので…
0042B449  |> 837D F0 02     CMP     DWORD PTR SS:[EBP-10], 2	←大砲を持っている
0042B44D  |. 7C 1A          JL      SHORT TENSHOU.0042B469
0042B44F  |. 43             INC     EBX							←持っていれば射程を１増やす。ここで弓隊なら本来の射程をEBXに上書きすれば良い。
							└ここを JMP TSMod.OnTSExeSetCanTaihouAndTekkousenDistance と書き換えてやる実際の処理
0042B450  |. 3B5D F0        CMP     EBX, DWORD PTR SS:[EBP-10]
0042B453  |. 72 14          JB      SHORT TENSHOU.0042B469		
0042B455  |. 57             PUSH    EDI
*/
int pTenshouExeJumpFromToOnTenshouExeSetCanTaihouAndTekkousenDistance		=0x42B44F; // 関数はこのアドレスから、OnTenshouExePreviousBattleMenuInBattleへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeSetCanTaihouAndTekkousenDistance   =0x42B4B8; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLbl1FromOnTenshouExeSetCanTaihouAndTekkousenDistance	=0x42B469; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
int pTenshouExeReturnLbl2FromOnTenshouExeSetCanTaihouAndTekkousenDistance	=0x42B455; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeSetCanTaihouAndTekkousenDistance() {
	// スタックにためておく
	__asm {
		
		mov iCanTaihouAndTekkousenDistance, ebx // 現状のEBXをコピー

		mov ebx, DWORD PTR SS:[EBP-0x10] // 比較対象をebxにコピー

		mov iCanTaihouAndTekkousenDistanceCompareVariable, ebx // それもコピー

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}
	
	OnTenshouExeSetCanTaihouAndTekkousenDistanceExecute();

	if ( iCanTaihouAndTekkousenDistance < iCanTaihouAndTekkousenDistanceCompareVariable ) { // CMP   EBX, DWORD PTR SS:[EBP-0x10]

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

			mov ebx, iCanTaihouAndTekkousenDistance // EBXに上書き

			jmp pTenshouExeReturnLbl1FromOnTenshouExeSetCanTaihouAndTekkousenDistance
		}

	} else {
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

			mov ebx, iCanTaihouAndTekkousenDistance // EBXに上書き

			jmp pTenshouExeReturnLbl2FromOnTenshouExeSetCanTaihouAndTekkousenDistance;
		}
	}
}





char cmdOnTenshouExeJumpFromSetCanTaihouAndTekkousenDistance[7]="\xE9\x90\x90\x90\x90\x90";
// 元の命令が６バイト、以後の関数で生まれる命令が合計６バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeSetCanTaihouAndTekkousenDistance() {

	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeSetCanTaihouAndTekkousenDistance;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeSetCanTaihouAndTekkousenDistance + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeSetCanTaihouAndTekkousenDistance  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromSetCanTaihouAndTekkousenDistance+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeSetCanTaihouAndTekkousenDistance), cmdOnTenshouExeJumpFromSetCanTaihouAndTekkousenDistance, 6, NULL); //6バイトのみ書き込む
}


