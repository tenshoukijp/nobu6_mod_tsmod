#include "WinTarget.h"
#include <math.h>
#include <stdlib.h>
#include <time.h>

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"


extern int isCurBushouHasATouken(int iBushouID);

// 季節が変わる度に４等級以下の剣家宝について、剣を持ち合わせていない剣豪武将に、ごく低い確率で渡るようにする。
void EXServieExecute_NPCKengouBushouHasAKahouSword() {

	// ロード直後は発動しない
	if ( nb6progressseason <= 1 ) { return; }

	// 武将を一通りなめる。剣豪武将を探す
	for ( int iBushouID = 0; iBushouID < GAMEDATASTRUCT_BUSHOU_NUM; iBushouID++ ) {

		// 剣豪武将でなければ無視。次の人
		if ( nb6bushouref[iBushouID].job != 0x30 ) { continue; }

		// 死亡していれば無視。次の人
		if ( nb6bushouname[iBushouID].State == 7 ) { continue; }

		// 該当武将の戦闘能力が高すぎたら改造武将か、超姫武将。持たせない。
		if ( nb6bushouref[iBushouID].maxbat > 100 ) { continue; }

		// 該当武将が刀剣を持っていたら、無視。次の人
		if ( isCurBushouHasATouken( iBushouID ) ) { continue; }

		// プレイヤー配下の武将であれば、無視
		if ( isPlayerBushou( iBushouID ) ) { continue; }

		// 1人の剣豪あたり、1/30しかもてる確率はない
		if ( rand() % 30 != 0 ) { continue; }

		// 家宝を一通り調べる
		for (int iKahouID=0; iKahouID<GAMEDATASTRUCT_KAHOU_NUM; iKahouID++) {

			int iTargetKahouRank = rand() % 7; // (0～6) 
			iTargetKahouRank += 1; // (上昇幅1～7 = 10等級～４等級)

			// 対象の家宝が「刀剣」系である。 所有者が商人である(0xFFFF = 65535)。そして家宝の上昇値が、iTargetKahouRankと一致する。
			if (nb6kahou[iKahouID].type == 5 && nb6kahou[iKahouID].attach == 0xFFFF && nb6kahou[iKahouID].param == iTargetKahouRank ) {

				// その家宝を該当武将に持たせる
				nb6kahou[iKahouID].attach = iBushouID + 1;

				// 戦闘能力を該当数値分アップする。
				nb6bushouref[iBushouID].maxbat += nb6kahou[iKahouID].param;

				// 1人で一気に何個ももたないようにbreakする必要あり!!
				break;
				
			}
		}
	}

}



// return 刀剣の上昇値(1～10)
int isCurBushouHasATouken(int iBushouID) {
	int iToukenMaxRank = 0;

	// 家宝を一通り調べる
	for (int iKahouID=0; iKahouID<GAMEDATASTRUCT_KAHOU_NUM; iKahouID++) {

		// 所有者の武将IDが一致する場合のみ判定の対象
		if (nb6kahou[iKahouID].attach-1 == iBushouID) {

			// 対象の家宝が「刀剣」系である。
			if ( nb6kahou[iKahouID].type == 5 ) {

				// 他に持っていた刀剣よりもランクが高ければ、それが本人が所持するよりより家宝上昇値
				if ( iToukenMaxRank < nb6kahou[iKahouID].param ) {
					iToukenMaxRank = nb6kahou[iKahouID].param;
				}
			}
		}
	}
	// ランクを返す。最小0(所持していない)、最大10
	return iToukenMaxRank;
}

int iCurTypeDamegeOfDoor = -1;				// ドアのタイプ
int iDamageOfDoorExecute = -1;
int iCurBushouIDInDamageOfDoor = -1;		// ドアの場合はebpから武将IDが、城の場合はebxから武将IDがコピーされる。
int iCurBushouIDInDamageOfCastle = -1;		// ドアの場合はebpから武将IDが、城の場合はebxから武将IDがコピーされる。

void OnTenshouExeDamagedOfDoorExecute() {
	// 城の場合(edi==0xffff) NPCの場合は(edi=ebxの下４bit)
	if ( (iCurTypeDamegeOfDoor == 0xFFFF) || (iCurTypeDamegeOfDoor == (iCurBushouIDInDamageOfCastle & 0xFFFF) ) ) {
		// 下位４バイトが武将IDなので、マスク
		iCurBushouIDInDamageOfDoor = iCurBushouIDInDamageOfCastle & 0xFFFF;
	} else {
		// 下位４バイトが武将IDなので、マスク
		iCurBushouIDInDamageOfDoor = iCurBushouIDInDamageOfDoor & 0xFFFF;
	}
	// 武将が一定の範囲であること
	if (0 <= iCurBushouIDInDamageOfDoor-1 && iCurBushouIDInDamageOfDoor-1 < GAMEDATASTRUCT_BUSHOU_NUM) {
		
		// 剣豪でなく、騎士でもなければ、この人に扉への特殊なダメージ増幅はしない
		if ( nb6bushouref[iCurBushouIDInDamageOfDoor-1].job != 0x30 && nb6bushouref[iCurBushouIDInDamageOfDoor-1].job != 0xA0 ) { return; }

		byte iKahouParam = isCurBushouHasATouken(iCurBushouIDInDamageOfDoor-1);
		// 家宝を持っていれば…
		if ( iKahouParam ) {
			iKahouParam += 2; // (最小3～最大12へ)
			// ドアのダメージが約家の上昇値倍
			iDamageOfDoorExecute = iDamageOfDoorExecute * iKahouParam;
			iDamageOfDoorExecute = iDamageOfDoorExecute / 2; //(最小1.5倍～最大6.0倍)
			// 最大値は100
			if (iDamageOfDoorExecute > 100) { iDamageOfDoorExecute = 100; }
		}
	}
}


/*
00432F14  |. 8D4406 01      LEA     EAX, DWORD PTR DS:[ESI+EAX+1]  EAX=2B
00432F18  |. 5E             POP     ESI
				└この２行を JMP TSMod.OnTSExeDamagedOfDoor と書き換えてやる実際の処理
00432F19  \. C3             RETN
*/
int pTenshouExeJumpFromToOnTenshouExeDamagedOfDoor		=0x432F14; // 関数はこのアドレスから、OnTenshouExeDamagedOfDoorへとジャンプしてくる。
int pTenshouExeReturnLblFromOnTenshouExeDamagedOfDoor	=0x432F19; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeDamagedOfDoor() {
	// スタックにためておく
	__asm {
		mov iCurTypeDamegeOfDoor, edi		// ediが0xFFFFなら城ダメージ 1ならドアダメージ。ＮＰＣの場合は、edi=ebxの下４bitの状況が城の模様。

		lea eax, DWORD PTR DS:[esi+eax+1]

		mov iCurBushouIDInDamageOfDoor, ebp			// ドアの場合に武将IDをコピーただし、ebpの下位4バイトが武将IDなので、マスクする必要あり。
		mov iCurBushouIDInDamageOfCastle, ebx;		// 城の場合に武将IDをコピー

		// ダメージを保存
		mov iDamageOfDoorExecute, eax;

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}
	
	// ゲームデータをプレイヤーターンが変わった段階で読み込み更新しておく
	OnTenshouExeDamagedOfDoorExecute();

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

		// 修正したダメージ値をここでeaxに逆に戻す
		mov eax, iDamageOfDoorExecute

		pop	esi

		jmp pTenshouExeReturnLblFromOnTenshouExeDamagedOfDoor
	}
}

/*
00432F14  |. 8D4406 01      LEA     EAX, DWORD PTR DS:[ESI+EAX+1]  EAX=2B
00432F18  |. 5E             POP     ESI
				└この２行を JMP TSMod.OnTSExeDamagedOfDoor と書き換えてやる実際の処理
00432F19  \. C3             RETN
*/
char cmdOnTenshouExeJumpFromDamagedOfDoor[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeDamagedOfDoor() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeDamagedOfDoor;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeDamagedOfDoor + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeDamagedOfDoor  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromDamagedOfDoor+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeDamagedOfDoor), cmdOnTenshouExeJumpFromDamagedOfDoor, 5, NULL); //5バイトのみ書き込む
}




