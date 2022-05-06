#define _CRT_SECURE_NO_WARNINGS

#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"

#include "SetOverWriteButaiMei.h"


/*
 * 部隊編集しようとした時、ユニットの部隊種類を選択して長い名前が要求された
 */ 


/*
00460150  |. 6A 00          PUSH    0
00460152  |. 68 80134D00    PUSH    TENSHOU.004D1380
00460157  |. FF7424 28      PUSH    DWORD PTR SS:[ESP+28]
*/

// 表示しようとしている武将番号
int iBushouIDOnTenshouExeSelectingMainButaiHensyuUnitExecute = -1;
extern int pOrgButaiMeiArrayOnExecute; // 元々TENSHOU.EXE内にあったCALL先

extern int pNewButaiMeiArrayOnExecute;
extern int pOrgButaiFormNo;

// 最終的に見せる用の配列
char szCmd0ButaiLongNameAshigaru[19] = "足軽";
char szCmd1ButaiLongNameKiba[19] = "騎馬";
char szCmd0ButaiLongNameTeppou[19] = "鉄砲";
char szCmd0ButaiLongNameTeppouKiba[19] = "騎馬鉄砲";

int iNewAddressCmdsSelectingMainButaiHensyuUnitExecute[4] = {
	(int)szCmd0ButaiLongNameAshigaru,
	(int)szCmd1ButaiLongNameKiba,
	(int)szCmd0ButaiLongNameTeppou,
	(int)szCmd0ButaiLongNameTeppouKiba,
};


void OnTenshouExeSelectingMainButaiHensyuUnitExecute() {
	// ESIのうち、下のSIの部分だけが武将番号に相当する
	iBushouIDOnTenshouExeSelectingMainButaiHensyuUnitExecute = iBushouIDOnTenshouExeSelectingMainButaiHensyuUnitExecute & 0xFFFF;

	int iBushouID = iBushouIDOnTenshouExeSelectingMainButaiHensyuUnitExecute - 1;

	// 必ず元の、足軽、騎馬、鉄砲、鉄砲騎馬の文字で初期化
	// オリジナルものをコピーしておいて…
	pOrgButaiMeiArrayOnExecute = 0x4D1380;

	char **pOrg = (char **)pOrgButaiMeiArrayOnExecute;
	strcpy(szCmd0ButaiLongNameAshigaru,   pOrg[0]);
	strcpy(szCmd1ButaiLongNameKiba,	      pOrg[1]);
	strcpy(szCmd0ButaiLongNameTeppou,     pOrg[2]);
	strcpy(szCmd0ButaiLongNameTeppouKiba, pOrg[3]);


	// ４回それぞれの兵態でシュミレーションしてみる。
	int iOriginalForm = nb6bushouref[iBushouID].form;

	// まず足軽でシュミレート
	nb6bushouref[iBushouID].form = 0;
	// 部隊名配列ポインタを上書きする。
	int iAddress = GetOverWriteButaiMeiArrayPointer(iBushouID);
	if ( iAddress ) {
		char **p = (char **)iAddress;
		strcpy(szCmd0ButaiLongNameAshigaru, p[0]);
	}

	// 次に騎馬ででシュミレート
	nb6bushouref[iBushouID].form = 1;
	// 部隊名配列ポインタを上書きする。
	iAddress = GetOverWriteButaiMeiArrayPointer(iBushouID);
	if ( iAddress ) {
		char **p = (char **)iAddress;
		strcpy(szCmd1ButaiLongNameKiba, p[1]);
	}

	// 次に鉄砲ででシュミレート
	nb6bushouref[iBushouID].form = 2;
	// 部隊名配列ポインタを上書きする。
	iAddress = GetOverWriteButaiMeiArrayPointer(iBushouID);
	if ( iAddress ) {
		char **p = (char **)iAddress;
		strcpy(szCmd0ButaiLongNameTeppou, p[2]);
	}

	// 最後に騎馬鉄砲ででシュミレート
	nb6bushouref[iBushouID].form = 3;
	// 部隊名配列ポインタを上書きする。
	iAddress = GetOverWriteButaiMeiArrayPointer(iBushouID);
	if ( iAddress ) {
		char **p = (char **)iAddress;
		strcpy(szCmd0ButaiLongNameTeppouKiba, p[3]);
	}

	// 兵態を元へと戻す
	nb6bushouref[iBushouID].form = iOriginalForm;

	// 表示すべき、兵態int配列のアドレスを、整数として渡す
	pNewButaiMeiArrayOnExecute = int(&iNewAddressCmdsSelectingMainButaiHensyuUnitExecute);
}



/*
// この段階でEAXに武将番号が入っているが、実際の武将番号はAX部分(下位ビット)
00460150  |. 6A 00          PUSH    0
00460152  |. 68 80134D00    PUSH    TENSHOU.004D1380
				└ここを JMP TSMod.OnTSExeSelectingMainButaiHensyuUnit と書き換えてやる実際の処理
00460157  |. FF7424 28      PUSH    DWORD PTR SS:[ESP+28]
*/
int pTenshouExeJumpFromToOnTenshouExeSelectingMainButaiHensyuUnit	  =0x460152; // 関数はこのアドレスから、OnTenshouExeSelectingMainButaiHensyuUnitへとジャンプしてくる。
int pTenshouExeReturnLblFromOnTenshouExeSelectingMainButaiHensyuUnit  =0x460157; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeSelectingMainButaiHensyuUnit() {
	// スタックにためておく
	__asm {

		// 武将番号の保存
		mov iBushouIDOnTenshouExeSelectingMainButaiHensyuUnitExecute, eax

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
	OnTenshouExeSelectingMainButaiHensyuUnitExecute();

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
		push pNewButaiMeiArrayOnExecute

		jmp pTenshouExeReturnLblFromOnTenshouExeSelectingMainButaiHensyuUnit
	}
}




char cmdOnTenshouExeJumpFromSelectingMainButaiHensyuUnit[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計5バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeSelectingMainButaiHensyuUnit() {

	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeSelectingMainButaiHensyuUnit;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeSelectingMainButaiHensyuUnit + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeSelectingMainButaiHensyuUnit  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromSelectingMainButaiHensyuUnit+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeSelectingMainButaiHensyuUnit), cmdOnTenshouExeJumpFromSelectingMainButaiHensyuUnit, 5, NULL); //5バイトのみ書き込む
}




