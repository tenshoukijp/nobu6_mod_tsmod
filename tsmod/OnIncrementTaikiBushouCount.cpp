#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "SetBushouAvgParam.h"
#include "ScenarioMod.h"


extern void EXSuperService_IfAbilityIsUnderdevelopedOnAlwaysMainGame(int iBushouID);
extern void EXSuperService_IfFixExperienceMaxOnAlwaysMainGame(int iBushouID);
extern void GeneralBushouBornExecute(); // 汎用武将等誕生処理。

// 姫のダメな顔を矯正する
extern void ModifyWrongFaceDamegao(int iBushouID);


/*
00443DF2   56               |PUSH    ESI
00443DF3   E8 45000000      |CALL    TENSHOU.00443E3D   ← 待機武将の誕生と思われる
00443DF8   83C4 04          |ADD     ESP, 4
00443DFB   85C0             |TEST    EAX, EAX
00443DFD  ^74 BA            |JE      SHORT TENSHOU.00443DB9
00443DFF   66:FF05 37FC4C00 |INC     WORD PTR DS:[4CFC37]  ← 待機武将の登場数が増える時
00443E06   56               |PUSH    ESI
00443E07   E8 06CA0200      CALL    TENSHOU.00470812　　← 若干なぞ。メッセージとか出すのかな？
00443E0C   83C4 04          |ADD     ESP, 4
*/


// 待機武将が誕生する直前
bool isRequestSkipOfIncrementTaikiBushouCount = false;
int ESI_OF_IncrementTaikiBushouCount = -1;

// スキップすることを覚えておく。
vector<int> registerSkippedOfIncrementTaikiBushouCount;


void OnTenshouExeIncrementTaikiBushouCountExecute() {
	// スキップはデフォルトではせず、ちゃんとそのまま誕生
	isRequestSkipOfIncrementTaikiBushouCount = false;

	int iBushouID = (ESI_OF_IncrementTaikiBushouCount & 0xFFFF) -1; // ESIのうち、SIの部分だけ

	if (0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM) {

		/*
		NB6TAIKIBUSHOUNAME* p待機Array = GetTaikiBushouNameArrayPointer();
		OutputDebugStream("%d", (int)p待機Array);

		int b = getTouzyouBushouNum();
		OutputDebugStream("★");
		OutputDebugStream(p待機Array[b].familyname);
		OutputDebugStream(p待機Array[b].fastname);
		*/

		// ScenarioModへと歴史武将の誕生を通知
		if (p_snOnSkippedRekishiBushou) {
			int isMustRegist = p_snOnSkippedRekishiBushou(iBushouID, getTouzyouBushouNum(), getTaikiBushouDataNum());

			if (isMustRegist == 0) {
				registerSkippedOfIncrementTaikiBushouCount.push_back(iBushouID);

				// 該当スロットは利用せず、武将はスキップすべし
				isRequestSkipOfIncrementTaikiBushouCount = true;
			}
		}

	}

}


/*
00443DF2   56               |PUSH    ESI
00443DF3   E8 45000000      |CALL    TENSHOU.00443E3D   ← 待機武将の誕生と思われる
└ここを JMP TSMod.OnTSExeIncrementTaikiBushouCount と書き換えてやる実際の処理
00443DF8   83C4 04          |ADD     ESP, 4
00443DFB   85C0             |TEST    EAX, EAX

*/
int pTenshouExeJumpFromToOnTenshouExeIncrementTaikiBushouCount = 0x443DF3; // 関数はこのアドレスから、OnTenshouExeIncrementTaikiBushouCountへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeIncrementTaikiBushouCount = 0x443E3D; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeIncrementTaikiBushouCount = 0x443DF8; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
int pTenshouExeReturn2LblFromOnTenshouExeIncrementTaikiBushouCount = 0x443DFB; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec(naked) void WINAPI OnTenshouExeIncrementTaikiBushouCount() {
	// スタックにためておく
	__asm {

		mov ESI_OF_IncrementTaikiBushouCount, ESI

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}

	OnTenshouExeIncrementTaikiBushouCountExecute();

	// スキップするべきであれば…
	if (isRequestSkipOfIncrementTaikiBushouCount) {

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

			// callはせずに…

			// 空ふやしする。すなわち、この待機武将の登場はスルーされた
			INC     WORD PTR DS : [GAMEDATASTRUCT_TOUZYOU_BUSHOU_NUM_ADDRESS] //  ← 待機武将の登場数を増やす。

			mov eax, 0

			jmp pTenshouExeReturn2LblFromOnTenshouExeIncrementTaikiBushouCount // 別の場所(Return2)へとジャンプ
		}

	}
	else {

		// 通常通り スタックに引き出す
		__asm {
			pop edi
			pop esi
			pop ebp
			pop esp
			pop edx
			pop ecx
			pop ebx
			pop eax

			call pTenshouExeJumpCallFromToOnTenshouExeIncrementTaikiBushouCount

			jmp pTenshouExeReturnLblFromOnTenshouExeIncrementTaikiBushouCount
		}
	}
}





char cmdOnTenshouExeJumpFromIncrementTaikiBushouCount[6] = "\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeIncrementTaikiBushouCount() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeIncrementTaikiBushouCount;
	int SubAddress = iAddress - (pTenshouExeJumpFromToOnTenshouExeIncrementTaikiBushouCount + 5);
	// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeIncrementTaikiBushouCount  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromIncrementTaikiBushouCount + 1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory(CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeIncrementTaikiBushouCount), cmdOnTenshouExeJumpFromIncrementTaikiBushouCount, 5, NULL); //5バイトのみ書き込む
}






















/*
00443DF2   56               |PUSH    ESI
00443DF3   E8 45000000      |CALL    TENSHOU.00443E3D   ← 待機武将の誕生と思われる
00443DF8   83C4 04          |ADD     ESP, 4
00443DFB   85C0             |TEST    EAX, EAX
00443DFD  ^74 BA            |JE      SHORT TENSHOU.00443DB9
00443DFF   66:FF05 37FC4C00 |INC     WORD PTR DS:[4CFC37]  ← 待機武将の登場数が増える時
00443E06   56               |PUSH    ESI
00443E07   E8 06CA0200      CALL    TENSHOU.00470812　　← 若干なぞ。メッセージとか出すのかな？
00443E0C   83C4 04          |ADD     ESP, 4
*/

// 登場直後の早い段階で処理をしたことを覚えておく。二十処理で後から上書きしてしまわないように。
vector<int> registerModifiedOfIncrementTaikiBushouCount;

int ESI_OF_IncrementTaikiBushouCountAfter = -1;
void OnTenshouExeIncrementTaikiBushouCountAfterExecute() {

	int iBushouID = (ESI_OF_IncrementTaikiBushouCountAfter & 0xFFFF) - 1; // ESIのうち、SIの部分だけ

	if (0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM) {

		// 姫のダメ顔矯正をする。
		ModifyWrongFaceDamegao(iBushouID);

		// 技能が未開発な場合の処理
		EXSuperService_IfAbilityIsUnderdevelopedOnAlwaysMainGame(iBushouID);

		// 武将の熟練度がＭＡＸ固定であった場合の処理
		EXSuperService_IfFixExperienceMaxOnAlwaysMainGame(iBushouID);

		// 歴代シリーズの武将の能力を反映する
		SetBushouAvgParamAppearanceBushou(iBushouID);

		// リストに追加
		registerModifiedOfIncrementTaikiBushouCount.push_back(iBushouID);

		// ScenarioModへと歴史武将の誕生を通知
		if (p_snOnBornRekishiBushou) {
			p_snOnBornRekishiBushou(iBushouID);
		}
	}
}


/*
00443DFF   66:FF05 37FC4C00 |INC     WORD PTR DS:[4CFC37]  ← 待機武将の登場数が増える時
00443E06   56               |PUSH    ESI
00443E07   E8 06CA0200      CALL    TENSHOU.00470812　　← 若干なぞ。メッセージとか出すのかな？
└ここを JMP TSMod.OnTSExeIncrementTaikiBushouCountAfter と書き換えてやる実際の処理
00443E0C   83C4 04          |ADD     ESP, 4

*/
int pTenshouExeJumpFromToOnTenshouExeIncrementTaikiBushouCountAfter = 0x443E07; // 関数はこのアドレスから、OnTenshouExeIncrementTaikiBushouCountAfterへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeIncrementTaikiBushouCountAfter = 0x470812; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeIncrementTaikiBushouCountAfter = 0x443E0C; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec(naked) void WINAPI OnTenshouExeIncrementTaikiBushouCountAfter() {
	// スタックにためておく
	__asm {

		mov ESI_OF_IncrementTaikiBushouCountAfter, ESI
			
		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}

	OnTenshouExeIncrementTaikiBushouCountAfterExecute();

	// スキップするべきであれば…
	// 通常通り スタックに引き出す
	__asm {
		pop edi
		pop esi
		pop ebp
		pop esp
		pop edx
		pop ecx
		pop ebx
		pop eax

		call pTenshouExeJumpCallFromToOnTenshouExeIncrementTaikiBushouCountAfter

		jmp pTenshouExeReturnLblFromOnTenshouExeIncrementTaikiBushouCountAfter
	}
}





char cmdOnTenshouExeJumpFromIncrementTaikiBushouCountAfter[6] = "\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeIncrementTaikiBushouCountAfter() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeIncrementTaikiBushouCountAfter;
	int SubAddress = iAddress - (pTenshouExeJumpFromToOnTenshouExeIncrementTaikiBushouCountAfter + 5);
	// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeIncrementTaikiBushouCountAfter  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromIncrementTaikiBushouCountAfter + 1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory(CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeIncrementTaikiBushouCountAfter), cmdOnTenshouExeJumpFromIncrementTaikiBushouCountAfter, 5, NULL); //5バイトのみ書き込む
}




