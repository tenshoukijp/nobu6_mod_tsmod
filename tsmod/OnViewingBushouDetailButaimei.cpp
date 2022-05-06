#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"

#include "SetOverWriteButaiMei.h"
#include "EXBushou_All.h"

#include "ScenarioMod.h"


/*
 * 武将詳細画面を見た際の部隊名
 */ 


/*
00493C75  |. 83C4 04        ADD     ESP, 4
00493C78  |. FF3485 80134D0>PUSH    DWORD PTR DS:[EAX*4+4D1380]
00493C7F  |. 68 982E4D00    PUSH    TENSHOU.004D2E98                 ;  ASCII "%s隊"
00493C84  |. 55             PUSH    EBP
*/

// ロスローリアンの場合用の特別処理
char *szLoslorienDan = "%s団";
int pAddressSzIsLoslorienDan = (int)szLoslorienDan;
bool isLoslorienViewingBushouDetailButaimeiExecute = false;

// 何も付けない場合の特別処理
char *szButaiSuffixNon = "%s";
int pAddressSzIsButaiSuffixNon = (int)szButaiSuffixNon;
bool isButaiSuffixNonViewingBushouDetailButaimeiExecute = false;

// ScenarioModの場合用の特別処理
char szScenarioModCustomSuffix[11] = "%s軍神";
int pAddressSzScenarioModCustomSuffix = (int)szScenarioModCustomSuffix;
bool isScenarioModCustomViewingBushouDetailButaimeiExecute = false;



// 表示しようとしている武将番号
int iBushouIDOnTenshouExeViewingBushouDetailButaimeiExecute = -1;
int pOrgButaiMeiArrayOnExecute = 0x4D1380;; // 元々TENSHOU.EXE内にあったCALL先

int pNewButaiMeiArrayOnExecute = -1;
int pOrgButaiFormNo = -1;

bool IsMustButaimeiDan(int iBushouID);

void OnTenshouExeViewingBushouDetailButaimeiExecute() {
	// 毎度falseで初期化
	isLoslorienViewingBushouDetailButaimeiExecute = false;
	isButaiSuffixNonViewingBushouDetailButaimeiExecute = false;
	isScenarioModCustomViewingBushouDetailButaimeiExecute = false;

	// ESIのうち、下のSIの部分だけが武将番号に相当する
	iBushouIDOnTenshouExeViewingBushouDetailButaimeiExecute = iBushouIDOnTenshouExeViewingBushouDetailButaimeiExecute & 0xFFFF;

	int iBushouID = iBushouIDOnTenshouExeViewingBushouDetailButaimeiExecute - 1;

	pOrgButaiMeiArrayOnExecute = 0x4D1380;

	// 部隊名配列ポインタを上書きする。
	int p = GetOverWriteButaiMeiArrayPointer(iBushouID);
	if ( p ) {
		pNewButaiMeiArrayOnExecute = p + pOrgButaiFormNo*4;
	} else {
		// 部隊名ラベルは必ずオリジナルで上書きされる。
		pNewButaiMeiArrayOnExecute = 0x4D1380 + pOrgButaiFormNo*4;
	}


	// 語尾書式の取得
	if ( p_snOnGettingButaimeiGobiSprintf ) {
		char szpSzModifyDan[11] = "";
		int ret = p_snOnGettingButaimeiGobiSprintf(NULL, szpSzModifyDan); // 語尾書式をScenarioModから取得
		if (ret) {
			strcpy(szScenarioModCustomSuffix, szpSzModifyDan);
			isScenarioModCustomViewingBushouDetailButaimeiExecute = true;
		}
	}

	// 団であるべきならば
	if ( IsMustButaimeiDan(iBushouID) ) {
		isLoslorienViewingBushouDetailButaimeiExecute = true;
	}

}

// "団"であるべきか
bool IsMustButaimeiDan(int iBushouID) {

	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		//　ロスローリアンなら、"隊"ではなく"団"にするため
		if ( isLancelotTartare(iBushouID ) || IsLoslorien(iBushouID) ) {
			return true;
		}
		// 騎士系となるため。
		if ( nb6bushouref[iBushouID].job == 0xA0 && Is_FieldWar() ) {
			// 今戦闘していて、海の上にいる
			HEX_POSITION p = GetFieldHexUnitPos( iBushouID );
			// 武将の位置の役割を得る
			int iHexRole = GetFieldHexRole( p );

			// 海の上
			if ( iHexRole != FIELD_HEX_ROLE::海 && iHexRole != FIELD_HEX_ROLE::湖 ) {
				// ギルバルド意外ならば、団
				if ( strcmp(nb6bushouname[iBushouID].familyname, "")!=0 ) {
					return true;
				}
			}
		}
	}

	return false;
}


/*
// この段階でESIに武将番号が入っているが、実際の武将番号はSI部分(下位ビット)
00493C70  |. E8 1462F7FF    CALL    TENSHOU.00409E89
				└ここを JMP TSMod.OnTSExeViewingBushouDetailButaimei と書き換えてやる実際の処理
00493C75  |. 83C4 04        ADD     ESP, 4
00493C78  |. FF3485 80134D0>PUSH    DWORD PTR DS:[EAX*4+4D1380]				eax はnb6bushou[iBushouID].form と同値
00493C7F  |. 68 982E4D00    PUSH    TENSHOU.004D2E98                 ;  ASCII "%s隊"
00493C84  |. 55             PUSH    EBP

*/
int pTenshouExeJumpFromToOnTenshouExeViewingBushouDetailButaimei	 =0x493C70; // 関数はこのアドレスから、OnTenshouExeViewingBushouDetailButaimeiへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeViewingBushouDetailButaimei =0x409E89; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeViewingBushouDetailButaimei  =0x493C84; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeViewingBushouDetailButaimei() {
	// スタックにためておく
	__asm {
		// 元の処理をここに記載
		call pTenshouExeJumpCallFromToOnTenshouExeViewingBushouDetailButaimei

		// 元の処理
		ADD     ESP, 4

		// nb6bushou[iBushouID].form 相当を保存
		mov pOrgButaiFormNo, eax

		// 武将番号の保存
		mov iBushouIDOnTenshouExeViewingBushouDetailButaimeiExecute, esi

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
	OnTenshouExeViewingBushouDetailButaimeiExecute();

	// ScenarioModによって修正されている場合
	if ( isScenarioModCustomViewingBushouDetailButaimeiExecute ) {
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

			// pNewButaiMeiArrayOnExecuteは少なくともオリジナルか、上書きされた、char *[4]の配列。
			mov eax, pNewButaiMeiArrayOnExecute
			PUSH    DWORD PTR DS:[eax] 

			// eax を元来の値に戻す
			mov eax, pOrgButaiFormNo

			// TENSHOU.EXE に元々あったもの
			push pAddressSzScenarioModCustomSuffix

			jmp pTenshouExeReturnLblFromOnTenshouExeViewingBushouDetailButaimei
		}

	// ロスローリアン武将("～団")
	} else if ( isLoslorienViewingBushouDetailButaimeiExecute ) {
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

			// pNewButaiMeiArrayOnExecuteは少なくともオリジナルか、上書きされた、char *[4]の配列。
			mov eax, pNewButaiMeiArrayOnExecute
			PUSH    DWORD PTR DS:[eax] 

			// eax を元来の値に戻す
			mov eax, pOrgButaiFormNo

			// TENSHOU.EXE に元々あったもの
			push pAddressSzIsLoslorienDan

			jmp pTenshouExeReturnLblFromOnTenshouExeViewingBushouDetailButaimei
		}

	// 通常の武将("～隊")
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

			// pNewButaiMeiArrayOnExecuteは少なくともオリジナルか、上書きされた、char *[4]の配列。
			mov eax, pNewButaiMeiArrayOnExecute
			PUSH    DWORD PTR DS:[eax] 

			// eax を元来の値に戻す
			mov eax, pOrgButaiFormNo

			// TENSHOU.EXE に元々あったもの
			push 0x4D2E98

			jmp pTenshouExeReturnLblFromOnTenshouExeViewingBushouDetailButaimei
		}
	}
}




char cmdOnTenshouExeJumpFromViewingBushouDetailButaimei[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計5バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeViewingBushouDetailButaimei() {

	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeViewingBushouDetailButaimei;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeViewingBushouDetailButaimei + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeViewingBushouDetailButaimei  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromViewingBushouDetailButaimei+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeViewingBushouDetailButaimei), cmdOnTenshouExeJumpFromViewingBushouDetailButaimei, 5, NULL); //5バイトのみ書き込む
}




