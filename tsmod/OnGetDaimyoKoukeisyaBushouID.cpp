#include "WinTarget.h"
#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "TSModCommand.h"
#include "EXBushou_All.h"
#include "ScenarioMod.h"


/*
 大名の後継者武将ＩＤが必要とされた時
 */


/*

0040BAB8  |. E8 9A080500    CALL    TENSHOU.0045C357

0045C357  /$ 56             PUSH    ESI
0045C358  |. 57             PUSH    EDI
0045C359  |. 66:8B7424 0C   MOV     SI, WORD PTR SS:[ESP+C]
0045C35E  |. 56             PUSH    ESI
0045C35F  |. E8 768C0400    CALL    TENSHOU.004A4FDA
0045C364  |. 66:A3 907C4D00 MOV     WORD PTR DS:[4D7C90], AX
0045C36A  |. 83C4 04        ADD     ESP, 4
0045C36D  |. 56             PUSH    ESI
0045C36E  |. E8 FFF4FFFF    CALL    TENSHOU.0045B872
0045C373  |. 83C4 04        ADD     ESP, 4
0045C376  |. 56             PUSH    ESI
0045C377  |. E8 7EFBFFFF    CALL    TENSHOU.0045BEFA	← 後継者ＵＩ。この中が、後述の 0045BF30  |. E8 6DFCFFFF    CALL    TENSHOU.0045BBA2 へと続く
0045C37C  |. 66:8BF8        MOV     DI, AX				← ＡＸに後継者武将番号 ＦＦＦＦだと滅亡。後継者の処理ＵＩが出た後にここにくる。
0045C37F  |. 83C4 04        ADD     ESP, 4
0045C382  |. 57             PUSH    EDI
0045C383  |. E8 6AEBFAFF    CALL    TENSHOU.0040AEF2
0045C388  |. 83C4 04        ADD     ESP, 4
0045C38B  |. 85C0           TEST    EAX, EAX
0045C38D  |. 74 09          JE      SHORT TENSHOU.0045C398
0045C38F  |. 56             PUSH    ESI                              ; /Arg1
0045C390  |. E8 39FEFFFF    CALL    TENSHOU.0045C1CE                 ; \TENSHOU.0045C1CE
0045C395  |. 83C4 04        ADD     ESP, 4
0045C398  |> 66:8BC7        MOV     AX, DI
0045C39B  |. 5F             POP     EDI
0045C39C  |. 5E             POP     ESI
0045C39D  \. C3             RETN


・ＥＤＩのうち、ＤＩが現在の大名の武将番号（＋１のほう）
・ＥＢＰが、０ｘ１９（大名番号の＋１の方）

・ＥＢＸが２３
プレイヤの場合は、
0045BF19  |> 55             PUSH    EBP
0045BF30  |. E8 6DFCFFFF    CALL    TENSHOU.0045BBA2
0045BF35  |. 66:8BD8        MOV     BX, AX
ＡＸに新武将大名の武将番号を入れる。
で、新大名武将ＩＤの決定が行われる。

ＮＰＣの場合も同様

*/


/*
 大名の後継者武将ＩＤが必要とされた時
 */





WORD iOrgBushouIDOfOnDaimyoKoukeishaBushouID = 0xFFFF;	// 死んだ大名武将ＩＤ
WORD iNewBushouIDOfOnDaimyoKoukeishaBushouID = 0xFFFF;	// 変更するならここに新たな武将ＩＤが入る
int iOrgDaimyoIDOfOnDaimyoKoukeishaBushouID = 0xFFFF;	// 該当武将の大名勢力のＩＤ

void OnTenshouExeGetDaimyoKoukeishaBushouIDExecute() {

	int iBushouID = iOrgBushouIDOfOnDaimyoKoukeishaBushouID-1; // AX部分だけが武将番号

	// 武将でなかったらみ。いなかったら0xFFFFにもどす。
	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// ScenarioModのカスタム条件で軍師を設定する
		if ( p_snOnCustomCondition ) {															                                  //┌大名番号                              ┌死亡大名武将番号                     ┌後継者予定者武将番号
			int isNewDaimyoKoukeishaBushouID = ((PFNSNONCUSTOMCONDITION) p_snOnCustomCondition)("AS条件::メイン::大名後継者武将番号", iOrgDaimyoIDOfOnDaimyoKoukeishaBushouID, iOrgBushouIDOfOnDaimyoKoukeishaBushouID, iNewBushouIDOfOnDaimyoKoukeishaBushouID, "", "", 0.0, 0.0);
			// 軍師が変更となっている。
			if ( isNewDaimyoKoukeishaBushouID != CUSTOM_CONDITION_NOMODIFY ) {
				iNewBushouIDOfOnDaimyoKoukeishaBushouID = isNewDaimyoKoukeishaBushouID;
			}
		}

	}
}


/*
0045BF19  |> 55             PUSH    EBP
0045BF30  |. E8 6DFCFFFF    CALL    TENSHOU.0045BBA2
				└ここを JMP TSMod.OnTSExeGetDaimyoKoukeishaBushouID と書き換えてやる実際の処理
0045BF35  |. 66:8BD8        MOV     BX, AX
*/
int pTenshouExeJumpFromToOnTenshouExeGetDaimyoKoukeishaBushouID	     =0x45BF30; // 関数はこのアドレスから、OnTenshouExeGetDaimyoKoukeishaBushouIDへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeGetDaimyoKoukeishaBushouID	 =0x45BBA2; // 関数はこのアドレスから、OnTenshouExeGetDaimyoKoukeishaBushouIDへとジャンプしてくる。
int pTenshouExeReturnLblFromOnTenshouExeGetDaimyoKoukeishaBushouID   =0x45BF35; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeGetDaimyoKoukeishaBushouID() {
	// スタックにためておく
	__asm {

		mov iOrgBushouIDOfOnDaimyoKoukeishaBushouID, AX // 死んだ武将のＩＤを保存

		mov iOrgDaimyoIDOfOnDaimyoKoukeishaBushouID, EBP // 大名勢力ＩＤ

		// 元々tenshou.exeにあった処理をここで
		call pTenshouExeJumpCallFromToOnTenshouExeGetDaimyoKoukeishaBushouID

		mov iNewBushouIDOfOnDaimyoKoukeishaBushouID, AX // 後継者武将のＩＤを保存

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}

	OnTenshouExeGetDaimyoKoukeishaBushouIDExecute();

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

		// 上書きすべきなら上書きする。
		MOV     AX, iNewBushouIDOfOnDaimyoKoukeishaBushouID

		jmp pTenshouExeReturnLblFromOnTenshouExeGetDaimyoKoukeishaBushouID
	}
}



char cmdOnTenshouExeJumpFromGetDaimyoKoukeishaBushouID[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeGetDaimyoKoukeishaBushouID() {

	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeGetDaimyoKoukeishaBushouID;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeGetDaimyoKoukeishaBushouID + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeGetDaimyoKoukeishaBushouID  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromGetDaimyoKoukeishaBushouID+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeGetDaimyoKoukeishaBushouID), cmdOnTenshouExeJumpFromGetDaimyoKoukeishaBushouID, 5, NULL); //5バイトのみ書き込む
}

