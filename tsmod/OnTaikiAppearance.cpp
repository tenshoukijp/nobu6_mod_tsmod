#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "SetBushouAvgParam.h"
#include "ScenarioMod.h"
/*
 待機武将が登場する
 */
/*
00441B19   > E8 46030000    CALL    TENSHOU.00441E64		
00441B1E   . E8 1F220000    CALL    TENSHOU.00443D42		← 待機武将が登場する。
00441B23   . E8 0EB00000    CALL    TENSHOU.0044CB36
*/

/*
00443D5B  /$ B8 2B3E4400    MOV     EAX, TENSHOU.00443E2B
00443D60  |. E8 17C70600    CALL    TENSHOU.004B047C
00443D65  |. 83EC 20        SUB     ESP, 20
00443D68  |. 8D4D D4        LEA     ECX, DWORD PTR SS:[EBP-2C]
00443D6B  |. 56             PUSH    ESI
00443D6C  |. E8 8B18FCFF    CALL    TENSHOU.004055FC
00443D71  |. 33C0           XOR     EAX, EAX
00443D73  |. BE C08C4C00    MOV     ESI, TENSHOU.004C8CC0            ;  ASCII "B:TAIKI.N6P"
00443D78  |. 50             PUSH    EAX
00443D79  |. 8D4D D4        LEA     ECX, DWORD PTR SS:[EBP-2C]
00443D7C  |. 56             PUSH    ESI
00443D7D  |. 8945 FC        MOV     DWORD PTR SS:[EBP-4], EAX
00443D80  |. E8 643D0300    CALL    TENSHOU.00477AE9
00443D85  |. 85C0           TEST    EAX, EAX
00443D87  |. 75 09          JNZ     SHORT TENSHOU.00443D92
00443D89  |. 56             PUSH    ESI
*/

extern void EXSuperService_IfAbilityIsUnderdevelopedOnAlwaysMainGame(int iBushouID);
extern void EXSuperService_IfFixExperienceMaxOnAlwaysMainGame(int iBushouID);
extern void GeneralBushouBornExecute(); // 汎用武将等誕生処理。

// 姫のダメな顔を矯正する
extern void ModifyWrongFaceDamegao(int iBushouID);

/*
 * 
 */
int faceListOnTaikiAppearance[GAMEDATASTRUCT_BUSHOU_NUM] = {-1};

// 待機武将が登場する前に、顔一覧を一端保存
void OnTenshouExeTaikiAppearanceExecuteBefore() {
	for (int iBushouID=0; iBushouID < GAMEDATASTRUCT_BUSHOU_NUM; iBushouID++ ) {

		// ダメ顔矯正をする。
		ModifyWrongFaceDamegao(iBushouID);

		faceListOnTaikiAppearance[iBushouID] = nb6bushouname[iBushouID].number;
	}
}


extern vector<int> registerSkippedOfIncrementTaikiBushouCount;
extern vector<int> registerModifiedOfIncrementTaikiBushouCount;


// 待機武将が登場した後、顔一覧と比較して、食い違っている武将をいろいろ処理
void OnTenshouExeTaikiAppearanceExecuteAfter() {
	// TSMod.txtの設定によっては、死亡枠に追加で、汎用武将を誕生させる。
	if ( nb6year.season == 0 ) { // 春限定
		GeneralBushouBornExecute(); // 汎用武将等誕生処理。
	}

	for (int iBushouID=0; iBushouID < GAMEDATASTRUCT_BUSHOU_NUM; iBushouID++ ) { // 最後の２人はチェックするべきではない
		// 番号が食い違っているということは、ここで武将が新規登場した。
		if ( faceListOnTaikiAppearance[iBushouID] != nb6bushouname[iBushouID].number ) {

			auto ret1 = std::find(registerSkippedOfIncrementTaikiBushouCount.begin(), registerSkippedOfIncrementTaikiBushouCount.end(), iBushouID);

			// スキップなら何もしない
			if (ret1 != registerSkippedOfIncrementTaikiBushouCount.end()) {
				// 新たな番号をスロットに入れる。
				faceListOnTaikiAppearance[iBushouID] = nb6bushouname[iBushouID].number;

				continue;
			}

			auto ret2 = std::find(registerModifiedOfIncrementTaikiBushouCount.begin(), registerModifiedOfIncrementTaikiBushouCount.end(), iBushouID);

			// 処理済みリストに見つかったら何もしない
			if (ret2 != registerModifiedOfIncrementTaikiBushouCount.end()) {

				// 新たな番号をスロットに入れる。
				faceListOnTaikiAppearance[iBushouID] = nb6bushouname[iBushouID].number;

				continue;


			} else {

				// ダメ顔矯正をする。
				ModifyWrongFaceDamegao(iBushouID);

				// 新たな番号をスロットに入れる。
				faceListOnTaikiAppearance[iBushouID] = nb6bushouname[iBushouID].number;

				// 技能が未開発な場合の処理
				EXSuperService_IfAbilityIsUnderdevelopedOnAlwaysMainGame(iBushouID);

				// 武将の熟練度がＭＡＸ固定であった場合の処理
				EXSuperService_IfFixExperienceMaxOnAlwaysMainGame(iBushouID);

				// 歴代シリーズの武将の能力を反映する
				SetBushouAvgParamAppearanceBushou(iBushouID);

				// 最後の２人はチェックするべきではないので絞る
				if (iBushouID < GAMEDATASTRUCT_BUSHOU_NUM - 2) {

					// ScenarioModへと仮想武将の誕生を通知
					if (p_snOnBornGeneralBushou) {
						p_snOnBornGeneralBushou(iBushouID);
					}
				}
			}
		}
	}

	// このタイミングで、今季ScenarioModでスキップしてといった武将情報のデータはクリア
	registerSkippedOfIncrementTaikiBushouCount.clear();
	// このタイミングで、今季登場した武将リストはクリア
	registerModifiedOfIncrementTaikiBushouCount.clear();

}


/*
00441B19   > E8 46030000    CALL    TENSHOU.00441E64		
00441B1E   . E8 1F220000    CALL    TENSHOU.00443D42		← 待機武将が登場する。
							└ここを JMP TSMod.OnTSExeTaikiAppearance と書き換えてやる実際の処理
00441B23   . E8 0EB00000    CALL    TENSHOU.0044CB36

*/
int pTenshouExeJumpFromToOnTenshouExeTaikiAppearance	=0x441B1E; // 関数はこのアドレスから、OnTenshouExeTaikiAppearanceへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeTaikiAppearance=0x443D42; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeTaikiAppearance	=0x441B23; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeTaikiAppearance() {
	// スタックにためておく
	__asm {

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}
	
	OnTenshouExeTaikiAppearanceExecuteBefore();

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
		call pTenshouExeJumpCallFromToOnTenshouExeTaikiAppearance

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}

	OnTenshouExeTaikiAppearanceExecuteAfter();

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

		jmp pTenshouExeReturnLblFromOnTenshouExeTaikiAppearance
	}
}





char cmdOnTenshouExeJumpFromTaikiAppearance[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeTaikiAppearance() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeTaikiAppearance;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeTaikiAppearance + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeTaikiAppearance  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromTaikiAppearance+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeTaikiAppearance), cmdOnTenshouExeJumpFromTaikiAppearance, 5, NULL); //5バイトのみ書き込む
}


